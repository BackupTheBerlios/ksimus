/***************************************************************************
                          ksimiojoinlibraryitem.h  -  description
                             -------------------
    begin                : Fri Apr 18 2003
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

#ifndef KSIMIOJOINLIBRARYITEM_H
#define KSIMIOJOINLIBRARYITEM_H

#include "baselibraryitem.h"

/**The library item for io joins
  *@author Rasmus Diekenbrock
  */

class KSimIoJoinInfo;

class KSimIoJoinLibraryItem : public BaseLibraryItem
{

public:
	KSimIoJoinLibraryItem(const KSimIoJoinInfo * ji, const PackageInfo * packageInfo);
	
	const KSimIoJoinInfo * getKSimIoJoinInfo() const { return (const KSimIoJoinInfo * )BaseLibraryItem::getBaseInfo(); };
	
protected:

};


#endif
