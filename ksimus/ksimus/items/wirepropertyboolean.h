/***************************************************************************
                          wirepropertyboolean.h  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#ifndef WIREPROPERTYBOOLEAN_H
#define WIREPROPERTYBOOLEAN_H

#include "wireproperty.h"

/**Wire property for boolean connector
  *@author Rasmus Diekenbrock
  */

extern const WirePropertyInfo wirePropertyBooleanInfo;

class WirePropertyBoolean : public WirePropertySingleOutput
{

	Q_OBJECT
	
public:
	WirePropertyBoolean(Wire * wire);
	virtual ~WirePropertyBoolean();

	/** Returns a pointer to the current data */	
	virtual const void * getCurrentData() const;
	
	// Setup the colors, brushs, and fills for the connector
	virtual void setupColorScheme (QPainter * p) const;

};

#endif
