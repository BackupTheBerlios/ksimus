/***************************************************************************
                          packageinfo.cpp  -  description
                             -------------------
    begin                : Thu Dec 27 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "config.h"
#include "packageinfo.h"

// Forward declaration


class PackageInfo::Private
{
public:
	Private(const QString _packageName,
	        KInstance * _instance,
	        const char * _packageVersion)
	:	packageName(_packageName),
		instance(_instance),
		packageVersion(_packageVersion),
		componentList(0),
		connectorList(0),
		wirePropertyList(0),
		implicitConverterList(0),
		ioDeviceList(0),
		ioJoinList(0)
	{
	};

	const QString packageName;
	KInstance * instance;
	const char * packageVersion;
	const ComponentInfoList * componentList;
	const ConnectorInfoList * connectorList;
	const WirePropertyInfoList * wirePropertyList;
	const ImplicitConverterInfoList * implicitConverterList;
	const KSimIoDeviceInfoList * ioDeviceList;
	const KSimIoJoinInfoList * ioJoinList;
};


//#############################################################
//#############################################################


PackageInfo::PackageInfo(const char * packageName,
                         KInstance * instance,
                         const char * packageVersion)
{
	m_p = new Private(QString::fromLatin1(packageName), instance, packageVersion);
	Q_CHECK_PTR(m_p);
}

PackageInfo::~PackageInfo()
{
	delete m_p;
}

const QString & PackageInfo::getPackageName() const
{
	return m_p->packageName;
}

KInstance * PackageInfo::getInstance() const
{
	return m_p->instance;
}

const char * PackageInfo::getPackageVersion() const
{
	return m_p->packageVersion;
}



void PackageInfo::insert(const ComponentInfoList & componentList)
{
	m_p->componentList = &componentList;
}

const ComponentInfoList & PackageInfo::getComponentList() const
{
	Q_ASSERT(hasComponentList());
	return *m_p->componentList;
}

bool PackageInfo::hasComponentList() const
{
	return m_p->componentList != 0;
}



void PackageInfo::insert(const ConnectorInfoList & connectorList)
{
	m_p->connectorList = &connectorList;
}

const ConnectorInfoList & PackageInfo::getConnectorList() const
{
	Q_ASSERT(hasConnectorList());
	return *m_p->connectorList;
}

bool PackageInfo::hasConnectorList() const
{
	return m_p->connectorList != 0;
}



void PackageInfo::insert(const WirePropertyInfoList & wirePropertyList)
{
	m_p->wirePropertyList = &wirePropertyList;
}

const WirePropertyInfoList & PackageInfo::getWirePropertyList() const
{
	Q_ASSERT(hasWirePropertyList());
	return *m_p->wirePropertyList;
}

bool PackageInfo::hasWirePropertyList() const
{
	return m_p->wirePropertyList != 0;
}



void PackageInfo::insert(const ImplicitConverterInfoList & implicitConverterList)
{
	m_p->implicitConverterList = &implicitConverterList;
}

const ImplicitConverterInfoList & PackageInfo::getImplicitConverterList() const
{
	Q_ASSERT(hasImplicitConverterList());
	return *m_p->implicitConverterList;
}

bool PackageInfo::hasImplicitConverterList() const
{
	return m_p->implicitConverterList != 0;
}


void PackageInfo::insert(const KSimIoDeviceInfoList & ioDeviceList)
{
	m_p->ioDeviceList = &ioDeviceList;
}

const KSimIoDeviceInfoList & PackageInfo::getKSimIoDeviceList() const
{
	Q_ASSERT(hasKSimIoDeviceList());
	return *m_p->ioDeviceList;
}

bool PackageInfo::hasKSimIoDeviceList() const
{
	return m_p->ioDeviceList != 0;
}



void PackageInfo::insert(const KSimIoJoinInfoList & ioJoinList)
{
	m_p->ioJoinList = &ioJoinList;
}

const KSimIoJoinInfoList & PackageInfo::getKSimIoJoinList() const
{
	Q_ASSERT(hasKSimIoJoinList());
	return *m_p->ioJoinList;
}

bool PackageInfo::hasKSimIoJoinList() const
{
	return m_p->ioJoinList != 0;
}




