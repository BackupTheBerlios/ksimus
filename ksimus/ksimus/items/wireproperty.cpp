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

#include "ksimus.h"
#include "ksimusdoc.h"
#include "ksimdata.h"
#include "wirepropertyinfo.h"
#include "wire.h"
#include "ksimdebug.h"
#include "component.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "externalconnector.h"
#include "module.h"


KSIMDEBUG_INSTANCE_COUNTER(WireProperty);

WireProperty::WireProperty(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	QObject(wire),
		ComponentItem(wire),
		m_wirePropertyInfo(wirePropertyInfo),
		m_inputCounter(0),
		m_outputCounter(0),
		m_tristateCounter(0)
{
	KSIMDEBUG_INSTANCE_INC(WireProperty);
}

WireProperty::~WireProperty()
{
	KSIMDEBUG_INSTANCE_DEC(WireProperty);
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
	m_tristateCounter = 0;
	
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
		else if (it.current()->isTristate())
		{
			m_tristateCounter++;
		}
		else
		{
			KSIMDEBUG("unknown connector direction");
		}
	}

	//*** min 1 Output or Tristate required
	if ((m_outputCounter == 0) && (m_tristateCounter == 0))
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
	KMessageBox::sorry( getComponent()->getDoc()->getApp(),
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
	if (m_tristateCounter != 0)
	{
		logError(i18n("Tristates outputs are not allowed"));
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
		it.current()->setWireProperty(this);
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
					if (-1 == m_inZeroDelayConnectorList->findRef(it.current()))
						m_inZeroDelayConnectorList->append(it.current());
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
			}
			else
			{
				// Regular connector
				if (-1 == m_inConnectorList->findRef(it.current()))
					m_inConnectorList->append(it.current());
			}
		}
		else if (it.current()->isOutput())
		{
			m_outConnector = it.current();
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
		it.current()->putData(pData);
		it.current()->executeComponentNext();
	}
	
	FOR_EACH_CONNECTOR(it, *m_inZeroDelayConnectorList)
	{
		it.current()->putData(pData);
	}
	
	FOR_EACH_COMP(itExt, *m_zeroDelayList)
	{
		// Execute zero delay components
		itExt.current()->calculate();
	}
}


//##################################################################################
//##################################################################################


WirePropertyInvalidBase::WirePropertyInvalidBase(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	: WireProperty(wire, wirePropertyInfo)
{
}

/*WirePropertyInvalid::~WirePropertyInvalid()
{
} */

void WirePropertyInvalidBase::setupCircuit()
{
	return;
}

void WirePropertyInvalidBase::execute()
{
	return;
}

const void * WirePropertyInvalidBase::readoutData() const
{
	return (const void *)0;
}

	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyInvalidBase::getColorScheme() const
{
	static WireColorScheme colorScheme(Qt::black, Qt::white);

	return colorScheme;
}


//##################################################################################
//##################################################################################

static WireProperty * createWirePropertyInvalidDifferentOutputs(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyInvalidDifferentOutputs(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getWirePropertyInvalidDifferentOutputsInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Invalid Different Outputs"),
	                                   QString::fromLatin1("wire/property/invalid different outputs"),
	                                   QString::fromLatin1("Invalid Different Outputs"),
	                                   createWirePropertyInvalidDifferentOutputs );
	return &Info;
}


QString WirePropertyInvalidDifferentOutputs::m_errorMsg = QString();


WirePropertyInvalidDifferentOutputs::WirePropertyInvalidDifferentOutputs(Wire * wire)
	:	WirePropertyInvalidBase(wire, getWirePropertyInvalidDifferentOutputsInfo())
{
	logWarning(getErrorMsg());
};

/*WirePropertyInvalidDifferentOutputs::~WirePropertyInvalidDifferentOutputs()
{

} */

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
int WirePropertyInvalidDifferentOutputs::checkCircuit()
{
	logError(getErrorMsg());

	return 1;
}

	/** Returns the error message. */
QString WirePropertyInvalidDifferentOutputs::getErrorMsg()
{
	if (m_errorMsg.isNull())
	{
		m_errorMsg = i18n("The wire is connected to output connectors with different data types.\n");
	}

	return m_errorMsg;
}


//##################################################################################
//##################################################################################

static WireProperty * createWirePropertyInvalidIncompatibleInputs(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyInvalidIncompatibleInputs(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getWirePropertyInvalidIncompatibleInputsInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Invalid Incompatible Inputs"),
	                                   QString::fromLatin1("wire/property/invalid incompatible inputs"),
	                                   QString::fromLatin1("Invalid Incompatible Inputs"),
	                                   createWirePropertyInvalidIncompatibleInputs );
	return &Info;
}


QString WirePropertyInvalidIncompatibleInputs::m_errorMsg = QString();


WirePropertyInvalidIncompatibleInputs::WirePropertyInvalidIncompatibleInputs(Wire * wire)
	:	WirePropertyInvalidBase(wire, getWirePropertyInvalidDifferentOutputsInfo())
{
	logWarning(getErrorMsg());
};

/*WirePropertyInvalidIncompatibleInputs::~WirePropertyInvalidIncompatibleInputs()
{

} */

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
int WirePropertyInvalidIncompatibleInputs::checkCircuit()
{
	logError(getErrorMsg());

	return 1;
}

	/** Returns the error message. */
QString WirePropertyInvalidIncompatibleInputs::getErrorMsg()
{
	if (m_errorMsg.isNull())
	{
		m_errorMsg = i18n("The wire is connected to one or more input connectors with incompatible data types.");
	}

	return m_errorMsg;
}


//##################################################################################
//##################################################################################

static WireProperty * createWirePropertyInvalidDifferentInputsNoOutput(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyInvalidDifferentInputsNoOutput(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getWirePropertyInvalidDifferentInputsNoOutputInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Invalid Different Inputs No Output"),
	                                   QString::fromLatin1("wire/property/invalid different inputs no output"),
	                                   QString::fromLatin1("Invalid Different Inputs No Output"),
	                                   createWirePropertyInvalidDifferentInputsNoOutput );
	return &Info;
}


QString WirePropertyInvalidDifferentInputsNoOutput::m_errorMsg = QString();


WirePropertyInvalidDifferentInputsNoOutput::WirePropertyInvalidDifferentInputsNoOutput(Wire * wire)
	:	WirePropertyInvalidBase(wire, getWirePropertyInvalidDifferentInputsNoOutputInfo())
{
	logWarning(getErrorMsg());
};

/*WirePropertyInvalidDifferentInputsNoOutput::~WirePropertyInvalidDifferentInputsNoOutput()
{

} */

	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
int WirePropertyInvalidDifferentInputsNoOutput::checkCircuit()
{
	logError(getErrorMsg());

	return 1;
}

	/** Returns the error message. */
QString WirePropertyInvalidDifferentInputsNoOutput::getErrorMsg()
{
	if (m_errorMsg.isNull())
	{
		m_errorMsg = i18n("The wire is connected to input connectors with different data types.");
	}

	return m_errorMsg;
}


#include "wireproperty.moc"

