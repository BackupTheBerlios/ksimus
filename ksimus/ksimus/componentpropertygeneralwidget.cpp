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
#include <qgrid.h>
#include <qlayout.h>

// KDE-Includes
#include <kdialog.h>
#include <klocale.h>

// Project-Includes
#include "componentpropertygeneralwidget.h"
#include "component.h"
#include "componentinfo.h"
#include "ksimdebug.h"

// Forward declaration


ComponentPropertyGeneralWidget::ComponentPropertyGeneralWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
	QGridLayout * layout;
	QString str;
	QLabel * label;
	
	m_grid = new QGrid(2, /*QGrid::Horizontal,*/ this);
	CHECK_PTR(m_grid);
	m_grid->setMargin(KDialog::marginHint());
	m_grid->setSpacing(KDialog::spacingHint());
	
	
	// Name edit	
	str = i18n("Change the name of the component here");
	label = new QLabel(i18n("Name:"), m_grid,"LineEditLabel");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
	m_nameEdit = new QLineEdit(m_grid,"LineEdit");
	CHECK_PTR(m_nameEdit);
	m_nameEdit->setText(getComponent()->getName());
	QToolTip::add(m_nameEdit, str);

	// Component type
	str = i18n("Shows the component type.");
	
	label = new QLabel(i18n("Type:"), m_grid);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
	label = new QLabel(i18n(getComponent()->getInfo()->getName().latin1()), m_grid);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	
	
	// Set main layout
	layout = new QGridLayout(this,2,1);
	layout->addWidget(m_grid,0,0);
	layout->setRowStretch(1,1);
	

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

	m_nameEdit->setText(getComponent()->getInfo()->getName());
	
}
