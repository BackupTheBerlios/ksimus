/***************************************************************************
                          boolean.h  -  description
                             -------------------
    begin                : Son Mai  6 22:47:09 CEST 2001
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
#include "ksimus/componentinfo.h"

// Forward declaration
class KLocale;

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
#define PACKAGE_NAME              "Boolean"

/**  Enter the lower case package name. E.g. "boolean" */
#define PACKAGE_LOWER_NAME        "ksimus-boolean"

/**  Enter the library namespace. Format KSimLib + package name. E.g. KSimLibBoolean */
#define LIB_NAMESPACE             KSimLibBoolean

/**  Enter the init function name. Format init_lib + lower case package name.
     E.g. init_libboolean  (use "_" instead of "-") */
#define PACKAGE_INIT_FUNCTION     init_libksimus_boolean



/******************************************************************************************
 ******************************************************************************************
 **
 **  Changes only the namespace below !!!
 **
 ******************************************************************************************
 ******************************************************************************************/

namespace KSimLibBoolean
{
extern KInstance * instance;
extern const PackageInfo * packageInfo;


};  //namespace KSimLibBoolean

/** The init function. PACKAGE_INIT_FUNCTION is a macro which expands to the real function name. */
extern "C" const PackageInfo * PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale);

#endif
