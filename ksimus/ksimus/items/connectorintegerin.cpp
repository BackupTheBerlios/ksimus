/***************************************************************************
                          connectorintegerin.cpp  -  description
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
#include "connectorintegerin.h"
#include "connectorinfo.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "wirepropertyinteger.h"
#include "watchiteminteger.h"


ConnectorBase * ConnectorIntegerIn::create(Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos)
{
	return new ConnectorIntegerIn(comp, name, i18nName, pos);
}

const ConnectorInfo * ConnectorIntegerIn::getStaticInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Integer Input"),
	                                QString::fromLatin1("Integer Input"),
	                                QString::fromLatin1("Integer"),
	                                create );
	return &Info;
}


ConnectorIntegerIn::ConnectorIntegerIn(Component * comp, const QString & name,
                                   const QString & i18nName, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getStaticInfo())
{
	init();
}

ConnectorIntegerIn::ConnectorIntegerIn(	Component * comp,	const QString & name, const QString & i18nName,
                                  const QString & descr, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getStaticInfo())
{
	init();
	new ConnectorLabel(this, descr);
}


ConnectorIntegerIn::ConnectorIntegerIn(Component * comp, const QString & name, const QString & i18nName,
                                   const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorInputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}


void ConnectorIntegerIn::init()
{
	setNegateEnabled(false);
	m_data = 0;
}

// Get the colors for the connector
const WireColorScheme & ConnectorIntegerIn::getColorScheme() const
{
	return WirePropertyInteger::colorScheme();
}

/** Resets the connector
*/
void ConnectorIntegerIn::reset()
{
	ConnectorInputBase::reset();
	
	m_data = 0;
}

/** Returns a pointer to the data that's read from the component
  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
const void * ConnectorIntegerIn::readoutData() const
{
	return &m_data;
}
	
	/** Copies the data where the p points to into a local variable.
	  * The function must be implemented by a sub class. */
void ConnectorIntegerIn::copyData(const void * p)
{
	int tmp = *(const int *)p;
	if (tmp != m_data)
	{
		m_data = tmp;
		executeComponentNext();
	}
}

/** Returns the input data */
int ConnectorIntegerIn::getInput() const
{
	return m_data;
}

/** Returns a text which represents the current value. */
QString ConnectorIntegerIn::getValueText() const
{
	return QString::number(getInput());
}


WatchItemBase * ConnectorIntegerIn::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemIntegerConnector(this);
	CHECK_PTR(wi);
	return wi;
}

#include "connectorintegerin.moc"
