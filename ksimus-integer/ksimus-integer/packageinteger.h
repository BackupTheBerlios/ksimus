/***************************************************************************
                          packageinteger.h  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef PACKAGEINTEGER_H
#define PACKAGEINTEGER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes

// Forward declaration
class KLocale;
class KInstance;
class PackageInfo;

/** The integer package
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
#define KSIMUS_PACKAGE_NAME              "Integer"

/**  Enter the lower case package name. E.g. "boolean" */
#define KSIMUS_PACKAGE_LOWER_NAME        "ksimus-integer"

/**  Enter the init function name. Format init_lib + lower case package name.
     E.g. init_libboolean  (use "_" instead of "-") */
#define KSIMUS_PACKAGE_INIT_FUNCTION     init_libksimus_integer



/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

namespace KSimLibInteger
{
	KInstance * getPackageInstance();
	const PackageInfo * getPackageInfo();

};  //namespace KSimLibInteger

/** The init function. PACKAGE_INIT_FUNCTION is a macro which expands to the real function name. */
extern "C" const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale);

#endif   // PACKAGEINETGER_H
