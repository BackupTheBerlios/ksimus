/***************************************************************************
                          ksimiodevicelibraryitem.h  -  description
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

#ifndef KSIMIODEVICELIBRARYITEM_H
#define KSIMIODEVICELIBRARYITEM_H

#include "baselibraryitem.h"

/**The library item for io devices
  *@author Rasmus Diekenbrock
  */

class KSimIoDeviceInfo;

class KSimIoDeviceLibraryItem : public BaseLibraryItem
{

public:
	KSimIoDeviceLibraryItem(const KSimIoDeviceInfo * ci, const PackageInfo * packageInfo);
	
	const KSimIoDeviceInfo * getKSimIoDeviceInfo() const { return (const KSimIoDeviceInfo * )BaseLibraryItem::getBaseInfo(); };
	
protected:

};


#endif
