/***************************************************************************
                          connectorlibrary.h  -  description
                             -------------------
    begin                : Tue Oct 3 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef CONNECTORLIBRARY_H
#define CONNECTORLIBRARY_H

#include "baselibrary.h"
#include "connectorinfo.h"


/**The library for all connectors

  *@author Rasmus Diekenbrock
  */

class ConnectorLibrary : public BaseLibrary
{

public:
	ConnectorLibrary();
	~ConnectorLibrary();

	bool insert (const ConnectorInfo * ci, const PackageInfo * packageInfo);
	bool insert (const ConnectorInfoList cil, const PackageInfo * packageInfo);
	
	bool create(ConnectorBase * * conn, Component * comp,	const QString & name, const QPoint & pos,const char * libName) const;

};

#endif
