/***************************************************************************
                          componentlibrary.cpp  -  description
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

#include "componentlibrary.h"
#include "componentlibraryitem.h"
#include "componentinfo.h"
#include "componentdirectory.h"
#include "ksimdebug.h"

ComponentLibrary::ComponentLibrary()
{
	m_additionalLibNames = new QDict<ComponentLibraryItem>;
	CHECK_PTR(m_additionalLibNames);

	m_dirBase = new ComponentDirectorySubMenu(QString::fromLatin1("Root"));
	CHECK_PTR(m_dirBase);
}

ComponentLibrary::~ComponentLibrary()
{
	delete m_dirBase;
	delete m_additionalLibNames;
}

bool ComponentLibrary::insert (const ComponentInfo * ci, const PackageInfo * packageInfo, bool addToMenu)
{
	bool res;
	
	ComponentLibraryItem * cli = new ComponentLibraryItem(ci, packageInfo, addToMenu);
	CHECK_PTR(cli);
	
	res = _insert_(cli);
	
	if (res)
	{
		if (addToMenu)
			m_dirBase->insert(ci->getLibName(), ci);
		
		if (cli->getAdditionalI18nLibNames())
		{
			for (QStringList::Iterator it = cli->getAdditionalI18nLibNames()->begin(); it != cli->getAdditionalI18nLibNames()->end(); ++it)
			{
				if (!m_oldLibNames->find(*it))
				{
					m_additionalLibNames->insert(*it, cli);
					if (addToMenu)
						m_dirBase->insert(*it, ci);
				}
				else
				{
					KSIMDEBUG("Multiple addtionalLibNames");
					KSIMDEBUG_VAR("",*it);
					KSIMDEBUG_VAR("",cli->getBaseInfo()->getLibName());
				}
			}
		}   	
		emit signalInsert(cli);		
	}
	else
	{
		delete cli;
	}
	return res;
}	


bool ComponentLibrary::insert (const ComponentInfo * ci, const PackageInfo * packageInfo)
{
	return insert(ci, packageInfo, true);
}

bool ComponentLibrary::insertInternal (const ComponentInfo * ci)
{
	return insert(ci, (const PackageInfo *)0, false);
}

bool ComponentLibrary::insert (const ComponentInfoList & cil, const PackageInfo * packageInfo)
{
	bool res = true;
	FOR_EACH_COMPONENT_INFO(it, cil)
	{
		res &= insert(it.current(), packageInfo);
	}
	return res;
}
bool ComponentLibrary::insertInternal (const ComponentInfoList & cil)
{
	bool res = true;
	FOR_EACH_COMPONENT_INFO(it, cil)
	{
		res &= insertInternal(it.current());
	}
	return res;
}


bool ComponentLibrary::remove(const QString & libName)
{
	ComponentLibraryItem * cli;
	
	if ((cli = (ComponentLibraryItem *)m_libNames->find(libName)))
	{
		signalRemove(cli);
		
		if (cli->getAdditionalI18nLibNames())
		{
			for (QStringList::Iterator it = cli->getAdditionalI18nLibNames()->begin(); it != cli->getAdditionalI18nLibNames()->end(); ++it)
			{
				if (cli == m_additionalLibNames->find(*it))
				{
					m_additionalLibNames->remove(*it);
				}
			}
		}
	}
	
	return BaseLibrary::remove(libName);
}


bool ComponentLibrary::remove(const ComponentInfo * ci)
{
	return remove(ci->getLibName());
}
	
const ComponentInfo * ComponentLibrary::findLibName(const QString & libName) const
{
	return (const ComponentInfo *)BaseLibrary::findLibName(libName);
}

const ComponentInfo * ComponentLibrary::findOldLibName(const QString & oldLibName) const
{
	return (const ComponentInfo *)BaseLibrary::findOldLibName(oldLibName);
}

const ComponentInfo * ComponentLibrary::findIndex(int idx) const
{
	return (const ComponentInfo *)BaseLibrary::findIndex(idx);
}

const ComponentInfo * ComponentLibrary::findAdditionalLibName(const QString & additionalLibName) const
{
	ComponentLibraryItem * cli = m_additionalLibNames->find(additionalLibName);
	if(cli)
	{
		return (const ComponentInfo *)cli->getBaseInfo();
	}
	return 0;
}

/** Create a new component */
bool ComponentLibrary::createComponent(Component * * newComp, CompContainer * container, const QString & libName) const
{
	const ComponentInfo * ci = findLibName(libName);
	
	if (!ci)
	{
		// Search old lib name
		ci = findOldLibName(libName);
		if (ci)
		{
			KSIMDEBUG_VAR("Found old lib name:", libName);
		}
	}
	
	if (ci)
	{
		*newComp = ci->createComponent(container);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Object", libName);
		*newComp = 0;
		return false;
	}
}

ComponentLibraryItem * ComponentLibrary::getItem(const QString & libName) const
{
	return (ComponentLibraryItem *)BaseLibrary::getItem(libName);
}

