/***************************************************************************
                          ksimpackage.cpp  -  description
                             -------------------
    begin                : Sat May 12 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes
#include <dlfcn.h>


// QT-Includes
#include <qfileinfo.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimpackage.h"
#include "ksimdebug.h"

// Forward declaration



KSimPackageHandle::KSimPackageHandle(const QString & filename)
	: m_filename(filename),
		m_error(NEW),
		m_errorMsg(QString::null),
		m_fileHandle(0),
		m_opened(false),
		m_componentInfoList(0),
		m_connectorInfoList(0),
		m_wirePropertyInfoList(0)
{
}

KSimPackageHandle::~KSimPackageHandle()
{
	if (m_fileHandle)
		dlclose(m_fileHandle);
}

QString KSimPackageHandle::getFilename() const
{
	return m_filename;
}

KSimPackageHandle::eResult KSimPackageHandle::open()
{
	QFileInfo fileInfo (getFilename());
	
	if(isOpened())
	{
		m_errorMsg = i18n("Package %1 is already opened.").arg(getFilename());
		m_error = OPENED;
	}
	else if(!fileInfo.exists())
	{
		m_errorMsg = i18n("File %1 not exist.").arg(getFilename());
		m_error = BAD_FILE;
	}
	else if(!fileInfo.isFile())
	{
		m_errorMsg = i18n("%1 is not a file.").arg(getFilename());
		m_error = BAD_FILE;
	}
	else if(!fileInfo.isReadable())
	{
		m_errorMsg = i18n("File %1 is not readable.").arg(getFilename());
		m_error = BAD_FILE;
	}
	else
	{
		m_fileHandle = (void *) dlopen(getFilename(), RTLD_NOW);
		if (m_fileHandle == 0)
		{
  		m_errorMsg = dlerror();
			m_error = TRY_AGAIN;
	  }
	  else
	  {
			m_componentInfoList = (const ComponentInfoList *) dlsym(m_fileHandle, "distributeComponents");
			
			if (!m_componentInfoList)
			{
			  m_errorMsg = dlerror();
				m_error = TRY_AGAIN;
		  }
		  else
		  {
				// Successful
				m_opened = true;
				m_error = OPENED;
			}
		}
	}
	
	if (m_error == NEW)
	{
		// should never be
		KSIMDEBUG_VAR("m_error failed", getFilename());
	}
	
	return m_error;
}

bool KSimPackageHandle::isOpened() const
{
	return m_opened;
}	

KSimPackageHandle::eResult KSimPackageHandle::error() const
{
	return m_error;
}


QString KSimPackageHandle::errorMsg() const
{
	return m_errorMsg;
}



bool KSimPackageHandle::hasComponents() const
{
	return (m_componentInfoList != 0);
}

const ComponentInfoList * KSimPackageHandle::getComponentInfoList() const
{
	return m_componentInfoList;
}
