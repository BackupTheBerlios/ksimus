/***************************************************************************
                          componentmap.cpp  -  description
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

#include <limits.h>
#include <qpainter.h>
#include <queue>


#include "resource.h"

#include "componentmap.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "ksimdebug.h"

typedef queue<QPoint> tPointQueue;

//#define SETUP_COST_MAP_SHOW_LOOPS

ComponentMap::ComponentMap()
	:	costNone(10),
		costComponent(1000),
		costWire(100)
{
	map = new QArray<mapItem>;
	costMap = new QArray<mapItem>;
}
ComponentMap::~ComponentMap()
{
	delete costMap;
	delete map;
}
/** (re)size the map. Deletes data */
void ComponentMap::setSize(QSize newSize)
{
	int elements = newSize.width() * newSize.height();
	if ((!map->resize(elements)) || (!costMap->resize(elements)))
	{
		KSIMDEBUG("resize failed");
	}
	else
	{
		mapSize = newSize;
	}
	map->fill(costNone);
	costMap->fill(INT_MAX);
}
/** Set the value of a item */
void ComponentMap::setItem(unsigned int x, unsigned int y, mapItem value)
{
	map->at( getIndex(x,y) ) = value;
}
/** get the value of a item */
mapItem ComponentMap::getItem(unsigned int x, unsigned int y) const
{
	return map->at( getIndex(x,y) );
}
  /** Add the value of a item to the actual value */
void ComponentMap::addItem(unsigned int x, unsigned int y, mapItem value)
{
	map->at( getIndex(x,y) ) += value;
}
/** set all items to value */
void ComponentMap::fill(mapItem value)
{
	map->fill(value);
}
/** Returns the cost of the given place */
int ComponentMap::getPlace(QRect & place) const
{
	int i,j;
	int cost = 0;
	
    for (i=place.top(); i <= place.bottom(); i++)
    {
    	for (j=place.left(); j <= place.right(); j++)
    	{
			cost += map->at( getIndex(j,i) );
		}
	}
	return cost;
}    		

void ComponentMap::addPlace(QRect & place, mapItem value)
{
	int i,j, index;
	
	QRect _place = place.normalize();
	
	for (i=_place.top(); i <= _place.bottom(); i++)
	{
		index = getIndex(_place.left(), i);
		for (j=_place.left(); j <= _place.right(); j++)
		{
				map->at(index++) += value;
		}
	}
}    		
void ComponentMap::addRoute(CPointList * route, mapItem value)
{

    for (unsigned int i=0; i < route->count()-1; i++)
    {
    	QRect rect(*route->at(i),*route->at(i+1));
    	addPlace(rect, value);
//		map->at( getIndex(route->at(i)->x(),route->at(i)->y()) ) += value;
	}
}    		
void ComponentMap::addRouteSegment(CPointList * route, mapItem value, int start)
{
    for (unsigned int i=start; i < route->count(); i++)
    {
		map->at( getIndex(route->at(i)->x(),route->at(i)->y()) ) += value;
	}
}    		
void ComponentMap::addRouteList(CPointListList * routeList, mapItem value)
{
    for (unsigned int i=0; i < routeList->count(); i++)
    {
		addRoute(routeList->at(i), value);
		if (i != 0)
		{
			// Don't mark the first grid
			addItem(routeList->at(i)->at(0)->x(),routeList->at(i)->at(0)->y(), -value);
		}
	}
}    		
/** Draws the map (for debuging only) */
void ComponentMap::draw (QPainter * p)
{
	int i,j;

    for (i=0; i < mapSize.height(); i++)
    {
    	for (j=0; j < mapSize.width(); j++)
    	{
			mapItem value = map->at(getIndex(j,i));
			if (value == costNone)
			{
			// draw nothing
			}
			else if (value == costComponent+costNone)
			{
				p->fillRect(j*gridX, i*gridY, gridX, gridY, Qt::lightGray);
			}
			else if (value == costWire+costNone)
			{
				p->fillRect(j*gridX, i*gridY, gridX, gridY, Qt::gray);
			}
			else
			{
				p->fillRect(j*gridX, i*gridY, gridX, gridY, Qt::darkGray);
			}
		}
	}
}
/**  */
QSize ComponentMap::getSize() const
{
	return mapSize;
}

bool ComponentMap::findRoute(ConnectorList * connList, CPointListList * routeList)
{
	ConnectorBase * startConn;
	ConnectorBase * destConn;
	ConnectorBase * conn;
	CPointList * wireRoute;
	ConnectorList tmpConnList;
	CPointListList tmpRouteList;
	QList<int> tmpStartList;
	QPoint center;
	
		
	// Wire connects more than one connectors
	if (connList->count() > 1)
	{
		tmpStartList.setAutoDelete(true);
		tmpRouteList.setAutoDelete(true);
		
		//*** Sort connectors ***
		// Search center of all connectors
		center = QPoint(0,0);
		for (unsigned int i = 0; i < connList->count(); i++)
		{
			center += connList->at(i)->getWirePos();
		}
		center /= (int)connList->count();
		
		FOR_EACH_CONNECTOR(itConn, *connList)
		{
			int diffA, diffB;
			unsigned int j;
			conn = itConn.current();
			diffA = abs(center.x()-conn->getWirePos().x())
					+ abs(center.y()-conn->getWirePos().y());
			for (j = 0; j < tmpConnList.count(); j++)
			{
				diffB = abs(center.x()-tmpConnList.at(j)->getWirePos().x())
						+ abs(center.y()-tmpConnList.at(j)->getWirePos().y());
				if (diffA < diffB)
					break;
			}
			tmpConnList.insert(j, conn);
		}
		
		// Remove old routes
		while (!routeList->isEmpty())
		{
			routeList->remove();
		}
			
		startConn = tmpConnList.first();
		for(unsigned int i = 1; tmpConnList.count() > i; i++)
		{
			destConn = tmpConnList.at(i);
			
			QPoint stop(destConn->getWireGridPos());
			
			QPoint start(tmpConnList.first()->getWireGridPos());
			for(unsigned int j = 1; i > j; j++)
			{
				QPoint next(tmpConnList.at(j)->getWireGridPos());
				if (abs(next.x()-stop.x())+abs(next.y()-stop.y())
					< abs(start.x()-stop.x())+abs(start.y()-stop.y()))
				{
					start = next;
				}
			}
			
			wireRoute = new CPointList;
			wireRoute->setAutoDelete(true);

			if (findRoute(&start,&stop,wireRoute))
			{
				// search common routes
				int startIndex = searchCommonRoute(&tmpRouteList, wireRoute);
				// add new route
				int * pInt = new int;
				*pInt = startIndex;
				tmpStartList.append(pInt);
				tmpRouteList.append(wireRoute);
				
				// No cost if same route for same wire
				if (tmpRouteList.count() == 1)
				{
					addRouteSegment(wireRoute, -(costNone-1), 0);
				}
				else
				{
					//Don't mark the first grid
					addRouteSegment(wireRoute, -(costNone-1), 1);
				}
			}
			else
			{
				KSIMDEBUG("No route found");
				delete wireRoute;
			}
		}
	}
	else
	{
		KSIMDEBUG("Wire with one or no connectors");
	}
	
	// Reset cost map
	// Remove common routes
	for (unsigned int i = 0; i< tmpRouteList.count(); i++)
	{
		QPoint lastEdge, lastPos, pos, *newPos;
		
		if (i == 0)
		{
			addRouteSegment(tmpRouteList.at(i), costNone-1,0);
		}
		else
		{
			//Don't mark the first grid
			addRouteSegment(tmpRouteList.at(i), costNone-1,1);
		}

		
		wireRoute = new CPointList;
		wireRoute->setAutoDelete(true);
		lastPos = lastEdge = *tmpRouteList.at(i)->first();
		newPos = new QPoint(lastPos);
		wireRoute->append(newPos);
		
 		for (unsigned int j = 0; j< tmpRouteList.at(i)->count(); j++)
		{
			pos = *tmpRouteList.at(i)->at(j);
			if ((pos.x() != lastEdge.x()) && (pos.y() != lastEdge.y()))
			{
				lastEdge = lastPos;
				newPos = new QPoint(lastPos);
				wireRoute->append(newPos);
			}
			lastPos = pos;
			
		}
		// save stop position	
		newPos = new QPoint(lastPos);
		wireRoute->append(newPos);
		
		routeList->append(wireRoute);
	}
	return true;
}	

/** Search a "optimal" route "from" "to".
After return, the start point, stop point, and edges are stored in route.
The return value is true, if a route is found and false, if no route is found */
bool ComponentMap::findRoute(QPoint * from, QPoint * to, CPointList * route)
{
	// first remove all old Point from route
	while (!route->isEmpty())
	{
		route->remove();
	}
	
	setupCostMap(to, from);
	findBackRoute(to, from, route);

	return true;
}

int ComponentMap::searchCommonRoute(CPointListList * routeList, CPointList * actRoute)
{
	bool found = false;
	int res = 0;
	
	if (routeList->count() > 0)
	{
		QListIterator<CPointList> it(*routeList);

		// Test points beginning last
		for (int i = (int)actRoute->count()-1; i>=0 && !found; i--)
		{
			// Search longest common route
			for (it.toFirst(); it.current() && !found; ++it)
			{
				for (int j = (int)it.current()->count()-1; j>=0 && !found; j--)
				{
					if (*it.current()->at(j) == *actRoute->at(i))
					{
						res = i;
						found = true;
					}
				}
			}
		}
		
		unsigned int a = actRoute->count() - res;
		// Remove common route
		while (actRoute->count() > a)
		{
			actRoute->removeFirst();
		}
	}
	return res;
}

/** Setup the cost value for each (requiered) position */
void ComponentMap::setupCostMap(QPoint * from, QPoint * to)
{
	// Queue for points to visit
	tPointQueue pointQueue;
	// Queue for points to visit
	// These points lays on a component and are normaly not used
	tPointQueue pointQueueCross;
	bool ready = false;
					
	QPoint pos;
	int localCost;
	int maxCost;
	int newCost;
	unsigned int index;
	unsigned int testIndex;
	mapItem oldStopValue;
	
	QPoint start(*from);
	QPoint stop(*to);
	
	maxCost = INT_MAX;
	// setup costMap
	costMap->fill(INT_MAX);
	
	// Set start pos
	pointQueue.push(start);
	costMap->at(getIndex(start.x(), start.y())) = 0;
	
	// Store stop point value
	oldStopValue = map->at(getIndex(stop.x(), stop.y()));
	map->at(getIndex(stop.x(), stop.y())) = 0;
	
	#ifdef SETUP_COST_MAP_SHOW_LOOPS
		int count = 0;
	#endif
	
	while (!ready)
	{
		#ifdef SETUP_COST_MAP_SHOW_LOOPS
			count ++;
		#endif
		if (!pointQueue.empty())
		{
			pos = pointQueue.front();
			pointQueue.pop();
		}
		else if (!pointQueueCross.empty())
		{
			pos = pointQueueCross.front();
			pointQueueCross.pop();
		}
		else
		{
			ready = true;
		}
		
		index = getIndex(pos.x(), pos.y());
		localCost = costMap->at(index);
		
		if (pos == stop)
		{
			// Destination reached, remember max cost
			if (maxCost > localCost)
				maxCost = localCost;
		}
		else if (maxCost > localCost)
		{
			// Test relative position (-1,0)
			if (pos.x() > 0)
			{
				testIndex = index - 1;
				newCost = localCost + map->at(testIndex);
				if ((newCost < costMap->at(testIndex)))// && (newCost < maxCost ))
				{
					costMap->at(testIndex) = newCost;
					if (newCost < costComponent)
						pointQueue.push(pos + QPoint(-1,0));
					else
						pointQueueCross.push(pos + QPoint(-1,0));
				}
			}
			
			// Test relative position (0,-1)
			if (pos.y() > 0)
			{
				testIndex = index - mapSize.width();
				newCost = localCost + map->at(testIndex);
				if ((newCost < costMap->at(testIndex)))// && (newCost < maxCost ))
				{
					costMap->at(testIndex) = newCost;
					if (newCost < costComponent)
						pointQueue.push(pos + QPoint(0,-1));
					else
						pointQueueCross.push(pos + QPoint(0,-1));
				}
			}
			
			// Test relative position (+1,0)
			if (pos.x() < mapSize.width()-1)
			{
				testIndex = index + 1;
				newCost = localCost + map->at(testIndex);
				if ((newCost < costMap->at(testIndex)))// && (newCost < maxCost ))
				{
					costMap->at(testIndex) = newCost;
					if (newCost < costComponent)
						pointQueue.push(pos + QPoint(+1,0));
					else
						pointQueueCross.push(pos + QPoint(+1,0));
				}
			}
			
			// Test relative position (0,+1)
			if (pos.y() < mapSize.height()-1)
			{
				testIndex = index + mapSize.width();
				newCost = localCost + map->at(testIndex);
				if ((newCost < costMap->at(testIndex)))// && (newCost < maxCost ))
				{
					costMap->at(testIndex) = newCost;
					if (newCost < costComponent)
						pointQueue.push(pos + QPoint(0,+1));
					else
						pointQueueCross.push(pos + QPoint(0,+1));
				}
			}
		}
	}

	// Restore stop point value
	map->at(getIndex(stop.x(), stop.y())) = oldStopValue;
	
	#ifdef SETUP_COST_MAP_SHOW_LOOPS
		KSIMDEBUG_VAR("Loops",count);
		KSIMDEBUG_VAR("maxCost",maxCost);
	#endif
}

/** Determine the route back */
void ComponentMap::findBackRoute(QPoint * from, QPoint * to, CPointList * route)
{
	tPointQueue backRoute;
	QPoint pos;
	int newCost;
	unsigned int index;
	int dir = 0;
	int bestDir = 0;
	
	QPoint start(*from);
	QPoint stop(*to);
	
	backRoute.push(stop);

    while (stop != start)
    {
	   	newCost = INT_MAX;
    	
    	index = getIndex(stop.x(), stop.y());
        	
    	for (int i=0;i<4;i++)
    	{
	    	switch(dir)
    		{
	    		case 0:
	    		if (stop.x() > 0)
	    		{
    				if (newCost > costMap->at(index - 1))
    				{
	    				newCost = costMap->at(index - 1);
			    		pos = stop + QPoint(-1,0);
			    		bestDir = 0;
				    }
				}
				break;
    	
	    		case 1:
	    		if (stop.y() > 0)
		    	{
    				if (newCost > costMap->at(index - mapSize.width()))
    				{
    					newCost = costMap->at(index - mapSize.width());
		    			pos = stop + QPoint(0,-1);
				    	bestDir = 1;
				    }
				}
				break;

		    	case 2:
    			if (stop.x() < mapSize.width()-1)
	    		{
    				if (newCost > costMap->at(index + 1))
    				{
    					newCost = costMap->at(index + 1);
			    		pos = stop + QPoint(1,0);
				    	bestDir = 2;
				    }
				}
				break;

		    	case 3:
    			if (stop.y() < mapSize.height()-1)
	    		{
    				if (newCost > costMap->at(index + mapSize.width()))
	    			{
    					newCost = costMap->at(index + mapSize.width());
			    		pos = stop + QPoint(0,1);
				    	bestDir = 3;
			    	}
				}
				break;
			}
		dir = (dir+1) & 0x03;
		}
		
		dir = bestDir;
		stop = pos;
		backRoute.push(stop);
	}

	
	{
		QPoint pos, *newPos;

		while (!backRoute.empty())
		{
			pos = backRoute.front();
			backRoute.pop();
			newPos = new QPoint(pos);
			route->append(newPos);
		}
	}
}
/** Searches the "best" place and returns the top left position */
QPoint ComponentMap::findBestPlace(const QSize & size) const
{
#define ADD_X	2
#define ADD_Y	ADD_X
	
	QPoint bestPos(0,0);
	int bestCost = INT_MAX;
	int minCost = size.height() * size.width() * costNone;
	int cost;
	
	int x = 0;
	int y = 0;
	
	do
	{
		y = 0;
		do		
		{
			QRect place(QPoint(x,y), size);
			cost = getPlace (place);
			
			// min cost found -> exit
			if (cost == minCost)
			{
				return QPoint(x,y);
			}
			// "Better" place found ?
			if (bestCost > cost)
			{
				bestCost = cost;
				bestPos = QPoint(x,y);
			}
			
			y += ADD_Y;
		}
		while ( (y + size.height()) < mapSize.height());
		x += ADD_X;
	}
	while ( (x + size.width()) < mapSize.width());
	
	return bestPos;
}
