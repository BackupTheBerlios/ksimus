/***************************************************************************
                          ksimiopin.cpp  -  description
                             -------------------
    begin                : Sun Mar 30 2003
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

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "ksimiopin.h"
#include "ksimiodevice.h"
#include "ksimdebug.h"

// Forward declaration


KSimIoPin::KSimIoPin(KSimIoDevice * parentDevice, int pinID, const QString & name, const QString & i18nName)
	:	QObject(parentDevice,name.latin1()),
		m_parentDevice(parentDevice),
		m_pinID(pinID),
		m_defaultName(name),
		m_defaultI18nName(i18nName),
		m_name(QString::null)
{
	KSIMDEBUG_VAR("KSimIoPin::KSimIoPin", getName());
}

KSimIoPin::~KSimIoPin()
{
	KSIMDEBUG_VAR("KSimIoPin::~KSimIoPin", getName());
}

void KSimIoPin::addPinInfo(const KSimIoJoinInfo * joinInfo)
{
	m_joinInfoList.append(joinInfo);
}

void KSimIoPin::setName(const QString & name)
{
	if (name == getDefaultI18nName())
	{
		m_name = QString::null;
	}
	else
	{
		m_name = name;
	}
}

QString KSimIoPin::getName() const
{
	if (m_name == QString::null)
	{
		return getDefaultI18nName();
	}
	return m_name;
}


//################################################################################
//################################################################################


const KSimIoPin * KSimIoPin::List::find(const QString & deviceName, int pinID) const
{
	FOR_EACH_IO_PIN(it, *this)
	{
		if ( (it.current()->getPinID() == pinID)
		  && (it.current()->getDevice()->getName() == deviceName))
		{
			//Found
			return it.current();
		}
	}

	// Not found
	return (const KSimIoPin *)0;
}


//################################################################################
//################################################################################


KSimIoPin::Pool & KSimIoPin::Pool::get()
{
	static Pool sPool = Pool();
	return sPool;
}

