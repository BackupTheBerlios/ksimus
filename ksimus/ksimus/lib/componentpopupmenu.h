/***************************************************************************
                          componentpopupmenu.h  -  description
                             -------------------
    begin                : Sat Sep 30 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef COMPONENTPOPUPMENU_H
#define COMPONENTPOPUPMENU_H

#include <qpopupmenu.h>

class ComponentDirectorySubMenu;
class ComponentInfo;

/**Popup menu with all components
  *@author Rasmus Diekenbrock
  */

class ComponentPopupMenu : public QPopupMenu
{
	Q_OBJECT
public: 
	ComponentPopupMenu(QWidget * parent = 0, const char * name = 0);
	~ComponentPopupMenu();

protected:
	ComponentPopupMenu(ComponentPopupMenu *first, const char * name = 0);
	void setupPopupMenu(const ComponentDirectorySubMenu * dir);
    ComponentPopupMenu * m_first;

protected slots:
	void slotSelected(int idx);

signals:
	void signalSelection(const QString & libName);
	void signalSelection(const ComponentInfo * ci);

};

#endif
