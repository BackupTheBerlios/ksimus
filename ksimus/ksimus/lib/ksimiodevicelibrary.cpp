/***************************************************************************
                          ksimiodevicelibrary.cpp  -  description
                             -------------------
    begin                : Fri Apr 4 2003
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

#include "ksimiodevicelibrary.h"
#include "ksimiodevicelibraryitem.h"
#include "ksimdebug.h"
#include "packageinfo.h"

KSimIoDeviceLibrary::KSimIoDeviceLibrary()
{
}

KSimIoDeviceLibrary::~KSimIoDeviceLibrary()
{
}

bool KSimIoDeviceLibrary::insert (const KSimIoDeviceInfo * di, const PackageInfo * packageInfo)
{
	bool res;
	
	CHECK_PTR(di);
	
	KSimIoDeviceLibraryItem * dli = new KSimIoDeviceLibraryItem(di, packageInfo);
	CHECK_PTR(dli);
	
	res = _insert_(dli);
	
	return res;
}

bool KSimIoDeviceLibrary::insert (const KSimIoDeviceInfoList & dil, const PackageInfo * packageInfo)
{
	bool res = true;
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	unsigned int cnt = 0;
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	FOR_EACH_IO_DEVICE_INFO(it, dil)
	{
		res &= insert(it.current(), packageInfo);
		#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
		cnt++;
		#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	}
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	if (cnt)
	{
		KSIMDEBUG(QString::fromLatin1("Package %1: Load %2 io device")
		          .arg(packageInfo->getPackageName())
		          .arg(cnt));
	}
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	return res;
}


bool KSimIoDeviceLibrary::create(KSimIoDevice * * device, const char * libName) const
{
	const KSimIoDeviceInfo * di = (KSimIoDeviceInfo *) findLibName(QString::fromLatin1(libName));
	
	if (di)
	{
		*device = di->create(di);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown IO Device", libName);
		*device = (KSimIoDevice *)0;
		return false;
	}
}
