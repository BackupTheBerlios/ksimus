/***************************************************************************
                          connectorinfo.cpp  -  description
                             -------------------
    begin                : Sun Oct 1 2000
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

#include "connectorinfo.h"

ConnectorInfo::ConnectorInfo(	const QString & name,
								const QString & libName,
								const QString & dataType,
								ConnectorBase * (*factory)(Component* , const QString & , const QString & , const QPoint&),
								const QString & shortDescr,
								const QString & HTMLDescr,
								const QString & oldLibNames)
	:	BaseInfo(INFO_CONNECTOR, name, libName, shortDescr, HTMLDescr, oldLibNames),
		m_dataType(dataType),
		m_factory(factory)
{
}

const QString & ConnectorInfo::getDataType() const
{
	return m_dataType;
}

ConnectorBase * ConnectorInfo::create(Component* comp , const QString & name ,
                                      const QString & i18nName, const QPoint& pos) const
{
	CHECK_PTR(m_factory);
	return m_factory(comp, name, i18nName, pos);
}	
