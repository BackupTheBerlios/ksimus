/***************************************************************************
                          componentlistview.cpp  -  description
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

#include <qstringlist.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include <klocale.h>
#include <kinstance.h>
#include <kapp.h>

#include "library.h"
#include "componentlibrary.h"
#include "componentlibraryitem.h"
#include "componentlistview.h"
#include "componentlistviewitem.h"
#include "componentdirectory.h"
#include "packageinfo.h"

#include "ksimdebug.h"

ComponentListView::ComponentListView(QWidget *parent, const char *name )
	:	QListView(parent,name),
		m_leastSelected(0),
		m_currentView(APP_SHEET_VIEW),
		componentRoot(0)
{
	addColumn(i18n("Component"));
	
	componentRoot = new ComponentListViewItem(this, i18n("Component"));
	CHECK_PTR(componentRoot);
	componentRoot->setOpen(true);
	
//	setupTreeView(g_library->getComponentLib()->m_dirBase, componentRoot);
	
	connect(g_library->getComponentLib(),SIGNAL(signalInsert(const ComponentLibraryItem *)),
					this,SLOT(slotInsert(const ComponentLibraryItem *)));
	
	QListIterator<BaseLibraryItem> it(*g_library->getComponentLib()->m_library);
	for(;it.current();++it)	
	{
		insert( (ComponentLibraryItem *)it.current(), false );
		
	}
	
	setShowSortIndicator(true);

	
//	connect(this, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slotSelection(QListViewItem*)));
//	connect(this, SIGNAL(returnPressed(QListViewItem*)), this, SLOT(slotSelection(QListViewItem*)));
	
	connect(this, SIGNAL(clicked(QListViewItem*)), this, SLOT(slotSelection(QListViewItem*)));

	connect(this, SIGNAL(rightButtonPressed(QListViewItem *, const QPoint &, int)),
	        this, SLOT(slotRightButtonPressed(QListViewItem *, const QPoint &, int)));

}


ComponentListView::~ComponentListView()
{
}

void ComponentListView::setupTreeView(ComponentDirectorySubMenu * dir, ComponentListViewItem * clvi)
{
	QListIterator<ComponentDirectoryItem> it(* dir->getDirList());
	
	for (;it.current();++it)
	{
		ComponentListViewItem * newClvi;
		
		if(it.current()->isSubMenu())
		{
			newClvi = new ComponentListViewItem(clvi, it.current()->textEntry());
			CHECK_PTR(newClvi);
//			newClvi->setSelectable(false);
			setupTreeView((ComponentDirectorySubMenu *)it.current(), newClvi);
		}
		else
		{
			newClvi = new ComponentListViewItem(clvi, it.current()->textEntry(), ((ComponentDirectoryEntry*)it.current())->getComponentInfo());
			CHECK_PTR(newClvi);
		}
	}
}

void ComponentListView::slotSelection(QListViewItem * lvi)
{
	m_leastSelected = (ComponentListViewItem *)lvi;
	if (m_leastSelected && (m_leastSelected->getComponentInfo()) && m_leastSelected->isCorrectView())
	{
		emit signalSelection(m_leastSelected->getComponentInfo());
//		KSIMDEBUG_VAR("",m_leastSelected->getComponentInfo()->getName());
	}
	else
	{
		clearSelection();
		emit signalSelection(0);
	}
}

void ComponentListView::slotUnselect()
{
	if (m_leastSelected)
		setSelected(m_leastSelected, false);
}

eAppViewType ComponentListView::getCurrentView() const
{
	return m_currentView;
}

void ComponentListView::slotSetCurrentView(eAppViewType currentView)
{
	if (m_currentView != currentView)
	{
		m_currentView = currentView;
		triggerUpdate();
	}
}

void ComponentListView::insert(const ComponentLibraryItem * cli, bool open)
{
	if (cli->isAddToMenu())
	{
		insert(cli->getLibName(), cli->getComponentInfo(), open);
	
		QStringList * libNames = cli->getAdditionalLibNames();
	
		if (libNames)
		{
			QStringList::Iterator it;
			for (it = libNames->begin(); it != libNames->end(); ++it)
			{
				insert(*it, cli->getComponentInfo(), open);
			}
		}
	}
}

void ComponentListView::slotInsert(const ComponentLibraryItem * cli)
{
	insert(cli,true);
}

void ComponentListView::insert(const QString & libName, const ComponentInfo * ci, bool open)
{
	QStringList sl = QStringList::split('/', i18n(libName.latin1()));
	unsigned int count;
	
	ComponentListViewItem * clvi = componentRoot;
	ComponentListViewItem * nextClvi;
	
	for (count = 0; count < sl.count(); count++)
	{
		nextClvi = clvi->findChild(sl[count]);
		if(nextClvi)
		{
			if (nextClvi->isEntry())
			{
				// Entry exist - bad
				KSIMDEBUG_VAR("Entry exist - skipped",i18n(libName.latin1()));
				KSIMDEBUG_VAR("Entry info",nextClvi->text(0));
				break;
			}
			if (open) nextClvi->setOpen(true);
		}
		else
		{
			// Create sub menus
			for (; count < sl.count()-1; count++)
			{
				nextClvi = new ComponentListViewItem(clvi, sl[count]);
				if (open) nextClvi->setOpen(true);
				clvi = nextClvi;
			}
			
			// Create entry
			nextClvi = new ComponentListViewItem(clvi, sl[sl.count()-1], ci);
		}
			
		clvi = nextClvi;
	}			
}

void ComponentListView::contentsMousePressEvent(QMouseEvent * ev)
{
/*	if (ev->button() == RightButton)
	{
		QPopupMenu * menu = new QPopupMenu();
		
		menu->insertItem(i18n("&Fold Tree"),this, SLOT(slotFoldTree()));
		menu->insertItem(i18n("&Unfold Tree"),this, SLOT(slotUnfoldTree()));
		menu->insertSeparator();
		menu->insertItem(i18n("&Hide Component Supplier"),this, SLOT(slotHideMe()));
	
    menu->exec(QCursor::pos());

    delete menu;
	}
	else*/
	{
		QListView::contentsMousePressEvent(ev);
	}
}

static void foldRecursive(QListViewItem * parent, bool close)
{
	// Do not close if base item
	if (!((parent->parent() == 0) && close))
		parent->setOpen(!close);
	
	QListViewItem * item = parent->firstChild();
	
	while(item)
	{
		foldRecursive(item,close);
		item = item->nextSibling();
	}
}


void ComponentListView::slotFoldTree()
{
	QListViewItem * topLevelitem = firstChild();
	
	
	while(topLevelitem)
	{
		topLevelitem->setOpen(true);
		
		QListViewItem * item = topLevelitem->firstChild();
		while(item)
		{
			foldRecursive(item,true);
			item = item->nextSibling();
		}
		topLevelitem = topLevelitem->nextSibling();
	}
}

void ComponentListView::slotUnfoldTree()
{
	QListViewItem * item = firstChild();
	
	if(item)
	{
		foldRecursive(item, false);
	}
}

void ComponentListView::slotHideMe()
{
	emit signalHideMe();
}

void ComponentListView::slotRightButtonPressed(QListViewItem * item, const QPoint & p, int c)
{
	int idxFoldAll = 0;
	int idxUnfoldAll = 0;
	int idxHelp = 0;
	int res;
	QListViewItem * dirItem;
	bool isDir;

	if (!item) item = firstChild();
	
	if (item->childCount() != 0)
	{
		isDir = true;
		dirItem = item;
	}
	else
	{
		isDir = false;
		dirItem = item->parent();
	}
	
	QPopupMenu * menu = new QPopupMenu();
		
	if (dirItem == firstChild())
	{
		idxFoldAll = menu->insertItem(i18n("&Fold Tree"));
		idxUnfoldAll = menu->insertItem(i18n("&Unfold Tree"));
	}
	else
	{
		idxFoldAll = menu->insertItem(i18n("&Fold %1 Tree").arg(dirItem->text(0)));
		idxUnfoldAll = menu->insertItem(i18n("&Unfold %1 Tree").arg(dirItem->text(0)));
	}	
	menu->insertSeparator();
	menu->insertItem(i18n("&Hide Component Supplier"),this, SLOT(slotHideMe()));
	
  if (!isDir)
  {
		ComponentListViewItem * clvi = (ComponentListViewItem *)item;
		if ((clvi->getComponentInfo() != 0) && (clvi->getComponentInfo()->getHTMLDescr() != QString::null))
		{
			menu->insertSeparator();
			idxHelp = menu->insertItem(i18n("&Help %1").arg(clvi->getComponentInfo()->getName()));
		}
	}

  res = menu->exec(QCursor::pos());


  if (res == idxFoldAll)
  {
		foldRecursive(dirItem, true);
	}
  else if (res == idxUnfoldAll)
  {
		foldRecursive(dirItem, false);
	}
  else if (res == idxHelp)
  {
		ComponentListViewItem * clvi = (ComponentListViewItem *)item;
		const ComponentInfo * ci = clvi->getComponentInfo();
		const PackageInfo * pi = g_library->getComponentLib()->getPackageInfo(ci->getLibName());
			
		if (pi)
		{
			kapp->invokeHelp(ci->getHTMLDescr(), pi->getInstance()->instanceName());
		}
	}

	delete menu;
	
}




