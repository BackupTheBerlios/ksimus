/***************************************************************************
                          componentlistviewitem.h  -  description
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

#ifndef COMPONENTLISTVIEWITEM_H
#define COMPONENTLISTVIEWITEM_H

#include <qlistview.h>

class ComponentListView;
class ComponentInfo;

/**
  *@author Rasmus Diekenbrock
  */

class ComponentListViewItem : public QListViewItem
{
class Private;


public:
	ComponentListViewItem(ComponentListViewItem * clvi, const QString & name, const ComponentInfo * ci = 0);
	ComponentListViewItem(ComponentListView * clv, const QString & name, const ComponentInfo * ci = 0);
	~ComponentListViewItem();
	
	const ComponentInfo * getComponentInfo() const;
	bool isEntry() const;
	
	virtual void paintCell(QPainter *p ,const QColorGroup & cg, int column, int width, int alignment);
	bool isCorrectView() const;

	/** Find child with the name childName. Returns a pointer to the child, or a null pointer if no child is found */
	ComponentListViewItem * findChild(const QString & childname);

protected:
	const ComponentInfo * m_compInfo;

private:
	Private * m_p;

};

#endif
