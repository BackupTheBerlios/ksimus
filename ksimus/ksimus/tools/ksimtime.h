/***************************************************************************
                          ksimtime.h  -  description
                             -------------------
    begin                : Sat Nov 4 2000
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

#ifndef KSIMTIME_H
#define KSIMTIME_H

// C-Includes

// QT-Includes
#include <qstring.h>


// KDE-Includes

// Project-Includes
#include "ksimtimebase.h"

// Forward declaration
class KSimTimeServer;


/**Base class for all timing classes
  *@author Rasmus Diekenbrock
  */


class KSimTime : public KSimTimeBase
{
public:
	KSimTime(const KSimTimeServer & time);
	KSimTime(const KSimTime & time);
	KSimTime(const KSimTimeBase & time);
	~KSimTime();
	
};


#endif
