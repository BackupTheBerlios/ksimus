/***************************************************************************
                          connectorlibrary.cpp  -  description
                             -------------------
    begin                : Tue Oct 3 2000
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

#include "connectorlibrary.h"
#include "connectorlibraryitem.h"
#include "ksimdebug.h"
#include "packageinfo.h"

ConnectorLibrary::ConnectorLibrary()
{
}

ConnectorLibrary::~ConnectorLibrary()
{
}

bool ConnectorLibrary::insert (const ConnectorInfo * ci, const PackageInfo * packageInfo)
{
	bool res;
	
	Q_CHECK_PTR(ci);
	
	ConnectorLibraryItem * cli = new ConnectorLibraryItem(ci, packageInfo);
	Q_CHECK_PTR(cli);
	
	res = _insert_(cli);
	
	return res;
}
	
bool ConnectorLibrary::insert (const ConnectorInfoList & cil, const PackageInfo * packageInfo)
{
	bool res = true;
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	unsigned int cnt = 0;
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	FOR_EACH_CONNECTOR_INFO(it, cil)
	{
		res &= insert(it.current(), packageInfo);
		#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
		cnt++;
		#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	}
	#if defined(DEBUG) && !defined(NO_KSIMDEBUG)
	if (cnt)
	{
		KSIMDEBUG(QString::fromLatin1("Package %1: Load %2 connector")
		          .arg(packageInfo->getPackageName())
		          .arg(cnt));
	}
	#endif // defined(DEBUG) && !defined(NO_KSIMDEBUG)
	return res;
}


bool ConnectorLibrary::create(ConnectorBase * * conn, Component * comp, const QString & name,
                              const QString & i18nName, const QPoint & pos,const char * libName) const
{
	const ConnectorInfo * ci = (ConnectorInfo *) findLibName(QString::fromLatin1(libName));
	
	if (ci)
	{
		*conn = ci->create(comp, name, i18nName, pos);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Connector", libName);
		*conn = (ConnectorBase *)0;
		return false;
	}
}

#include "connectorlibrary.moc"
