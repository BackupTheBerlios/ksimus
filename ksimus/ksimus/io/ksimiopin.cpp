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
#include <kconfig.h>

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
		m_name(QString::null),
		m_selectedJoinInfo(0)
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
	if (m_selectedJoinInfo == 0)
	{
		m_selectedJoinInfo = joinInfo; // First is default
	}
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


void KSimIoPin::setSelectedJoinInfo(const KSimIoJoinInfo * joinInfo)
{
	ASSERT(joinInfo != 0);
	if (getJoinInfoList().containsRef(joinInfo))
	{
		m_selectedJoinInfo = joinInfo;
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("JoinInfo not found (Join %1, Device %2, Pin %3)")
		                              .arg(joinInfo->getLibName())
		                              .arg(getDevice()->getName())
		                              .arg(getName()));
	}
}


void KSimIoPin::save(KConfigBase & config) const
{
	config.writeEntry("Name", getName());

	if (getJoinInfoList().count() > 1)
	{
		config.writeEntry("Type", getSelectedJoinInfo()->getLibName());
	}
}

bool KSimIoPin::load(KConfigBase & config)
{
	bool found = true;

	setName(config.readEntry("Name", getName()));

	if (config.hasKey("Type"))
	{
		found = false;
		QString type = config.readEntry("Type");
		FOR_EACH_IO_JOIN_INFO(it, getJoinInfoList())
		{
			if (it.current()->getLibName() == type)
			{
				m_selectedJoinInfo = it.current();
				found = true;
				break;
			}
		}
	}

	return found;
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

#include "ksimiopin.moc"
