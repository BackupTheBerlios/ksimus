/***************************************************************************
                          componentpropertydialog.h  -  description
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

#ifndef COMPONENTPROPERTYDIALOG_H
#define COMPONENTPROPERTYDIALOG_H

// C-Includes

// QT-Includes

// KDE-Includes
//#include <kdialogbase.h>

// Project-Includes
#include "componentitem.h"
#include "propertywidget.h"
#include "ksimdialog.h"

// Forward declaration
class Component;

/**Base dialog for component property
  *@author Rasmus Diekenbrock
  */

//class ComponentPropertyDialog : public KDialogBase, public ComponentItem
class ComponentPropertyDialog : public KSimDialog, public ComponentItem
{
// Some internal classes
class Private;

   Q_OBJECT
public: 
	ComponentPropertyDialog(Component *comp, const QString & caption, QWidget *parent=0, const char *name=0);
	~ComponentPropertyDialog();
	
	/** Executes a ComponentPropertyDialog.
	    Do some addtional stuff like size storing. */
	static int execute(Component *comp, const QString & caption, QWidget *parent=0, const char *name=0);

protected slots:
	virtual void slotDataChanged();
	virtual void slotOk();

private:
	Private * m_p;
};

#endif
