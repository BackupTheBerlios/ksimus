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

KSimIoJoinLibrary::KSimIoJoinLibrary()
{
}

KSimIoJoinLibrary::~KSimIoJoinLibrary()
{
}

bool KSimIoJoinLibrary::insert (const KSimIoJoinInfo * ji, const PackageInfo * packageInfo)
{
	bool res;
	
	CHECK_PTR(ji);
	
	KSimIoJoinLibraryItem * jli = new KSimIoJoinLibraryItem(ji, packageInfo);
	CHECK_PTR(jli);
	
	res = _insert_(jli);
	
	return res;
}

bool KSimIoJoinLibrary::insert (const KSimIoJoinInfoList & jil, const PackageInfo * packageInfo)
{
	bool res = true;
	FOR_EACH_IO_JOIN_INFO(it, jil)
	{
		res &= insert(it.current(), packageInfo);
	}
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
