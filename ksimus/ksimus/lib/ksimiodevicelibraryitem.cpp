/***************************************************************************
                          ksimiodevicelibraryitem.cpp  -  description
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


#include "ksimiodevicelibraryitem.h"
#include "ksimiodeviceinfo.h"

KSimIoDeviceLibraryItem::KSimIoDeviceLibraryItem(const KSimIoDeviceInfo * di, const PackageInfo * packageInfo)
	:	BaseLibraryItem(di, packageInfo)
{
}

/*const KSimIoDeviceInfo * KSimIoDeviceLibraryItem::getKSimIoDeviceInfo() const
{
 return (const KSimIoDeviceInfo * )BaseLibraryItem::getBaseInfo();
} */
