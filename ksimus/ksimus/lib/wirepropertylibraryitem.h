/***************************************************************************
                          wirepropertylibraryitem.h  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#ifndef WIRELIBRARYITEM_H
#define WIRELIBRARYITEM_H

#include "baselibraryitem.h"

/**Library Item for wire properties
  *@author Rasmus Diekenbrock
  */

class WirePropertyInfo;

class WirePropertyLibraryItem : public BaseLibraryItem  {
public: 
	WirePropertyLibraryItem(const WirePropertyInfo * wireInfo, const PackageInfo * packageInfo);
	
	const WirePropertyInfo * getWirePropertyInfo() const;
	
protected:
};


#endif
