/***************************************************************************
                          wirepropertyfloatingpoint.cpp  -  description
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

// C-Includes

// QT-Includes
#include <qpainter.h>

// KDE-Includes

// Project-Includes
#include "wirepropertyfloatingpoint.h"
#include "connectorfloatout.h"
#include "ksimdebug.h"
#include "wire.h"
#include "loglist.h"
#include "wirepropertyinfo.h"

// Forward declaration


static WireProperty * create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyFloatingPoint(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo wirePropertyFloatingPointInfo("Wire Property Floating Point",
                                                     "wire/property/floating point",
                                                     "Floating Point",
                                                     create
                                                     );
												


WirePropertyFloatingPoint::WirePropertyFloatingPoint(Wire * wire)
	:	WirePropertySingleOutput(wire, &wirePropertyFloatingPointInfo)
{
}

WirePropertyFloatingPoint::~WirePropertyFloatingPoint()
{
}
	
/** Returns a pointer to the current data */	
const void * WirePropertyFloatingPoint::getCurrentData() const
{
	ConnectorBase * conn = getWire()->getConnList()->at(0);
	
	if (conn)
	{
		return ((ConnectorFloatOut*)conn)->getData();
	}
	else
	{
		getWire()->getLogList()->logDebug(QString("Wire %1 returns <NULL> connector").arg(getWire()->getName()));
		return 0;
	}
}

// Setup the colors, brushs, and fills for the connector
void WirePropertyFloatingPoint::setupColorScheme(QPainter * p) const
{
	colorScheme(p);
}
											
void WirePropertyFloatingPoint::colorScheme(QPainter * p)
{
	p->setPen(QPen(red, 2));
	p->setBrush(red);
}

