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
#include "watchitemfloatingpoint.h"

// Forward declaration


static WireProperty * create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyFloatingPoint(wire);
	Q_CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getPropertyFloatingPointInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Floating Point"),
	                                   QString::fromLatin1("wire/property/floating point"),
	                                   QString::fromLatin1("Floating Point"),
	                                   create );
	return &Info;
}


WirePropertyFloatingPoint::WirePropertyFloatingPoint(Wire * wire)
	:	WirePropertySingleOutput(wire, getPropertyFloatingPointInfo())
{
}

WirePropertyFloatingPoint::~WirePropertyFloatingPoint()
{
}
	
	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyFloatingPoint::getColorScheme() const
{
	return colorScheme();
}
											
const WireColorScheme & WirePropertyFloatingPoint::colorScheme()
{
	static WireColorScheme colorScheme(red);
	
	return colorScheme;
}
                           
WatchItemBase * WirePropertyFloatingPoint::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemFloatingPointWireProperty(this);
	Q_CHECK_PTR(wi);
	return wi;
}

#include "wirepropertyfloatingpoint.moc"

