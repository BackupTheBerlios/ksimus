/***************************************************************************
                          connectorbooloutpropertywidget.cpp  -  description
                             -------------------
    begin                : Sat Jan 13 2001
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
#include <qlayout.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qtooltip.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "connectorbooloutpropertywidget.h"
#include "connectorboolout.h"

// Forward declaration


ConnectorBoolOutPropertyWidget::ConnectorBoolOutPropertyWidget(ConnectorBoolOut * connector, QWidget *parent, const char *name)
	:	ConnectorBoolInPropertyWidget(connector, parent, name)
{
	QRadioButton * but;
	m_buttons = new QButtonGroup(3, Qt::Vertical, i18n("Reset Mode"), boxWidget(), "ResetModeButtons");
	
	
	// Default
	but = new QRadioButton(i18n("Default"), m_buttons, "ResetModeDefault");
	m_buttons->insert(but, ConnectorBoolOut::ResetDefault);
    QToolTip::add(but,i18n("Sets output false, if the connector is not negated, or true if connector is negated"));
	
	// Low
	but = new QRadioButton(i18n("False"), m_buttons, "ResetModeFalse");
	m_buttons->insert(but, ConnectorBoolOut::ResetFalse);
    QToolTip::add(but,i18n("Sets output false, if reset is executed"));
	
	// High
	but = new QRadioButton(i18n("True"), m_buttons, "ResetModeTrue");
	m_buttons->insert(but, ConnectorBoolOut::ResetTrue);
    QToolTip::add(but,i18n("Sets output true, if reset is executed"));

	connect(m_buttons, SIGNAL(clicked(int)), this, SLOT(slotButtonResult(int)));
	m_result = connector->getResetMode();
	m_buttons->setButton(connector->getResetMode());

}



ConnectorBoolOutPropertyWidget::~ConnectorBoolOutPropertyWidget()
{

}

void ConnectorBoolOutPropertyWidget::slotButtonResult(int id)
{
	m_result = id;
}

/** The function slotAccept() is called, if changes are accepted.
	You have to reimplement this slot, if you add new properties.
	If you do so, then first emit signalChangeData() and than changed data!
 */
void ConnectorBoolOutPropertyWidget::slotAccept()
{
	ConnectorBoolInPropertyWidget::slotAccept();

	if (m_result != ((ConnectorBoolOut*)m_conn)->getResetMode())
	{
		changeData();
		((ConnectorBoolOut*)m_conn)->setResetMode((ConnectorBoolOut::eResetMode)m_result);
	}
}

/** The function slotDefault() is called, if user wants to set the default values.
	You have to reimplement this slot, if you add new properties.
 */
void ConnectorBoolOutPropertyWidget::slotDefault()
{
	ConnectorBoolInPropertyWidget::slotDefault();

	m_result = ((ConnectorBoolOut*)m_conn)->getInitResetMode();
	m_buttons->setButton(((ConnectorBoolOut*)m_conn)->getInitResetMode());
}

