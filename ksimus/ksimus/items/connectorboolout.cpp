/***************************************************************************
                          connectorboolout.cpp  -  description
                             -------------------
    begin                : Sun Oct 1 2000
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

// QT includes
#include <qpainter.h>
#include <qdict.h>
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "ksimdata.h"
#include "connectorinfo.h"
#include "connectorboolout.h"
#include "connectorbooloutpropertywidget.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "component.h"
#include "enumdict.h"

// Forward declaration
class ConnectorBoolOutWidget;


static ConnectorBase * create(Component * comp, const QString & name, const QPoint & pos)
{
	return new ConnectorBoolOut(comp, name, pos);
}

const ConnectorInfo ConnectorBoolOutInfo (	"Boolean Output",
											"booleanOutput",
											"Boolean",
											create );

ConnectorBoolOut::ConnectorBoolOut(Component * comp, const char * name, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, pos, CO_RIGHT, &ConnectorBoolOutInfo),
		m_data(false)
{
	init();
}
											
ConnectorBoolOut::ConnectorBoolOut(Component * comp, const char * name, const QString & descr, const QPoint & pos)
	:	ConnectorOutputBase(comp, name, pos, CO_RIGHT, &ConnectorBoolOutInfo),
		m_data(false)
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolOut::ConnectorBoolOut( Component * comp, const char * name, const QPoint & pos,
                                    ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorOutputBase(comp, name, pos, orient, ci)
{
	init();
}

void ConnectorBoolOut::init()
{
	setNegateEnabled(true);
}

// Setup the colors, brushs, and fills for the connector
void ConnectorBoolOut::setupColorScheme (QPainter * p) const
{
	p->setPen(QPen(darkGreen, 2));
	p->setBrush(darkGreen);
}
											
											
/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorBoolOut::copyData(const void * pData)
{
	m_data = *(bool*)pData ^ isNegated();
}

/** Set the current output */
void ConnectorBoolOut::setOutput(bool out)
{
	m_data = out ^ isNegated();
}


/** Returns a pointer to the data of this output connector */
const void * ConnectorBoolOut::getData() const
{
	return &m_data;
}											
																						
	

/** Creates the property widget */
QWidget* ConnectorBoolOut::propertyWidget(QWidget * parent)
{
	return new ConnectorBoolOutPropertyWidget(this, parent, getName());
}

/** Add menu items depend on connetor properties */
bool ConnectorBoolOut::initPopupMenu(QPopupMenu * popup)
{
	
	ConnectorOutputBase::initPopupMenu(popup);
	
	if (isNegateEnabled())
	{
		idNegate = popup->insertItem((const char*)i18n("&Negate connector"), this,SLOT(slotToggleNegType()));
		popup->setItemChecked(idNegate, isNegated());
	}
	
	return true;
}	

/** Display a help for the popup menu */
void ConnectorBoolOut::popupMenuHighlighted(int msg) const
{
	ConnectorOutputBase::popupMenuHighlighted(msg);
	
	if (msg == idNegate)
	{
		getComponent()->statusHelpMsg(i18n("Negate the boolean connector"));
	}
}


/** Toggles the negated type */
void ConnectorBoolOut::slotToggleNegType()
{
	getComponent()->undoChangeProperty(i18n("Negate connector"));
	setNegate(!isNegated());
	getComponent()->setModified();
	getComponent()->refresh();
}

