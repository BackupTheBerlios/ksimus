/***************************************************************************
                          watchviewitem.cpp  -  description
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
#include <qpixmap.h>

// KDE-Includes

// Project-Includes
#include "watchviewitem.h"
#include "watchlistview.h"
#include "watchitembase.h"
#include "ksimdebug.h"

// Forward declaration


#define BREAK_COL 0
#define NAME_COL  1
#define VALUE_COL 2



WatchViewItem::WatchViewItem(WatchItemBase * watch, WatchListView * listview)
	: QListViewItem(listview),
		m_watch(watch)
{
	setName(getWatchItem()->getWatchName());
//	setValueText(getWatchItem()->)
}

WatchViewItem::~WatchViewItem()
{
	KSIMDEBUG_VAR("WatchViewItem::~WatchViewItem()", text(NAME_COL));
}

void WatchViewItem::setValueText(const QString & value)
{
	setText(VALUE_COL, value);
}

void WatchViewItem::setName(const QString & name)
{
	setText(NAME_COL, name);
}

void WatchViewItem::setBreakEvents(int noOfEvents)
{
	if (noOfEvents < 0)
	{
		setText(BREAK_COL, QString::null);
		setPixmap(BREAK_COL, QPixmap());
	}
	else if (noOfEvents == 0)
	{
		setText(BREAK_COL, QString::null);
		setPixmap(BREAK_COL, WatchListView::getPixmapBreaked());
	}
	else
	{
		setText(BREAK_COL, QString::number(noOfEvents));
		setPixmap(BREAK_COL, QPixmap());
	}
}
