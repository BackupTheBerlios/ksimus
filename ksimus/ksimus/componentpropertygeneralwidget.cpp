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
	
	m_grid = new QGrid(2, /*QGrid::Horizontal,*/ this);
	m_grid->setMargin(KDialog::marginHint());
	m_grid->setSpacing(KDialog::spacingHint());
	
	new QLabel(i18n("Name: "), m_grid,"LineEditLabel");
	
	m_nameEdit = new QLineEdit(m_grid,"LineEdit");
	m_nameEdit->setText(getComponent()->getName());
	QToolTip::add(m_nameEdit, i18n("Change the name of the component here"));

	
	// Set main layout
	layout = new QGridLayout(this,2,1);
	layout->addWidget(m_grid,0,0);
	layout->setRowStretch(1,1);
	

}

ComponentPropertyGeneralWidget::~ComponentPropertyGeneralWidget()
{
}

void ComponentPropertyGeneralWidget::slotAccept()
{
	ComponentPropertyBaseWidget::slotAccept();

	if (getComponent()->getName() != m_nameEdit->text())
	{
		changeData();
		getComponent()->setName(m_nameEdit->text());
	}

}

void ComponentPropertyGeneralWidget::slotDefault()
{
	ComponentPropertyBaseWidget::slotDefault();

	m_nameEdit->setText(getComponent()->getInfo()->getName());
	
}
