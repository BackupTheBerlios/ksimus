/***************************************************************************
                          ksimiostorage.cpp  -  description
                             -------------------
    begin                : Mon Apr 7 2003
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
#include <qstring.h>

// KDE includes
#include <kstandarddirs.h>
#include "ksimpleconfig.h"

// Project includes
#include "ksimiostorage.h"
#include "ksimiodevice.h"
#include "ksimiodevicelist.h"
#include "ksimdebug.h"
#include "library.h"
#include "ksimiodevicelibrary.h"
#include "ksimiodeviceinfo.h"

// Forward declaration

// file name
const char * const KSimIoStorage::s_filename   = "ksimus/devices";
const char * const KSimIoStorage::s_type       = "data";
// entries
const char * const KSimIoStorage::s_storage     = "/Devices/";
const char * const KSimIoStorage::s_deviceList = "Device List";
const char * const KSimIoStorage::s_lastSerial = "Last Serial";
// devices
const char * const KSimIoStorage::s_deviceDir  = "/Device %1/";
const char * const KSimIoStorage::s_deviceType = "Type";



KSimIoStorage::KSimIoStorage()
	:	m_conf((KSimpleConfig *)0),
		m_readOnly(true)
{
}

KSimIoStorage::~KSimIoStorage()
{
	if (m_conf != (KSimpleConfig*)0)
	{
		close();
	}
}

bool KSimIoStorage::open(bool readOnly)
{
	Q_ASSERT(m_conf == (KSimpleConfig*)0);
	
	QString filename(locateLocal(s_type, QString::fromLatin1(s_filename)));
//	KSIMDEBUG_VAR("", filename);
	m_conf = new KSimpleConfig(filename, readOnly);
	Q_CHECK_PTR(m_conf);
	m_conf->setGroup(s_storage);
	m_readOnly = readOnly;
	return true;
}

void KSimIoStorage::close()
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);

	delete m_conf;
	m_conf = (KSimpleConfig *)0;
	m_readOnly = true;
}

void KSimIoStorage::loadSerialList()
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);

	bool ok = true;
	m_deviceSerialList.clear();
	m_conf->setGroup(s_storage);
	QStringList serialList = m_conf->readListEntry(s_deviceList);
	for (QStringList::ConstIterator it = serialList.begin(); it != serialList.end(); ++it )
	{
		unsigned int u = (*it).toUInt(&ok);
		if (ok)
		{
			m_deviceSerialList.append(u);
		}
		else
		{
			KSIMDEBUG_VAR("FIX ME: QString is not a uint", *it);
		}
	}
}

void KSimIoStorage::saveSerialList() const
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);
	Q_ASSERT(!isReadOnly());

	QStringList serialList;
	QValueList<unsigned int>::ConstIterator it;
	for (it = m_deviceSerialList.begin(); it != m_deviceSerialList.end(); ++it)
	{
		serialList.append(QString::number(*it));
	}
	m_conf->setGroup(s_storage);
	m_conf->writeEntry(s_deviceList, serialList);
}

bool KSimIoStorage::existDeviceSerial(unsigned int lastSerial) const
{
	// Check existing devices
	FOR_EACH_IO_DEVICE(it, *KSimIoDeviceList::getList())
	{
		if (it.current()->getSerial() == lastSerial)
		{
			// Found ID ==> try next ID
			return true;
		}
	}
	return false;
}

unsigned int KSimIoStorage::getNextSerial()
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);
	Q_ASSERT(!isReadOnly());

	m_conf->setGroup(s_storage);
	unsigned int lastSerial = m_conf->readUnsignedNumEntry(s_lastSerial, 0);
	loadSerialList();

	do
	{
		lastSerial++;
		// Check existing devices
		if (existDeviceSerial(lastSerial)) continue;
		// Check stored devices
		if (-1 != m_deviceSerialList.findIndex(lastSerial)) continue;
	}
	while (lastSerial == 0);

	m_conf->setGroup(s_storage);
	m_conf->writeEntry(s_lastSerial, lastSerial);

	return lastSerial;
}

bool KSimIoStorage::load(unsigned int serial, KSimIoDevice * * device)
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);
	Q_ASSERT(device != (KSimIoDevice * *)0);
	Q_ASSERT(serial != 0);

	QString group(QString::fromLatin1(s_deviceDir).arg(serial));

	if (!m_conf->hasGroup(group))
	{
		KSIMDEBUG_VAR("IO Device not in storgage!", serial);
		return false;
	}

	
	m_conf->setGroup(group);

	QString type = m_conf->readEntry(s_deviceType);

	if (*device == (KSimIoDevice *)0)
	{
		// Device does not exist ==> create it
		bool res = g_library->getIoDeviceLib()->create(device, type);
		if (!res)
		{
			KSIMDEBUG_VAR("Create IO Device failed", type);
			return false;
		}
		Q_CHECK_PTR(*device);
	}
	else
	{
		// Device exist ==> check type
		if ((*device)->getInfo()->getLibName() == type)
		{
			// Same device type ==> reload
			
		}
		else
		{
			// Different type ==> failed
			KSIMDEBUG(QString::fromLatin1("Stored dev type (%1) don't match given type (%2)")
			                .arg(type).arg((*device)->getInfo()->getLibName()));

			return false;
		}
	}

	// Load commons
	(*device)->setSerial(serial);

	// Load device specifics
	bool res = (*device)->load(*m_conf);

//	(*device)->addPinsToPool(); Done by device now // TODO remove comment

	return res;
}

bool KSimIoStorage::loadAll(KSimIoDeviceList * list)
{
	Q_ASSERT(list != (KSimIoDeviceList*)0);
	Q_ASSERT(m_conf != (KSimpleConfig*)0);

	loadSerialList();

	bool commonRes = true;

	// Load stored devices
	for (QValueList<unsigned int>::ConstIterator it = m_deviceSerialList.begin(); it != m_deviceSerialList.end(); ++it )
	{
		KSimIoDevice * device = (KSimIoDevice *)0;

		commonRes &= load(*it, &device);
		if (device != (KSimIoDevice *)0)
		{
			list->append(device);
		}
	}
	return commonRes;
}

void KSimIoStorage::save(KSimIoDevice * device)
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);
	Q_ASSERT(device != (KSimIoDevice*)0);
	Q_ASSERT(!isReadOnly());

	if (device->getSerial() == 0)
	{
		// Set serial
		device->setSerial(getNextSerial());
	}

	// Set Dir
	m_conf->setGroup(QString::fromLatin1(s_deviceDir).arg(device->getSerial()));

	m_conf->writeEntry(s_deviceType, device->getInfo()->getLibName());

	// save device properties
	device->save(*m_conf);

	// Maintain serial list
	loadSerialList();
	if (-1 == m_deviceSerialList.findIndex(device->getSerial()))
	{
		// Serial ID not found
		// Add serial ID
		m_deviceSerialList.append(device->getSerial());
		saveSerialList();
	}
}

void KSimIoStorage::remove(KSimIoDevice * device)
{
	Q_ASSERT(m_conf != (KSimpleConfig*)0);
	Q_ASSERT(device != (KSimIoDevice*)0);
	Q_ASSERT(!isReadOnly());

	QString devDir = QString::fromLatin1(s_deviceDir).arg(device->getSerial());

	QStringList grpList = m_conf->groupList();

	for (QStringList::ConstIterator it = grpList.begin(); it != grpList.end(); ++it)
	{
		QStringList dirParts = QStringList::split('/', *it);
		QString first = '/' + dirParts.first() + '/';
		KSIMDEBUG(QString::fromLatin1("Dir '%1' First '%2'").arg(*it).arg(first));
		if (first == devDir)
		{
			m_conf->deleteGroup(*it);
		}
	}

//	m_conf->setGroup(s_storage);
	loadSerialList();
	m_deviceSerialList.remove(device->getSerial());
	saveSerialList();
}
