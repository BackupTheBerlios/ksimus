/***************************************************************************
                          iodevicevirtualbool.cpp  -  description
                             -------------------
    begin                : Sun May 18 2003
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
#include <qvbox.h>
#include <qlabel.h>

// KDE includes
#include <klocale.h>
#include <kmessagebox.h>
#include <kconfig.h>

// KSimus includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimiodeviceinfo.h"
#include "ksimus/ksimiojoinboolin.h"
#include "ksimus/ksimiojoinboolout.h"

// Project includes
#include "iodevicevirtualbool.h"

// Forward declaration


//###############################################################

namespace KSimLibBoolean
{


KSimIoDevice * IoDeviceVirtualBool::create(const KSimIoDeviceInfo * info)
{
	return new IoDeviceVirtualBool(info);
}

const KSimIoDeviceInfo * IoDeviceVirtualBool::getStaticInfo()
{
//	KSimIoDeviceInfo(const QString & name,
//	                 const QString & i18nLibName,
//	                 const QString & libName,
//	                 const QString & additionalI18nLibNames,
//	                 KSimIoDevice * (*factory)(const KSimIoDeviceInfo *),
//	                 const QString & shortDescr = QString::null,
//	                 const QString & HTMLDescr = QString::null,
//	                 const QString & oldLibNames = QString::null );

	static const KSimIoDeviceInfo Info(QString::fromLatin1("Virtual Bool IO Device"),
	                                   i18n("Virtual Boolean IO Device"),
	                                   QString::fromLatin1("Virtual Bool IO Device"),
	                                   QString::null,
	                                   create );
	return &Info;
}
                

// Definition:
// Pin ID = 0 ==> bool input  1
// Pin ID = 1 ==> bool output 1
// Pin ID = 2 ==> bool input  2
// Pin ID = 3 ==> bool output 2
//  .
//  .
//  .

#define DEFAULT_CHANNEL_COUNT    1
#define MAX_CHANNEL_COUNT        1024

unsigned int IoDeviceVirtualBool::tempConnCount;
IoDeviceVirtualBool * IoDeviceVirtualBool::tempConnCountOwner = (IoDeviceVirtualBool *)0;

IoDeviceVirtualBool::IoDeviceVirtualBool(const KSimIoDeviceInfo * info)
	:	KSimIoDevice(info),
		m_maxPinID(0)
{
	KSIMDEBUG("IoDeviceVirtualBool::IoDeviceVirtualBool");
	setChannelCount(DEFAULT_CHANNEL_COUNT);

//	setExclusive(true);
}

IoDeviceVirtualBool::~IoDeviceVirtualBool()
{
	KSIMDEBUG("IoDeviceVirtualBool::~IoDeviceVirtualBool");
}

void IoDeviceVirtualBool::setIO(int ioPinID, const void * pValue)
{
	if (!pValue)
	{
		ASSERT(pValue != (const void *)0);
		return;
	}

	if (ioPinID >= 0 && ioPinID <= m_maxPinID)
	{
		// ioPinID okay
		if (ioPinID & 0x0001)
		{
			int i = ioPinID / 2;  // Get index
			m_bitArray[i] = *(const bool *)pValue;
		}
		else
		{
			KSIMDEBUG_VAR("Pin is not a output", ioPinID);
		}
	}
}

void IoDeviceVirtualBool::getIO(int ioPinID, void * pValue) const
{
	if (!pValue)
	{
		ASSERT(pValue != (void *)0);
		return;
	}

	if (ioPinID >= 0 && ioPinID <= m_maxPinID)
	{
		int i = ioPinID / 2;  // Get index
		*(bool *)pValue = m_bitArray[i];
	}
}

void IoDeviceVirtualBool::setChannelCount(unsigned int count)
{
	unsigned int i;
	unsigned int minCount = getMinChannelCount();
	if (count < minCount)
	{
		count = minCount;
		KSIMDEBUG(QString::fromLatin1("count < minCount (%1,%2)").arg(count).arg(minCount));
	}

	// First remove not longer needed pins
	while((i = getChannelCount()) > count)
	{
		deletePin((int) i*2-1);
		deletePin((int) i*2-2);
	}


	// now add missing pins
	while((i = getChannelCount()) < count)
	{
		KSimIoPin * pin;
		pin = new KSimIoPin(this, i*2, QString::fromLatin1("Input %1").arg(i),
		                    QString::fromLatin1("Input %1").arg(i+1));    // TODO: pin names
		CHECK_PTR(pin);
		pin->addPinInfo(KSimIoJoinBoolIn::getStaticInfo());
		addPin(pin);

		pin = new KSimIoPin(this, i*2+1, QString::fromLatin1("Output %1").arg(i),
		                    QString::fromLatin1("Output %1").arg(i+1));    // TODO: pin names
		CHECK_PTR(pin);
		pin->addPinInfo(KSimIoJoinBoolOut::getStaticInfo());
		addPin(pin);
	}

	m_maxPinID = count * 2 - 1;
	m_bitArray.resize(count);
}

void IoDeviceVirtualBool::setChannelCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

unsigned int IoDeviceVirtualBool::getChannelCount() const
{
	unsigned int i = getPinList().count() / 2;
	KSIMDEBUG_VAR("IoDeviceVirtualBool::getChannelCount()", i);
	return getPinList().count() / 2;
}

unsigned int IoDeviceVirtualBool::getMinChannelCount() const
{
	int i = getPinList().count() - 1; // Max ID

	while(!isPinUsed(i) && (i > 0))
	{
		i--;
	}

	KSIMDEBUG_VAR("IoDeviceVirtualBool::getMinChannelCount()", (i / 2)+1);
	return (i / 2)+1;
}

void IoDeviceVirtualBool::save(KConfigBase & config) const
{
	KSimIoDevice::save(config);
	
	config.writeEntry("Channel Count", getChannelCount());
}

bool IoDeviceVirtualBool::load(KConfigBase & config)
{
	// Load pin cout first, then load pin properties (via KSimIoDevice::load)
	unsigned int count = config.readUnsignedNumEntry("Channel Count", DEFAULT_CHANNEL_COUNT);
	setChannelCount(count);

	return KSimIoDevice::load(config);
}

KSimIoDevicePropertyBaseWidget * IoDeviceVirtualBool::createGeneralProperty(QWidget *parent)
{
	IoDeviceVirtualBoolPropertyGeneralWidget * wid;
	wid = new IoDeviceVirtualBoolPropertyGeneralWidget(this, parent, "IoDeviceVirtualBool General Settings");
	CHECK_PTR(wid);

	return wid;
}

void IoDeviceVirtualBool::menuExecuted()
{
	KSimIoDevice::menuExecuted();

	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (IoDeviceVirtualBool *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


IoDeviceVirtualBoolPropertyGeneralWidget::IoDeviceVirtualBoolPropertyGeneralWidget(IoDeviceVirtualBool * device, QWidget *parent, const char *name)
	:	KSimIoDevicePropertyGeneralWidget(device, parent, name)
{
	QString tip;

	m_channelsLabel = new QLabel(i18n("Boolean", "Number of channels: "), this, "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);

	m_channels = new KSimSpinBox(device->getMinChannelCount(), MAX_CHANNEL_COUNT, 1, this, "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("Boolean", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);


	// Setup value
	m_channels->setValue(device->getChannelCount());
};

//	IoDeviceVirtualBoolPropertyGeneralWidget::~IoDeviceVirtualBoolPropertyGeneralWidget();


void IoDeviceVirtualBoolPropertyGeneralWidget::acceptPressed()
{
	KSimIoDevicePropertyGeneralWidget::acceptPressed();

	if (getIoDeviceVirtualBool()->getChannelCount() != (unsigned int)m_channels->value())
	{
		changeData();
		getIoDeviceVirtualBool()->setChannelCountDelayed(m_channels->value());
	}
}

void IoDeviceVirtualBoolPropertyGeneralWidget::defaultPressed()
{
	KSimIoDevicePropertyGeneralWidget::defaultPressed();
	m_channels->setValue(QMAX(DEFAULT_CHANNEL_COUNT, getIoDeviceVirtualBool()->getMinChannelCount()));
}

#undef DEFAULT_CHANNEL_COUNT
#undef MAX_CHANNEL_COUNT


//###############################################################

}; //namespace KSimLibBoolean



