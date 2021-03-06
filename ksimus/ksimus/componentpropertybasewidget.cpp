/***************************************************************************
                          componentpropertybasewidget.cpp  -  description
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

// KDE-Includes

// Project-Includes
#include "componentpropertybasewidget.h"

// Forward declaration

ComponentPropertyBaseWidget::ComponentPropertyBaseWidget(Component * comp, int n, QWidget *parent, const char *name, WFlags f)
	:	PropertyWidget(n,parent,name,f),
		ComponentItem(comp)
{
	setCenterRowStretch(0);
	setBottomRowStretch(1);
}

ComponentPropertyBaseWidget::~ComponentPropertyBaseWidget()
{
}

#include "componentpropertybasewidget.moc"
