/***************************************************************************
                          wirepropertymultipleoutput.cpp  -  description
                             -------------------
    begin                : Fri Jun 28 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "wirepropertymultipleoutput.h"
#include "wire.h"
#include "ksimusdoc.h"
#include "connectorbase.h"
#include "externalconnector.h"
#include "module.h"
#include "ksimdebug.h"


// Forward declaration


WirePropertyMultipleOutput::WirePropertyMultipleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	WireProperty(wire, wirePropertyInfo),
		m_activeConnectors(0),
		m_masterWP((WirePropertyMultipleOutput *) 0)
{
	m_connectorList = new ConnectorTriStateList();
	CHECK_PTR(m_connectorList);
	
	m_executeNextList = new ComponentList();
	CHECK_PTR(m_executeNextList);
	
	m_zeroDelayList = new ComponentList();
	CHECK_PTR(m_zeroDelayList);
	
	m_wirePropertyList = new WirePropertyList();
	CHECK_PTR(m_wirePropertyList);
	
	
	connect(getWire()->getDoc(), SIGNAL(signalPreReset()), SLOT(slotPreReset()));
}

WirePropertyMultipleOutput::~WirePropertyMultipleOutput()
{
	delete m_connectorList;
	delete m_executeNextList;
	delete m_zeroDelayList;
	delete m_wirePropertyList;
}


WirePropertyMultipleOutput * WirePropertyMultipleOutput::getMasterWireProperty() const
{
	return m_masterWP;
}

void WirePropertyMultipleOutput::setMasterWireProperty(WirePropertyMultipleOutput * masterWP)
{
	m_masterWP = masterWP;
}
	

void WirePropertyMultipleOutput::slotPreReset()
{
	setMasterWireProperty((WirePropertyMultipleOutput *) 0);
}

/** Setup the Wire property for a new circuit execution.
 * The sub class has to implement this function.
 */
void WirePropertyMultipleOutput::setupCircuit()
{
	m_activeConnectors = 0;
	
	if (getMasterWireProperty())
	{
//		KSIMDEBUG_VAR("setupCircuit() skipped", getWire()->getName());
		return;
	}
	
//	KSIMDEBUG_VAR("setupCircuit()", getWire()->getName());
	
	Component * comp;
	m_connectorList->clear();
	m_executeNextList->clear();
	m_zeroDelayList->clear();
	m_wirePropertyList->clear();
	
	// Setup Wire Properties
	setupInternal(this);
	
	FOR_EACH_WIREPROPERTY(itWp, *m_wirePropertyList)
	{
		WireProperty * wp = itWp.current();
		((WirePropertyMultipleOutput*)itWp.current())->setMasterWireProperty(this);
//		KSIMDEBUG_VAR("setup WP", itWp.current()->getWire()->getName());
		
		FOR_EACH_CONNECTOR(it, *wp->getWire()->getConnList())
		{
			it.current()->setWireProperty(this);
			comp = it.current()->getComponent();
			if (comp->isModule() || comp->isExtConn())
			{
				// Nothing todo
			}
			else if (!it.current()->inherits("ConnectorTriStateBase"))
			{
				KSIMDEBUG(QString::fromLatin1("Connector '%1' is not a ConnectorTriStateBase (Component %2)")
				          .arg(it.current()->getName())
				          .arg(comp->getName()));
			}
			else if (-1 == m_connectorList->findRef((ConnectorTriStateBase*)it.current()))
			{
				m_connectorList->append((ConnectorTriStateBase*)it.current());
				if (comp->isZeroDelayComponent())
				{
					// Component has to caclulate immediatly
					if (-1 == m_zeroDelayList->findRef(comp))
						m_zeroDelayList->append(comp);
				}
				else
				{
					// Component has to caclulate in next cycle
					if (-1 == m_executeNextList->findRef(comp))
						m_executeNextList->append(comp);
				}
			}
		}
	}
}

void WirePropertyMultipleOutput::setupInternal(WirePropertyMultipleOutput * wireProperty)
{
	if (wireProperty && (-1 == m_wirePropertyList->findRef(wireProperty)))
	{
//		KSIMDEBUG_VAR("Add WP to list",wireProperty->getWire()->getName());
		// Not included yet
		m_wirePropertyList->append(wireProperty);
		FOR_EACH_CONNECTOR(it, *wireProperty->getWire()->getConnList())
		{
			Component * comp = it.current()->getComponent();
			if (comp->isModule())
			{
				// Module
				ExternalConnector * extConn = ((Module *)comp)->searchExtConn(it.current());
				if (extConn)
				{
					setupInternalAddWireProperty(extConn->getExternalConn());
					setupInternalAddWireProperty(extConn->getInternalConn());
				}
				else
				{
					KSIMDEBUG("ExternalConnector not found");
					ASSERT(extConn);
				}
			}
			else if (comp->isExtConn())
			{
				// ExternalConnector
				ExternalConnector * extConn = (ExternalConnector *)comp;
				setupInternalAddWireProperty(extConn->getExternalConn());
				setupInternalAddWireProperty(extConn->getInternalConn());
			}
		}
	}
	else
	{
//		KSIMDEBUG_VAR("WP already in list",wireProperty->getWire()->getName());
	}
}

void WirePropertyMultipleOutput::setupInternalAddWireProperty(ConnectorBase * conn)
{
	if (conn && conn->getWire())
	{
		// is connected
		WireProperty * wp = conn->getWire()->getWireProperty();
		if (!wp)
		{
			KSIMDEBUG("Wire has no WireProperty");
		}
		else if (wp->inherits("WirePropertyMultipleOutput"))
		{
			setupInternal((WirePropertyMultipleOutput *) wp);
		}
		else
		{
			KSIMDEBUG("WireProperty is not a WirePropertyMultipleOutput");
		}
	}
}

unsigned int WirePropertyMultipleOutput::getActiveConnectorCount() const
{
	if (getMasterWireProperty())
	{
		return getMasterWireProperty()->m_activeConnectors;
	}
	
	return 0;
}
