/***************************************************************************
                          componentpopupmenu.cpp  -  description
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

#include "componentpopupmenu.h"
#include "componentdirectory.h"
#include "componentlibrary.h"
#include "componentinfo.h"
#include "library.h"

ComponentPopupMenu::ComponentPopupMenu(QWidget * parent, const char * name)
	: QPopupMenu(parent, name),
		m_first(this)
{
	setupPopupMenu(g_library->getComponentLib()->m_dirBase);
	connect(this, SIGNAL(activated(int)), SLOT(slotSelected(int)));
}

ComponentPopupMenu::ComponentPopupMenu(ComponentPopupMenu *parent, const char * name)
	: QPopupMenu(parent, name),
		m_first(parent)
{
	connect(this, SIGNAL(activated(int)), m_first, SLOT(slotSelected(int)));
}

ComponentPopupMenu::~ComponentPopupMenu()
{
}

void ComponentPopupMenu::setupPopupMenu(const ComponentDirectorySubMenu * dir)
{
	QListIterator<ComponentDirectoryItem> it(* dir->getDirList());
	
	for (;it.current();++it)
	{
		if(it.current()->isSubMenu())
		{
			ComponentPopupMenu * newPm = new ComponentPopupMenu(m_first);
			CHECK_PTR(newPm);
			insertItem(it.current()->textEntry(), newPm, -2);
			newPm->setupPopupMenu((const ComponentDirectorySubMenu *) it.current());
		}
		else
		{
			int idx = g_library->getComponentLib()->getIndex(((const ComponentDirectoryEntry *)it.current())->getComponentInfo()->getLibName());
			insertItem(it.current()->textEntry(), idx);
		}
	}
	
}	

void ComponentPopupMenu::slotSelected(int idx)
{
	const ComponentInfo * ci;
	ci = g_library->getComponentLib()->findIndex(idx);
	
	if (ci)
	{
		debug("ComponentPopupMenu::slotSelected(int idx) = %i", idx);
		emit signalSelection(ci);
		emit signalSelection(ci->getName());
	}
}
