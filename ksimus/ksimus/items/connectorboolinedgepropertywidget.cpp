/***************************************************************************
                          connectorboolinedgepropertywidget.cpp  -  description
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
#include <qcheckbox.h>
#include <qtooltip.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "connectorboolinedgepropertywidget.h"
#include "connectorboolinedge.h"

// Forward declaration



ConnectorBoolInEdgePropertyWidget::ConnectorBoolInEdgePropertyWidget(ConnectorBoolInEdge * connector, QWidget *parent, const char *name )
	:	ConnectorBoolInPropertyWidget(connector, parent,name)
{
	if (getConn()->isEdgeSensitiveChangeEnable())
	{
		m_edgeSensitive = new QCheckBox(i18n("&Edge sensitive"),settingWidget(),"EdgeCheck");
		m_edgeSensitive->setChecked(getConn()->isEdgeSensitive());
		QToolTip::add(m_edgeSensitive,i18n("Toogles the sensitive (edge/level) of the connector"));
	}
	else
	{
		m_edgeSensitive = 0;
	}
}

/*ConnectorBoolInEdgePropertyWidget::~ConnectorBoolInEdgePropertyWidget()
{
} */

ConnectorBoolInEdge * ConnectorBoolInEdgePropertyWidget::getConn() const
{
	return (ConnectorBoolInEdge *)m_conn;
}
ConnectorBoolInEdge * ConnectorBoolInEdgePropertyWidget::getConn()
{
	return (ConnectorBoolInEdge *)m_conn;
}

void ConnectorBoolInEdgePropertyWidget::acceptPressed()
{
	ConnectorBoolInPropertyWidget::acceptPressed();
	
	if (m_edgeSensitive)
	{
		if (getConn()->isEdgeSensitive() != m_edgeSensitive->isChecked())
		{
			changeData();
			getConn()->setEdgeSensitive(m_edgeSensitive->isChecked());
		}
	}
}

void ConnectorBoolInEdgePropertyWidget::defaultPressed()
{
	ConnectorBoolInPropertyWidget::defaultPressed();
	
	if (m_edgeSensitive)
	{
		m_edgeSensitive->setChecked(getConn()->isInitEdgeSensitive());
	}
}


