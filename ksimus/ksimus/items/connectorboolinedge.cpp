/***************************************************************************
                          connectorboolinedge.cpp  -  description
                             -------------------
    begin                : Fri Nov 23 2001
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
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "connectorboolinedgepropertywidget.h"
#include "connectorboolinedge.h"
#include "component.h"
#include "connectorlabel.h"
#include "connectorinfo.h"

// Forward declaration


#define INIT_SENSITIVE        0x0001
#define CHANGE_SENSITIVE_ENA  0x0002


static ConnectorBase * create(Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolIn(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolInEdgeInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean Input Edge"),
	                                QString::fromLatin1("Boolean Input Edge"),
	                                QString::fromLatin1("Boolean"),
	                                create );
	return &Info;
}


int ConnectorBoolInEdge::s_idSensitive;
static const char * const sSensitive = "Edge sensitive";


ConnectorBoolInEdge::ConnectorBoolInEdge(Component * comp, const QString & name,
                                         const QString & i18nName, const QPoint & pos)
	:	ConnectorBoolIn(comp, name, i18nName, pos, CO_LEFT, getConnectorBoolInEdgeInfo()),
		m_edgeSensitive(true),
		m_flags(INIT_SENSITIVE | CHANGE_SENSITIVE_ENA)
{
}

ConnectorBoolInEdge::ConnectorBoolInEdge(Component * comp, const QString & name, const QString & i18nName,
						                             const QString & descr, const QPoint & pos)
	:	ConnectorBoolIn(comp, name, i18nName, pos, CO_LEFT, getConnectorBoolInEdgeInfo()),
		m_edgeSensitive(true),
		m_flags(INIT_SENSITIVE | CHANGE_SENSITIVE_ENA)
{
	new ConnectorLabel(this, descr);
}
						
ConnectorBoolInEdge::ConnectorBoolInEdge( Component * comp, const QString & name, const QString & i18nName,
                                          const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBoolIn(comp, name, i18nName, pos, orient, ci),
		m_edgeSensitive(true),
		m_flags(INIT_SENSITIVE | CHANGE_SENSITIVE_ENA)
{
}
						
/** True sets the connector edge sensitive. False sets the connector level sensitive.
 */
void ConnectorBoolInEdge::setEdgeSensitive(bool edge, bool init)
{
	m_edgeSensitive = edge;
	if (init)
	{	
		if(edge)
		{
			m_flags |= INIT_SENSITIVE;
		}
		else
		{
			m_flags &= ~INIT_SENSITIVE;
		}
	}
}
/** Returns true if the connector is edge sensitive. False, if the connector is level sensitive.
 */
bool ConnectorBoolInEdge::isEdgeSensitive() const
{
	return m_edgeSensitive;
}
	
/** Returns true if the init sensitive is edge sensitive. False, if the init sensitive is level sensitive.
 */
bool ConnectorBoolInEdge::isInitEdgeSensitive() const
{
	return (m_flags & INIT_SENSITIVE);
}

/** True enables the user to change the sensitive type. */
void ConnectorBoolInEdge::setEdgeSensitiveChangeEnable(bool enable)
{
	if(enable)
	{
		m_flags |= CHANGE_SENSITIVE_ENA;
	}
	else
	{
		m_flags &= ~CHANGE_SENSITIVE_ENA;
	}
}
/** Returns true if the user is able to change the sensitive type. */
bool ConnectorBoolInEdge::isEdgeSensitiveChangeEnable() const
{
	return (m_flags & CHANGE_SENSITIVE_ENA);
}

/** Returns the input data */
bool ConnectorBoolInEdge::getInput()
{
	bool res;
	
	if(isEdgeSensitive())
	{
		bool current = ConnectorBoolIn::getInput();
		// rising edge ?
		res = (!m_last && current);
		m_last = current;
		// Execute the next cycle because the input is than low !!!
		getComponent()->executeNext();
	}
	else
	{
		res  = ConnectorBoolIn::getInput();
	}

	return res;
}
		
/** Resets the connector.
  */
void ConnectorBoolInEdge::reset()
{
	ConnectorBoolIn::reset();
	
	m_last = isNegated();
}

/** Creates the property widget */
PropertyWidget* ConnectorBoolInEdge::propertyWidget(QWidget * parent)
{
	return new ConnectorBoolInEdgePropertyWidget(this, parent, getName());
}

/** Add menu items depend on connetor properties */
bool ConnectorBoolInEdge::initPopupMenu(QPopupMenu * popup)
{
	ConnectorBoolIn::initPopupMenu(popup);
	
	if (isEdgeSensitiveChangeEnable())
	{
		s_idSensitive = popup->insertItem(i18n("&Edge sensitive"), this,SLOT(slotToggleEdgeSensitive()));
		popup->setItemChecked(s_idSensitive, isEdgeSensitive());
	}
	else
	{
		s_idSensitive = 0;
	}

	return true;
}	

/** Display a status help message for popup menu entries, if highlighted */
void ConnectorBoolInEdge::popupMenuHighlighted(int msg) const
{
	ConnectorBoolIn::popupMenuHighlighted(msg);
	
	if (msg == s_idSensitive)
	{
		getComponent()->statusHelpMsg(i18n("Toogles the sensitive (edge/level) of the connector"));
	}
}

/** Toggles the sensitive type */
void ConnectorBoolInEdge::slotToggleEdgeSensitive()
{
	getComponent()->undoChangeProperty(i18n("Change Connector Properties"));
	setEdgeSensitive(!isEdgeSensitive());
	getComponent()->setModified();
}

/** Load properties
	Returns true if successful */
bool ConnectorBoolInEdge::load(KSimData & file)
{
	bool res = ConnectorBoolIn::load(file);
	
	setEdgeSensitive(file.readBoolEntry(sSensitive, isInitEdgeSensitive()));
	
	return res;
}
	
/** Save properties */
void ConnectorBoolInEdge::save(KSimData & file) const
{
	ConnectorBoolIn::save(file);
	
	// save only if different
	if (isInitEdgeSensitive() != isEdgeSensitive())
	{
		file.writeEntry(sSensitive, isEdgeSensitive());
	}
}

#include "connectorboolinedge.moc"
