/***************************************************************************
                          ksimfile.cpp  -  description
                             -------------------
    begin                : Thu May 18 2000
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

#include <qstring.h>
#include <kapplication.h>
#include <qfileinfo.h>
#include <qptrlist.h>

#include <klocale.h>
#include <kstandarddirs.h>

#include "ksimfile.h"
#include "ksimdebug.h"

//#define TMP_DIR "tmp"


KSimFile::KSimFile()
	:	setupOk(false)
{
}
KSimFile::~KSimFile()
{
	if(setupOk)
	{
		// Remove only if setup was okay
		QDir dir = getSessionDir();
		removeDir(dir);
	}
}
bool KSimFile::setupDir()
{
	bool res;
	
	res = setTmpDir();
	if (res == false)
	{
		return false;
	}

	res = setSessionDir();
	if (res == false)
	{
		return false;
	}
 	setupOk = true;
 	return true;
}


bool KSimFile::setTmpDir()
{
	QString tmpName = locateLocal("tmp",QString::fromLatin1("ksimus/"));
	if ( !tmpDir.cd(tmpName) )
	{
		errorText = i18n("Can't find directory %1").arg(tmpName);
		return false;
	}
	return true;
}

bool KSimFile::setSessionDir()
{
	sessionDir = getTmpDir();

	int a = 0;
	QString newDir;
	do
	{
		newDir.sprintf("session%i", a++);
	}
	while (sessionDir.exists(newDir));
	if (!sessionDir.mkdir(newDir))
	{
		errorText = i18n("Can't create the session directory");
		return false;
	}
	if ( !sessionDir.cd(newDir) )
	{
		errorText = i18n("Can't enter the session directory");
		return false;
	}
	return true;
}

const QString & KSimFile::getErrorText() const
{
	return errorText;
}

void KSimFile::removeDir(QDir & dir)
{
	QFileInfoListIterator fileInfoIt(*dir.entryInfoList());
	QFileInfo * file;
	for (; fileInfoIt.current(); ++fileInfoIt)
	{
		file = fileInfoIt.current();
		if ((file->fileName() != QString::fromLatin1(".")) && (file->fileName() != QString::fromLatin1("..")))
		{
			if (file->isFile())
			{
				QFile::remove(file->absFilePath());
			}
			else if (file->isDir())
			{
				QDir olddir(file->absFilePath());
				removeDir (olddir);
			}
			else
			{
				KSIMDEBUG("unknown file type");
			}
		}
	}
	QDir delDir(dir);
	dir.cdUp();
	dir.rmdir(delDir.dirName());
}

			
		
			/** Returns the name of copy past file */
QString KSimFile::getCopyPastFilename() const
{
	QString name = getTmpDir().absPath()+"/copyAndPast.sim";
	return name;	
}
	
/** Returns true if copy past file exists */
bool KSimFile::existCopyPastFile()
{
	return QFile::exists(getCopyPastFilename());
}


QString KSimFile::relativePath(const QString & filename, const QString & relPath)
{
	QStringList fl(QStringList::split('/',filename));
	QStringList rl(QStringList::split('/',relPath));
	QString result;

	int common = 0;
	int i;

	// Find common part
	while ((common < (int)fl.count()) && (common < (int)rl.count()) && (fl[common] == rl[common]))
	{
//		kdDebug() << fl[common] << endl;
		common++;
	};

	// Change dir up to the common part
	i = common;
	while (i < (int)rl.count())
	{
		result += QString::fromLatin1("../");
		i++;
	};

	// Append the path to the file
	i = common;
	while (i < (int)fl.count()-1)
	{
		result += fl[i] + '/';
		i++;
	};
	if (i < (int)fl.count())
	{
		result += fl[i];
	};

	return result;
}
	
QString KSimFile::absolutePath(const QString & absPath, const QString & relFilename)
{
	if (QDir::isRelativePath(absPath))
	{
		KSIMDEBUG_VAR("should be absolute (Terminate function)", absPath);
		return QString::null;
	}
	
	if (!QDir::isRelativePath(relFilename))
	{
		KSIMDEBUG_VAR("should be relative (Terminate function)", relFilename);
		return QString::null;
	}
	
	return QDir::cleanDirPath(absPath + QString::fromLatin1("/") + relFilename);
}
