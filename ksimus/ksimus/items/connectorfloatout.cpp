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

// Forward declaration


static ConnectorBase * create(Component * comp, const QString & name, const QPoint & pos)
{
	return new ConnectorFloatOut(comp, name, pos);
}

const ConnectorInfo ConnectorFloatOutInfo("Floating Point Output",
                                          "Floating Point Output",
                                          "Floating Point",
                                          create );

ConnectorFloatOut::ConnectorFloatOut(Component * comp, const QString & name, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, pos, CO_RIGHT, &ConnectorFloatOutInfo),
		m_data(false)
{
	init();
}
											
ConnectorFloatOut::ConnectorFloatOut(Component * comp, const QString & name, const QString & descr, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, pos, CO_RIGHT, &ConnectorFloatOutInfo),
		m_data(false)
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorFloatOut::ConnectorFloatOut( Component * comp, const QString & name, const QPoint & pos,
                                    ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorOutputBase(comp, name, pos, orient, ci)
{
	init();
}

void ConnectorFloatOut::init()
{
	setNegateEnabled(true);
}

// Setup the colors, brushs, and fills for the connector
void ConnectorFloatOut::setupColorScheme (QPainter * p) const
{
	WirePropertyFloatingPoint::colorScheme(p);
/*	p->setPen(QPen(red, 2));
	p->setBrush(red);*/
}
											
											
/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorFloatOut::copyData(const void * pData)
{
	m_data = *(double*)pData;
}

/** Set the current output */
void ConnectorFloatOut::setOutput(double out)
{
	m_data = out;
}


/** Returns a pointer to the data of this output connector */
const void * ConnectorFloatOut::getData() const
{
	return &m_data;
}											
																						
	

