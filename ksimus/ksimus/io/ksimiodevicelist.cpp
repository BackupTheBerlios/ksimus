/***************************************************************************
                          ksimiodevicelist.cpp  -  description
                             -------------------
    begin                : Sat Apr 5 2003
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

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "ksimiodevicelist.h"
#include "ksimiostorage.h"

// Forward declaration



KSimIoDeviceList::KSimIoDeviceList()
{
}

KSimIoDeviceList::~KSimIoDeviceList()
{
}


KSimIoDevice * KSimIoDeviceList::findByName(const QString & deviceName) const
{
	FOR_EACH_IO_DEVICE(it, *this)
	{
		if (it.current()->getName() == deviceName)
		{
			// Found
			return it.current();
		}
	}
	// Not found
	return (KSimIoDevice *)0;
}


KSimIoDeviceList * KSimIoDeviceList::getList()
{
	static KSimIoDeviceList * list = (KSimIoDeviceList *)0;

	if (list == (KSimIoDeviceList *)0)
	{
		list = new KSimIoDeviceList();
		CHECK_PTR(list);
		list->setAutoDelete(true);
	}
	return list;
}

bool KSimIoDeviceList::loadActive()
{
	KSimIoStorage storage;

	storage.open(true); // read only
	bool res = storage.loadAll(this);
	storage.close();

	return res;
}

void KSimIoDeviceList::checkCircuit(const KSimusDoc * doc, QStringList & errorMsg)
{
	FOR_EACH_IO_DEVICE(it, *this)
	{
		it.current()->checkCircuit(doc, errorMsg);
	}

}

