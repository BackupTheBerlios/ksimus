/***************************************************************************
                          wireproperty.cpp  -  description
                             -------------------
    begin                : Sun Oct 15 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
    email                : ksimus@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// C includes

// QT includes

// KDE includes
#include <klocale.h>
#include <kmessagebox.h>

// Project includes
#include "wireproperty.h"

#include "ksimdata.h"
#include "wirepropertyinfo.h"
#include "wire.h"
#include "ksimdebug.h"
#include "component.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "externalconnector.h"
#include "module.h"


WireProperty::WireProperty(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	QObject(wire),
		ComponentItem(wire),
		m_wirePropertyInfo(wirePropertyInfo),
		m_inputCounter(0),
		m_outputCounter(0),
		m_triStateCounter(0)
{
}

WireProperty::~WireProperty()
{
}

/** save wire properties */
void WireProperty::save(KSimData &) const
{
	// nothing to do
}

/** load wire properties
	Returns true if successful */
bool WireProperty::load(KSimData &)
{
	// nothing to do
	return true;
}

/** Checks the connected component
	eg. No output connected or more than one connected
	Returns the number of errors
*/
int WireProperty::checkCircuit()
{
	int errors = 0;
	
	
	//*** Count inputs, outputs and triStates
	m_inputCounter = 0;
	m_outputCounter = 0;
	m_triStateCounter = 0;
	
	FOR_EACH_CONNECTOR(it,*getWire()->getConnList())
	{
		if (it.current()->isInput())
		{
			m_inputCounter++;
		}
		else if (it.current()->isOutput())
		{
			m_outputCounter++;
		}
		else if (it.current()->isTriState())
		{
			m_triStateCounter++;
		}
		else
		{
			KSIMDEBUG("unknown connector direction");
		}
	}

	//*** min 1 Output or TriState required
	if ((m_outputCounter == 0) && (m_triStateCounter == 0))
	{
		// Wire without controlling output
		logError(i18n("Wire without controlling output"));
		errors += 1;
	}
	
	return errors;
}

	
/** Add menu items depend on wire properties
  *	Return true, if items are added */
bool WireProperty::initPopupMenu(QPopupMenu *) const
{
	return false;
}
	
/** Returns the associated WirePropertyInfo */
const WirePropertyInfo * WireProperty::getInfo() const
{
	return m_wirePropertyInfo;
}

WatchItemBase * WireProperty::makeWatchItem()
{
	KMessageBox::sorry( (QWidget *)0,
	                    i18n("The watch functionality is not implementated yet.\n"
	                         "Wire Type: %1")
	                         .arg(this->getInfo()->getName()));
	return (WatchItemBase *)0;
}


//##############################################################################
//##############################################################################

WirePropertySingleOutput::WirePropertySingleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	WireProperty(wire, wirePropertyInfo),
		m_outConnector(0)
{
	m_inConnectorList = new ConnectorList();
	CHECK_PTR(m_inConnectorList);
	m_inZeroDelayConnectorList = new ConnectorList();
	CHECK_PTR(m_inZeroDelayConnectorList);
	
	m_zeroDelayList = new ComponentList();
	CHECK_PTR(m_zeroDelayList);
}

WirePropertySingleOutput::~WirePropertySingleOutput()
{
	delete m_inConnectorList;
	delete m_inZeroDelayConnectorList;
	delete m_zeroDelayList;
}

/** Checks the connected component
	eg. No output connected or more than one connected
	Returns the number of errors
*/
int WirePropertySingleOutput::checkCircuit()
{
	int errors = WireProperty::checkCircuit();
	
	//*** 1 Output required
	if (m_outputCounter > 1)
	{
		logError(i18n("Wire has more than one connected output"));
		errors += 1;
	}
	
	//*** no tristate allowed
	if (m_triStateCounter != 0)
	{
		logError(i18n("TriStates outputs are not allowed"));
		errors += 1;
	}
	
	return errors;
}
	
/** Setup the Wire property for a new circuit execution.
 * The sub class has to implement this function.
 */
void WirePropertySingleOutput::setupCircuit()
{
	m_inConnectorList->clear();
	m_inZeroDelayConnectorList->clear();
	m_zeroDelayList->clear();
	m_outConnector = 0;
	
	FOR_EACH_CONNECTOR(it, *getWire()->getConnList())
	{
		if (it.current()->isInput())
		{
			if (it.current()->getComponent()->isModule())
			{
				// Module
				Module * module = (Module *)it.current()->getComponent();
				ExternalConnector * extConn = module->searchExtConn(it.current());
				if (extConn)
				{
					if (-1 == m_zeroDelayList->findRef(extConn))
						m_zeroDelayList->append(extConn);
					if (-1 == m_inZeroDelayConnectorList->findRef(extConn->getExternalConn()))
						m_inZeroDelayConnectorList->append(extConn->getExternalConn());
					extConn->getExternalConn()->setWireProperty(this);
				}
				else
				{
					KSIMDEBUG("ExternalConnector not found");
					ASSERT(extConn);
				}
			}
			else if (it.current()->getComponent()->isZeroDelayComponent())
			{
				// Zero Delay Components
				if (-1 == m_zeroDelayList->findRef(it.current()->getComponent()))
					m_zeroDelayList->append(it.current()->getComponent());
				if (-1 == m_inZeroDelayConnectorList->findRef(it.current()))
					m_inZeroDelayConnectorList->append(it.current());
				it.current()->setWireProperty(this);
			}
			else
			{
				// Regular connector
				if (-1 == m_inConnectorList->findRef(it.current()))
					m_inConnectorList->append(it.current());
				it.current()->setWireProperty(this);
			}
		}
		else if (it.current()->isOutput())
		{
			if (it.current()->getComponent()->isModule())
			{
				// Module
				Module * module = (Module *)it.current()->getComponent();
				ExternalConnector * extConn = module->searchExtConn(it.current());
				if (extConn)
				{
					m_outConnector = extConn->getExternalConn();
					extConn->getExternalConn()->setWireProperty(this);
				}
				else
				{
					KSIMDEBUG("ExternalConnector not found");
					ASSERT(extConn);
				}
			}
			else
			{
				// Regular connector
				m_outConnector = it.current();
				it.current()->setWireProperty(this);
			}
		}
		else
		{
			KSIMDEBUG("Bad connector");
		}
	}
	
	ASSERT(m_outConnector);
}

const void * WirePropertySingleOutput::readoutData() const
{
	if (m_outConnector) return m_outConnector->readoutData();
	return (const void *)0;
}

/** Executes the WireProperty. This means copies the data from the output connector
  * to the input connector. */
void WirePropertySingleOutput::execute()
{
	const void * pData = m_outConnector->readoutData();
	FOR_EACH_CONNECTOR(it, *m_inConnectorList)
	{
		it.current()->copyData(pData);
		it.current()->executeComponentNext();
	}
	
	FOR_EACH_CONNECTOR(it, *m_inZeroDelayConnectorList)
	{
		it.current()->copyData(pData);
	}
	
	FOR_EACH_COMP(itExt, *m_zeroDelayList)
	{
		// Execute zero delay components
		itExt.current()->calculate();
	}
}
