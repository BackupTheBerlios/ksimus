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
#include <ksimpleconfig.h>
#include <klocale.h>

// Project includes
#include "connectorinfo.h"
#include "connectorboolout.h"
#include "connectorbooloutpropertywidget.h"
#include "ksimdebug.h"
#include "component.h"
#include "enumdict.h"

// Forward declaration
class ConnectorBoolOutWidget;

/*static const char * sDefault   = "Default";
static const char * sTrue      = "True";
static const char * sFalse     = "False";*/
static const char * sResetMode = "Reset Mode";



EnumDict<ConnectorBoolOut::eResetMode>::tData EnumDict<ConnectorBoolOut::eResetMode>::data[]
			= { {"Reset", ConnectorBoolOut::ResetDefault},
          {"True",  ConnectorBoolOut::ResetTrue},
					{"False", ConnectorBoolOut::ResetFalse},
					{0,(ConnectorBoolOut::eResetMode)0}};

EnumDict<ConnectorBoolOut::eResetMode> resetDict;


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
		m_data(false),
		m_resetMode(ResetDefault),
		m_resetModeInit(ResetDefault)
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
																						
// ### Reset mode functions
void ConnectorBoolOut::setResetMode(eResetMode mode, bool init)
{
	bool changed(false);
	
	if (m_resetMode != mode)
	{
		m_resetMode = mode;
		changed = true;
	}
	if (init)
	{
		m_resetModeInit = mode;
	}
	if(changed)
	{
		emit signalProperty();
	}
}
/** Returns the reset mode */
ConnectorBoolOut::eResetMode ConnectorBoolOut::getResetMode() const
{
	return m_resetMode;
}
/** Returns the reset mode at init time */
ConnectorBoolOut::eResetMode ConnectorBoolOut::getInitResetMode() const
{
	return m_resetModeInit;
}
	
/** Resets the connector
*/
void ConnectorBoolOut::reset()
{
	switch (m_resetMode)
	{
		case ResetDefault:
			m_data = isNegated();
			break;
			
		case ResetTrue:
			m_data = true;
			break;
	
		case ResetFalse:
			m_data = false;
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown reset mode", (int)m_resetMode);
			break;
	}
}

/** Load properties
	Returns true if successful */
bool ConnectorBoolOut::load(KSimpleConfig & file)
{
	bool res;
	res = ConnectorOutputBase::load(file);
	
	setResetMode(resetDict.load(file, sResetMode, getInitResetMode()));
	
	return res;
}
	
/** Save properties */
void ConnectorBoolOut::save(KSimpleConfig & file) const
{
	ConnectorOutputBase::save(file);
	
	// save only if set
	if (getResetMode() != getInitResetMode())
	{
		resetDict.save(file, sResetMode, getResetMode());
	}
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

