/***************************************************************************
                          implicitconverterlibrary.h  -  description
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

#ifndef IMPLICITCONVERTERLIBRARY_H
#define IMPLICITCONVERTERLIBRARY_H

#include "baselibrary.h"
#include "implicitconverterinfo.h"

/**Library for all implicit converter
  *@author Rasmus Diekenbrock
  */

class ImplicitConverterLibraryItem;

class ImplicitConverterLibrary : public BaseLibrary
{
   Q_OBJECT

public: 
	ImplicitConverterLibrary();
	~ImplicitConverterLibrary();

	bool insert (const ImplicitConverterInfo * implicitConverterInfo, const PackageInfo * packageInfo);
	bool insert (const ImplicitConverterInfoList & implicitConverterInfoList, const PackageInfo * packageInfo);
	
	bool create(ImplicitConverter * * implicitConverter, ConnectorBase * connector, const QString & libName) const;
	
	/** Return the ImplicitConverterInfo for the given data types.
		Returns 0, if no comination is not found in the library */
	const ImplicitConverterInfo * findDataType(const QString & inDataType, const QString & outDataType) const;

protected:
	QDict<ImplicitConverterLibraryItem> * m_dataTypes;

};

#endif
