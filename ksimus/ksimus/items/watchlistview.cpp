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
#include <kdialogbase.h>

// Project-Includes
#include "watchwidget.h"
#include "watchlistview.h"
#include "watchitembase.h"
#include "watchviewitem.h"
#include "ksimdebug.h"
#include "watchbreak_xpm.xpm"

// Forward declaration


WatchListView::WatchListView(WatchWidget * watchWidget, QWidget *parent, const char *name )
	:	QListView(parent,name),
		m_watchWidget(watchWidget)
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

	connect (this, SIGNAL(doubleClicked(QListViewItem *)),
	         this, SLOT(slotDoubleClicked(QListViewItem *)));
}

WatchListView::~WatchListView()
{
//	KSIMDEBUG("WatchListView::~WatchListView()");
}

int WatchListView::countSelected() const
{
	int i = 0;
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
	int idDelete = 0;
	int idDeleteSel = 0;
	int idDeleteAll = 0;
	int idEnableBreak = 0;
	int idEnableBreakSel = 0;
	int idDisableBreakSel = 0;
	int idDisableBreakAll = 0;
	int idPropertyDialog = 0;
	int idGeneralPropertyDialog = 0;

	int multiSelect = countSelected();
	WatchViewItem * wvi = lvi ? (WatchViewItem *)lvi : (WatchViewItem *)0;
	WatchItemBase * wib = wvi ? wvi->getWatchItem()  : (WatchItemBase *)0;
	
	QPopupMenu popup;


	if (multiSelect > 1)
	{
		idEnableBreakSel = popup.insertItem(i18n("watchpoint enable trigger", "Enable selected triggers"));
		idDisableBreakSel = popup.insertItem(i18n("watchpoint disable trigger", "Disable selected triggers"));
		idDeleteSel = popup.insertItem(i18n("watchpoint delete", "Delete selected watchpoints"));
		popup.insertSeparator();
	}
	else if (lvi)
	{
		if (wib->isBreakEnabled())
		{
			idEnableBreak = popup.insertItem(i18n("watchpoint disable trigger", "Disable trigger"));
		}
		else
		{
			idEnableBreak = popup.insertItem(i18n("watchpoint enable trigger", "Enable trigger"));
		}
		idDelete = popup.insertItem(i18n("watchpoint delete", "Delete selected watchpoint"));
		idPropertyDialog = popup.insertItem(i18n("watchpoint property", "Watchpoint settings"));
		popup.insertSeparator();
	}
	
	if ((childCount() != multiSelect) && !((childCount() == 1) && (wvi != 0)))
	{
		idDisableBreakAll = popup.insertItem(i18n("watchpoint disable trigger", "Disable all triggers"));
		idDeleteAll = popup.insertItem(i18n("watchpoint delete", "Delete all watchpoints"));
		popup.insertSeparator();
	}
	idGeneralPropertyDialog = popup.insertItem(i18n("watchpoint property", "General settings"));

	int id = popup.exec(pos);

	if (id == idDeleteSel)
	{
		deleteSelected();
	}
	else if (id == idDeleteAll)
	{
		deleteAll();
	}
	else if (id == idEnableBreakSel)
	{
		enableBreakSelected(true);
	}
	else if (id == idDisableBreakSel)
	{
		enableBreakSelected(false);
	}
	else if (id == idDisableBreakAll)
	{
		enableBreakAll(false);
	}
	else if (id == idDelete)
	{
		delete wib;
	}
	else if (id == idEnableBreak)
	{
		wib->setBreakEnabled(!wib->isBreakEnabled());
	}
	else if (id == idPropertyDialog)
	{
		wib->executePropertyDialog(this);
	}
	else if (id == idGeneralPropertyDialog)
	{
		generalPropertyDialog();
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
	// If you get a compiler error in the line below a possible reason is a missing
	// const in the XPM-File "items/watchbreak_xpm.xpm"
	// The 2nd line must be: static const char *watchbreak_xpm[]={
	// If this don't help please try to reactivate the uncommented code below. If this works
	// please send me an email with OS and compiler version to ksimus@gmx.de. Thanks!
	static QPixmap breakPix(/*(const char**)*/watchbreak_xpm);
	return breakPix;
}

void WatchListView::slotDoubleClicked(QListViewItem * lvi)
{
	if (lvi)
	{
		((WatchViewItem *)lvi)->getWatchItem()->executePropertyDialog(this);
	}
	
}

void WatchListView::generalPropertyDialog()
{
	KDialogBase * dia;
	dia = new KDialogBase(KDialogBase::TreeList,
	                      i18n("Watch Settings"),
	                      KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
	                      KDialogBase::Ok,
	                      this);
	QVBox * page;
	WatchWidgetPropertyWidget * wid;
	page = dia->addVBoxPage(i18n("Watch Settings"));
	wid = new WatchWidgetPropertyWidget(m_watchWidget, page, "watch setting");
	connect(dia, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
	connect(dia, SIGNAL(defaultClicked()), wid, SLOT(slotDefault()));
	connect(dia, SIGNAL(cancelClicked()), wid, SLOT(slotCancel()));
	dia->exec();

	delete dia;
}
