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

// Forward declaration



/**Contains infromation about packages.
  *@author Rasmus Diekenbrock
  */

class PackageInfo
{
public: 
	PackageInfo(const QString m_packageName,
	            KInstance * instance,
	            const char * packageVersion,
	            const ComponentInfoPtr * componentList,
	            const ConnectorInfoPtr * connectorList,
	            const WirePropertyInfoPtr * wirePropertyList
);


//	~PackageInfo();

	
	const QString & getPackageName() const;
	KInstance * getInstance() const;
	const char * getPackageVersion() const;
	const ComponentInfoPtr * getComponentList() const;
	const ConnectorInfoPtr * getConnectorList() const;
	const WirePropertyInfoPtr * getWirePropertyList() const;
	
	
	
	
private:
	const QString m_packageName;
	KInstance * m_instance;
	const char * m_packageVersion;
	const ComponentInfoPtr * m_componentList;
	const ConnectorInfoPtr * m_connectorList;
	const WirePropertyInfoPtr * m_wirePropertyList;
};

#endif
