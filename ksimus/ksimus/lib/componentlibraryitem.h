/***************************************************************************
                          componentlibraryitem.h  -  description
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

#ifndef COMPONENTLIBRARYITEM_H
#define COMPONENTLIBRARYITEM_H

#include "baselibraryitem.h"
#include "componentinfo.h"

/**The library item for component infos
  *@author Rasmus Diekenbrock
  */

class ComponentLibraryItem : public BaseLibraryItem
{

public:
	ComponentLibraryItem(const ComponentInfo * ci, const PackageInfo * packageInfo, bool addToMenu);
	virtual ~ComponentLibraryItem();
	
	const ComponentInfo * getComponentInfo() const;
	ViewAttribute getViewAttr() const;
	
	QStringList * getAdditionalLibNames() const;
	/** Returns true, if name is a old library name */
	bool isAdditionalLibName(const QString & name) const;
	
	bool isAddToMenu() const;

protected:
	QStringList * m_additionalLibNames;
	bool m_addToMenu;

};

#endif
