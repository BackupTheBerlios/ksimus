/***************************************************************************
                          ksimiodevice.cpp  -  description
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
#include <qvbox.h>

// KDE includes
#include <klocale.h>
#include <kmessagebox.h>
#include <kconfig.h>

// Project includes
#include "ksimdebug.h"
#include "ksimiodevice.h"
#include "ksimiodevicelist.h"
#include "ksimiodeviceinfo.h"
#include "ksimiodevicepropertydialog.h"
#include "ksimiodevicepropertybasewidget.h"
#include "ksimiodevicepropertygeneralwidget.h"

// Forward declaration


#if QT_VERSION >= 200 && QT_VERSION < 300
// version 2.x
#else
// version 3.x
#endif


class KSimIoDevice::Private
{
public:
	// Some statics
	static const char * const s_deviceName;
	static const char * const s_deviceDescription;
};



const char * const KSimIoDevice::Private::s_deviceName         = "Name";
const char * const KSimIoDevice::Private::s_deviceDescription  = "Description";


KSimIoDevice::KSimIoDevice(const KSimIoDeviceInfo * info, QObject *parent, const char *name)
	:	QObject(parent,name),
		m_deviceName(info->getI18nLibName()),
		m_deviceDescription(QString::null),
		m_info(info),
		m_mySerial(0)
{
//	m_pinList.setAutoDelete(true); Are object childs from device!
}

KSimIoDevice::~KSimIoDevice()
{
	KSimIoPin::Pool & pool = KSimIoPin::Pool::get();
	FOR_EACH_IO_PIN(it, getPinList())
	{
		pool.remove(it);
	}
}

void KSimIoDevice::setName(const QString & newName)
{
	m_deviceName = newName;
}


void KSimIoDevice::setDescription(const QString & newDescription)
{
	m_deviceDescription = newDescription;
}

void KSimIoDevice::addPins2Pool()
{
	KSimIoPin::Pool & pool = KSimIoPin::Pool::get();
	FOR_EACH_IO_PIN(it, getPinList())
	{
		pool.append(it);
	}
}


void KSimIoDevice::save(KConfigBase & config) const
{
	config.writeEntry(Private::s_deviceName, getName());
	config.writeEntry(Private::s_deviceDescription, getDescription());
}

bool KSimIoDevice::load(KConfigBase & config)
{
	setName(config.readEntry(Private::s_deviceName));
	setDescription(config.readEntry(Private::s_deviceDescription));
}


void KSimIoDevice::setSerial(unsigned int serial)
{
	ASSERT(serial != 0);

	m_mySerial = serial;
}

//#####################################
// Pins

const KSimIoPin * KSimIoDevice::findPinByID(int ioPinID) const
{
	FOR_EACH_IO_PIN(it, getPinList())
	{
		if (it.current()->getPinID() == ioPinID)
		{
			// Found
			return it.current();
		}
	}
	// Not found
	return (const KSimIoPin *)0;
}


//################################################################################
// Property dialog

void KSimIoDevice::initPropertyDialog(KSimIoDevicePropertyDialog * dialog)
{
	addGeneralProperty(dialog);
}

void KSimIoDevice::addGeneralProperty(KSimIoDevicePropertyDialog * dialog)
{
	static const QString i18nTitel(i18n("Device property dialog", "General"));

	QVBox * page = dialog->addVBoxPage(i18nTitel);
	KSimIoDevicePropertyBaseWidget * wid = createGeneralProperty(page);
	dialog->connectSlots(wid);
}

KSimIoDevicePropertyBaseWidget * KSimIoDevice::createGeneralProperty(QWidget *parent)
{
	KSimIoDevicePropertyBaseWidget * wid = new KSimIoDevicePropertyGeneralWidget(this, parent, "General Settings");
	CHECK_PTR(wid);
	return wid;
}


void KSimIoDevice::checkProperty(QStringList & errorMsg)
{
	// Names must be unique
	FOR_EACH_IO_DEVICE(it, *KSimIoDeviceList::getList())
	{
		if ((it.current() != this)
		 && (it.current()->getName() == getName()))
		{
			errorMsg.append(i18n("IO Device Error", "The name of the IO Device has to be unique!"));
			break;
		}
	}
}

unsigned int KSimIoDevice::executePropertyCheck(QWidget * parent)
{
	QStringList errMsg;

	checkProperty(errMsg);

	if(errMsg.count())
	{
		// Error detected
		QString caption((errMsg.count() == 1)
		                ? i18n("Property Error")
		                : i18n("Property Errors"));

		QString errText(caption
		                + QString::fromLatin1(":\n\n")
		                + errMsg.join(QString::fromLatin1("\n")));

//		logError(errText);

		KMessageBox::error(parent, errText, caption);

	}
	return errMsg.count();
}

//################################################################################
//################################################################################
#include "ksimiojoinboolin.h"

KSimIoDevice * KSimIoDeviceTest::create(const KSimIoDeviceInfo * info)
{
	return new KSimIoDeviceTest(info);
}

const KSimIoDeviceInfo * KSimIoDeviceTest::getStaticInfo()
{
	static const KSimIoDeviceInfo Info(QString::fromLatin1("Test Device"),
	                                   i18n("Test Device"),
	                                   QString::fromLatin1("Test Device"),
	                                   QString::null,
	                                   create );
	return &Info;
}
                
KSimIoDeviceTest::KSimIoDeviceTest(const KSimIoDeviceInfo * info, QObject *parent, const char *name)
	:	KSimIoDevice(info, parent, name)
{
	KSimIoPin * pin;
	pin = new KSimIoPin(this, 0, QString::fromLatin1("PA 1"), i18n("PA 1"));
	CHECK_PTR(pin);
	pin->addPinInfo(KSimIoJoinBoolIn::getStaticInfo());
	getPinList().append(pin);

	pin = new KSimIoPin(this, 1, QString::fromLatin1("PA 2"), i18n("PA 2"));
	CHECK_PTR(pin);
	pin->addPinInfo(KSimIoJoinBoolOut::getStaticInfo());
	getPinList().append(pin);

	
}

void KSimIoDeviceTest::setIO(int ioPinID, const void * pValue)
{
	ASSERT(pValue != (const void *)0);

	switch(ioPinID)
	{
		case 0:
			// Input!!! Do nothing
			KSIMDEBUG("Pin 0 is not a output");
			break;

		case 1:
			m_inBool1 = m_outBool1 = *(const bool *)pValue;
			break;
	}
}

void KSimIoDeviceTest::getIO(int ioPinID, void * pValue) const
{
	ASSERT(pValue != (const void *)0);

	switch(ioPinID)
	{
		case 0:
			*(bool *)pValue = m_inBool1;
			break;

		case 1:
			*(bool *)pValue = m_outBool1;
			break;
	}
}



