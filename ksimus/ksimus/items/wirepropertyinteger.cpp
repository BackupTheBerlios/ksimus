/***************************************************************************
                          wirepropertyinteger.cpp  -  description
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

// C-Includes

// QT-Includes
#include <qpainter.h>

// KDE-Includes

// Project-Includes
#include "wirepropertyinteger.h"
#include "connectorintegerout.h"
#include "ksimdebug.h"
#include "wire.h"
#include "loglist.h"
#include "wirepropertyinfo.h"
#include "watchiteminteger.h"

// Forward declaration


WireProperty * WirePropertyInteger::create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyInteger(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * WirePropertyInteger::getStaticInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Integer"),
	                                   QString::fromLatin1("wire/property/integer"),
	                                   QString::fromLatin1("Integer"),
	                                   create );
	return &Info;
}


WirePropertyInteger::WirePropertyInteger(Wire * wire)
	:	WirePropertySingleOutput(wire, getStaticInfo())
{
}

WirePropertyInteger::~WirePropertyInteger()
{
}
	
	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyInteger::getColorScheme() const
{
	return colorScheme();
}

const WireColorScheme & WirePropertyInteger::colorScheme()
{
	static WireColorScheme colorScheme(blue);
	
	return colorScheme;
}
                           
WatchItemBase * WirePropertyInteger::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemIntegerWireProperty(this);
	CHECK_PTR(wi);
	return wi;
}
