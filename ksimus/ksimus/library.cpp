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

#include "componentlibrary.h"
#include "connectorlibrary.h"
#include "wirepropertylibrary.h"
#include "implicitconverterlibrary.h"

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

#include "wirepropertyboolean.h"
#include "wirepropertybooltristate.h"
#include "wirepropertyfloatingpoint.h"

#include "implicitconverter.h"

#include "boolean/booleanbutton.h"
#include "boolean/booleanled.h"
#include "boolean/booleanand.h"
#include "boolean/clockgenerator.h"
#include "boolean/simtimeinput.h"
#include "boolean/controlclose.h"
#include "boolean/controlstop.h"
#include "boolean/controlpause.h"

#include "ksimdebug.h"


//#############################################################################
//#############################################################################


static const ComponentInfoList & getDistComponents()
{
	static ComponentInfoList * pDistComponents = 0;
	
	if (pDistComponents == 0)
	{
		// Initialize
		pDistComponents = new ComponentInfoList;
		CHECK_PTR(pDistComponents);
		
		// Add your component info here
		pDistComponents->append(getBooleanAndInfo());
		pDistComponents->append(getBooleanNandInfo());
		pDistComponents->append(getBooleanButtonInfo());
		pDistComponents->append(getBooleanToggleButtonInfo());
		pDistComponents->append(getExtConnBoolInInfo());
		pDistComponents->append(getExtConnBoolOutInfo());
		pDistComponents->append(getBooleanLedInfo());
		pDistComponents->append(getClockGeneratorInfo());
		pDistComponents->append(getSimTimeInputInfo());
		pDistComponents->append(getTickTimeInputInfo());
		pDistComponents->append(getControlCloseInfo());
		pDistComponents->append(getControlStopInfo());
		pDistComponents->append(getControlPauseInfo());
	}
	
	return *pDistComponents;
}




static const ConnectorInfoList & getDistConnector()
{
	static ConnectorInfoList * pDistConnector = 0;
	
	if (pDistConnector == 0)
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
		pDistConnector->append(getConnectorBoolTriStateInfo());
		pDistConnector->append(getConnectorBoolTriStateSpecialInfo());
	}
	
	return *pDistConnector;
}


	
static const WirePropertyInfoList & getDistWireProperty()
{
	static WirePropertyInfoList * pDistWireProp = 0;
	
	if (pDistWireProp == 0)
	{
		// Initialize
		pDistWireProp = new WirePropertyInfoList;
		CHECK_PTR(pDistWireProp);
		
		// Add your wireproperty info here
		pDistWireProp->append(getWirePropertyBooleanInfo());
		pDistWireProp->append(getPropertyFloatingPointInfo());
		pDistWireProp->append(getWirePropertyBoolTriStateInfo());

		// Some "error" WireProperties
		pDistWireProp->append(getWirePropertyInvalidDifferentOutputsInfo());
		pDistWireProp->append(getWirePropertyInvalidIncompatibleInputsInfo());
		pDistWireProp->append(getWirePropertyInvalidDifferentInputsNoOutputInfo());
	}
	
	return *pDistWireProp;
}

static const ImplicitConverterInfoList & getImplicitConverterProperty()
{
	static ImplicitConverterInfoList * pImplicitConverterProp = 0;

	if (pImplicitConverterProp == 0)
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

	

Library * g_library = 0;

//#############################################################################
//#############################################################################

class LibraryPrivate
{
public:
	LibraryPrivate()
	{
		m_handleList.setAutoDelete(true);
	};
	
	QList<KSimPackageHandle> m_handleList;

};

#define FOR_EACH_HANDLE(_it_,_handleList_)	\
		for(QListIterator<KSimPackageHandle> _it_(_handleList_);_it_.current();++_it_)


//#############################################################################
//#############################################################################


Library::Library()
{
	PackageInfo * KSimusPackageInfo =
	new PackageInfo(QString::fromLatin1("KSimus"), KGlobal::instance(), VERSION, getDistComponents(),
	                getDistConnector(), getDistWireProperty(), getImplicitConverterProperty());
	
	m_p = new LibraryPrivate;
	
	m_messages = new QStringList;
	CHECK_PTR(m_messages);
	
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

	insertPackage(KSimusPackageInfo);
		
	loadPackageFiles();

}

Library::~Library()
{
	delete m_implicitConverterLibrary;
	delete m_wirePropertyLibrary;
	delete m_connectorLibrary;
	delete m_componentLibrary;
	delete m_messages;
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

void Library::insertPackage(const PackageInfo * packageInfo)
{
	m_componentLibrary->insert(packageInfo->getComponentList(), packageInfo);
	
	m_connectorLibrary->insert(packageInfo->getConnectorList(), packageInfo);
	
	m_wirePropertyLibrary->insert(packageInfo->getWirePropertyList(), packageInfo);
	
	m_implicitConverterLibrary->insert(packageInfo->getImplicitConverterList(), packageInfo);
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
							m_messages->append(msg);
//							KSIMDEBUG(msg);
							
							// Add package to lib
							insertPackage(package->getPackageInfo());
							if (package->getPackageInfo()->getInstance())
							{
								// Add translations
//								KGlobal::locale()->insertCatalogue(package->getPackageInfo()->getInstance()->instanceName());
							}
							else
							{
								msg = i18n("Contains no KInstance!");
								m_messages->append(msg);
								KSIMDEBUG(msg);
							}
							end = false;
						}
						else
						{
							msg = i18n ("Package %1 has no components.").arg(package->getFilename());
							KSIMDEBUG(msg);
							m_messages->append(msg);
						}
						break;
						
					case KSimPackageHandle::BAD_FILE:
						msg = i18n ("Package %1: %2").arg(package->getFilename()).arg(package->errorMsg());
						m_messages->append(msg);
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
			m_messages->append(msg);
			KSIMDEBUG(msg);
			m_p->m_handleList.remove(it1.current());
		}
		else
		{
			++it1;
		}
	}
	
	m_componentLibrary->resize();
	m_connectorLibrary->resize();
	m_wirePropertyLibrary->resize();
}

void Library::addPackageDirs()
{
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Packages");
	
	QStringList dirList;
	
	dirList = config->readListEntry("Directories");
	
	unsigned int u;
		
	for (u = 0; u < dirList.count(); u++)
	{
		scanPackageDir(dirList[u]);
	}

	config->setGroup(group);
}

void Library::scanPackageDir(const QString & dirname)
{
	QFileInfo fi(dirname);
	
	if (fi.exists() && fi.isDir() && fi.isReadable())
	{
		QDir dir(dirname);
		
		QStringList list = dir.entryList(QDir::Dirs | QDir::Readable);
		
		unsigned int u;
		
		for (u = 0; u < list.count(); u++)
		{
			if ((list[u] != QString::fromLatin1(".")) && (list[u] != QString::fromLatin1("..")))
			{
				scanPackageDir(dirname + list[u]);
			}
		}
		
		const QFileInfoList * infoList = dir.entryInfoList(QDir::Files | QDir::Readable);
		for(QListIterator<QFileInfo> it(*infoList);it.current();++it)
		{
			makeHandle(dirname + it.current()->baseName());
		}
	}
}


void Library::addPackageFiles()
{
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Packages");
	
	QStringList fileList;
	
	fileList = config->readListEntry("Files");
	
	unsigned int u;
		
	for (u = 0; u < fileList.count(); u++)
	{
		makeHandle(fileList[u]);
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
		}
	}
	
	if (!found)
	{
		KSimPackageHandle * package;
/*		QString msg = i18n("Found package %1").arg(filename);
		KSIMDEBUG(msg);
		m_messages->append(msg);*/
		package = new KSimPackageHandle(filename);
		m_p->m_handleList.append(package);
	}
}

void Library::makeHandle(const QStringList & filenames)
{
	unsigned int u;
	
	for (u = 0; u < filenames.count(); u++)
	{
		makeHandle(filenames[u]);
	}
}


	
	
	
