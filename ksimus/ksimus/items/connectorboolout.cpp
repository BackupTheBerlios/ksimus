/***************************************************************************
                          connectorboolout.cpp  -  description
                             -------------------
    begin                : Sun Oct 1 2000
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

// QT includes
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "connectorinfo.h"
#include "connectorboolout.h"
#include "connectorbooloutpropertywidget.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "component.h"
#include "wirepropertyboolean.h"
#include "wire.h"
#include "watchitemboolean.h"

// Forward declaration


static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolOut(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolOutInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean Output"),
	                                QString::fromLatin1("Boolean Output"),
	                                QString::fromLatin1("Boolean"),
	                                create );
	return &Info;
}

ConnectorBoolOut::ConnectorBoolOut(Component * comp, const QString & name,
                                   const QString & i18nName, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolOutInfo()),
		m_data(false)
{
	init();
}
											
ConnectorBoolOut::ConnectorBoolOut(Component * comp, const QString & name, const QString & i18nName,
                                   const QString & descr, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolOutInfo()),
		m_data(false)
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolOut::ConnectorBoolOut(Component * comp, const QString & name, const QString & i18nName,
                                   const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorOutputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}

void ConnectorBoolOut::init()
{
	setNegateEnabled(true);
}

// Get the colors for the connector
const WireColorScheme & ConnectorBoolOut::getColorScheme() const
{
	static WireColorScheme colorScheme(darkGreen);
	
	return colorScheme;
}
	
/** Resets the connector
*/
void ConnectorBoolOut::reset()
{
	ConnectorOutputBase::reset();
	
	m_data = isNegated();
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorBoolOut::copyData(const void * pData)
{
	setOutput(*(bool*)pData);
}

/** Set the current output */
void ConnectorBoolOut::setOutput(bool out, bool exeWirePropNext)
{
	bool tmpData = out ^ isNegated();
	if (tmpData != m_data)
	{
		// Value changed
		m_data = tmpData;
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

/** Return the current output */
bool ConnectorBoolOut::getOutput() const
{
	return m_data ^ isNegated();
}

/** Returns a pointer to the data that's read from the component. */
const void * ConnectorBoolOut::readoutData() const
{
	return &m_data;
}

/** Creates the property widget */
PropertyWidget* ConnectorBoolOut::propertyWidget(QWidget * parent)
{
	return new ConnectorBoolOutPropertyWidget(this, parent, getName());
}

/** Returns a text which represents the current value. */
QString ConnectorBoolOut::getValueText() const
{
	return WirePropertyBoolean::getI18nTextValue(m_data);
}

WatchItemBase * ConnectorBoolOut::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBooleanConnector(this);
	Q_CHECK_PTR(wi);
	return wi;
}

#include "connectorboolout.moc"
