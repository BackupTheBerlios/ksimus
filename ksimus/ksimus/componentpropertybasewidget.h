/***************************************************************************
                          componentpropertybasewidget.h  -  description
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

#ifndef COMPONENTPROPERTYBASEWIDGET_H
#define COMPONENTPROPERTYBASEWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"
#include "componentitem.h"

// Forward declaration


/**Base widget for component properties
  *@author Rasmus Diekenbrock
  */

class ComponentPropertyBaseWidget : public PropertyWidget, public ComponentItem
{
	Q_OBJECT

public:
	ComponentPropertyBaseWidget(Component * comp, int n, QWidget *parent=0, const char *name=0, WFlags f=0);
	~ComponentPropertyBaseWidget();
	
private:	
	
};

#endif
