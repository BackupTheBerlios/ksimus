/***************************************************************************
                          ksimdebug.cpp  -  description
                             -------------------
    begin                : Sun Feb 22 2004
    copyright            : (C) 2004 by Rasmus Diekenbrock
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
#include "ksimdebug.h"


// Forward declaration
 
KSimDebugIndent __kSimDebugIndent__;

QString KSimDebugIndent::print() const
{
	QString s; 
	return s.fill(QChar(' '), 2 * m_indent);
};

void KSimDebugInstanceCounter::printResult() const
{
	fprintf(stderr, "InstanceCounter: %s count: %i\n", m_name, m_cnt);
}

