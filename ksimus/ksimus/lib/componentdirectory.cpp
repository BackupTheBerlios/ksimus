/***************************************************************************
                          componentdirectory.cpp  -  description
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

#include <qstring.h>

#include "componentdirectory.h"
#include "componentinfo.h"
#include "ksimdebug.h"


//###################################################################################
//###################################################################################


ComponentDirectoryItem::ComponentDirectoryItem(const QString & menuName)
	:	m_menuName(menuName),
		m_isMenu(false)
{
}
ComponentDirectoryItem::~ComponentDirectoryItem()
{
}
	
bool ComponentDirectoryItem::isSubMenu() const
{
	return m_isMenu;
}

bool ComponentDirectoryItem::isMenuEntry() const
{
	return !m_isMenu;
}

const QString & ComponentDirectoryItem::textEntry() const
{
	return m_menuName;
}


//###################################################################################
//###################################################################################


ComponentDirectoryEntry::ComponentDirectoryEntry(const QString & menuName, const ComponentInfo * compInfo)
	:	ComponentDirectoryItem(menuName),
		m_ci(compInfo)
{
}

const ComponentInfo * ComponentDirectoryEntry::getComponentInfo() const
{
	return m_ci;
}


//###################################################################################
//###################################################################################

ComponentDirectorySubMenu::ComponentDirectorySubMenu(const QString & menuName)
	:	ComponentDirectoryItem(menuName)
{
	m_isMenu = true;
	
	m_dirList = new ComponentDirectoryList();
	Q_CHECK_PTR(m_dirList);
}

ComponentDirectorySubMenu::~ComponentDirectorySubMenu()
{
	delete m_dirList;
}

void ComponentDirectorySubMenu::insert(const QString & longMenuEntry, const ComponentInfo * ci)
{
	QString lme(longMenuEntry);
	int idx = lme.find('/');
	
	if (idx == -1)
	{
		// No further sub menu
		ComponentDirectoryEntry * cde = new ComponentDirectoryEntry(longMenuEntry, ci);
		Q_CHECK_PTR(cde);
		if (-1 == m_dirList->find(cde))
		{
			m_dirList->inSort(cde);
		}
		else
		{
			KSIMDEBUG_VAR("Entry exists", longMenuEntry);
			delete cde;
		}
	}
	else
	{
		// A sub menu
		QString dir = lme.left(idx+1);
		lme = lme.right(lme.length()-idx-1);
		
		ComponentDirectorySubMenu * cds = new ComponentDirectorySubMenu(dir);
		Q_CHECK_PTR(cds);
		int ref = m_dirList->find(cds);
		if(-1 == ref)
		{
			m_dirList->inSort(cds);
		}
		else
		{
			delete cds;
			cds = (ComponentDirectorySubMenu*)m_dirList->at(ref);
		}
		cds->insert(lme, ci);
	}
}

const ComponentDirectoryList * ComponentDirectorySubMenu::getDirList() const
{
	return m_dirList;
}

	
//###############################################################################	
//###############################################################################	
	
int ComponentDirectoryList::compareItems(QPtrCollection::Item cdiA, QPtrCollection::Item cdiB)
{
	int res;
	
	if (((ComponentDirectoryItem*)cdiA)->isSubMenu() == ((ComponentDirectoryItem*)cdiB)->isSubMenu())
	{
		// Both SubMenus or Entries
		res = strcmp(((ComponentDirectoryItem*)cdiA)->textEntry(), ((ComponentDirectoryItem*)cdiB)->textEntry());
	}
	else if (((ComponentDirectoryItem*)cdiA)->isSubMenu())
	{
		// di1 is SubMenu di2 is Entry
		res = -1;
	}
	else
	{
		// di1 is Entry di2 is SubMenu
		res = 1;
	}
	return res;
}

