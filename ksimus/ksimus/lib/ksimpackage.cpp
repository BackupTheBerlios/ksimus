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


// QT-Includes
#include <qfileinfo.h>

// KDE-Includes
#include <kglobal.h>
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
		m_packageInfo(0)
{
#ifdef USE_DLFCN
	// nothing todo
#else
	int res = lt_dlinit();
	if (res != 0)
	{
		KSIMDEBUG_VAR("ERROR: lt_dlinit()",lt_dlerror());
	}
#endif
}

KSimPackageHandle::~KSimPackageHandle()
{
#ifdef USE_DLFCN
	if (m_fileHandle)
		dlclose(m_fileHandle);
#else
	int res;
	
	if (m_fileHandle)
	{
		res = lt_dlclose(m_fileHandle);
		if (res != 0)
		{
			KSIMDEBUG_VAR("ERROR: lt_dlclose()",lt_dlerror());
		}
	}
	
	res = lt_dlexit();
	if (res != 0)
	{
		KSIMDEBUG_VAR("ERROR: lt_dlexit()",lt_dlerror());
	}
#endif
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
/*	else if(!fileInfo.exists())
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
	}*/
	else
	{
		#ifdef USE_DLFCN
			m_fileHandle = dlopen(getFilename(), RTLD_NOW);
		#else
			m_fileHandle = lt_dlopenext(getFilename());
		#endif
		//KSIMDEBUG_VAR("",getFilename());
		if (m_fileHandle == 0)
		{
			#ifdef USE_DLFCN
  			m_errorMsg = dlerror();
			#else
	  		m_errorMsg = lt_dlerror();
			#endif
			m_error = TRY_AGAIN;
	  }
	  else
	  {
			#ifdef USE_DLFCN
				void *sym = dlsym(m_fileHandle, getInitFunctionName());
			#else
				void *sym = lt_dlsym(m_fileHandle, getInitFunctionName());
			#endif
				
			if (sym)
			{
//				KSIMDEBUG("Exec init function 1");
				typedef const PackageInfo * (*t_func)(KLocale *);
				t_func func = (t_func)sym;
				m_packageInfo = func(KGlobal::locale());
//				KSIMDEBUG_VAR("Exec init function",getFilename());
				if (!m_packageInfo)
				{
				m_error = TRY_AGAIN;
				}
				else
				{
					// Successful
					m_opened = true;
					m_error = OPENED;
				}
			}
			else
			{
				KSIMDEBUG_VAR("No init function",getFilename());
				#ifdef USE_DLFCN
				m_errorMsg = dlerror();
				#else
				m_errorMsg = lt_dlerror();
				#endif
				m_error = TRY_AGAIN;
			}
		}
	}
	
	if (m_error == NEW)
	{
		// should be never
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



bool KSimPackageHandle::isPackage() const
{
	return (m_packageInfo != 0);
}

const PackageInfo * KSimPackageHandle::getPackageInfo() const
{
	return m_packageInfo;
}

QCString KSimPackageHandle::getInitFunctionName() const
{
	int i;
	QCString funcname;
	QFileInfo fi(getFilename());
	QString filename(fi.baseName());
	
	// Relaces "-" with "_"
	while (-1 != (i = filename.find(QString::fromLatin1("-"))))
	{
		filename.replace(i, 1, QString::fromLatin1("_"));
	}
	
	
	funcname.sprintf("init_%s", filename.latin1());
	
	return funcname;
}
