/***************************************************************************
                          connectorintegerout.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include "connectorintegerout.h"
#include "connectorinfo.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "wirepropertyinteger.h"
#include "watchiteminteger.h"

// Forward declaration


ConnectorBase * ConnectorIntegerOut::create(Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos)
{
	return new ConnectorIntegerOut(comp, name, i18nName, pos);
}

const ConnectorInfo * ConnectorIntegerOut::getStaticInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Integer Output"),
	                                QString::fromLatin1("Integer Output"),
	                                QString::fromLatin1("Integer"),
	                                create );
	return &Info;
}

ConnectorIntegerOut::ConnectorIntegerOut(Component * comp, const QString & name,
                                     const QString & i18nName, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getStaticInfo()),
		m_data(0)
{
	init();
}

ConnectorIntegerOut::ConnectorIntegerOut(Component * comp, const QString & name, const QString & i18nName,
                                     const QString & descr, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, i18nName, pos, CO_RIGHT, getStaticInfo()),
		m_data(0)
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorIntegerOut::ConnectorIntegerOut(Component * comp, const QString & name, const QString & i18nName,
                                     const QPoint & pos,ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorOutputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}

void ConnectorIntegerOut::init()
{
	setNegateEnabled(false);
}

// Get the colors for the connector
const WireColorScheme & ConnectorIntegerOut::getColorScheme() const
{
	return WirePropertyInteger::colorScheme();
}


/** Resets the connector
*/
void ConnectorIntegerOut::reset()
{
	ConnectorOutputBase::reset();
	
	m_data = 0;
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorIntegerOut::copyData(const void * pData)
{
	setOutput(*(const int*)pData);
}

/** Returns a pointer to the data that's read from the component. */
const void * ConnectorIntegerOut::readoutData() const
{
	return &m_data;
}

/** Set the current output */
void ConnectorIntegerOut::setOutput(int out, bool exeWirePropNext)
{
	if (out != m_data)
	{
		// Value changed
		m_data = out;
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

/** Return the current output */
int ConnectorIntegerOut::getOutput() const
{
	return m_data;
}

/** Returns a pointer to the data of this output connector */
const void * ConnectorIntegerOut::getData() const
{
	return &m_data;
}


/** Returns a text which represents the current value. */
QString ConnectorIntegerOut::getValueText() const
{
	return QString::number(m_data);
}

WatchItemBase * ConnectorIntegerOut::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemIntegerConnector(this);
	CHECK_PTR(wi);
	return wi;
}

#include "connectorintegerout.moc"

