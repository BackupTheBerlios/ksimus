/***************************************************************************
                          ksimiodevicelibrary.h  -  description
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

#ifndef KSIMIODEVICELIBRARY_H
#define KSIMIODEVICELIBRARY_H

#include "baselibrary.h"
#include "ksimiodeviceinfo.h"

class KSimIoDevice;


/**The library for all io devices

  *@author Rasmus Diekenbrock
  */

class KSimIoDeviceLibrary : public BaseLibrary
{

public:
	KSimIoDeviceLibrary();
	~KSimIoDeviceLibrary();

	bool insert (const KSimIoDeviceInfo * di, const PackageInfo * packageInfo);
	bool insert (const KSimIoDeviceInfoList & dil, const PackageInfo * packageInfo);
	
	bool create(KSimIoDevice * * device, const char * libName) const;

};

#endif
