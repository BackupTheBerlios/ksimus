/***************************************************************************
                          ksimpackage.h  -  description
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

#ifndef KSIMPACKAGE_H
#define KSIMPACKAGE_H

//#define USE_DLFCN 1


// C-Includes
#ifdef USE_DLFCN
#  include <dlfcn.h>
#else
#  include <ltdl.h>
#endif

// QT-Includes
#include <qstring.h>
#include <qcstring.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class PackageInfo;

/**This class handles the additional packages.
  *@author Rasmus Diekenbrock
  */


class KSimPackageHandle
{
public: 
	
enum eResult { NEW, OPENED, BAD_FILE, TRY_AGAIN };
	
	/** Creates a new package handle.
		* The assigned package is filename.
		* The package is not accessed during creation.
		*/
	KSimPackageHandle(const QString & filename);
	
	/** The destructor */
	~KSimPackageHandle();
	
	/** Returns the filename of the package.
		*/
	QString getFilename() const;
	
	/** Opens the package.
		*	Returns a the result of the operation.
		*/
	eResult open();
	/** Returns true, if package is opended.
		*/
	bool isOpened() const;	
  /** Returns the latest error no.
  	*/
  eResult error() const;
	
  /** Returns the latest (error) message.
  	*/
  QString errorMsg() const;
	
	/** Returns true, if the opened file is a package.
		*/
	bool isPackage() const;
	/** Returns the PackageInfo of the package.
		*	Returns a null pointer, if no info is distributed by the package,
		* package is not opened, or something else goes wrong.
		*/
	const PackageInfo * getPackageInfo() const;

private:
	QString m_filename;
	eResult m_error;
	QString m_errorMsg;	
#ifdef USE_DLFCN
	void * m_fileHandle;
#else
	lt_dlhandle m_fileHandle;
#endif
	bool m_opened;
	const PackageInfo * m_packageInfo;
};

#endif
