/***************************************************************************
                          connectorlibraryitem.cpp  -  description
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


#include "connectorlibraryitem.h"
#include "connectorinfo.h"

ConnectorLibraryItem::ConnectorLibraryItem(const ConnectorInfo * ci)
	:	BaseLibraryItem(ci)
{
}

const ConnectorInfo * ConnectorLibraryItem::getConnectorInfo() const
{
	return (const ConnectorInfo * )BaseLibraryItem::getBaseInfo();
}
