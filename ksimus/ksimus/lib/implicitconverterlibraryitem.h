/***************************************************************************
                          implicitconverterlibraryitem.h  -  description
                             -------------------
    begin                : Wed Aug 14 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#ifndef IMPLICITCONVERTERLIBRARYITEM_H
#define IMPLICITCONVERTERLIBRARYITEM_H

#include "baselibraryitem.h"

/**Library Item for implicit converter
  *@author Rasmus Diekenbrock
  */

class ImplicitConverterInfo;

class ImplicitConverterLibraryItem : public BaseLibraryItem  {
public: 
	ImplicitConverterLibraryItem(const ImplicitConverterInfo * implicitConverterInfo, const PackageInfo * packageInfo);
	
	const ImplicitConverterInfo * getImplicitConverterInfo() const;
	
protected:
};


#endif
