/***************************************************************************
                          watchlistview.cpp  -  description
                             -------------------
    begin                : Tue Jul 16 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "watchlistview.h"
#include "watchitembase.h"
#include "watchviewitem.h"
#include "ksimdebug.h"
#include "watchbreak_xpm.xpm"
// Forward declaration


static int idDelete;
static int idDeleteAll;
static int idEnableBreak;
static int idEnableBreakSelected;
static int idDisableBreakSelected;
static int idDisableBreakAll;
static int idPropertyDialog;

WatchListView::WatchListView(QWidget *parent, const char *name )
	:	QListView(parent,name)
{
	addColumn(QIconSet(getPixmapBreaked()),QString::null);
	setColumnAlignment(0, AlignCenter);
	addColumn(i18n("Watch List","Name"));
	addColumn(i18n("Watch List","Value"));
	setAllColumnsShowFocus(true);
	setSelectionMode(Extended);
	setSorting(1);
	setShowSortIndicator(true);
	
	connect (this, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
	         this, SLOT(slotRightButtonPressed(QListViewItem *, const QPoint &, int)));

}

WatchListView::~WatchListView()
{
	KSIMDEBUG("WatchListView::~WatchListView()");
}

unsigned int WatchListView::countSelected() const
{
	unsigned int i = 0;
	QListViewItem * child = firstChild();
	while (child)
	{
		if (child->isSelected()) i++;
		child = child->nextSibling();
	}
//	KSIMDEBUG_VAR("countSelected()",i);
	return i;
}

void WatchListView::slotRightButtonPressed(QListViewItem * lvi, const QPoint & pos, int /*cloumn*/)
{
	if (countSelected() > 1)
	{
//		KSIMDEBUG("Multi Selection");
		QPopupMenu popup;
		
		idEnableBreakSelected = popup.insertItem(i18n("watchpoint break enable", "Enable selected break conditions"));
		idDisableBreakSelected = popup.insertItem(i18n("watchpoint break enable", "Disable selected break conditions"));
		idDelete = popup.insertItem(i18n("watchpoint delete", "Delete selected watchpoints"));
		popup.insertSeparator();
		idDisableBreakAll = popup.insertItem(i18n("watchpoint break enable", "Disable all break conditions"));
		idDeleteAll = popup.insertItem(i18n("watchpoint delete", "Delete all watchpoints"));
		
		
		int id = popup.exec(pos);
		
		if (id == idDelete)
		{
			deleteSelected();
		}
		else if (id == idDeleteAll)
		{
			deleteAll();
		}
		else if (id == idEnableBreakSelected)
		{
			enableBreakSelected(true);
		}
		else if (id == idDisableBreakSelected)
		{
			enableBreakSelected(false);
		}
		else if (id == idDisableBreakAll)
		{
			enableBreakAll(false);
		}
	}
	else if (lvi)
	{
//		KSIMDEBUG("Direct Selection");
		QPopupMenu popup;
		WatchViewItem * wvi = (WatchViewItem *)lvi;
		WatchItemBase * wib = wvi->getWatchItem();
		
		if (wib->isBreakEnabled())
		{
			idEnableBreak = popup.insertItem(i18n("watchpoint break enable", "Disable break condition"));
		}
		else
		{
			idEnableBreak = popup.insertItem(i18n("watchpoint break enable", "Enable break condition"));
		}
		idDelete = popup.insertItem(i18n("watchpoint delete", "Delete selected watchpoint"));
		popup.insertSeparator();
		idPropertyDialog = popup.insertItem(i18n("watchpoint property", "Watchpoint settings"));
		popup.insertSeparator();
		idDisableBreakAll = popup.insertItem(i18n("watchpoint break enable", "Disable all break conditions"));
		idDeleteAll = popup.insertItem(i18n("watchpoint delete", "Delete all watchpoints"));
		
		int id = popup.exec(pos);
		
		if (id == idDelete)
		{
			delete wib;
		}
		else if (id == idDeleteAll)
		{
			deleteAll();
		}
		else if (id == idEnableBreak)
		{
			wib->setBreakEnabled(!wib->isBreakEnabled());
		}
		else if (id == idPropertyDialog)
		{
			wib->executePropertyDialog(this);
		}
		else if (id == idDisableBreakAll)
		{
			enableBreakAll(false);
		}
	}
}

void WatchListView::deleteSelected() const
{
	QListViewItem * child = firstChild();
	while (child)
	{
		if (child->isSelected())
		{
			WatchViewItem * wvi = (WatchViewItem *)child;
			child = child->nextSibling();
			delete wvi->getWatchItem();
		}
		else
		{
			child = child->nextSibling();
		}
	}
}

void WatchListView::deleteAll() const
{
	QListViewItem * child = firstChild();
	while (child)
	{
		WatchViewItem * wvi = (WatchViewItem *)child;
		child = child->nextSibling();
		delete wvi->getWatchItem();
	}
}


void WatchListView::enableBreakSelected(bool ena) const
{
	QListViewItem * child = firstChild();
	while (child)
	{
		if (child->isSelected())
		{
			WatchViewItem * wvi = (WatchViewItem *)child;
			wvi->getWatchItem()->setBreakEnabled(ena);
		}
		child = child->nextSibling();
	}
}

void WatchListView::enableBreakAll(bool ena) const
{
	QListViewItem * child = firstChild();
	while (child)
	{
		WatchViewItem * wvi = (WatchViewItem *)child;
		wvi->getWatchItem()->setBreakEnabled(ena);
		child = child->nextSibling();
	}
}


void WatchListView::slotCurrentChanged(QListViewItem * lvi)
{
	if (lvi)
	{
		
	}
}

QPixmap WatchListView::getPixmapBreaked()
{
	static QPixmap breakPix((const char**)watchbreak_xpm);
	return breakPix;
}

