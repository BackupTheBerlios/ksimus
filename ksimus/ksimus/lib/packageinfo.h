/***************************************************************************
                          packageinfo.h  -  description
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

#ifndef PACKAGEINFO_H
#define PACKAGEINFO_H

// C-Includes


// QT-Includes
#include <qstring.h>

// KDE-Includes
#include <kinstance.h>

// Project-Includes
#include "componentinfo.h"
#include "connectorinfo.h"
#include "wirepropertyinfo.h"
#include "implicitconverterinfo.h"
#include "ksimiodeviceinfo.h"
#include "ksimiojoininfo.h"

// Forward declaration


/**Contains information about packages.
  *@author Rasmus Diekenbrock
  */

class PackageInfo
{
class Private;

public:
	PackageInfo(const char * packageName,
	            KInstance * instance,
	            const char * packageVersion
	);

	~PackageInfo();

	
	const QString & getPackageName() const;
	KInstance * getInstance() const;
	const char * getPackageVersion() const;

	void insert(const ComponentInfoList & componentList);
	const ComponentInfoList & getComponentList() const;
	bool hasComponentList() const;

	void insert(const ConnectorInfoList & connectorList);
	const ConnectorInfoList & getConnectorList() const;
	bool hasConnectorList() const;

	void insert(const WirePropertyInfoList & wirePropertyList);
	const WirePropertyInfoList & getWirePropertyList() const;
	bool hasWirePropertyList() const;

	void insert(const ImplicitConverterInfoList & implicitConverterList);
	const ImplicitConverterInfoList & getImplicitConverterList() const;
	bool hasImplicitConverterList() const;
	
	void insert(const KSimIoDeviceInfoList & ioDeviceList);
	const KSimIoDeviceInfoList & getKSimIoDeviceList() const;
	bool hasKSimIoDeviceList() const;

	void insert(const KSimIoJoinInfoList & ioJoinList);
	const KSimIoJoinInfoList & getKSimIoJoinList() const;
	bool hasKSimIoJoinList() const;


private:
	Private * m_p;
};

#endif
