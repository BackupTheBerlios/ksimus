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
#include "ksimusdoc.h"
#include "ksimiodevice.h"
#include "ksimiodevicelist.h"
#include "ksimiodeviceinfo.h"
#include "ksimiodevicepropertydialog.h"
#include "ksimiodevicepropertybasewidget.h"
#include "ksimiodevicepropertygeneralwidget.h"
#include "ksimiojoin.h"
#include "ksimiocomponent.h"

// Forward declaration


#if QT_VERSION >= 200 && QT_VERSION < 300
// version 2.x
#else
// version 3.x
#endif



class KSimIoDevice::JoinItem
{
public:
	JoinItem()
		:	join((KSimIoJoin *)0),
			pinId(-1)
	{};
	JoinItem(KSimIoJoin * _join, int _pinId)
		:	join(_join),
			pinId(_pinId)
	{};

	KSimIoJoin * join;
	int pinId;
};


//################################################################################
//################################################################################

class KSimIoDevice::JoinItemList : public QValueList<KSimIoDevice::JoinItem>
{
public:
	int findIndexByJoin(const KSimIoJoin * join) const;
	JoinItem * findByJoin(KSimIoJoin * join);

	JoinItemList findListByPinId(int pinId) const;
};

int KSimIoDevice::JoinItemList::findIndexByJoin(const KSimIoJoin * join) const
{
	for (unsigned int i = 0; i < count(); i++)
	{
		if ((*at(i)).join == join)
		{
			return (int)i;  // I don't think there are ever so much joins
		}
	}
	return -1;
}


KSimIoDevice::JoinItem * KSimIoDevice::JoinItemList::findByJoin(KSimIoJoin * join)
{
	Iterator it;

	for(it = begin(); it != end(); ++it)
	{
		if ((*it).join == join)
		{
			return &(*it);
		}
	}
	return (KSimIoDevice::JoinItem *)0;
}

KSimIoDevice::JoinItemList KSimIoDevice::JoinItemList::findListByPinId(int pinId) const
{
	JoinItemList list;

	ConstIterator it;
	for(it = begin(); it != end(); ++it)
	{
		if ((*it).pinId == pinId)
		{
			list.append(*it);
		}
	}
	return list;
}




//################################################################################
//################################################################################



class KSimIoDevice::Private
{
public:
	Private()
		:	openCnt(0)
	{
		flags.exclusive = 0;
	};

	JoinItemList joinList;

	struct
	{
		unsigned int exclusive :1;
	} flags;
	unsigned int openCnt;

	
	// Some statics
	static const char * const s_deviceName;
	static const char * const s_deviceDescription;
};




const char * const KSimIoDevice::Private::s_deviceName         = "Name";
const char * const KSimIoDevice::Private::s_deviceDescription  = "Description";

//################################################################################
//################################################################################


KSimIoDevice::KSimIoDevice(const KSimIoDeviceInfo * info)
	:	QObject((QObject*)0, info->getName().latin1()),
		m_deviceName(info->getI18nLibName()),
		m_deviceDescription(QString::null),
		m_info(info),
		m_mySerial(0)
{
	m_p = new Private;
	CHECK_PTR(m_p);
//	m_pinList.setAutoDelete(true); Are object childs from device!
}

KSimIoDevice::~KSimIoDevice()
{
	removeAllPins();
	delete m_p;
}

void KSimIoDevice::setName(const QString & newName)
{
	m_deviceName = newName;
}


void KSimIoDevice::setDescription(const QString & newDescription)
{
	m_deviceDescription = newDescription;
}

void KSimIoDevice::addPin(const KSimIoPin * ioPin)
{
	ASSERT(ioPin);
	
	getPinList().append(ioPin);
	KSimIoPin::Pool::get().append(ioPin);
}


void KSimIoDevice::removePin(const KSimIoPin * ioPin)
{
	if (ioPin)
	{
		KSimIoPin::Pool::get().remove(ioPin);
		getPinList().remove(ioPin);
	}
	else
	{
		KSIMDEBUG("KSimIoDevice::removePin with null pointer");
	}
}

void KSimIoDevice::removePin(int ioPinID)
{
	const KSimIoPin * ioPin = findPinByID(ioPinID);
	if (ioPin)
	{
		removePin(ioPin);
	}
	else
	{
		KSIMDEBUG_VAR("KSimIoDevice::removePin Unkown pin",ioPinID);
	}
}


void KSimIoDevice::removeAllPins()
{
	KSimIoPin::Pool & pool = KSimIoPin::Pool::get();
	FOR_EACH_IO_PIN(it, getPinList())
	{
		pool.remove(it);
	}
	getPinList().clear();
}

void KSimIoDevice::deletePin(const KSimIoPin * ioPin)
{
	if (ioPin)
	{
		KSimIoPin::Pool::get().remove(ioPin);
		getPinList().remove(ioPin);
		delete ioPin;
	}
	else
	{
		KSIMDEBUG("KSimIoDevice::deletePin with null pointer");
	}
}

void KSimIoDevice::deletePin(int ioPinID)
{
	const KSimIoPin * ioPin = findPinByID(ioPinID);
	if (ioPin)
	{
		deletePin(ioPin);
	}
	else
	{
		KSIMDEBUG_VAR("KSimIoDevice::deletePin Unkown pin",ioPinID);
	}
}

void KSimIoDevice::deleteAllPins()
{
	KSimIoPin::Pool & pool = KSimIoPin::Pool::get();
	FOR_EACH_IO_PIN(it, getPinList())
	{
		pool.remove(it);
	}
	bool autoDel = getPinList().autoDelete();
	getPinList().setAutoDelete(true);
	getPinList().clear();
	getPinList().setAutoDelete(autoDel);
}


unsigned int KSimIoDevice::getPinUsedCount(int ioPinID) const
{
	JoinItemList joinList = m_p->joinList.findListByPinId(ioPinID);
	return joinList.count();
}

bool KSimIoDevice::isPinUsed(int ioPinID) const
{
	return (getPinUsedCount(ioPinID) != 0);
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

	return true;
}


void KSimIoDevice::setSerial(unsigned int serial)
{
	ASSERT(serial != 0);

	m_mySerial = serial;
}

bool KSimIoDevice::isExclusive() const
{
	return (m_p->flags.exclusive != 0);
}

void KSimIoDevice::setExclusive(bool exclusive)
{
	m_p->flags.exclusive = exclusive ? 1 : 0;
}

void KSimIoDevice::checkCircuit(const KSimusDoc * doc, QStringList & errorMsgList)
{
	ASSERT(doc != (const KSimusDoc *)0);

	KSIMDEBUG_VAR("KSimIoDevice::checkCircuit", getName());
	// Device used by this doc?
	bool deviceUsed = false;
	JoinItemList::ConstIterator cJoinIt;
	for (cJoinIt = m_p->joinList.begin(); cJoinIt != m_p->joinList.end(); ++cJoinIt)
	{
		if ((*cJoinIt).join->getIoComponent()->getDoc() == doc)
		{
			deviceUsed = true;
			break;
		}
	}
	if (!deviceUsed)
		return;   // Device not used. No need to check!
	
	QStringList runningDocnameDeviceExclusive;
	
	// Exclusive execution of the io device
	if (isExclusive())
	{
		JoinItemList::ConstIterator it;
		for(it = m_p->joinList.begin(); it != m_p->joinList.end(); ++it)
		{
			KSimIoComponent * comp = (*it).join->getIoComponent();
			if ((comp->getDoc() != doc) && (comp->isRunning()))
			{
				// Used in another running document
				QString docName(comp->getDoc()->URL().path());
				if (!runningDocnameDeviceExclusive.contains(docName))
				{
					runningDocnameDeviceExclusive.append(docName);
				}
			}
		}
		if (runningDocnameDeviceExclusive.count())
		{
			QString errMsg = i18n("IO Device", "IO device '%1' is used in another running circuit (%2)")
			                 .arg(getName())
			                 .arg(runningDocnameDeviceExclusive.join(QString::fromLatin1(", ")));
			errorMsgList.append(errMsg);
			KSIMDEBUG_VAR("", errMsg);
		}
		
	}

	// Check pins
	QListIterator<KSimIoPin> itPin(getPinList());
	for (; itPin.current(); ++itPin)
	{
		unsigned int usedCnt = 0;
		QStringList runningDocName;
		const KSimIoPin * pin = itPin.current();
		JoinItemList joinList = m_p->joinList.findListByPinId(pin->getPinID());
		JoinItemList::ConstIterator itJoin;
		for(itJoin = joinList.begin(); itJoin != joinList.end(); ++itJoin)
		{
			const KSimIoJoin * join = (*itJoin).join;
			// Check exclusive joins
			if (join->isExclusive())
			{
				if (join->getIoComponent()->getDoc() == doc)
				{
					// Used in doc
					usedCnt++;
				}
				else if (join->getIoComponent()->isRunning())
				{
					// Used in another running document
					QString docName(join->getIoComponent()->getDoc()->URL().path());
					// Add docName if it is not in the list and if the doc does not causes an error during
					// device exclusive check
					if ( !runningDocName.contains(docName)
					  && !runningDocnameDeviceExclusive.contains(docName))
					{
						runningDocName.append(docName);
					}
				}
			}
		}
		if (usedCnt > 1)
		{
			QString errMsg = i18n("IO Device", "IO pin '%1::%2' is used more than once (used %3 times)")
			                 .arg(getName())
			                 .arg(pin->getName())
			                 .arg(usedCnt);
			errorMsgList.append(errMsg);
			KSIMDEBUG_VAR("", errMsg);
		}
		if (runningDocName.count())
		{
			QString errMsg = i18n("IO Device", "IO pin '%1::%2' is used in another running circuit (%3)")
			                 .arg(getName())
			                 .arg(pin->getName())
			                 .arg(runningDocName.join(QString::fromLatin1(", ")));
			errorMsgList.append(errMsg);
			KSIMDEBUG_VAR("", errMsg);
		}
	}
}

bool KSimIoDevice::openDevice(QString & /*errorMsg*/)
{
	m_p->openCnt++;
	return true;
}

void KSimIoDevice::closeDevice()
{
	ASSERT(m_p->openCnt > 0);
	m_p->openCnt--;
}

unsigned int KSimIoDevice::isDeviceOpened() const
{
	return m_p->openCnt;
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


//#####################################
// Joins

void KSimIoDevice::registerJoin(KSimIoJoin * join)
{
	ASSERT(join != (KSimIoJoin *)0);
	
	if ( -1 == m_p->joinList.findIndexByJoin(join))
	{
		m_p->joinList.append(JoinItem(join, join->getPin()->getPinID()));
	}
	else
	{
		KSIMDEBUG_VAR("Join already in list", join->getDefaultPinName());
	}
}

bool KSimIoDevice::unregisterJoin(KSimIoJoin * join)
{
	ASSERT(join != (KSimIoJoin *)0);

	int idx = m_p->joinList.findIndexByJoin(join);
	if (idx != -1)
	{
		m_p->joinList.remove(m_p->joinList.at(idx));
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("Join not in list", join->getDefaultPinName());
		return false;
	}
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

void KSimIoDevice::menuExecuted()
{
	
}



//################################################################################
//################################################################################
#include "ksimiojoinboolin.h"
#include "ksimiojoinboolout.h"

KSimIoDevice * KSimIoDeviceTest::create(const KSimIoDeviceInfo * info)
{
	return new KSimIoDeviceTest(info);
}

const KSimIoDeviceInfo * KSimIoDeviceTest::getStaticInfo()
{
//	KSimIoDeviceInfo(const QString & name,
//	                 const QString & i18nLibName,
//	                 const QString & libName,
//	                 const QString & additionalI18nLibNames,
//	                 KSimIoDevice * (*factory)(const KSimIoDeviceInfo *),
//	                 const QString & shortDescr = QString::null,
//	                 const QString & HTMLDescr = QString::null,
//	                 const QString & oldLibNames = QString::null );

	static const KSimIoDeviceInfo Info(QString::fromLatin1("Test Device"),
	                                   i18n("Test Device"),
	                                   QString::fromLatin1("Test Device"),
	                                   QString::null,
	                                   create );
	return &Info;
}
                
KSimIoDeviceTest::KSimIoDeviceTest(const KSimIoDeviceInfo * info)
	:	KSimIoDevice(info)
{
	KSimIoPin * pin;
	pin = new KSimIoPin(this, 0, QString::fromLatin1("PA 1"), i18n("PA 1"));
	CHECK_PTR(pin);
	pin->addPinInfo(KSimIoJoinBoolIn::getStaticInfo());
	addPin(pin);

	pin = new KSimIoPin(this, 1, QString::fromLatin1("PA 2"), i18n("PA 2"));
	CHECK_PTR(pin);
	pin->addPinInfo(KSimIoJoinBoolOut::getStaticInfo());
	addPin(pin);

//	setExclusive(true);
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
	ASSERT(pValue != (void *)0);

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



