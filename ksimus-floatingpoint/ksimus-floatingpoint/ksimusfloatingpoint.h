/***************************************************************************
                          ksimusfloatingpoint.h  -  description
                             -------------------
    begin                : Fre Feb  1 18:30:45 CET 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#ifndef KSIMUSFLOATINGPOINT_H
#define KSIMUSFLOATINGPOINT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/componentinfo.h"
#include "ksimusfloatingpoint.h"

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
#define KSIMUS_PACKAGE_NAME              "FloatingPoint"

/**  Enter the lower case package name. E.g. "boolean" */
#define KSIMUS_PACKAGE_LOWER_NAME        "ksimus-floatingpoint"

/**  Enter the init function name. Format init_lib + lower case package name. E.g. init_libboolean
     E.g. init_libboolean  (use "_" instead of "-") */
#define KSIMUS_PACKAGE_INIT_FUNCTION     init_libksimus_floatingpoint



/** The init function. KSIMUS_PACKAGE_INIT_FUNCTION is a macro which expands to the real function name. */
extern "C" const PackageInfo * KSIMUS_PACKAGE_INIT_FUNCTION(KLocale * ksimusLocale);

#endif  //KSIMUSFLOATINGPOINT_H
