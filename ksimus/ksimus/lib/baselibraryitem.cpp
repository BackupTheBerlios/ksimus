/***************************************************************************
                          baselibraryitem.cpp  -  description
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

#include "baselibraryitem.h"
#include "baseinfo.h"

BaseLibraryItem::BaseLibraryItem(const BaseInfo * bi)
	:	m_baseInfo(bi)
{
	if (!bi->getOldLibNames().isEmpty())
	{
		m_oldLibNames = new QStringList(QStringList::split(';',bi->getOldLibNames()));
		CHECK_PTR(m_oldLibNames);
	}
	else
	{
		m_oldLibNames = 0;
	}
}

BaseLibraryItem::~BaseLibraryItem()
{
	if (m_oldLibNames)
		delete m_oldLibNames;
}

const BaseInfo * BaseLibraryItem::getBaseInfo() const
{
	return m_baseInfo;
}

QStringList * BaseLibraryItem::getOldLibNames() const
{
	return m_oldLibNames;
}
	
/** Returns true, if name is a old library name */
bool BaseLibraryItem::isOldLibName(const QString & name) const
{
	bool found = false;
	
	if (m_oldLibNames)
	{
		for (QStringList::Iterator it = m_oldLibNames->begin(); it != m_oldLibNames->end(); ++it)
		{
			if (name == *it)
			{
				found = true;
			}
		}
	}
	
	return found;
}

const QString & BaseLibraryItem::getName() const
{
	return getBaseInfo()->getName();
}
const QString & BaseLibraryItem::getLibName() const
{
	return getBaseInfo()->getLibName();
}
const QString & BaseLibraryItem::getShortDescr() const
{
	return getBaseInfo()->getShortDescr();
}
const QString & BaseLibraryItem::getHTMLDescr() const
{
	return getBaseInfo()->getHTMLDescr();
}

