/***************************************************************************
                          ksimiodevicelist.h  -  description
                             -------------------
    begin                : Sam Apr 5 2003
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

#ifndef KSIMIODEVICELIST_H
#define KSIMIODEVICELIST_H

// C/C++ includes

// QT includes
#include <qlist.h>

// KDE includes

// Project includes
#include "ksimiodevice.h"

// Forward declaration



/**A list for KSimIoDevice.
Includes a static list for all used devices.
  *@author Rasmus Diekenbrock
  */

class KSimIoDeviceList : public QList<KSimIoDevice>
{
public: 
	KSimIoDeviceList();
	~KSimIoDeviceList();

	static KSimIoDeviceList * getList();

	KSimIoDevice * findByName(const QString & deviceName) const;

	bool loadActive();

	void checkCircuit(const KSimusDoc * doc, QStringList & errorMsg);
	
};

#define FOR_EACH_IO_DEVICE(_it_,_ioDeviceList_)  \
        for(QListIterator<KSimIoDevice> _it_(_ioDeviceList_);_it_.current();++_it_)


#endif
