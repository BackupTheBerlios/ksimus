/***************************************************************************
                          ksimtime.cpp  -  description
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

// C-Includes
#include <stdio.h>

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimdata.h"
#include "ksimtime.h"
#include "ksimdebug.h"

// Forward declaration

KSimTime::KSimTime(const KSimTimeServer & time)
	: KSimTimeBase(time)
{
}

KSimTime::KSimTime(const KSimTime & time)
	: KSimTimeBase(time)
{
}

KSimTime::KSimTime(const KSimTimeBase & time)
	: KSimTimeBase(time)
{
}

KSimTime::~KSimTime()
{
}
