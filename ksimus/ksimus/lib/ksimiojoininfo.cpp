/***************************************************************************
                          ksimiojoininfo.cpp  -  description
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

#include "ksimiojoininfo.h"


KSimIoJoinInfo::KSimIoJoinInfo(const QString & name,
                               const QString & i18nLibName,
                               const QString & libName,
                               const QString & additionalI18nLibNames,
                               KSimIoJoin * (*factory)(KSimIoComponent *, const KSimIoJoinInfo *),
                               const QString & shortDescr,
                               const QString & HTMLDescr,
                               const QString & oldLibNames)
	:	BaseInfo(INFO_IO_JOIN, name, libName, shortDescr, HTMLDescr, oldLibNames),
		m_i18nLibName(i18nLibName),
		m_additionalI18nLibNames(additionalI18nLibNames),
		m_factory(factory)
{
}

KSimIoJoin * KSimIoJoinInfo::create(KSimIoComponent * comp, const KSimIoJoinInfo * info) const
{
	Q_CHECK_PTR(m_factory);
	return m_factory(comp, info);
}	
