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

#include <qpainter.h>

#include <klocale.h>

#include "wirepropertyboolean.h"
#include "wirepropertyinfo.h"
#include "connectorboolout.h"
#include "connectorlist.h"

#include "wire.h"
#include "loglist.h"

static WireProperty * create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyBoolean(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo wirePropertyBooleanInfo(	"Wire Property Boolean",
												"wire/property/boolean",
												"Boolean",
												create
											   );
												


WirePropertyBoolean::WirePropertyBoolean(Wire * wire)
	:	WirePropertySingleOutput(wire, &wirePropertyBooleanInfo)
{
}

WirePropertyBoolean::~WirePropertyBoolean()
{
}
	
/** Returns a pointer to the current data */	
const void * WirePropertyBoolean::getCurrentData() const
{
	ConnectorBase * conn = getWire()->getConnList()->at(0);
	
	if (conn)
	{
		return ((ConnectorBoolOut*)conn)->getData();
	}
	else
	{
		getWire()->getLogList()->logDebug(QString("Wire %1 returns <NULL> connector").arg(getWire()->getName()));
		return 0;
	}
}

// Setup the colors, brushs, and fills for the connector
void WirePropertyBoolean::setupColorScheme (QPainter * p) const
{
	p->setPen(QPen(darkGreen, 2));
	p->setBrush(darkGreen);
}
											
