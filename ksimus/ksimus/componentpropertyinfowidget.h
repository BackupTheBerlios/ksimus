/***************************************************************************
                          componentpropertyinfowidget.h  -  description
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

#ifndef COMPONENTPROPERTYINFOWIDGET_H
#define COMPONENTPROPERTYINFOWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include <componentpropertybasewidget.h>

// Forward declaration
class QGrid;


/**Some information about the component
  *@author Rasmus Diekenbrock
  */

class ComponentPropertyInfoWidget : public ComponentPropertyBaseWidget
{
   Q_OBJECT
public: 
	ComponentPropertyInfoWidget(Component * comp, QWidget *parent=0, const char *name=0);
	~ComponentPropertyInfoWidget();

	/** Returns the grid which is used as layout for this dialog widget. The gris has two columns. Add
	  * The additional items to this layout.
	  */
	QGrid * getGrid() const { return m_grid; };

protected:
	QGrid * m_grid;

};

#endif
