/***************************************************************************
                          library.cpp  -  description
                             -------------------
    begin                : Tue Dec 26 2000
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

#include <qstringlist.h>
#include <qlist.h>
#include <qdir.h>

#include <kstddirs.h>
#include <klocale.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kapp.h>

#include "config.h"
#include "library.h"
#include "packageinfo.h"
#include "moduleinfo.h"

#include "componentlibrary.h"
#include "connectorlibrary.h"
#include "wirepropertylibrary.h"
#include "implicitconverterlibrary.h"
#include "ksimiodevicelibrary.h"
#include "ksimiojoinlibrary.h"

#include "extconnboolin.h"
#include "extconnboolout.h"

#include "ksimpackage.h"

#include "module.h"
#include "wire.h"

#include "connectorboolin.h"
#include "connectorboolinedge.h"
#include "connectorboolout.h"
#include "connectorbooltristate.h"
#include "connectorfloatin.h"
#include "connectorfloatout.h"
#include "connectorintegerin.h"
#include "connectorintegerout.h"

#include "wirepropertyboolean.h"
#include "wirepropertybooltristate.h"
#include "wirepropertyfloatingpoint.h"
#include "wirepropertyinteger.h"

#include "implicitconverter.h"

#include "ksimiodevice.h"
#include "ksimiojoinboolin.h"
#include "ksimiojoinboolout.h"

#include "boolean/booleanbutton.h"
#include "boolean/booleanled.h"
#include "boolean/booleanand.h"
#include "boolean/clockgenerator.h"
#include "boolean/simtimeinput.h"
#include "boolean/controlclose.h"
#include "boolean/controlstop.h"
#include "boolean/controlpause.h"
#include "ksimiocomponent.h"

#include "ksimdebug.h"


//#############################################################################
//#############################################################################

class Library::Private
{
public:
	Private()
	{
		m_handleList.setAutoDelete(true);
	};

	QList<KSimPackageHandle> m_handleList;
	QStringList m_moduleList;
	QStringList m_infoMessages;
	QStringList m_errorMessages;

	// removed from globale scope
	static const ComponentInfoList & getDistComponents();
	static const ConnectorInfoList & getDistConnector();
	static const WirePropertyInfoList & getDistWireProperty();
	static const ImplicitConverterInfoList & getDistImplicitConverter();
	static const KSimIoDeviceInfoList & getDistIoDevice();
	static const KSimIoJoinInfoList & getDistIoJoin();
};

#define FOR_EACH_HANDLE(_it_,_handleList_)	\
		for(QListIterator<KSimPackageHandle> _it_(_handleList_);_it_.current();++_it_)


//#############################################################################
//#############################################################################


const ComponentInfoList & Library::Private::getDistComponents()
{
	static ComponentInfoList * pDistComponents = (ComponentInfoList *)0;
	
	if (pDistComponents == (ComponentInfoList *)0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);
		
		// Add your component info here
		pDistComponents->append(BooleanAnd::getStaticAndInfo());
		pDistComponents->append(BooleanAnd::getStaticNandInfo());
		pDistComponents->append(BooleanButton::getStaticButtonInfo());
		pDistComponents->append(BooleanButton::getStaticToggleButtonInfo());
		pDistComponents->append(ExtConnBoolIn::getStaticInfo());
		pDistComponents->append(ExtConnBoolOut::getStaticInfo());
		pDistComponents->append(BooleanLed::getStaticInfo());
		pDistComponents->append(ClockGenerator::getStaticInfo());
		pDistComponents->append(SimTimeInput::getStaticInfo());
		pDistComponents->append(TickTimeInput::getStaticInfo());
		pDistComponents->append(ControlClose::getStaticInfo());
		pDistComponents->append(ControlStop::getStaticInfo());
		pDistComponents->append(ControlPause::getStaticInfo());
		pDistComponents->append(KSimIoComponent::getStaticInfo());
	}
	
	return *pDistComponents;
}


const ConnectorInfoList & Library::Private::getDistConnector()
{
	static ConnectorInfoList * pDistConnector = (ConnectorInfoList *)0;
	
	if (pDistConnector == (ConnectorInfoList *)0)
	{
		// Initialize
		pDistConnector = new ConnectorInfoList;
		CHECK_PTR(pDistConnector);
		
		// Add your connector info here
		pDistConnector->append(getConnectorBoolInInfo());
		pDistConnector->append(getConnectorBoolInEdgeInfo());
		pDistConnector->append(getConnectorBoolOutInfo());
		pDistConnector->append(getConnectorFloatInInfo());
		pDistConnector->append(getConnectorFloatOutInfo());
		pDistConnector->append(getConnectorBoolTristateInfo());
		pDistConnector->append(getConnectorBoolTristateSpecialInfo());
		pDistConnector->append(ConnectorIntegerIn::getStaticInfo());
		pDistConnector->append(ConnectorIntegerOut::getStaticInfo());
	}
	
	return *pDistConnector;
}


const WirePropertyInfoList & Library::Private::getDistWireProperty()
{
	static WirePropertyInfoList * pDistWireProp = (WirePropertyInfoList *)0;
	
	if (pDistWireProp == (WirePropertyInfoList *)0)
	{
		// Initialize
		pDistWireProp = new WirePropertyInfoList;
		CHECK_PTR(pDistWireProp);
		
		// Add your wireproperty info here
		pDistWireProp->append(getWirePropertyBooleanInfo());
		pDistWireProp->append(getPropertyFloatingPointInfo());
		pDistWireProp->append(getWirePropertyBoolTristateInfo());
		pDistWireProp->append(WirePropertyInteger::getStaticInfo());

		// Some "error" WireProperties
		pDistWireProp->append(getWirePropertyInvalidDifferentOutputsInfo());
		pDistWireProp->append(getWirePropertyInvalidIncompatibleInputsInfo());
		pDistWireProp->append(getWirePropertyInvalidDifferentInputsNoOutputInfo());
	}
	
	return *pDistWireProp;
}

const ImplicitConverterInfoList & Library::Private::getDistImplicitConverter()
{
	static ImplicitConverterInfoList * pImplicitConverterProp = (ImplicitConverterInfoList *)0;

	if (pImplicitConverterProp == (ImplicitConverterInfoList *)0)
	{
		// Initialize
		pImplicitConverterProp = new ImplicitConverterInfoList;
		CHECK_PTR(pImplicitConverterProp);

		// Add your implicit converter info here
		pImplicitConverterProp->append(getImplicitConverterBoolean2FloatInfo());
		pImplicitConverterProp->append(getImplicitConverterFloat2BooleanInfo());
	}

	return *pImplicitConverterProp;
}

const KSimIoDeviceInfoList & Library::Private::getDistIoDevice()
{
	static KSimIoDeviceInfoList * pIoDevice = (KSimIoDeviceInfoList *)0;

	if (pIoDevice == (KSimIoDeviceInfoList *)0)
	{
		// Initialize
		pIoDevice = new KSimIoDeviceInfoList;
		CHECK_PTR(pIoDevice);

		// Add your io devices info here
		pIoDevice->append(KSimIoDeviceTest::getStaticInfo());
	}

	return *pIoDevice;
}

const KSimIoJoinInfoList & Library::Private::getDistIoJoin()
{
	static KSimIoJoinInfoList * pIoJoin = (KSimIoJoinInfoList *)0;

	if (pIoJoin == (KSimIoJoinInfoList *)0)
	{
		// Initialize
		pIoJoin = new KSimIoJoinInfoList;
		CHECK_PTR(pIoJoin);

		// Add your io join info here
		pIoJoin->append(KSimIoJoinBoolIn::getStaticInfo());
		pIoJoin->append(KSimIoJoinBoolOut::getStaticInfo());
	}

	return *pIoJoin;
}

Library * g_library = (Library *)0;

//#############################################################################
//#############################################################################


Library::Library()
{
	m_p = new Private();
	CHECK_PTR(m_p);

	m_ksimusPackageInfo = new PackageInfo("KSimus", KGlobal::instance(), VERSION);
	CHECK_PTR(m_ksimusPackageInfo);
	m_ksimusPackageInfo->insert(Library::Private::getDistComponents());
	m_ksimusPackageInfo->insert(Library::Private::getDistConnector());
	m_ksimusPackageInfo->insert(Library::Private::getDistWireProperty());
	m_ksimusPackageInfo->insert(Library::Private::getDistImplicitConverter());
	m_ksimusPackageInfo->insert(Library::Private::getDistIoDevice());
	m_ksimusPackageInfo->insert(Library::Private::getDistIoJoin());

	m_componentLibrary = new ComponentLibrary;
	CHECK_PTR(m_componentLibrary);

	
	m_componentLibrary->insertInternal(getWireInfo());
	m_componentLibrary->insertInternal(getModuleBaseInfo());

	
	m_connectorLibrary = new ConnectorLibrary;
	CHECK_PTR(m_connectorLibrary);
	
	m_wirePropertyLibrary = new WirePropertyLibrary;
	CHECK_PTR(m_wirePropertyLibrary);
	
	m_implicitConverterLibrary = new ImplicitConverterLibrary;
	CHECK_PTR(m_implicitConverterLibrary);

	m_ioDeviceLibrary = new KSimIoDeviceLibrary;
	CHECK_PTR(m_ioDeviceLibrary);

	m_ioJoinLibrary = new KSimIoJoinLibrary;
	CHECK_PTR(m_ioJoinLibrary);

	m_packageList = new QList<PackageInfo>;
	CHECK_PTR(m_packageList);

	insertPackage(m_ksimusPackageInfo);

	loadPackageFiles();

	addModuleDirs();
	addModuleFiles();

	m_componentLibrary->resize();
	m_connectorLibrary->resize();
	m_wirePropertyLibrary->resize();
	m_implicitConverterLibrary->resize();
	m_ioDeviceLibrary->resize();
	m_ioJoinLibrary->resize();
}

Library::~Library()
{
	delete m_ioJoinLibrary;
	delete m_ioDeviceLibrary;
	delete m_implicitConverterLibrary;
	delete m_wirePropertyLibrary;
	delete m_connectorLibrary;
	delete m_componentLibrary;
	delete m_packageList;
	// delete m_ksimusPackageInfo; is deleted by m_packageList !!!
	delete m_p;
}

const ComponentLibrary * Library::getComponentLib() const
{
	return m_componentLibrary;
}

const ConnectorLibrary * Library::getConnectorLib() const
{
	return m_connectorLibrary;
}

const WirePropertyLibrary * Library::getWirePropertyLib() const
{
	return m_wirePropertyLibrary;
}
	

const ImplicitConverterLibrary * Library::getImplicitConverterLib() const
{
	return m_implicitConverterLibrary;
}

const KSimIoDeviceLibrary * Library::getIoDeviceLib() const
{
	return m_ioDeviceLibrary;
}

const KSimIoJoinLibrary * Library::getIoJoinLib() const
{
	return m_ioJoinLibrary;
}

const QList<PackageInfo> * Library::getPackageList() const
{
	return m_packageList;
}

void Library::insertPackage(const PackageInfo * packageInfo)
{
	m_packageList->append(packageInfo);
	
	if (packageInfo->hasComponentList())
	{
		m_componentLibrary->insert(packageInfo->getComponentList(), packageInfo);
	}
	
	if (packageInfo->hasConnectorList())
	{
		m_connectorLibrary->insert(packageInfo->getConnectorList(), packageInfo);
	}
	
	if (packageInfo->hasWirePropertyList())
	{
		m_wirePropertyLibrary->insert(packageInfo->getWirePropertyList(), packageInfo);
	}
	
	if (packageInfo->hasImplicitConverterList())
	{
		m_implicitConverterLibrary->insert(packageInfo->getImplicitConverterList(), packageInfo);
	}

	if (packageInfo->hasKSimIoDeviceList())
	{
		m_ioDeviceLibrary->insert(packageInfo->getKSimIoDeviceList(), packageInfo);
	}

	if (packageInfo->hasKSimIoJoinList())
	{
		m_ioJoinLibrary->insert(packageInfo->getKSimIoJoinList(), packageInfo);
	}
}



void Library::loadPackageFiles()
{
	bool end;
	KSimPackageHandle * package;
	QString msg;
	
	addPackageFiles();
	addPackageDirs();
	
	
	// Open packages
	do
	{
		end = true;
		FOR_EACH_HANDLE(it, m_p->m_handleList)
		{
			package = it.current();
			if (!package->isOpened())
			{
				KSimPackageHandle::eResult result;
				result = package->open();
				
				switch(result)
				{
					case KSimPackageHandle::NEW:
						KSIMDEBUG(QString::fromLatin1("ERROR: Package %1 returns NEW.").arg(package->getFilename()));
						m_p->m_handleList.remove(package);
						break;
						
					case KSimPackageHandle::OPENED:
						if (package->isPackage())
						{
							msg = i18n ("Load package %1 %2").arg(package->getPackageInfo()->getPackageName())
							                                 .arg(QString::fromLatin1(package->getPackageInfo()->getPackageVersion()));
							msg += " (" + package->getFilename() + ")";
							
							// Add package to lib
							insertPackage(package->getPackageInfo());
							if (package->getPackageInfo()->getInstance())
							{
								// Add translations
//								KGlobal::locale()->insertCatalogue(package->getPackageInfo()->getInstance()->instanceName());
							}
							else
							{
								msg += i18n("\nContains no KInstance!");
								KSIMDEBUG(msg);
							}
							m_p->m_infoMessages.append(msg);
//							KSIMDEBUG(msg);
							end = false;
						}
						else
						{
							msg = i18n ("Package %1 has no components.").arg(package->getFilename());
							KSIMDEBUG(msg);
							m_p->m_errorMessages.append(msg);
						}
						break;
						
					case KSimPackageHandle::BAD_FILE:
						msg = i18n ("Package %1: %2").arg(package->getFilename()).arg(package->errorMsg());
						m_p->m_errorMessages.append(msg);
						KSIMDEBUG(msg);
						m_p->m_handleList.remove(package);
						break;
					
					case KSimPackageHandle::TRY_AGAIN:
						// try again
						break;
				}
			}
		}
	}
	while (!end);
	
	for(QListIterator<KSimPackageHandle> it1(m_p->m_handleList);it1.current();)
	{
		if (!it1.current()->isOpened())
		{
			msg = i18n ("Package %1: %2").arg(it1.current()->getFilename()).arg(it1.current()->errorMsg());
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg);
			m_p->m_handleList.remove(it1.current());
		}
		else
		{
			++it1;
		}
	}
	
}

void Library::addPackageDirs()
{
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Packages");
	
	QStringList dirList(config->readListEntry("Directories"));
	
	for(QStringList::ConstIterator it = dirList.begin(); it != dirList.end(); ++it)
	{
		scanPackageDir(*it);
	}

	config->setGroup(group);
}

void Library::scanPackageDir(const QString & dirname)
{
	QFileInfo fi(dirname);
	
	if (fi.exists() && fi.isDir() && fi.isReadable())
	{
		QDir dir(dirname);
		QStringList list(dir.entryList(QDir::Dirs | QDir::Readable));
		
		for(QStringList::ConstIterator it = list.begin(); it != list.end(); ++it)
		{
			if ((*it != QString::fromLatin1(".")) && (*it != QString::fromLatin1("..")))
			{
				scanPackageDir(dirname + *it);
			}
		}
		
		const QFileInfoList * infoList = dir.entryInfoList(QDir::Files | QDir::Readable);
		for(QListIterator<QFileInfo> it(*infoList);it.current();++it)
		{
//			makeHandle(dirname + it.current()->baseName());
			makeHandle(QString::fromLatin1("%1/%2").arg(dirname).arg(it.current()->baseName()));
		}
	}
}


void Library::addPackageFiles()
{
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Packages");
	
	QStringList fileList(config->readListEntry("Files"));
	
	for(QStringList::ConstIterator it = fileList.begin(); it != fileList.end(); ++it)
	{
		QFileInfo fileInfo (*it);
		if(!fileInfo.exists())
		{
			QString msg(i18n("Package %1: File does not exist.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		else if(!fileInfo.isFile())
		{
			QString msg(i18n("Package %1: Is not a file.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		else if(!fileInfo.isReadable())
		{
			QString msg(i18n("Package %1: File is not readable.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		else
		{
			makeHandle(fileInfo.filePath());
		}
	}

	config->setGroup(group);
}

void Library::makeHandle(const QString & filename)
{
	bool found = false;
	unsigned int u;
	
	for (u=0; u < m_p->m_handleList.count(); u++)
	{
		if (m_p->m_handleList.at(u)->getFilename() == filename)
		{
			found = true;
			break;
		}
	}
	
	if (!found)
	{
/*		QString msg = i18n("Found package %1").arg(filename);
		KSIMDEBUG(msg);
		m_p->m_infoMessages.append(msg);*/
		KSimPackageHandle * package = new KSimPackageHandle(filename);
		CHECK_PTR(package);
		m_p->m_handleList.append(package);
	}
}

void Library::makeHandle(const QStringList & filenames)
{
	for(QStringList::ConstIterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		makeHandle(*it);
	}
}

void Library::addModuleDirs()
{
	KConfig * config = kapp->config();

	QString group(config->group());

	config->setGroup("Modules");

	QStringList dirList(config->readListEntry("Directories"));

	for(QStringList::ConstIterator it = dirList.begin(); it != dirList.end(); ++it)
	{
		scanModuleDir(*it);
	}

	config->setGroup(group);
}

void Library::scanModuleDir(const QString & dirname)
{
	//KSIMDEBUG(dirname);
	QFileInfo fi(dirname);

	if (fi.exists() && fi.isDir() && fi.isReadable())
	{
		QDir dir(dirname);

		QStringList list(dir.entryList(QDir::Dirs | QDir::Readable));

		for(QStringList::ConstIterator it = list.begin(); it != list.end(); ++it)
		{
			//KSIMDEBUG(*it);
			if ((*it != QString::fromLatin1(".")) && (*it != QString::fromLatin1("..")))
			{
				scanModuleDir(dirname + *it);
			}
		}

		dir.setNameFilter(QString::fromLatin1("*.sim"));
		const QFileInfoList * infoList = dir.entryInfoList(QDir::Files | QDir::Readable);
		for(QListIterator<QFileInfo> it(*infoList);it.current();++it)
		{
			loadModule(QString::fromLatin1("%1/%2.sim").arg(dirname).arg(it.current()->baseName()));
		}
	}
}


void Library::addModuleFiles()
{
	KConfig * config = kapp->config();
	QString group(config->group());
	config->setGroup("Modules");
	QStringList fileList(config->readListEntry("Files"));

	for(QStringList::ConstIterator it = fileList.begin(); it != fileList.end(); ++it)
	{
		QFileInfo fileInfo (*it);
		if(!fileInfo.exists())
		{
			QString msg(i18n("Module %1: File does not exist.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		else if(!fileInfo.isFile())
		{
			QString msg(i18n("Module %1: Is not a file.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		else if(!fileInfo.isReadable())
		{
			QString msg(i18n("Module %1: File is not readable.").arg(fileInfo.filePath()));
			m_p->m_errorMessages.append(msg);
			KSIMDEBUG(msg)
		}
		loadModule(fileInfo.filePath());
	}

	config->setGroup(group);
}

void Library::loadModule(const QString & filename)
{
	bool found = false;

	for (QStringList::ConstIterator it = m_p->m_moduleList.begin(); it != m_p->m_moduleList.end(); ++it)
	{
		if (*it == filename)
		{
			found = true;
			break;
		}
	}

	if (!found)
	{
		const ModuleInfo * mi = ModuleData::makeModuleInfo(filename);
		if (mi)
		{
			m_componentLibrary->insert(mi, 0);
			QString msg = i18n("Load module %1 (%2)").arg(mi->getName()).arg(filename);
			m_p->m_infoMessages.append(msg);
			//KSIMDEBUG(msg);
		}
		else
		{
			// No valid module
			QString msg = i18n ("Module %1: File contains no valid module.").arg(filename);
			m_p->m_errorMessages.append(msg);
			//KSIMDEBUG(msg);
		}
		m_p->m_moduleList.append(filename);
	}
}

void Library::clearMessageLists()
{
	m_p->m_infoMessages.clear();
	m_p->m_errorMessages.clear();
}

const QStringList & Library::getInfoMessages() const
{
	return m_p->m_infoMessages;
};

const QStringList & Library::getErrorMessages() const
{
	return m_p->m_errorMessages;
};

