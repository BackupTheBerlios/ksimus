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

ConnectorLibrary::ConnectorLibrary()
{
}

ConnectorLibrary::~ConnectorLibrary()
{
}

bool ConnectorLibrary::insert (const ConnectorInfo * ci, const PackageInfo * packageInfo)
{
	bool res;
	
	CHECK_PTR(ci);
	
	ConnectorLibraryItem * cli = new ConnectorLibraryItem(ci, packageInfo);
	CHECK_PTR(cli);
	
	res = _insert_(cli);
	
	return res;
}
	
bool ConnectorLibrary::insert (const ConnectorInfoList cil, const PackageInfo * packageInfo)
{
	bool res = true;
	const ConnectorInfo * ci;
	int idx = 0;
	
	while ((ci = cil[idx++]))
	{
		res &= insert(ci, packageInfo);
	}
	return res;
}


bool ConnectorLibrary::create(ConnectorBase * * conn, Component * comp, const QString & name, const QPoint & pos,const char * libName) const
{
	const ConnectorInfo * ci = (ConnectorInfo *) findLibName(libName);
	
	if (ci)
	{
		*conn = ci->create(comp, name, pos);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Connector", libName);
		*conn = 0;
		return false;
	}
}
