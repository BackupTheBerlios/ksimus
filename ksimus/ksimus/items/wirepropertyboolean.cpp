/***************************************************************************
                          wirepropertyboolean.cpp  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#include <klocale.h>

#include "wirepropertyboolean.h"
#include "wirepropertyinfo.h"
#include "watchitemboolean.h"

#include "wire.h"

static WireProperty * create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyBoolean(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getWirePropertyBooleanInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Boolean"),
	                                   QString::fromLatin1("wire/property/boolean"),
	                                   QString::fromLatin1("Boolean"),
	                                   create );
	return &Info;
}


WirePropertyBoolean::WirePropertyBoolean(Wire * wire)
	:	WirePropertySingleOutput(wire, getWirePropertyBooleanInfo())
{
}

WirePropertyBoolean::~WirePropertyBoolean()
{
}
	
	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyBoolean::getColorScheme() const
{
	static WireColorScheme colorScheme(darkGreen);
	
	return colorScheme;
}

const QString & WirePropertyBoolean::getI18nTextValue(bool value)
{
	return value ? getI18nTextTrue() : getI18nTextFalse();
}
const QString & WirePropertyBoolean::getI18nTextTrue()
{
	static QString text(i18n("Text for boolean data", "True"));
	return text;
}
const QString & WirePropertyBoolean::getI18nTextFalse()
{
	static QString text(i18n("Text for boolean data", "False"));
	return text;
}

WatchItemBase * WirePropertyBoolean::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBooleanWireProperty(this);
	CHECK_PTR(wi);
	return wi;
}

#include "wirepropertyboolean.moc"
