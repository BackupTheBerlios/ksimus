/***************************************************************************
                          baselibrary.cpp  -  description
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


#include "baselibrary.h"
#include "baselibraryitem.h"
#include "baseinfo.h"

#include "globals.h"
#include "ksimdebug.h"

BaseLibrary::BaseLibrary()
{
	m_library = new QList<BaseLibraryItem>;
	CHECK_PTR(m_library);
	m_library->setAutoDelete(true);
	
	m_libNames = new QDict<BaseLibraryItem>;
	CHECK_PTR(m_libNames);
	
	m_oldLibNames = new QDict<BaseLibraryItem>;
	CHECK_PTR(m_oldLibNames);
	
}

BaseLibrary::~BaseLibrary()
{
	delete m_oldLibNames;
	delete m_libNames;
	delete m_library;
}

bool BaseLibrary::insert(const BaseInfo * bi, const PackageInfo * packageInfo)
{
	bool res;
	
	BaseLibraryItem * bli = new BaseLibraryItem(bi, packageInfo);
	CHECK_PTR(bli);
	
	res = _insert_(bli);
	
	if (!res)
	{
		delete bli;
	}
	else
	{
		emit signalInsert(bli);
		emit signalChanged();
	}
	
	return res;
}

bool BaseLibrary::_insert_(const BaseLibraryItem * bli)
{
	CHECK_PTR(bli);
	
	// Unique libName required
	if ( ! m_libNames->find(bli->getBaseInfo()->getLibName()) )
	{
		m_library->append(bli);
		
		m_libNames->insert(bli->getBaseInfo()->getLibName(), bli);
	
		if (bli->getOldLibNames())
		{
			for (QStringList::Iterator it = bli->getOldLibNames()->begin(); it != bli->getOldLibNames()->end(); ++it)
			{
				if (!m_oldLibNames->find(*it))
				{
					m_oldLibNames->insert(*it, bli);
				}
				else
				{
					KSIMDEBUG_VAR("multiple oldLibNames",*it);
					KSIMDEBUG_VAR("",bli->getBaseInfo()->getLibName());
				}
			}
		}   	
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unique libName required", bli->getBaseInfo()->getLibName());
		return false;
	}
}

bool BaseLibrary::remove(const QString & libName)
{
	BaseLibraryItem * bli;
	
	if ((bli = m_libNames->find(libName)))
	{
		emit signalRemove(bli);
		emit signalChanged();
		
		m_libNames->remove(libName);
		
		if (bli->getOldLibNames())
		{
			for (QStringList::Iterator it = bli->getOldLibNames()->begin(); it != bli->getOldLibNames()->end(); ++it)
			{
				if (bli == m_oldLibNames->find(*it))
				{
					m_oldLibNames->remove(*it);
				}
			}
		}
		m_library->remove(bli);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("libName not found", libName);
		return false;
	}
}

bool BaseLibrary::remove(const BaseInfo * bi)
{
	return remove(bi->getLibName());
}
	
const BaseInfo * BaseLibrary::findLibName(const QString & libName) const
{
	BaseLibraryItem * bli = m_libNames->find(libName);
	if(bli)
	{
		return bli->getBaseInfo();
	}
	return (const BaseInfo *)0;
}

const BaseInfo * BaseLibrary::findOldLibName(const QString & oldLibName) const
{
	BaseLibraryItem * bli = m_oldLibNames->find(oldLibName);
	if(bli)
	{
		return bli->getBaseInfo();
	}
	return (const BaseInfo *)0;
}
	
const QString & BaseLibrary::getName(const QString & libName) const
{
	const BaseInfo * bi = findLibName(libName);
	if(bi)
	{
		return bi->getName();
	}
	return QString::null;
}

const QString & BaseLibrary::getShortDescr(const QString & libName) const
{
	const BaseInfo * bi = findLibName(libName);
	if(bi)
	{
		return bi->getShortDescr();
	}
	return QString::null;
}

const QString & BaseLibrary::getHTMLDescr(const QString & libName) const
{
	const BaseInfo * bi = findLibName(libName);
	if(bi)
	{
		return bi->getHTMLDescr();
	}
	return QString::null;
}
	
/** Resize QDict. Call after latest insert */
void BaseLibrary::resize()
{
	m_libNames->resize(getGreaterPrim(m_libNames->count()));
	m_oldLibNames->resize(getGreaterPrim(m_oldLibNames->count()));
}

int BaseLibrary::getIndex(const QString & libName) const
{
	BaseLibraryItem * bli = m_libNames->find(libName);
	
	return m_library->findRef(bli);
}
	
const BaseInfo * BaseLibrary::findIndex(int idx) const
{
	BaseLibraryItem * bli = m_library->at(idx);
	
	return bli->getBaseInfo();
}

BaseLibraryItem * BaseLibrary::getItem(const QString & libName) const
{
	BaseLibraryItem * bli = m_libNames->find(libName);
	
	if (!bli)
	{
		KSIMDEBUG_VAR("No BaseLibraryItem found", libName);
	}
	
	return bli;
}

const PackageInfo * BaseLibrary::getPackageInfo(const QString & libName) const
{
	BaseLibraryItem * bli = getItem(libName);
	
	if (bli)
	{
		const PackageInfo * pi = bli->getPackageInfo();
		
		if (!pi)
		{
			KSIMDEBUG_VAR("No PackageInfo found", libName);
		}
		
		return pi;
	}
	return (const PackageInfo *)0;
}

