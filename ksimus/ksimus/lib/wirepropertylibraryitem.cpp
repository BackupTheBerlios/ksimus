/***************************************************************************
                          wirepropertylibraryitem.cpp  -  description
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

#include "wirepropertylibraryitem.h"
#include "wirepropertyinfo.h"


WirePropertyLibraryItem::WirePropertyLibraryItem(const WirePropertyInfo * wireInfo, const PackageInfo * packageInfo)
	:	BaseLibraryItem(wireInfo, packageInfo)
{
}

const WirePropertyInfo * WirePropertyLibraryItem::getWirePropertyInfo() const
{
	return (const WirePropertyInfo * )BaseLibraryItem::getBaseInfo();
}
