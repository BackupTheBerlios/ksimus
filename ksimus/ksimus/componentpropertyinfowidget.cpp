/***************************************************************************
                          componentpropertyinfowidget.cpp  -  description
                             -------------------
    begin                : Sun Feb 11 2001
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
#include <qgrid.h>
#include <qlayout.h>

// KDE-Includes
#include <kdialog.h>
#include <klocale.h>

// Project-Includes
#include "componentpropertyinfowidget.h"
#include "component.h"
#include "compview.h"
#include "componentinfo.h"
#include "ksimdebug.h"
#include "packageinfo.h"

// Forward declaration


ComponentPropertyInfoWidget::ComponentPropertyInfoWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
	QGridLayout * layout;
	QString str;
	
	m_grid = new QGrid(2, /*QGrid::Horizontal,*/ this);
	m_grid->setMargin(KDialog::marginHint());
	m_grid->setSpacing(KDialog::spacingHint());
	
	
	
	new QLabel(i18n("Name:"), m_grid);
	new QLabel(comp->getInfo()->getName(), m_grid);
	
	new QLabel(i18n("Library Name:"), m_grid);
	new QLabel(comp->getInfo()->getLibName(), m_grid);
	
	const PackageInfo * package = comp->getPackageInfo();
	if(package)
	{
		// Package exists
		new QLabel(i18n("Package Name:"), m_grid);
		new QLabel(package->getPackageName(), m_grid);
		
		new QLabel(i18n("Package Version:"), m_grid);
		new QLabel(package->getPackageVersion(), m_grid);
	}		
	
	new QLabel(i18n("View Attribute:"), m_grid);
	switch(comp->getInfo()->getViewAttr())
	{
		case VA_SHEETVIEW:
			str = "sheet view";
			break;
		case VA_SHEETVIEW | VA_WINDOWVIEW:
			str = "sheet view, window view";
			break;
		case VA_USERVIEW:
			str = "user view";
			break;
		case VA_USERVIEW | VA_WINDOWVIEW:
			str = "user view, window view";
			break;
		case VA_SHEET_AND_USER:
			str = "sheet view, user view";
			break;
		case VA_SHEET_AND_USER | VA_WINDOWVIEW:
			str = "sheet view, user view, window view";
			break;
		case VA_SHEET_XOR_USER:
			str = "sheet view or user view";
			break;
		default:
			str.sprintf("unknown combination - %i",(int)comp->getInfo()->getViewAttr());
			KSIMDEBUG_VAR("unknown combination",(int)comp->getInfo()->getViewAttr());
			break;
	}
	new QLabel(str, m_grid);
	
	new QLabel(i18n("Serial Number:"), m_grid);
	str.setNum(comp->getSerialNumber());
	new QLabel(str, m_grid);
	

	if (comp->getSheetView())
	{
		new QLabel(i18n("Sheet Pos:"), m_grid);
		str = QString(i18n("X: %1 Y: %2"))	.arg(comp->getSheetView()->getPos().x())
											.arg(comp->getSheetView()->getPos().y());
		new QLabel(str, m_grid);

		new QLabel(i18n("Sheet Size:"), m_grid);
		str = QString(i18n("Width: %1 Height: %2"))	.arg(comp->getSheetView()->getPlace().width())
													.arg(comp->getSheetView()->getPlace().height());
		new QLabel(str, m_grid);
	}

	if (comp->getUserView())
	{
		new QLabel(i18n("User Pos:"), m_grid);
		str = QString(i18n("X: %1 Y: %2"))	.arg(comp->getUserView()->getPos().x())
											.arg(comp->getUserView()->getPos().y());
		new QLabel(str, m_grid);

		new QLabel(i18n("User Size:"), m_grid);
		str = QString(i18n("Width: %1 Height: %2"))	.arg(comp->getUserView()->getPlace().width())
													.arg(comp->getUserView()->getPlace().height());
		new QLabel(str, m_grid);
	}

	// Set main layout
	layout = new QGridLayout(this,2,2);
	layout->addWidget(m_grid,0,0);
	layout->setRowStretch(1,1);
	layout->setColStretch(1,1);
}

ComponentPropertyInfoWidget::~ComponentPropertyInfoWidget()
{
}

