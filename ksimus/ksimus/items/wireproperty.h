/***************************************************************************
                          wireproperty.h  -  description
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

#ifndef WIREPROPERTY_H
#define WIREPROPERTY_H

#include <qobject.h>
#include "componentitem.h"

class Wire;
class WirePropertyInfo;
class QPopupMenu;
class KSimData;
class QPainter;

/**Base class for all wire properties
  *@author Rasmus Diekenbrock
  */

class WireProperty : public QObject, public ComponentItem
{

	Q_OBJECT

public:
	virtual ~WireProperty();
	
	/** Save wire properties. */
	virtual void save(KSimData & file) const;
	/** Load wire properties.
		*	Returns true if successful. */
	virtual bool load(KSimData & file);

	/** Checks the connected component.
	*   eg. No output connected or more than one connected.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	/** Returns a pointer to the current data.
	*   Returns null, if no output is active (TRISTATE).
	*/	
	virtual const void * getCurrentData() const = 0;
	
	/** Add menu items depend on wire properties.
	  *	Return true, if items are added. */
	virtual bool initPopupMenu(QPopupMenu * popup) const;
	
	/** Returns the associated WirePropertyInfo. */
	const WirePropertyInfo * getInfo() const;

	/** Returns the wire. */
	Wire * getWire() const { return (Wire *)getComponent(); };
		
	/** Setup the colors, brushs, and fills for the connector. */
	virtual void setupColorScheme (QPainter * p) const = 0;

protected:	
	WireProperty(Wire * wire, const WirePropertyInfo * wirePropertyInfo);

	const WirePropertyInfo * m_wirePropertyInfo;
	int m_inputCounter;
	int m_outputCounter;
	int m_triStateCounter;
	
};

class WirePropertySingleOutput : public WireProperty
{

	Q_OBJECT

public:	
	/** Checks the connected component.
		eg. No output connected or more than one connected.
		Returns the number of errors.
	*/
	virtual int checkCircuit();

protected:	
	WirePropertySingleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo);

};

#endif
