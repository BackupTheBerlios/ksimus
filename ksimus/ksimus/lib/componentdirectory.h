/***************************************************************************
                          componentdirectory.h  -  description
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

#ifndef COMPONENTDIRECTORY_H
#define COMPONENTDIRECTORY_H

/**Classes for the sorted component dirctory
  *@author Rasmus Diekenbrock
  */

#include <qstring.h>
#include <qptrlist.h>

class ComponentInfo;
class ComponentDirectoryList;

class ComponentDirectoryItem
{
public: 
	bool isSubMenu() const;
	bool isMenuEntry() const;
	const QString & textEntry() const;
	virtual ~ComponentDirectoryItem();
			
protected:
	ComponentDirectoryItem(const QString & menuName);
	QString m_menuName;
	
	bool m_isMenu;
};

//###################################################################################
//###################################################################################

class ComponentDirectoryEntry : public ComponentDirectoryItem
{

public:
	ComponentDirectoryEntry(const QString & menuName, const ComponentInfo * compInfo);
	const ComponentInfo * getComponentInfo() const;

protected:
	const ComponentInfo * m_ci;
};

//###################################################################################
//###################################################################################

class ComponentDirectorySubMenu : public ComponentDirectoryItem
{

public:
	ComponentDirectorySubMenu(const QString & menuName);
	virtual ~ComponentDirectorySubMenu();
	void insert(const QString & longMenuName, const ComponentInfo * ci);
	const ComponentDirectoryList * getDirList() const;


protected:
	ComponentDirectoryList * m_dirList;
};
	
//###################################################################################
//###################################################################################

class ComponentDirectoryList : public QPtrList<ComponentDirectoryItem>
{
protected:
	virtual int compareItems(QPtrCollection::Item di1, QPtrCollection::Item di2);
};
	
#endif
