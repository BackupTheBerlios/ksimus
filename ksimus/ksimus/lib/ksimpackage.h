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

// C-Includes

// QT-Includes
#include <qstring.h>

// KDE-Includes

// Project-Includes
#include "componentinfo.h"
#include "connectorinfo.h"
#include "wirepropertyinfo.h"

// Forward declaration

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
	
	/** Returns true, if the opened package distributes components.
		*/
	bool hasComponents() const;
	/** Returns a simple array of ComponentsInfo.
		*	Returns a null pointer, if no componenets are distributed by the package,
		* or package is not opened.
		*/
	const ComponentInfoList * getComponentInfoList() const;


private:
	QString m_filename;
	eResult m_error;
	QString m_errorMsg;	
	void * m_fileHandle;
	bool m_opened;
	const ComponentInfoList * m_componentInfoList;
	const ConnectorInfoList * m_connectorInfoList;
	const WirePropertyInfoList * m_wirePropertyInfoList;
};

#endif
