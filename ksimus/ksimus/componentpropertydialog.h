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
#include <kdialogbase.h>

// Project-Includes
#include "componentitem.h"

// Forward declaration
class Component;

/**Base dialog for component property
  *@author Rasmus Diekenbrock
  */

class ComponentPropertyDialog : public KDialogBase, public ComponentItem
{
   Q_OBJECT
public: 
	ComponentPropertyDialog(Component *comp, QString *caption=0, QWidget *parent=0, const char *name=0);
	~ComponentPropertyDialog();

	/** Connects dialog signals okClicked(), defaultClicked() and cancelClicked() to the widget slots
		slotAccept(), slotDefault() and slotCancel(). Connects the widget signal signalDataChanged() to
		the slot signalChangeData().
	*/
	void connectSlots(QWidget * wid);

protected slots:
	void slotDataChanged();
	void slotOk();

private:
	bool m_dataChanged;

};

#endif
