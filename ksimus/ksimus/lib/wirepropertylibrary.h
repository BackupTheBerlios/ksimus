/***************************************************************************
                          wirepropertylibrary.h  -  description
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

#ifndef WIRELIBRARY_H
#define WIRELIBRARY_H

#include "baselibrary.h"
#include "wirepropertyinfo.h"

/**Library for all wire properties
  *@author Rasmus Diekenbrock
  */

class WirePropertyLibraryItem;

class WirePropertyLibrary : public BaseLibrary
{
public: 
	WirePropertyLibrary();
	~WirePropertyLibrary();

	bool insert (const WirePropertyInfo * wireInfo);
	bool insert (const WirePropertyInfoList wireInfoList);
	
	bool create(WireProperty * * wireProperty, Wire * wire, const QString & libName) const;
	
	/** Return the WirePropertyInfo for the given dataType.
		Returns 0, if dataType is not found in the library */
	const WirePropertyInfo * findDataType(const QString & dataType) const;

protected:		
	QDict<WirePropertyLibraryItem> * m_dataTypes;

};

#endif
