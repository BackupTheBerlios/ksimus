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
#include "compcontainer.h"
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
	m_connectorList = new ConnectorTristateList();
	Q_CHECK_PTR(m_connectorList);
	
	m_connectorInputList = new ConnectorInputList();
	Q_CHECK_PTR(m_connectorInputList);

	m_executeNextList = new ComponentList();
	Q_CHECK_PTR(m_executeNextList);
	
	m_zeroDelayList = new ComponentList();
	Q_CHECK_PTR(m_zeroDelayList);
	
	m_wirePropertyList = new WirePropertyList();
	Q_CHECK_PTR(m_wirePropertyList);
	
	
	connect(getWire()->getDoc(), SIGNAL(signalPreReset()), SLOT(slotPreReset()));
}

WirePropertyMultipleOutput::~WirePropertyMultipleOutput()
{
	delete m_connectorList;
	delete m_connectorInputList;
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
	m_connectorInputList->clear();
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
			if (comp->isModule())
			{
				// Get the external connector if comp is a module
				ExternalConnector * extConn = ((Module *)comp)->searchExtConn(it.current());
				if (extConn)
				{
					comp = extConn;
				}
				else
				{
					KSIMDEBUG(QString::fromLatin1("ExternalConnector not found Module %1 Connector %2")
					          .arg(comp->getName()).arg(it.current()->getFullName()));
					Q_ASSERT(extConn);
				}
			}
			if (it.current()->inherits("ConnectorInputBase"))
			{
				if (-1 == m_connectorInputList->findRef((ConnectorInputBase*)it.current()))
				{
					m_connectorInputList->append((ConnectorInputBase*)it.current());
					if (comp->isZeroDelayComponent())
					{
//						KSIMDEBUG_VAR("Input Zero", comp->getName());
						// Component has to caclulate immediatly
						if (-1 == m_zeroDelayList->findRef(comp))
							m_zeroDelayList->append(comp);
					}
					else
					{
//						KSIMDEBUG_VAR("Input Next", comp->getName());
						// Component has to caclulate in next cycle
						if (-1 == m_executeNextList->findRef(comp))
							m_executeNextList->append(comp);
					}
				}
			}
			else if (it.current()->inherits("ConnectorTristateBase"))
			{
				// Add connector if not in list and is not a connected Module or ExtConn connector
				if ((-1 == m_connectorList->findRef((ConnectorTristateBase*)it.current())))
				{
					m_connectorList->append((ConnectorTristateBase*)it.current());
					if (comp->isZeroDelayComponent())
					{
						// Component has to calculate immediatly
						if (-1 == m_zeroDelayList->findRef(comp))
							m_zeroDelayList->append(comp);
					}
					else
					{
						// Component has to calculate in next cycle
						if (-1 == m_executeNextList->findRef(comp))
							m_executeNextList->append(comp);
						}
				}
			}
			else
			{
				KSIMDEBUG(QString::fromLatin1("Connector '%1' is not a ConnectorTristateBase (Component %2)")
				          .arg(it.current()->getName())
				          .arg(comp->getName()));
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
					setupInternalAddWireProperty(it.current());
					setupInternalAddWireProperty(extConn->getInternalConn());
				}
				else
				{
					KSIMDEBUG(QString::fromLatin1("ExternalConnector not found Module %1 Connector %2")
					          .arg(comp->getName()).arg(it.current()->getFullName()));
					Q_ASSERT(extConn);
				}
			}
			else if (comp->isExtConn())
			{
				// ExternalConnector
				ExternalConnector * extConn = (ExternalConnector *)comp;
				setupInternalAddWireProperty(extConn->getInternalConn());
				setupInternalAddWireProperty(extConn->getUsedExternalConn());
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
//			KSIMDEBUG("WireProperty is not a WirePropertyMultipleOutput");
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

#include "wirepropertymultipleoutput.moc"

