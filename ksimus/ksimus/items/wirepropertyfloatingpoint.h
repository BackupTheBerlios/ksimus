/***************************************************************************
                          wirepropertyfloatingpoint.h  -  description
                             -------------------
    begin                : Fri Dec 28 2001
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

#ifndef WIREPROPERTYFLOATINGPOINT_H
#define WIREPROPERTYFLOATINGPOINT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "wireproperty.h"

// Forward declaration


/**Wire property for floating point connector
  *@author Rasmus Diekenbrock
  */

extern const WirePropertyInfo wirePropertyFloatingPointInfo;

class WirePropertyFloatingPoint : public WirePropertySingleOutput
{

	Q_OBJECT
	
public:
	WirePropertyFloatingPoint(Wire * wire);
	virtual ~WirePropertyFloatingPoint();

	/** Returns a pointer to the current data */	
	virtual const void * getCurrentData();
	
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;


	static void colorScheme(QPainter * p);

};

#endif
