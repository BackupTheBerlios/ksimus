/***************************************************************************
                          watchviewitem.h  -  description
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

#ifndef WATCHVIEWITEM_H
#define WATCHVIEWITEM_H

// C-Includes

// QT-Includes
#include <qlistview.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class WatchItemBase;
class WatchListView;

/**The list view items for the watches.
  *@author Rasmus Diekenbrock
  */

class WatchViewItem : public QListViewItem
{
public: 
	WatchViewItem(WatchItemBase * watch, WatchListView * listview);
	virtual ~WatchViewItem();
	
	void setValueText(const QString & value);
	void setName(const QString & value);
	void setBreakEvents(int noOfEvents);
	
	WatchItemBase * getWatchItem() const { return m_watch; };
	
private:
	WatchItemBase * m_watch;
};

#endif
