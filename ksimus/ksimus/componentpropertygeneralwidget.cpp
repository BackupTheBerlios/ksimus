/***************************************************************************
                          componentpropertygeneralwidget.cpp  -  description
                             -------------------
    begin                : Sat Feb 10 2001
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
#include <qlabel.h>
#include <qlineedit.h>
#include <qtooltip.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "componentpropertygeneralwidget.h"
#include "component.h"
#include "componentinfo.h"
#include "ksimdebug.h"

// Forward declaration


ComponentPropertyGeneralWidget::ComponentPropertyGeneralWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 2, parent, name)
{
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QString str;
	QLabel * label;
	
	// Name edit	
	str = i18n("Change the name of the component here");
	label = new QLabel(i18n("Name:"), this,"LineEditLabel");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
	m_nameEdit = new QLineEdit(this,"LineEdit");
	CHECK_PTR(m_nameEdit);
	m_nameEdit->setText(getComponent()->getName());
	QToolTip::add(m_nameEdit, str);

	// Component type
	str = i18n("Shows the component type.");
	
	label = new QLabel(i18n("Type:"), this);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
	label = new QLabel(getComponent()->getInfo()->getName(), this);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
}

ComponentPropertyGeneralWidget::~ComponentPropertyGeneralWidget()
{
}

void ComponentPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	if (getComponent()->getName() != m_nameEdit->text())
	{
		changeData();
		getComponent()->setName(m_nameEdit->text());
	}

}

void ComponentPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	m_nameEdit->setText(getComponent()->getDefaultName());
	
}
