/***************************************************************************
                          ksimiojoinlibraryitem.cpp  -  description
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


#include "ksimiojoinlibraryitem.h"
#include "ksimiojoininfo.h"

KSimIoJoinLibraryItem::KSimIoJoinLibraryItem(const KSimIoJoinInfo * ji, const PackageInfo * packageInfo)
	:	BaseLibraryItem(ji, packageInfo)
{
}

/*const KSimIoJoinInfo * KSimIoJoinLibraryItem::getKSimIoJoinInfo() const
{
 return (const KSimIoJoinInfo * )BaseLibraryItem::getBaseInfo();
} */
