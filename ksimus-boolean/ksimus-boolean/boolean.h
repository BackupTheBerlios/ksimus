/***************************************************************************
                          boolean.h  -  description
                             -------------------
    begin                : Sun May  6 22:47:09 CEST 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BOOLEAN_H
#define BOOLEAN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes

// Forward declaration
class KLocale;
class KInstance;
class PackageInfo;

/** The boolean package
  * @author Rasmus Diekenbrock
  */

/******************************************************************************************
 ******************************************************************************************
 **
 **  Modify the package properties below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

/**  Enter the package name. E.g. "Boolean" */
#define KSIMUS_PACKAGE_NAME              "Boolean"

/**  Enter the lower case package name. E.g. "boolean" */
#define KSIMUS_PACKAGE_LOWER_NAME        "ksimus-boolean"

/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

namespace KSimLibBoolean
{
	KInstance * getPackageInstance();
	const PackageInfo * getPackageInfo();

};  //namespace KSimLibBoolean



/** The init function. Don't change the name or signature! */
extern "C" const PackageInfo * init_ksimus_package(KLocale * ksimusLocale);

#endif
