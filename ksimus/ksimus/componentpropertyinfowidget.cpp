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

// KDE-Includes
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
	:	ComponentPropertyBaseWidget(comp, 2, parent, name)
{
	//	setColStretch(0,0);
	setColStretch(1,1);
	
	QString str;
	
	new QLabel(i18n("Name:"), this);
	new QLabel(comp->getInfo()->getName(), this);
	
	new QLabel(i18n("Library Name:"), this);
	new QLabel(comp->getInfo()->getLibName(), this);
	
	const PackageInfo * package = comp->getPackageInfo();
	if(package)
	{
		// Package exists
		new QLabel(i18n("Package Name:"), this);
		new QLabel(package->getPackageName(), this);
		
		new QLabel(i18n("Package Version:"), this);
		new QLabel(QString::fromLatin1(package->getPackageVersion()), this);
	}		
	
	new QLabel(i18n("View Attribute:"), this);
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
	new QLabel(str, this);
	
	new QLabel(i18n("Serial Number:"), this);
	str.setNum(comp->getSerialNumber());
	new QLabel(str, this);
	

	if (comp->getSheetView())
	{
		new QLabel(i18n("Sheet Pos:"), this);
		str = QString(i18n("X: %1 Y: %2")).arg(comp->getSheetView()->getPos().x())
		                                  .arg(comp->getSheetView()->getPos().y());
		new QLabel(str, this);

		new QLabel(i18n("Sheet Size:"), this);
		str = QString(i18n("Width: %1 Height: %2")).arg(comp->getSheetView()->getPlace().width())
		                                           .arg(comp->getSheetView()->getPlace().height());
		new QLabel(str, this);
	}

	if (comp->getUserView())
	{
		new QLabel(i18n("User Pos:"), this);
		str = QString(i18n("X: %1 Y: %2")).arg(comp->getUserView()->getPos().x())
		                                  .arg(comp->getUserView()->getPos().y());
		new QLabel(str, this);

		new QLabel(i18n("User Size:"), this);
		str = QString(i18n("Width: %1 Height: %2")).arg(comp->getUserView()->getPlace().width())
		                                           .arg(comp->getUserView()->getPlace().height());
		new QLabel(str, this);
	}
}

ComponentPropertyInfoWidget::~ComponentPropertyInfoWidget()
{
}

#include "componentpropertyinfowidget.moc"
