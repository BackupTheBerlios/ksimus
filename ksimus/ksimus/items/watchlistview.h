/***************************************************************************
                          watchlistview.h  -  description
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

#ifndef WATCHLISTVIEW_H
#define WATCHLISTVIEW_H

// C-Includes

// QT-Includes
#include <qlistview.h>


// KDE-Includes

// Project-Includes

// Forward declaration

/**The view for the watch list.
  *@author Rasmus Diekenbrock
  */

class WatchListView : public QListView
{
   Q_OBJECT
public: 
	WatchListView(QWidget *parent=0, const char *name=0);
	~WatchListView();
	
	/** Counts the selected items. */
	unsigned int countSelected() const;

	/** Returns a pixmap which pictures a watch thats breaks the simulation. */
	static QPixmap getPixmapBreaked();

private slots:
	void slotRightButtonPressed(QListViewItem * lvi, const QPoint & pos, int cloumn);
	void slotCurrentChanged(QListViewItem * lvi);

private:
	void deleteSelected() const;
	void deleteAll() const;
	void enableBreakSelected(bool ena) const;
	void enableBreakAll(bool ena) const;
};

#endif
