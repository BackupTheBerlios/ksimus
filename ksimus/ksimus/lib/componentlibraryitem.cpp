/***************************************************************************
                          componentlibraryitem.cpp  -  description
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

#include "componentlibraryitem.h"

ComponentLibraryItem::ComponentLibraryItem(const ComponentInfo * ci, const PackageInfo * packageInfo, bool addToMenu)
	:	BaseLibraryItem(ci, packageInfo),
	  m_addToMenu(addToMenu)
{
	if (!ci->getAdditionalI18nLibNames().isEmpty())
	{
		m_additionalI18nLibNames = new QStringList(QStringList::split(';', ci->getAdditionalI18nLibNames()));
		CHECK_PTR(m_additionalI18nLibNames);
	}
	else
	{
		m_additionalI18nLibNames = (QStringList *)0;
	}
}

ComponentLibraryItem::~ComponentLibraryItem()
{
}

const QString & ComponentLibraryItem::getI18nLibName() const
{
	return getComponentInfo()->getI18nLibName();
}

QStringList * ComponentLibraryItem::getAdditionalI18nLibNames() const
{
	return m_additionalI18nLibNames;
}
	
/** Returns true, if name is a additional library name */
bool ComponentLibraryItem::isAdditionalLibName(const QString & name) const
{
	bool found = false;
	
	if (m_additionalI18nLibNames)
	{
		for (QStringList::Iterator it = m_additionalI18nLibNames->begin(); it != m_additionalI18nLibNames->end(); ++it)
		{
			if (name == *it)
			{
				found = true;
			}
		}
	}
	
	return found;
}
	
const ComponentInfo * ComponentLibraryItem::getComponentInfo() const
{
	return (const ComponentInfo *)BaseLibraryItem::getBaseInfo();
}

ViewAttribute ComponentLibraryItem::getViewAttr() const
{
	return getComponentInfo()->getViewAttr();
}

bool ComponentLibraryItem::isAddToMenu() const
{
	return m_addToMenu;
}

