/***************************************************************************
                          componentlistviewitem.cpp  -  description
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

// C-Includes

// QT-Includes
#include <qpalette.h>

// KDE-Includes

// Project-Includes
#include "componentlistviewitem.h"
#include "componentlistview.h"
#include "componentinfo.h"
#include "types.h"
#include "ksimdebug.h"

// Forward declaration

ComponentListViewItem::ComponentListViewItem(ComponentListViewItem * clvi, const QString & name, const ComponentInfo * ci)
	:	QListViewItem(clvi, name),
		m_compInfo(ci)
{
}

ComponentListViewItem::ComponentListViewItem(ComponentListView * clv, const QString & name, const ComponentInfo * ci)
	:	QListViewItem(clv, name),
		m_compInfo(ci)
{
}

ComponentListViewItem::~ComponentListViewItem()
{
}

const ComponentInfo * ComponentListViewItem::getComponentInfo() const
{
	return m_compInfo;
}

bool ComponentListViewItem::isEntry() const
{
	return m_compInfo;
}

void ComponentListViewItem::paintCell(QPainter *p ,const QColorGroup & cg, int column, int width, int alignment)
{
	QColorGroup colorGroup(cg);

	if ((m_compInfo) && (!isCorrectView()))
	{
		colorGroup.setColor(QColorGroup::Text, colorGroup.mid());
	}

	QListViewItem::paintCell(p, colorGroup, column, width, alignment);
}

bool ComponentListViewItem::isCorrectView() const
{
	ComponentListView * clv = (ComponentListView*)listView();
	bool res = false;

	if (m_compInfo)
	{
		if ((clv->getCurrentView() == APP_SHEET_VIEW) && (m_compInfo->getViewAttr() & VA_SHEETVIEW))
		{
			res = true;
		}
		else if ((clv->getCurrentView() == APP_USER_VIEW) && (m_compInfo->getViewAttr() & VA_USERVIEW))
		{
			res = true;
		}
	}
	return res;
}
	
ComponentListViewItem * ComponentListViewItem::findChild(const QString & childName)
{
	QListViewItem * child = firstChild();
	
	while (child)
	{
		if (child->text(0) == childName)
			return (ComponentListViewItem *) child;
		child = child->nextSibling();
	}
	return (ComponentListViewItem *)0;
}

