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

#include <dlfcn.h>

#include <qstringlist.h>
#include <qlist.h>
#include <qdir.h>

#include <kstddirs.h>
#include <klocale.h>
#include <kconfig.h>

#include "library.h"

#include "componentlibrary.h"
#include "connectorlibrary.h"
#include "wirepropertylibrary.h"

#include "extconnboolin.h"
#include "extconnboolout.h"

#include "ksimpackage.h"

#include "module.h"
#include "wire.h"

#include "connectorboolin.h"
#include "connectorboolout.h"

#include "wirepropertyboolean.h"

#include "boolean/booleanbutton.h"
#include "boolean/booleanled.h"
#include "boolean/booleanand.h"
#include "boolean/clockgenerator.h"
#include "ksimdebug.h"


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



//#############################################################################
//#############################################################################


Library::Library()
{
	
	m_p = new LibraryPrivate;
	
	m_messages = new QStringList;
	CHECK_PTR(m_messages);
	
	m_componentLibrary = new ComponentLibrary;
	CHECK_PTR(m_componentLibrary);

	m_componentLibrary->insert(&ExtConnBoolInInfo);
	m_componentLibrary->insert(&ExtConnBoolOutInfo);
	m_componentLibrary->insert(BooleanButtonList);
	m_componentLibrary->insert(&BooleanLedInfo);
	m_componentLibrary->insert(BooleanAndList);
	m_componentLibrary->insert(&ClockGeneratorInfo);
	
	m_componentLibrary->insertInternal(&WireInfo);
	m_componentLibrary->insertInternal(&ModuleBaseInfo);

	
	m_connectorLibrary = new ConnectorLibrary;
	CHECK_PTR(m_connectorLibrary);
	
	m_connectorLibrary->insert(&ConnectorBoolInInfo);
	m_connectorLibrary->insert(&ConnectorBoolOutInfo);

	
	m_wirePropertyLibrary = new WirePropertyLibrary;
	CHECK_PTR(m_wirePropertyLibrary);
	
	m_wirePropertyLibrary->insert(&wirePropertyBooleanInfo);
	
	
	loadPackageFiles();


//	const char * libname = "/localhome/lokal/boolean/boolean/.libs/libboolean.so";
//	const char * libname = "/home/rasmus/boolean/boolean/.libs/libboolean.so";
/*	const char * libname = "/opt/kde2/lib/ksimus/libboolean.so";
	KSimPackageHandle * package = new KSimPackageHandle(libname);
	
	KSimPackageHandle::eResult result;
	
	result = package->open();
	if (result != KSimPackageHandle::OPENED)
	{
		KSIMDEBUG(package->errorMsg());
	}
	else
	{
		if (package->hasComponents())
		{
			m_componentLibrary->insert(*package->getComponentInfoList());
		}
		else
		{
			KSIMDEBUG(QString("Package %1 has no components.").arg(package->getFilename()));
		}
	}*/
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
	
void Library::loadPackageFiles()
{
	
	unsigned int i;
	bool end;
	KSimPackageHandle * package;
/*	KStandardDirs dirs;
	QStringList packageList = dirs.findAllResources("lib","ksimus/*.so", true, true);
	
	// Find packages
	for (i = 0; i < packageList.count(); i++)
	{
		QString msg = i18n("Found package %1").arg(packageList[i]);
		KSIMDEBUG(msg);
		m_messages->append(msg);
		package = new KSimPackageHandle(packageList[i]);
    m_p->m_handleList.append(package);
	}*/
	
	addPackageDirs();
	addPackageFiles();
	
	
	// Open packages
	do
	{
		end = true;
		for (i = 0; i < m_p->m_handleList.count(); i++)
		{
			package = m_p->m_handleList.at(i);
			if (!package->isOpened())
			{
				KSimPackageHandle::eResult result;
				result = package->open();
				
				switch(result)
				{
					case KSimPackageHandle::NEW:
						KSIMDEBUG(QString("ERROR: Package %1 returns NEW.").arg(package->getFilename()));
						m_p->m_handleList.remove(i);
						break;
						
					case KSimPackageHandle::OPENED:
						if (package->hasComponents())
						{
							m_componentLibrary->insert(*package->getComponentInfoList());
							QString msg = i18n ("Load package %1").arg(package->getFilename());
							m_messages->append(msg);
							KSIMDEBUG(msg);
							end = false;
						}
						else
						{
							QString msg = i18n ("Package %1 has no components.").arg(package->getFilename());
							KSIMDEBUG(msg);
							m_messages->append(msg);
						}
						break;
						
					case KSimPackageHandle::BAD_FILE:
						m_messages->append(package->errorMsg());
						KSIMDEBUG(package->errorMsg());
						m_p->m_handleList.remove(package);
						break;
					
					case KSimPackageHandle::TRY_AGAIN:
						m_messages->append(package->errorMsg());
						KSIMDEBUG(package->errorMsg());
						break;
				}
			}
		}
	}
	while (!end);
	

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
		
		list = dir.entryList(QDir::Files | QDir::Readable);
		for (u = 0; u < list.count(); u++)
		{
			makeHandle(dirname + list[u]);
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
		QString msg = i18n("Found package %1").arg(filename);
		KSIMDEBUG(msg);
		m_messages->append(msg);
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


	
	
	
