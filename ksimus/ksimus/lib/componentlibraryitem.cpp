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

ComponentLibraryItem::ComponentLibraryItem(const ComponentInfo * ci, bool addToMenu)
	:	BaseLibraryItem(ci),
	  m_addToMenu(addToMenu)
{
	if (!ci->getAdditionalLibNames().isEmpty())
	{
		m_additionalLibNames = new QStringList(QStringList::split(';', ci->getAdditionalLibNames()));
		CHECK_PTR(m_additionalLibNames);
	}
	else
	{
		m_additionalLibNames = 0;
	}
}

ComponentLibraryItem::~ComponentLibraryItem()
{
}

QStringList * ComponentLibraryItem::getAdditionalLibNames() const
{
	return m_additionalLibNames;
}
	
/** Returns true, if name is a additional library name */
bool ComponentLibraryItem::isAdditionalLibName(const QString & name) const
{
	bool found = false;
	
	if (m_additionalLibNames)
	{
		for (QStringList::Iterator it = m_additionalLibNames->begin(); it != m_additionalLibNames->end(); ++it)
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

