/***************************************************************************
                          componentlistview.h  -  description
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

#ifndef COMPONENTLISTVIEW_H
#define COMPONENTLISTVIEW_H

// C-Includes

// QT-Includes
#include <qlistview.h>

// KDE-Includes

// Project-Includes
#include "types.h"

// Forward declaration




class ComponentDirectorySubMenu;
class ComponentListViewItem;
class ComponentLibraryItem;
class ComponentInfo;

/**A class for component selection like QListView

  *@author Rasmus Diekenbrock
  */

class ComponentListView : public QListView
{
   Q_OBJECT
public: 
	ComponentListView(QWidget *parent=0, const char *name=0);
	~ComponentListView();

	eAppViewType getCurrentView() const;

protected:
	void setupTreeView(ComponentDirectorySubMenu * dir, ComponentListViewItem * clvi);
	void insert(const QString & libName, const ComponentInfo * ci, bool open);
	void insert(const ComponentLibraryItem * cli, bool open);
	ComponentListViewItem * findChild(const QString & childName);
	
	ComponentListViewItem * m_leastSelected;


public slots:
	void slotUnselect();
	void slotSetCurrentView(eAppViewType currentView);
	void slotInsert(const ComponentLibraryItem *);
	void slotFoldTree();
	void slotUnfoldTree();

private slots:
	void slotSelection(QListViewItem * lvi);
	void slotHideMe();
	void slotRightButtonPressed(QListViewItem * item, const QPoint & p, int c);
	
private:
	eAppViewType m_currentView;
	
signals:
	void signalSelection(const ComponentInfo * ci);
	void signalHideMe();
	
};

#endif
