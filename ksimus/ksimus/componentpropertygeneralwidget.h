/***************************************************************************
                          componentpropertygeneralwidget.h  -  description
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

#ifndef COMPONENTPROPERTYGENERALWIDGET_H
#define COMPONENTPROPERTYGENERALWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentpropertybasewidget.h"

// Forward declaration
class QGrid;
class QLineEdit;

/**General properties
  *@author Rasmus Diekenbrock
  */


class ComponentPropertyGeneralWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	ComponentPropertyGeneralWidget(Component * comp, QWidget *parent=0, const char *name=0);
	~ComponentPropertyGeneralWidget();


public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void slotDefault();

	/** Returns the grid which is used as layout for this dialog widget. The gris has two columns. Add
	  * The additional items to this layout.
	  */
	QGrid * getGrid() const { return m_grid; };

protected:
	QGrid * m_grid;
	QLineEdit * m_nameEdit;

};

#endif
