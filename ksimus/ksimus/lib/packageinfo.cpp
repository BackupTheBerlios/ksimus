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


PackageInfo::PackageInfo(const QString packageName,
                         KInstance * instance,
                         const char * packageVersion,
                         const ComponentInfoPtr * componentList,
                         const ConnectorInfoPtr * connectorList,
                         const WirePropertyInfoPtr * wirePropertyList)
	:	m_packageName(packageName),
		m_instance(instance),
		m_packageVersion(packageVersion),
		m_componentList(componentList),
		m_connectorList(connectorList),
		m_wirePropertyList(wirePropertyList)
{
}

/*PackageInfo::~PackageInfo(){
} */

const QString & PackageInfo::getPackageName() const
{
	return m_packageName;
}

KInstance * PackageInfo::getInstance() const
{
	return m_instance;
}

const char * PackageInfo::getPackageVersion() const
{
	return m_packageVersion;
};

const ComponentInfoPtr * PackageInfo::getComponentList() const
{
	return m_componentList;
};

const ConnectorInfoPtr * PackageInfo::getConnectorList() const
{
	return m_connectorList;
};

const WirePropertyInfoPtr * PackageInfo::getWirePropertyList() const
{
	return m_wirePropertyList;
};


