/***************************************************************************
                          componentmap.h  -  description
                             -------------------
    begin                : Sun Apr 2 2000
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

#ifndef COMPONENTMAP_H
#define COMPONENTMAP_H

// C-Includes

// QT-Includes
#include <qmemarray.h>
#include <qsize.h>
#include <qptrlist.h>
#include <qpoint.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class QPainter;
class ConnectorList;


/**A 2D Array
  *@author Rasmus Diekenbrock
  */

typedef int mapItem;
//typedef QPtrList<QPoint> tPointList;

class CPointList     : public QPtrList<QPoint> {};
class CPointListList : public QPtrList<CPointList> {};




#define COST_NONE			10
#define COST_COMPONENT		2000
#define COST_WIRE			1000


class ComponentMap {
public: 
	ComponentMap();
	~ComponentMap();
	
	/** (re)size the map. Deletes data */
	void setSize(QSize newSize);
	/** set all items to value */
	void fill(mapItem value);

	/** Return the value of a item  */
	mapItem getItem(unsigned int x, unsigned int y) const;
	/** Set the value of a item */
	void setItem(unsigned int x, unsigned int y, mapItem value);
	/** Add the value of a item to the actual value */
	void addItem(unsigned int x, unsigned int y, mapItem value);
	/** Returns the cost of the given place */
	int getCost(QRect & place) const;
	void addPlace(const QRect & place, mapItem value);
	void addRoute(CPointList * route, mapItem value);
	void addRouteSegment(CPointList * route, mapItem value, int start);
	void addRouteList(CPointListList * routeList, mapItem value);
	/** Draws the map (for debuging only) */
	void draw (QPainter * p);
	/**  */
	QSize getSize() const;
	/** Search a "optimal" route "from" "to".
		After return, the start point, stop point, and edges are stored in route.
		The return value is true, if a route is founded and false, if no route is founded */
	bool findRoute(const QPoint & from, const QPoint & to, CPointList * route);
	bool findRoute(ConnectorList * connList, CPointListList * routeList);
  /** Searches the "best" place and returns the top left position */
  QPoint findBestPlace(const QSize & size) const;

	mapItem costNone;
	mapItem costComponent;
	mapItem costWire;

protected: // Protected methods
	int getIndex(int x, int y) const { return y * mapSize.width() + x; };
	QMemArray<mapItem> * costMap;
	QMemArray<mapItem> * map;
	QSize mapSize;
	/** Determine the route back */
	void findBackRoute(const QPoint & from, const QPoint & to, CPointList * route);
	/** Setup the cost value for each (requiered) position */
	void setupCostMap(const QPoint & from, const QPoint & to);
	static int searchCommonRoute(const CPointListList & routeList, CPointList * actRoute);
	void setDiagonalCost(const QPoint & pos, mapItem cost);
};

#endif
