/***************************************************************************
                          connectorfloatout.cpp  -  description
                             -------------------
    begin                : Fri Dec 28 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qpainter.h>

// KDE includes

// Project includes
#include "connectorfloatout.h"
#include "connectorinfo.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "wirepropertyfloatingpoint.h"
#include "watchitemfloatingpoint.h"

// Forward declaration


static ConnectorBase * create(Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos)
{
	return new ConnectorFloatOut(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorFloatOutInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Floating Point Output"),
	                                QString::fromLatin1("Floating Point Output"),
	                                QString::fromLatin1("Floating Point"),
	                                create );
	return &Info;
}

ConnectorFloatOut::ConnectorFloatOut(Component * comp, const QString & name,
                                     const QString & i18nName, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorFloatOutInfo()),
		m_data(false)
{
	init();
}

ConnectorFloatOut::ConnectorFloatOut(Component * comp, const QString & name, const QString & i18nName,
                                     const QString & descr, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorFloatOutInfo()),
		m_data(false)
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorFloatOut::ConnectorFloatOut(Component * comp, const QString & name, const QString & i18nName,
                                     const QPoint & pos,ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorOutputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}

void ConnectorFloatOut::init()
{
	setNegateEnabled(true);
}

// Get the colors for the connector
const WireColorScheme & ConnectorFloatOut::getColorScheme() const
{
	return WirePropertyFloatingPoint::colorScheme();
}


/** Resets the connector
*/
void ConnectorFloatOut::reset()
{
	ConnectorOutputBase::reset();
	
	m_data = 0.0;
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorFloatOut::copyData(const void * pData)
{
	setOutput(*(double*)pData);
}

/** Returns a pointer to the data that's read from the component. */
const void * ConnectorFloatOut::readoutData() const
{
	return &m_data;
}

/** Set the current output */
void ConnectorFloatOut::setOutput(double out, bool exeWirePropNext)
{
	if (out != m_data)
	{
		// Value changed
		m_data = out;
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

/** Return the current output */
double ConnectorFloatOut::getOutput() const
{
	return m_data;
}

/** Returns a pointer to the data of this output connector */
const void * ConnectorFloatOut::getData() const
{
	return &m_data;
}


/** Returns a text which represents the current value. */
QString ConnectorFloatOut::getValueText() const
{
	return QString::number(m_data);
}

WatchItemBase * ConnectorFloatOut::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemFloatingPointConnector(this);
	CHECK_PTR(wi);
	return wi;
}

