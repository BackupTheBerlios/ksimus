/***************************************************************************
                          wirepropertyinteger.h  -  description
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

#ifndef WIREPROPERTYINTEGER_H
#define WIREPROPERTYINTEGER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "wireproperty.h"
#include "ksimbaseint.h"

// Forward declaration


/**Wire property for integer connector
  *@author Rasmus Diekenbrock
  */


class WirePropertyInteger : public WirePropertySingleOutput
{

	Q_OBJECT
	
public:
	WirePropertyInteger(Wire * wire);
	virtual ~WirePropertyInteger();

	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const;


	static const WireColorScheme & colorScheme();

	/** Returns a @ref WatchItemIntegerWireProperty object. */
	virtual WatchItemBase * makeWatchItem();

	static const WirePropertyInfo * getStaticInfo();

protected:
	static WireProperty * create(Wire * wire);
};

#endif
