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
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "connectorboolin.h"
#include "connectorboolinpropertywidget.h"
#include "connectorinfo.h"
#include "connectorlabel.h"
#include "component.h"
#include "ksimdebug.h"
#include "wirepropertyboolean.h"
#include "wire.h"
#include "watchitemboolean.h"


static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolIn(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolInInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean Input"),
	                                QString::fromLatin1("Boolean Input"),
	                                QString::fromLatin1("Boolean"),
	                                create );
	return &Info;
}


ConnectorBoolIn::ConnectorBoolIn(Component * comp, const QString & name,
                                 const QString & i18nName, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getConnectorBoolInInfo())
{
	init();
}

ConnectorBoolIn::ConnectorBoolIn(	Component * comp,	const QString & name, const QString & i18nName,
                                  const QString & descr, const QPoint & pos)
	:	ConnectorInputBase(comp, name, i18nName, pos, CO_LEFT, getConnectorBoolInInfo())
{
	init();
	new ConnectorLabel(this, descr);
}


ConnectorBoolIn::ConnectorBoolIn( Component * comp, const QString & name, const QString & i18nName,
                                  const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorInputBase(comp, name, i18nName, pos, orient, ci)
{
	init();
}


void ConnectorBoolIn::init()
{
	setNegateEnabled(true);
	m_data = false;
}

// Get the colors for the connector
const WireColorScheme & ConnectorBoolIn::getColorScheme() const
{
	static WireColorScheme colorScheme(darkGreen);
	
	return colorScheme;
}


/** Resets the connector
*/
void ConnectorBoolIn::reset()
{
	ConnectorBase::reset();
	
	m_data = false;
}

/** Returns a pointer to the data that's read from the component
  * Reimplementations is required if the connector has to modify ths data (e.g. a neg. boolean input */
const void * ConnectorBoolIn::readoutData() const
{
	return &m_data;
}
	
	/** Copies the data where the p points to into a local variable.
	  * The function must be implemented by a sub class. */
void ConnectorBoolIn::copyData(const void * p)
{
	bool tmp = *(const bool *)p;
	if (tmp != m_data)
	{
		m_data = tmp;
	}
}

/** Returns the input data */
bool ConnectorBoolIn::getInput() const
{
	return m_data ^ isNegated();
}

/** Creates the property widget */
PropertyWidget* ConnectorBoolIn::propertyWidget(QWidget * parent)
{
	return new ConnectorBoolInPropertyWidget(this, parent, getName());
}

/** Add menu items depend on connetor properties */
bool ConnectorBoolIn::initPopupMenu(QPopupMenu * popup)
{
	
	ConnectorInputBase::initPopupMenu(popup);
	
	if (isNegateEnabled())
	{
		idNegate = popup->insertItem(i18n("&Negate connector"), this,SLOT(slotToggleNegType()));
		popup->setItemChecked(idNegate, isNegated());
	}
	else
	{
		idNegate = 0;
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

/** Returns a text which represents the current value. */
QString ConnectorBoolIn::getValueText() const
{
	return WirePropertyBoolean::getI18nTextValue(getInput());
}

WatchItemBase * ConnectorBoolIn::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBooleanConnector(this);
	CHECK_PTR(wi);
	return wi;
}

