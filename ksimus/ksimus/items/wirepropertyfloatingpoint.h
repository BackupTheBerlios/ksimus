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

const WirePropertyInfo * getPropertyFloatingPointInfo();

class WirePropertyFloatingPoint : public WirePropertySingleOutput
{

	Q_OBJECT
	
public:
	WirePropertyFloatingPoint(Wire * wire);
	virtual ~WirePropertyFloatingPoint();

	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const;


	static const WireColorScheme & colorScheme();

	/** Returns a @ref WatchItemFloatingPointWireProperty object. */
	virtual WatchItemBase * makeWatchItem();

};

#endif
