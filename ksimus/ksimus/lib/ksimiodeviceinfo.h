/***************************************************************************
                          ksimiodeviceinfo.h  -  description
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

#ifndef KSIMIODEVICEINFO_H
#define KSIMIODEVICEINFO_H

#include <qstring.h>
#include <qptrlist.h>

#include "baseinfo.h"

/**Information class for connectors
  *@author Rasmus Diekenbrock
  */

class KSimIoDevice;

class KSimIoDeviceInfo : public BaseInfo
{
public: 
	KSimIoDeviceInfo(const QString & name,
	                 const QString & i18nLibName,
	                 const QString & libName,
	                 const QString & additionalI18nLibNames,
	                 KSimIoDevice * (*factory)(const KSimIoDeviceInfo *),
	                 const QString & shortDescr = QString::null,
	                 const QString & HTMLDescr = QString::null,
	                 const QString & oldLibNames = QString::null );

	KSimIoDevice * create(const KSimIoDeviceInfo * info) const;
	
	const QString & getI18nLibName() const { return m_i18nLibName; };
	const QString & getAdditionalI18nLibNames() const { return m_additionalI18nLibNames; };

private:
	const QString m_i18nLibName;
	const QString m_additionalI18nLibNames;
	KSimIoDevice * (*m_factory)(const KSimIoDeviceInfo * info);
};




//################################################################################
//################################################################################




class KSimIoDeviceInfoList : public QPtrList<const KSimIoDeviceInfo>
{
};



//################################################################################
//################################################################################



#define FOR_EACH_IO_DEVICE_INFO(_it_,_ioDeviceInfoList_)	\
		for(QPtrListIterator<const KSimIoDeviceInfo> _it_(_ioDeviceInfoList_);_it_.current();++_it_)

#endif

