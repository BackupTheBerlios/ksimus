/***************************************************************************
                          connectorbooltristate.cpp  -  description
                             -------------------
    begin                : Sat Jun 29 2002
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

// QT includes
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "connectorinfo.h"
#include "connectorbooltristate.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "component.h"
#include "wirepropertybooltristate.h"
#include "wire.h"
#include "watchitembooltristate.h"

// Forward declaration


static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolTriState(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolTriStateInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean TriState"),
	                                QString::fromLatin1("Boolean TriState"),
	                                QString::fromLatin1("Boolean TriState"),
	                                create );
	return &Info;
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name,
		                                         const QString & i18nName, const QPoint & pos)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateInfo()),
		m_inData(),
		m_outData()
{
	init();
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name, const QString & i18nName,
		                                         const QString & descr, const QPoint & pos)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateInfo()),
		m_inData(),
		m_outData()
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name, const QString & i18nName,
		                                         const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, orient, ci),
		m_inData(),
		m_outData()
{
	init();
}

void ConnectorBoolTriState::init()
{
}

// Get the colors for the connector
const WireColorScheme & ConnectorBoolTriState::getColorScheme() const
{
	return WirePropertyBoolTriState::colorScheme();
}
	
/** Resets the connector
*/
void ConnectorBoolTriState::reset()
{
	ConnectorTriStateBase::reset();
	
	m_inData.set(0,0);
	m_outData.set(0,0);
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorBoolTriState::copyData(const void * pData)
{
	m_inData = *(const WireStateBoolTriState*)pData;
}

/** Set the current output */
void ConnectorBoolTriState::setOutput(bool out, bool active, bool exeWirePropNext)
{
	if ((out != m_outData.getState()) || (active != m_outData.isActive()))
	{
		// Value changed
		if (!active)
		{
			m_outData.set(0, 0);
		}
		else if (out)
		{
			m_outData.set(1, 0);
		}
		else
		{
			m_outData.set(0, 1);
		}
		// No Wire connected ?
		if (!getWireProperty())
		{
			m_inData = m_outData;
		}
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

/** Returns a pointer to the data that's read from the component. */
const void * ConnectorBoolTriState::readoutData() const
{
	return &m_outData;
}

const void * ConnectorBoolTriState::readoutInData() const
{
	return &m_inData;
}

/** Returns a text which represents the current value. */
QString ConnectorBoolTriState::getValueText() const
{
	return m_inData.getText();
}

WatchItemBase * ConnectorBoolTriState::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTriStateConnector(this);
	CHECK_PTR(wi);
	return wi;
}
