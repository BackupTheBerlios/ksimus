/***************************************************************************
                          wirepropertylibrary.cpp  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#include "wirepropertylibrary.h"
#include "wirepropertylibraryitem.h"
#include "ksimdebug.h"
#include "packageinfo.h"

WirePropertyLibrary::WirePropertyLibrary()
{
	m_dataTypes = new QDict<WirePropertyLibraryItem>;
	CHECK_PTR(m_dataTypes);
}

WirePropertyLibrary::~WirePropertyLibrary()
{
}
	
bool WirePropertyLibrary::insert (const WirePropertyInfo * wireInfo, const PackageInfo * packageInfo)
{
	bool res;
	
	CHECK_PTR(wireInfo);
	
	WirePropertyLibraryItem * wli = new WirePropertyLibraryItem(wireInfo, packageInfo);
	CHECK_PTR(wli);
	
	res = _insert_(wli);
	
	if (res)
	{
		if (wireInfo->getDataType())
		{
			m_dataTypes->insert(wireInfo->getDataType(), wli);
		}
		else
		{
			KSIMDEBUG_VAR("No dataType", wireInfo->getLibName());
		}
	}
	else
	{
		delete wli;
	}
	
	return res;
}

bool WirePropertyLibrary::insert(const WirePropertyInfoList & wirePropertyInfoList, const PackageInfo * packageInfo)
{
	bool res = true;
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	unsigned int cnt = 0;
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	FOR_EACH_WIREPROPERTY_INFO(it, wirePropertyInfoList)
	{
		res &= insert(it.current(), packageInfo);
		#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
		cnt++;
		#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	}
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	if (cnt)
	{
		KSIMDEBUG(QString::fromLatin1("Package %1: Load %2 wire property")
		          .arg(packageInfo->getPackageName())
		          .arg(cnt));
	}
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	return res;
}
	
bool WirePropertyLibrary::create(WireProperty * * wireProperty, Wire * wire, const QString & libName) const
{
	const WirePropertyInfo * wi = (WirePropertyInfo *) findLibName(libName);
	
	if (wi)
	{
		*wireProperty = wi->create(wire);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Wire property", libName);
		*wireProperty = (WireProperty *)0;
		return false;
	}
}

/** Return the WirePropertyInfo for the given dataType.
	Returns 0, if dataType is not found in the library */
const WirePropertyInfo * WirePropertyLibrary::findDataType(const QString & dataType) const
{
	WirePropertyLibraryItem * wli = m_dataTypes->find(dataType);
	if (wli)
	{
		return wli->getWirePropertyInfo();
	}
	return (const WirePropertyInfo *)0;
}
