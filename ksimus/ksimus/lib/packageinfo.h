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

// Forward declaration



/**Contains information about packages.
  *@author Rasmus Diekenbrock
  */

class PackageInfo
{
public: 
	PackageInfo(const QString m_packageName,
	            KInstance * instance,
	            const char * packageVersion,
	            const ComponentInfoList & componentList,
	            const ConnectorInfoList & connectorList,
	            const WirePropertyInfoList & wirePropertyList,
	            const ImplicitConverterInfoList & implicitConverterList
	);

//	~PackageInfo();

	
	const QString & getPackageName() const;
	KInstance * getInstance() const;
	const char * getPackageVersion() const;
	const ComponentInfoList & getComponentList() const;
	const ConnectorInfoList & getConnectorList() const;
	const WirePropertyInfoList & getWirePropertyList() const;
	const ImplicitConverterInfoList & getImplicitConverterList() const;
	
	
	
	
private:
	const QString m_packageName;
	KInstance * m_instance;
	const char * m_packageVersion;
	const ComponentInfoList & m_componentList;
	const ConnectorInfoList & m_connectorList;
	const WirePropertyInfoList & m_wirePropertyList;
	const ImplicitConverterInfoList & m_implicitConverterList;
};

#endif
