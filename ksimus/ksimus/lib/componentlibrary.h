/***************************************************************************
                          componentlibrary.h  -  description
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

#ifndef COMPONENTLIBRARY_H
#define COMPONENTLIBRARY_H

#include "baselibrary.h"

#include "componentinfo.h"

/**The library for all components
  *@author Rasmus Diekenbrock
  */

//class ComponentInfo;
class ComponentLibraryItem;
class Component;
class CompContainer;
class ComponentDirectorySubMenu;
class PackageInfo;

class ComponentLibrary : public BaseLibrary
{
   Q_OBJECT

friend class ComponentPopupMenu;
friend class ComponentListView;

public:
	ComponentLibrary();
	~ComponentLibrary();
	
	bool insert (const ComponentInfo * ci, const PackageInfo * packageInfo);
	bool insertInternal (const ComponentInfo * ci);
	bool insert (const ComponentInfoList & cil, const PackageInfo * packageInfo);
	bool insertInternal (const ComponentInfoList & cil);
	bool remove(const QString & libName);
	bool remove(const ComponentInfo * ci);

	const ComponentInfo * findLibName(const QString & libName) const;
	const ComponentInfo * findOldLibName(const QString & oldLibName) const;
	const ComponentInfo * findIndex(int idx) const;
	const ComponentInfo * findAdditionalLibName(const QString & additionalLibName) const;

	ComponentLibraryItem * getItem(const QString & libName) const;


	/** Create a new component */
	bool createComponent(Component * * newComp, CompContainer * container, const QString & libName) const;


protected:
	bool insert (const ComponentInfo * ci, const PackageInfo * packageInfo, bool addToMenu);
	QDict<ComponentLibraryItem> * m_additionalLibNames;
    ComponentDirectorySubMenu * m_dirBase;
	
signals:
	/** This signal is emitted, after a item is inserted */
	void signalInsert(const ComponentLibraryItem *);
	/** This signal is emitted, before a item is removed */
	void signalRemove(const ComponentLibraryItem *);
};

#endif
