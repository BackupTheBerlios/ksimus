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


static ConnectorBase * create(Component * comp, const QString & name, const QPoint & pos)
{
	return new ConnectorFloatIn(comp, name, pos);
}

const ConnectorInfo ConnectorFloatInInfo("Floating Point Input",
                                         "Floating Point Input",
                                         "Floating Point",
                                         create );
	


ConnectorFloatIn::ConnectorFloatIn(Component * comp, const QString & name, const QPoint & pos)
	:	ConnectorInputBase(comp, name, pos, CO_LEFT, &ConnectorFloatInInfo)
{
	init();
}

ConnectorFloatIn::ConnectorFloatIn(	Component * comp,	const QString & name,
                                  const QString & descr, const QPoint & pos)
	:	ConnectorInputBase(comp, name, pos, CO_LEFT, &ConnectorFloatInInfo)
{
	init();
	new ConnectorLabel(this, descr);
}


ConnectorFloatIn::ConnectorFloatIn( Component * comp, const QString & name, const QPoint & pos,
                                  ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorInputBase(comp, name, pos, orient, ci)
{
	init();
}


void ConnectorFloatIn::init()
{
	setNegateEnabled(true);
}

// Setup the colors, brushs, and fills for the connector
void ConnectorFloatIn::setupColorScheme (QPainter * p) const
{
	WirePropertyFloatingPoint::colorScheme(p);
/*	p->setPen(QPen(red, 2));
	p->setBrush(red);*/
}

/** Returns the input data */
double ConnectorFloatIn::getInput() const
{
	double * pData = (double *)readoutData();
	
	return pData ? *(double*)pData : 0.0;
}

/** Returns a text which represents the current value. */
QString ConnectorFloatIn::getValueText() const
{
	return QString::number(getInput());
}


