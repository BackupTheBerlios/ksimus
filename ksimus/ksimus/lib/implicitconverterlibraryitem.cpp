/***************************************************************************
                          implicitconverterlibraryitem.cpp  -  description
                             -------------------
    begin                : Wed Aug 14 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#include "implicitconverterlibraryitem.h"
#include "implicitconverterinfo.h"


ImplicitConverterLibraryItem::ImplicitConverterLibraryItem(const ImplicitConverterInfo * implicitConverterInfo, const PackageInfo * packageInfo)
	:	BaseLibraryItem(implicitConverterInfo, packageInfo)
{
}

const ImplicitConverterInfo * ImplicitConverterLibraryItem::getImplicitConverterInfo() const
{
	return (const ImplicitConverterInfo * )BaseLibraryItem::getBaseInfo();
}
