/***************************************************************************
                          connectorfloatin.cpp  -  description
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
#include "connectorfloatin.h"
#include "connectorinfo.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "wirepropertyfloatingpoint.h"
#include "watchitemfloatingpoint.h"


static ConnectorBase * create(Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos)
{
	return new ConnectorFloatIn(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorFloatInInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Floating Point Input"),
	                                QString::fromLatin1("Floating Point Input"),
	                                QString::fromLatin1("Floating Point"),
	                                create );
	return &Info;
}


ConnectorFloatIn::ConnectorFloatIn(Component * comp, const QString & name,
                                   const QString & i18nName, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getConnectorFloatInInfo())
{
	init();
}

ConnectorFloatIn::ConnectorFloatIn(	Component * comp,	const QString & name, const QString & i18nName,
                                  const QString & descr, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getConnectorFloatInInfo())
{
	init();
	new ConnectorLabel(this, descr);
}


ConnectorFloatIn::ConnectorFloatIn(Component * comp, const QString & name, const QString & i18nName,
                                   const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorInputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}


void ConnectorFloatIn::init()
{
	setNegateEnabled(true);
	m_data = 0.0;
}

// Get the colors for the connector
const WireColorScheme & ConnectorFloatIn::getColorScheme() const
{
	return WirePropertyFloatingPoint::colorScheme();
}

/** Resets the connector
*/
void ConnectorFloatIn::reset()
{
	ConnectorInputBase::reset();
	
	m_data = 0.0;
}

/** Returns a pointer to the data that's read from the component
  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
const void * ConnectorFloatIn::readoutData() const
{
	return &m_data;
}
	
	/** Copies the data where the p points to into a local variable.
	  * The function must be implemented by a sub class. */
void ConnectorFloatIn::copyData(const void * p)
{
	double tmp = *(const double *)p;
	if (tmp != m_data)
	{
		m_data = tmp;
		executeComponentNext();
	}
}

/** Returns the input data */
double ConnectorFloatIn::getInput() const
{
	return m_data;
}

/** Returns a text which represents the current value. */
QString ConnectorFloatIn::getValueText() const
{
	return QString::number(getInput());
}


WatchItemBase * ConnectorFloatIn::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemFloatingPointConnector(this);
	CHECK_PTR(wi);
	return wi;
}

