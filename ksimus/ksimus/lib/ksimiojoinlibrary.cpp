/***************************************************************************
                          ksimiojoinlibrary.cpp  -  description
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#include "ksimiojoinlibrary.h"
#include "ksimiojoinlibraryitem.h"
#include "ksimdebug.h"
#include "packageinfo.h"

KSimIoJoinLibrary::KSimIoJoinLibrary()
{
}

KSimIoJoinLibrary::~KSimIoJoinLibrary()
{
}

bool KSimIoJoinLibrary::insert (const KSimIoJoinInfo * ji, const PackageInfo * packageInfo)
{
	bool res;
	
	Q_CHECK_PTR(ji);
	
	KSimIoJoinLibraryItem * jli = new KSimIoJoinLibraryItem(ji, packageInfo);
	Q_CHECK_PTR(jli);
	
	res = _insert_(jli);
	
	return res;
}

bool KSimIoJoinLibrary::insert (const KSimIoJoinInfoList & jil, const PackageInfo * packageInfo)
{
	bool res = true;
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	unsigned int cnt = 0;
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	FOR_EACH_IO_JOIN_INFO(it, jil)
	{
		res &= insert(it.current(), packageInfo);
		#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
		cnt++;
		#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	}
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	if (cnt)
	{
		KSIMDEBUG(QString::fromLatin1("Package %1: Load %2 io join")
		          .arg(packageInfo->getPackageName())
		          .arg(cnt));
	}
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	return res;
}


bool KSimIoJoinLibrary::create(KSimIoJoin * * join, const char * libName, KSimIoComponent * comp) const
{
	const KSimIoJoinInfo * ji = (KSimIoJoinInfo *) findLibName(QString::fromLatin1(libName));
	
	if (ji)
	{
		*join = ji->create(comp, ji);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown IO Join", libName);
		*join = (KSimIoJoin *)0;
		return false;
	}
}

#include "ksimiojoinlibrary.moc"
