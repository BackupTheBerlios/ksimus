/***************************************************************************
                          wire.h  -  description
                             -------------------
    begin                : Wed Mar 29 2000
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

#ifndef WIRE_H
#define WIRE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// include QT-Header
#include <qlist.h>
#include <qpoint.h>


// include Project-Header
#include "types.h"
#include "component.h"
#include "compview.h"

class CompContainer;
class ConnectorBase;
class WireProperty;
class WirePropertyInfo;
class CPointList;
class CPointListList;

/**
  *@author Rasmus Diekenbrock
  */

extern const ComponentInfo WireInfo;

class WireSV: public CompView  {
public:
	WireSV(Component * comp);
	~WireSV();
	virtual void draw(QPainter * p);
	virtual void drawBound(QPainter * p);
	
	/** Hit point x,y the component ?
		NO_HIT
		NORMAL_HIT   - component is hit, KSimEditor controls mouse action
		CONNCTOR_HIT - connector of a component is hit
		WIRE_HIT     - wire is hit, KSimEditor controls mouse action
		SPECIAL_HIT  - component is hit, component controls mouse action */
	virtual eHitType isHit(int x, int y) const;
	static int isWireHit(CPointList * list, int x, int y);
	
	/** if insert = true, insert compview to sheet map
		if insert = true, delete compview to sheet map */
	virtual void updateSheetMap(bool insert);
	void route();
	/** New position of the wire view */
	virtual void setPos(const QPoint & pos);
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*	Returns true if successful */
	virtual bool load(KSimData & file);
	
	const CPointListList * getRouteList() const;
		

protected:
	CPointListList * routeList;	
};


class Wire : public Component
{

public:
	
	friend class WireSV;

	Wire(CompContainer * container, const ComponentInfo * ci);
	~Wire();
	
	/** Checks the component
	*   eg. all required inputs are connected.
	*   The implementation calls the checks functions of the wire property.
	*   Returns the number of errors
	*/
	int checkCircuit();
	
	void addConnector(ConnectorBase * conn);
	void delConnector(ConnectorBase * conn);
	
	/** save wire properties */
	virtual void save(KSimData & file) const;
	/** load wire properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Shift the result of calculation to output
	*		The implementaion clears the data cache */
	virtual void updateOutput();
	/** Reset all simulation variables */
	virtual void reset();
	
	/** Returns a pointer to the current data */	
	const void * getCurrentData();
	
	/** Set a new wire property */
	void setProperty(const WirePropertyInfo * wirePropInfo);
	
	/** Returns the current wire property */
	const WirePropertyInfo * getProperty() const;
	
	
	/** Find the suitable wire property class for the given connectors
	*   Returns a null pointer, if no suitable wire property class is found
	*/
	static const WirePropertyInfo * findWireProperty(const ConnectorList * list);
	/** Wrapper function for const WirePropertyInfo * Wire::findWireProperty(const ConnectorList * list) */
	static const WirePropertyInfo * findWireProperty(const ConnectorBase * start, const ConnectorBase * end);
	
protected: // Protected attributes
	WireProperty * m_wireProperty;
	const void * m_dataCache;	


};

#endif
