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

#include "library.h"
#include "packageinfo.h"

#include "componentlibrary.h"
#include "connectorlibrary.h"
#include "wirepropertylibrary.h"

#include "extconnboolin.h"
#include "extconnboolout.h"

#include "ksimpackage.h"

#include "module.h"
#include "wire.h"

#include "connectorboolin.h"
#include "connectorboolinedge.h"
#include "connectorboolout.h"
#include "connectorfloatin.h"
#include "connectorfloatout.h"

#include "wirepropertyboolean.h"
#include "wirepropertyfloatingpoint.h"

#include "boolean/booleanbutton.h"
#include "boolean/booleanled.h"
#include "boolean/booleanand.h"
#include "boolean/clockgenerator.h"
#include "boolean/simtimeinput.h"
#include "ksimdebug.h"


//#############################################################################
//#############################################################################

ComponentInfoList distComponent = { &ExtConnBoolInInfo,
                                    &ExtConnBoolOutInfo,
                                    &BooleanLedInfo,
                                    &ClockGeneratorInfo,
                                    &SimTimeInputInfo,
                                    &TickTimeInputInfo,
                                    0 };

ConnectorInfoList distConnector = { &ConnectorBoolInInfo,
                                    &ConnectorBoolInEdgeInfo,
                                    &ConnectorBoolOutInfo,
                                    &ConnectorFloatInInfo,
                                    &ConnectorFloatOutInfo,
                                    0 };
	
	
WirePropertyInfoList distWireProp = { &wirePropertyBooleanInfo,
                                      &wirePropertyFloatingPointInfo,
                                      0 };
	

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
		new PackageInfo("KSimus", KGlobal::instance(), VERSION, distComponent, distConnector, distWireProp);
	
	m_p = new LibraryPrivate;
	
	m_messages = new QStringList;
	CHECK_PTR(m_messages);
	
	m_componentLibrary = new ComponentLibrary;
	CHECK_PTR(m_componentLibrary);

	m_componentLibrary->insert(BooleanButtonList, KSimusPackageInfo);
	m_componentLibrary->insert(BooleanAndList, KSimusPackageInfo);
	
	
	
	m_componentLibrary->insertInternal(&WireInfo);
	m_componentLibrary->insertInternal(&ModuleBaseInfo);

	
	m_connectorLibrary = new ConnectorLibrary;
	CHECK_PTR(m_connectorLibrary);
	
	m_wirePropertyLibrary = new WirePropertyLibrary;
	CHECK_PTR(m_wirePropertyLibrary);
	
	insertPackage(KSimusPackageInfo);
		
	loadPackageFiles();

}

Library::~Library()
{
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
	

void Library::insertPackage(const PackageInfo * packageInfo)
{
	if (packageInfo->getComponentList() != 0)
	{
		const ComponentInfoPtr * comp = packageInfo->getComponentList();
		
		while (*comp)
		{
			m_componentLibrary->insert(*comp, packageInfo);
			comp++;
		};
	}
	
	if (packageInfo->getConnectorList() != 0)
	{
		const ConnectorInfoPtr * conn = packageInfo->getConnectorList();
		
		while (*conn)
		{
			m_connectorLibrary->insert(*conn, packageInfo);
			conn++;
		};
	}
	
	if (packageInfo->getWirePropertyList() != 0)
	{
		const WirePropertyInfoPtr * wireProp = packageInfo->getWirePropertyList();
		
		while (*wireProp)
		{
			m_wirePropertyLibrary->insert(*wireProp, packageInfo);
			wireProp++;
		};
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
						KSIMDEBUG(QString("ERROR: Package %1 returns NEW.").arg(package->getFilename()));
						m_p->m_handleList.remove(package);
						break;
						
					case KSimPackageHandle::OPENED:
						if (package->isPackage())
						{
							msg = i18n ("Load package %1 %2").arg(package->getPackageInfo()->getPackageName())
							                                 .arg(package->getPackageInfo()->getPackageVersion());
							msg += " (" + package->getFilename() + ")";
							m_messages->append(msg);
							KSIMDEBUG(msg);
							
							// Add package to lib
							insertPackage(package->getPackageInfo());
							if (package->getPackageInfo()->getInstance())
							{
								// Add translations
								KGlobal::locale()->insertCatalogue(package->getPackageInfo()->getInstance()->instanceName());
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
	
	FOR_EACH_HANDLE(it, m_p->m_handleList)
	{
		if (!it.current()->isOpened())
		{
			msg = i18n ("Package %1: %2").arg(it.current()->getFilename()).arg(it.current()->errorMsg());
			m_messages->append(msg);
			KSIMDEBUG(msg);
			m_p->m_handleList.remove(it.current());
		}
	}
	
	m_componentLibrary->resize();
	m_connectorLibrary->resize();
	m_wirePropertyLibrary->resize();
//	KGlobal::locale()->setLanguage("de");



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
			if ((list[u] != ".") && (list[u] != ".."))
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


	
	
	
