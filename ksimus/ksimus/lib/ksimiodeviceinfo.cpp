/***************************************************************************
                          ksimiodeviceinfo.cpp  -  description
                             -------------------
    begin                : Fri Apr 4 2003
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

//#include <qapplication.h>

#include "ksimiodeviceinfo.h"


KSimIoDeviceInfo::KSimIoDeviceInfo(const QString & name,
                                   const QString & i18nLibName,
                                   const QString & libName,
                                   const QString & additionalI18nLibNames,
                                   KSimIoDevice * (*factory)(const KSimIoDeviceInfo *),
                                   const QString & shortDescr,
                                   const QString & HTMLDescr,
                                   const QString & oldLibNames)
	:	BaseInfo(INFO_IO_DEVICE, name, libName, shortDescr, HTMLDescr, oldLibNames),
		m_i18nLibName(i18nLibName),
		m_additionalI18nLibNames(additionalI18nLibNames),
		m_factory(factory)
{
}

KSimIoDevice * KSimIoDeviceInfo::create(const KSimIoDeviceInfo * info) const
{
	Q_CHECK_PTR(m_factory);
	return m_factory(info);
}	
