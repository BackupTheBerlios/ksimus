/***************************************************************************
                          connectorlibraryitem.h  -  description
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

#ifndef CONNECTORLIBRARYITEM_H
#define CONNECTORLIBRARYITEM_H

#include "baselibraryitem.h"

/**The library item for connectors
  *@author Rasmus Diekenbrock
  */

class ConnectorInfo;

class ConnectorLibraryItem : public BaseLibraryItem
{

public:
	ConnectorLibraryItem(const ConnectorInfo * ci, const PackageInfo * packageInfo);
	
	const ConnectorInfo * getConnectorInfo() const;
	
protected:

};


#endif
