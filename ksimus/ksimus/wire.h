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

// include QT-Header
#include <qptrlist.h>
#include <qpoint.h>
#include <qcolor.h>


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

const ComponentInfo * getWireInfo();


//##################################################################
//##################################################################

class WireColorScheme
{
public:
	WireColorScheme();
	WireColorScheme(QColor wireColor, unsigned int width = 2);
	WireColorScheme(QColor wireForegroundColor, QColor wireBackgroundColor, unsigned int width = 2);
	
	bool isDualColor() const;
	const QColor & getColor() const { return m_foreground; };
	const QColor & getForegroundColor() const { return m_foreground; };
	const QColor & getBackgroundColor() const { return m_background; };
	unsigned int getWidth() const { return m_width; }

private:
	QColor m_foreground;
	QColor m_background;
	unsigned int m_width;
};




//##################################################################
//##################################################################


class WireSV: public CompView
{

Q_OBJECT

public:
	WireSV(Component * comp, const char * name);
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
	void protectConnectorEdgeing(bool ena);
	CPointListList * routeList;
	
private:
	class XpmBitmap;
	
};


//##################################################################
//##################################################################

class Wire : public Component
{

Q_OBJECT

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
	/** Checks the component property. The functions is called after the
	*   property dialog.
	*   The implementation checks nothing.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);
	
	void addConnector(ConnectorBase * conn);
	void delConnector(ConnectorBase * conn);
	
	/** save wire properties */
	virtual void save(KSimData & file) const;
	/** load wire properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Setup the Component for a new circuit execution.
	  * Calls the setup functions of wire properties.
	  */
	virtual void setupCircuit();
	
	/** Set a new wire property info. */
	void setPropertyInfo(const WirePropertyInfo * wirePropInfo);
	
	/** Returns the current wire property info. */
	const WirePropertyInfo * getPropertyInfo() const;
	
	/** Returns the current wire property. */
	WireProperty * getWireProperty() { return m_wireProperty; };

	/** Returns the current wire property. */
	const WireProperty * getWireProperty() const { return m_wireProperty; };

	/** Find the suitable wire property class for the given connectors
	*   Returns a null pointer, if no suitable wire property class is found
	*/
	static const WirePropertyInfo * findWirePropertyInfo(const ConnectorList * list);
	/** Wrapper function for const WirePropertyInfo * Wire::findWireProperty(const ConnectorList * list) */
	static const WirePropertyInfo * findWirePropertyInfo(const ConnectorBase * start, const ConnectorBase * end);
	
protected: // Protected attributes
	WireProperty * m_wireProperty;
};

#endif
