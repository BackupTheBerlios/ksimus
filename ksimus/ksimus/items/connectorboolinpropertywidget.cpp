/***************************************************************************
                          connectorboolinpropertywidget.cpp  -  description
                             -------------------
    begin                : Sat Jan 27 2001
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
#include "connectorboolinpropertywidget.h"
#include "connectorbase.h"

// Forward declaration



ConnectorBoolInPropertyWidget::ConnectorBoolInPropertyWidget(ConnectorBase * connector, QWidget *parent, const char *name)
	:	ConnectorPropertyWidget(connector, parent, name)
{
	if (m_conn->isNegateEnabled())
	{
		m_negate = new QCheckBox(i18n("Negate"),settingWidget(),"NegateCheck");
		m_negate->setChecked(m_conn->isNegated());
	    QToolTip::add(m_negate,i18n("Negate the connector"));
	
	}
	else
	{
		m_negate = 0;
	}
}

ConnectorBoolInPropertyWidget::~ConnectorBoolInPropertyWidget()
{
}

void ConnectorBoolInPropertyWidget::slotAccept()
{
	ConnectorPropertyWidget::slotAccept();
	
	if (m_negate)
	{
		if (m_conn->isNegated() != m_negate->isChecked())
		{
			changeData();
			m_conn->setNegate(m_negate->isChecked());
		}
	}
}

void ConnectorBoolInPropertyWidget::slotDefault()
{
	ConnectorPropertyWidget::slotDefault();
	
	if (m_negate)
	{
		m_negate->setChecked(m_conn->isInitNegate());
	}
}
