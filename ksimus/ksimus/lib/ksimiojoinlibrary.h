/***************************************************************************
                          ksimiojoinlibrary.h  -  description
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

#ifndef KSIMIOJOINLIBRARY_H
#define KSIMIOJOINLIBRARY_H

#include "baselibrary.h"
#include "ksimiojoininfo.h"

class KSimIoJoin;
class KSimIoComponent;


/**The library for all io joins

  *@author Rasmus Diekenbrock
  */

class KSimIoJoinLibrary : public BaseLibrary
{
   Q_OBJECT

public:
	KSimIoJoinLibrary();
	~KSimIoJoinLibrary();

	bool insert (const KSimIoJoinInfo * ji, const PackageInfo * packageInfo);
	bool insert (const KSimIoJoinInfoList & jil, const PackageInfo * packageInfo);
	
	bool create(KSimIoJoin * * join, const char * libName, KSimIoComponent * comp) const;

};

#endif
