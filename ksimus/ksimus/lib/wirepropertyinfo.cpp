/***************************************************************************
                          wirepropertyinfo.cpp  -  description
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

#include <qapplication.h>

#include "wirepropertyinfo.h"


WirePropertyInfo::WirePropertyInfo(	const QString & name,
					const QString & libName,
					const QString & dataType,
					WireProperty * (*factory)(Wire*),
					const QString & shortDescr,
					const QString & HTMLDescr,
					const QString & oldLibNames)
	:	BaseInfo(INFO_WIREPROPERTY, name, libName, shortDescr, HTMLDescr, oldLibNames),
		m_dataType(dataType),
		m_factory(factory)
{
}

const QString & WirePropertyInfo::getDataType() const
{
	return m_dataType;
}

WireProperty * WirePropertyInfo::create(Wire * wire) const
{
	CHECK_PTR(m_factory);
	return m_factory(wire);
}	
