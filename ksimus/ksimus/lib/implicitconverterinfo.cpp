/***************************************************************************
                          implicitconverterinfo.cpp  -  description
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

#include <qapplication.h>

#include "implicitconverterinfo.h"


ImplicitConverterInfo::ImplicitConverterInfo(const QString & name,
                                             const QString & libName,
                                             const QString & inDataType,
                                             const QString & outDataType,
                                             ImplicitConverter * (*factory)(ConnectorBase*),
                                             const QString & shortDescr,
                                             const QString & HTMLDescr,
                                             const QString & oldLibNames)
	:	BaseInfo(INFO_IMPLICITCONVERTER, name, libName, shortDescr, HTMLDescr, oldLibNames),
		m_inDataType(inDataType),
		m_outDataType(outDataType),
		m_factory(factory)
{
}

const QString & ImplicitConverterInfo::getInDataType() const
{
	return m_inDataType;
}

const QString & ImplicitConverterInfo::getOutDataType() const
{
	return m_outDataType;
}

ImplicitConverter * ImplicitConverterInfo::create(ConnectorBase * connector) const
{
	CHECK_PTR(m_factory);
	return m_factory(connector);
}
