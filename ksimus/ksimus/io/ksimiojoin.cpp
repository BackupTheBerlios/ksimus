/***************************************************************************
                          ksimiojoin.cpp  -  description
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

// C/C++ includes

// QT includes

// KDE includes
#include <klocale.h>

// Project includes
#include "ksimiocomponent.h"
#include "ksimiodevice.h"
#include "ksimiodevicelist.h"
#include "ksimiopin.h"
#include "ksimiojoin.h"
#include "connectorbase.h"
//#include "componentlayout.h"
#include "ksimdata.h"
#include "ksimdebug.h"
#include "compcontainer.h"

// Forward declaration

class KSimIoJoin::Private
{
public:
	Private()
	{
		flags.exclusive = 1;
	};

	int ioPinID;
	QString deviceName;
	QString defaultConnectorName;

	struct
	{
		unsigned int exclusive : 1;
	} flags;

	//Some statics
	static const char * const sDeviceName;
	static const char * const sIoPinID;
	static const char * const sDefaultConnectorName;
	static const char * const sConnectorDir;
};


const char * const KSimIoJoin::Private::sDeviceName           = "Device Name";
const char * const KSimIoJoin::Private::sIoPinID              = "IO Pin ID";
const char * const KSimIoJoin::Private::sDefaultConnectorName = "Default Conn Name";
const char * const KSimIoJoin::Private::sConnectorDir         = "Connector/";




KSimIoJoin::KSimIoJoin(KSimIoComponent* ioComp, const KSimIoJoinInfo* info)
	:	QObject(ioComp, info->getName().latin1()),
		ComponentItem(ioComp),
		m_pin((const KSimIoPin*) 0),
		m_device((KSimIoDevice *)0),
		m_info(info),
		m_connectorBase((ConnectorBase *)0),
		m_serialID(0)
{
	ASSERT(ioComp != (KSimIoComponent*)0);
	ASSERT(info != (const KSimIoJoinInfo*)0);

	m_p = new Private();
	CHECK_PTR(m_p);

	KSIMDEBUG("KSimIoJoin::KSimIoJoin");
}

KSimIoJoin::~KSimIoJoin()
{
	KSIMDEBUG_VAR("KSimIoJoin::~KSimIoJoin", getDefaultPinName());
	if(m_device)
	{
		m_device->unregisterJoin(this);
		getIoComponent()->getContainer()->unregisterIoDevice(m_device);
	}
	getIoComponent()->getConnList()->removeRef(getConnector());
	// delete getConnector(); is deleted by getIoComponent()->getConnList()
	getIoComponent()->removeJoin(this);
	delete m_p;
}

void KSimIoJoin::setPin(const QString & deviceName, int pinID)
{
	KSimIoDevice * dev = KSimIoDeviceList::getList()->findByName(deviceName);
	if (dev)
	{
		const KSimIoPin * pin = dev->findPinByID(pinID);
		if (pin != (const KSimIoPin *)0)
		{
			if (pin != getPin())
			{
				setPin(pin);
			}
		}
		else
		{
			KSIMDEBUG(QString::fromLatin1("IO Pin not found (Device %1, Pin ID %2)")
			  .arg(deviceName).arg(pinID));
		}
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("IO Device not found (Device %1)")
		  .arg(deviceName));
		
	}

}

void KSimIoJoin::setPin(const KSimIoPin * pin)
{

	ASSERT(pin != (const KSimIoPin*)0);

	if(m_pin)
	{
		disconnect(m_pin, SIGNAL(destroyed()), this, SLOT(slotPinDelete()));
	}
	if(m_device)
	{
		m_device->unregisterJoin(this);
		getIoComponent()->getContainer()->unregisterIoDevice(m_device);
	}

	m_pin = pin;
	m_p->ioPinID = getPin()->getPinID();

	m_device = m_pin->getDevice();
	CHECK_PTR(m_device);
	m_device->registerJoin(this);
	getIoComponent()->getContainer()->registerIoDevice(m_device);
	m_p->deviceName = getDevice()->getName();

	m_p->defaultConnectorName = QString::fromLatin1("%1(%2)").arg(m_p->deviceName)
	                                          .arg(getPin()->getDefaultI18nName());

	// Rename connector
	updateConnName();

	connect(m_pin, SIGNAL(destroyed()), this, SLOT(slotPinDelete()));

	KSIMDEBUG_VAR("KSimIoJoin::setPin", getDefaultPinName());
}

void KSimIoJoin::updateConnName()
{
	if (getConnector() && (getConnector()->getInitName() == getConnector()->getName()))
	{
		// No user defined name -> set new default name
		getConnector()->setName(m_p->defaultConnectorName, true);
	}
}


void KSimIoJoin::save(KSimData & file) const
{
	file.writeEntry(Private::sDeviceName, m_p->deviceName);
	file.writeEntry(Private::sIoPinID, m_p->ioPinID);
	file.writeEntry(Private::sDefaultConnectorName, m_p->defaultConnectorName);

	// Save connector properties
	file.pushGroupRel(Private::sConnectorDir);
	getConnector()->save(file);
	file.popGroup();
	
}

bool KSimIoJoin::load(KSimData & file)
{
	bool loadOk = true;
	
	m_p->deviceName = file.readEntry(Private::sDeviceName, QString::null);
	m_p->ioPinID = file.readNumEntry(Private::sIoPinID, 0);
	m_p->defaultConnectorName = file.readEntry(Private::sDefaultConnectorName);
	setPin(m_p->deviceName, m_p->ioPinID);

	// Load connector properties
	file.pushGroupRel(Private::sConnectorDir);
	loadOk &= getConnector()->load(file);
	file.popGroup();

	updateConnName();

	return loadOk;
}


void KSimIoJoin::setConnector(ConnectorBase * connector)
{
	ASSERT(connector != (ConnectorBase *)0);
	
	m_connectorBase = connector;
	getConnector()->setName(m_p->defaultConnectorName, true);
	getConnector()->setWireName(QString::fromLatin1("Conn %1").arg(getSerialID()));

	// The join saves the connector!
	getConnector()->getAction().disable(KSimAction::STORAGE);
	
	connect(getConnector(), SIGNAL(signalDeleteRequest(ConnectorBase *)),
	        this, SLOT(slotConnDeleteRequest(ConnectorBase *)));
}

QString KSimIoJoin::getDefaultPinName() const
{
	return m_p->defaultConnectorName;
}

QString KSimIoJoin::getDeviceName() const
{
	return m_p->deviceName;
}

void KSimIoJoin::calculate() const
{
}

void KSimIoJoin::reset()
{
	ASSERT(m_pin != (const KSimIoPin*) 0);
	ASSERT(m_device != (KSimIoDevice*) 0);
	// Nothing todo
}

bool KSimIoJoin::isExclusive() const
{
	return (m_p->flags.exclusive == 0);
}

void KSimIoJoin::setExclusive(bool exclusive)
{
	m_p->flags.exclusive = exclusive ? 1 : 0;
}


//#########################################
// Slots

void KSimIoJoin::slotPinDelete()
{
	if(m_device)
	{
		m_device->unregisterJoin(this);
		getIoComponent()->getContainer()->unregisterIoDevice(m_device);
	}

	m_pin = (const KSimIoPin*) 0;
	m_device = (KSimIoDevice *)0;
}

void KSimIoJoin::slotConnDeleteRequest(ConnectorBase * conn)
{
	if (conn == getConnector())
	{
		getIoComponent()->undoChangeProperty(i18n("Delete IO"));
		getIoComponent()->setModified();
		getIoComponent()->getConnList()->removeRef(getConnector());
		getIoComponent()->updateLayout();
		getIoComponent()->refresh();
		delete this;
	}
	else
	{
		KSIMDEBUG(QString::fromLatin1("Connector %1 is not from KSimIoJoin %2")
		          .arg(conn->getName()).arg(getDefaultPinName()));
	}
}


//###########################################################################
//###########################################################################

KSimIoJoin * KSimIoJoin::List::findBySerial(unsigned int serial)
{
	FOR_EACH_IO_JOIN(it, *this)
	{
		if (it.current()->getSerialID() == serial)
		{
			// Found
			return it.current();
		}
	}
	// Not found
	return (KSimIoJoin *)0;
}

#include "ksimiojoin.moc"
