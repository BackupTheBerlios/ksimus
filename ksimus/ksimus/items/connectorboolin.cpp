/***************************************************************************
                          connectorboolin.cpp  -  description
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
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "connectorboolin.h"
#include "connectorboolinpropertywidget.h"
#include "connectorinfo.h"
#include "component.h"
#include "ksimdebug.h"


static ConnectorBase * create(Component * comp, const QString & name, const QPoint & pos)
{
	return new ConnectorBoolIn(comp, name, pos);
}

const ConnectorInfo ConnectorBoolInInfo (	"Boolean Input",
											"booleanInput",
											"Boolean",
											create );
	


ConnectorBoolIn::ConnectorBoolIn(Component * comp, const char * name, const QPoint & pos)
	:	ConnectorInputBase(comp, name, pos, CO_LEFT, &ConnectorBoolInInfo)
{
	setNegateEnabled(true);
}

// Setup the colors, brushs, and fills for the connector
void ConnectorBoolIn::setupColorScheme (QPainter * p) const
{
	p->setPen(QPen(darkGreen, 2));
	p->setBrush(darkGreen);
}

/** Returns a pointer to the data that's read from the component
  * The default implementation calls the function getWireData()
  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
const void * ConnectorBoolIn::readoutData() const
{
	static bool out;
	bool * pBool = (bool*)getWireData();
	if (pBool)
	{
		out = *pBool ^ isNegated();
	}
	else
	{
		out = isNegated();
	}
	return &out;
}

/** Returns the input data */
bool ConnectorBoolIn::getInput() const
{
	return *(bool*)readoutData();
}

/** Creates the property widget */
QWidget* ConnectorBoolIn::propertyWidget(QWidget * parent)
{
	return new ConnectorBoolInPropertyWidget(this, parent, getName());
}

/** Add menu items depend on connetor properties */
bool ConnectorBoolIn::initPopupMenu(QPopupMenu * popup)
{
	
	ConnectorInputBase::initPopupMenu(popup);
	
	if (isNegateEnabled())
	{
		idNegate = popup->insertItem((const char*)i18n("&Negate connector"), this,SLOT(slotToggleNegType()));
		popup->setItemChecked(idNegate, isNegated());
	}

	return true;
}	

/** Display a help for the popup menu */
void ConnectorBoolIn::popupMenuHighlighted(int msg) const
{
	ConnectorInputBase::popupMenuHighlighted(msg);
	
	if (msg == idNegate)
	{
		getComponent()->statusHelpMsg(i18n("Negate the boolean connector"));
	}
}

/** Toggles the negated type */
void ConnectorBoolIn::slotToggleNegType()
{
	getComponent()->undoChangeProperty(i18n("Negate connector"));
	setNegate(!isNegated());
	getComponent()->setModified();
	getComponent()->refresh();
}

