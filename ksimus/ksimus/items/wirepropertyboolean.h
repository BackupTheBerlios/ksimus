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

const WirePropertyInfo * getWirePropertyBooleanInfo();

class WirePropertyBoolean : public WirePropertySingleOutput
{

	Q_OBJECT
	
public:
	WirePropertyBoolean(Wire * wire);
	virtual ~WirePropertyBoolean();

	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const;

	static const QString & getI18nTextValue(bool value);
	static const QString & getI18nTextTrue();
	static const QString & getI18nTextFalse();

	/** Returns a @ref WatchItemBooleanWireProperty object. */
	virtual WatchItemBase * makeWatchItem();

};

#endif
