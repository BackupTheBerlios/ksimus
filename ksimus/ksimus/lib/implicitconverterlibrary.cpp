/***************************************************************************
                          implicitconverterlibrary.cpp  -  description
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

#include "implicitconverterlibrary.h"
#include "implicitconverterlibraryitem.h"
#include "ksimdebug.h"

ImplicitConverterLibrary::ImplicitConverterLibrary()
{
	m_dataTypes = new QDict<ImplicitConverterLibraryItem>;
	CHECK_PTR(m_dataTypes);
	m_dataTypes->setAutoDelete(true);
}

ImplicitConverterLibrary::~ImplicitConverterLibrary()
{
	delete m_dataTypes;
}

bool ImplicitConverterLibrary::insert (const ImplicitConverterInfo * implicitConverterInfo, const PackageInfo * packageInfo)
{
	bool res;
	
	CHECK_PTR(implicitConverterInfo);
	
	ImplicitConverterLibraryItem * icli = new ImplicitConverterLibraryItem(implicitConverterInfo, packageInfo);
	CHECK_PTR(icli);
	
	res = _insert_(icli);
	
	if (res)
	{
		if (!implicitConverterInfo->getInDataType().isEmpty()
		 && !implicitConverterInfo->getOutDataType().isEmpty())
		{
			m_dataTypes->insert(implicitConverterInfo->getInDataType() + QString::fromLatin1("==>") + implicitConverterInfo->getOutDataType(), icli);
		}
		else
		{
			KSIMDEBUG_VAR("No dataType", implicitConverterInfo->getLibName());
		}
	}
	else
	{
		delete icli;
	}
	
	return res;
}

bool ImplicitConverterLibrary::insert(const ImplicitConverterInfoList & implicitConverterInfoList, const PackageInfo * packageInfo)
{
	bool res = true;
	FOR_EACH_IMPLICITCONVERTER_INFO(it, implicitConverterInfoList)
	{
		res &= insert(it.current(), packageInfo);
	}
	return res;
}
	
bool ImplicitConverterLibrary::create(ImplicitConverter * * implicitConverter, ConnectorBase * connector, const QString & libName) const
{
	const ImplicitConverterInfo * ici = (ImplicitConverterInfo *) findLibName(libName);
	
	if (ici)
	{
		*implicitConverter = ici->create(connector);
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Unknown implicit converter", libName);
		*implicitConverter = (ImplicitConverter *)0;
		return false;
	}
}

/** Return the ImplicitConverterInfo for the given dataType.
	Returns 0, if dataType is not found in the library */
const ImplicitConverterInfo * ImplicitConverterLibrary::findDataType(const QString & inDataType, const QString & outDataType) const
{
	ImplicitConverterLibraryItem * cili = m_dataTypes->find(inDataType + QString::fromLatin1("==>") + outDataType);
	if (cili)
	{
		return cili->getImplicitConverterInfo();
	}
	return 0;
}
