/***************************************************************************
                          baseinfo.cpp  -  description
                             -------------------
    begin                : Sat Sep 30 2000
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

#include "baseinfo.h"


//##########################################################
//##########################################################
// BaseInfo
	
BaseInfo::BaseInfo(eInfoType type,
                   const QString & name,
                   const QString & libName,
                   const QString & shortDescr,
                   const QString & HTMLDescr,
                   const QString & oldLibNames )
	:	m_type (type),
		m_name (name),
		m_libName(libName),
		m_shortDescr(shortDescr),
		m_HTMLDescr(HTMLDescr),
		m_oldLibNames(oldLibNames)
{
}
	
/*BaseInfo::eInfoType BaseInfo::getType() const
{
	return m_type;
}
const QString & BaseInfo::getName() const
{
	return m_name;
}
const QString & BaseInfo::getLibName() const
{
	return m_libName;
}
const QString & BaseInfo::getOldLibNames() const
{
	return m_oldLibNames;
}
const QString & BaseInfo::getShortDescr() const
{
	return m_shortDescr;
}
const QString & BaseInfo::getHTMLDescr() const
{
	return m_HTMLDescr;
} TODO remove */







