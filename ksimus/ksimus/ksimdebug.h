/***************************************************************************
                          ksimdebug.h  -  description
                             -------------------
    begin                : Tue Feb 6 2001
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

#ifndef _KSIMDEBUG_H_
#define _KSIMDEBUG_H_


#include <kdebug.h>

#ifdef DEBUG
#define __DEBUG_INFO__        __FILE__ << "(" << __LINE__ << ") "
#define KSIMDEBUG(str) 	 	 {kdDebug() << __DEBUG_INFO__ << str << endl;}
#define KSIMDEBUG_VAR(str,x) {kdDebug() << __DEBUG_INFO__ << str << " " << #x << " = " << x << endl;}
#define KSIMDEBUG_FUNCTION   {kdDebug() << k_funcinfo << endl;}
#else
#define KSIMDEBUG(str) 	
#define KSIMDEBUG_VAR(str,x)
#define KSIMDEBUG_FUNCTION
#endif


#endif  // _KSIMDEBUG_H_
