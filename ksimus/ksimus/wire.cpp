/***************************************************************************
                          wire.cpp  -  description
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


// Include C-Header
#include <qpainter.h>
#include <klocale.h>


// Include Project-Header
#include "ksimdata.h"
#include "compcontainer.h"
#include "wire.h"
#include "ksimdebug.h"
#include "resource.h"
#include "globals.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "library.h"
#include "componentinfo.h"
#include "connectorinfo.h"
#include "wirepropertylibrary.h"
#include "wirepropertyinfo.h"
#include "componentmap.h"

#include "wireproperty.h"

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Wire(container, ci);
}

const ComponentInfo WireInfo (	"Wire",
								"Wire/Wire",
								QString::null,
								VA_SHEETVIEW,
								create	);

//###############################################################

WireSV::WireSV(Component * comp)
	: CompView(comp, SHEET_VIEW)
{
	CompView::updateSheetMap(false);
	routeList = new CPointListList;	
	routeList->setAutoDelete(true);
}
WireSV::~WireSV()
{
	updateSheetMap(false);
	CompView::updateSheetMap(true);
	delete routeList;
}

void WireSV::draw(QPainter * p)
{
	CPointList * list;
	QPoint * first;
	
	Wire * wire = (Wire*)getComponent();
	
	if (wire->m_wireProperty)
	{
		wire->m_wireProperty->setupColorScheme(p);
	}
	else
	{
		KSIMDEBUG("wireProperty is NULL");
		wire->getConnList()->at(0)->setupColorScheme(p);
	}
	
	for (unsigned int j = 0; j < routeList->count(); j++)
	{
        list = routeList->at(j);
        first = list->at(0);
		
		if (j != 0)
		{
			p->drawEllipse(first->x()*gridX+gridX/2-3, first->y()*gridY+gridY/2-3, 6 , 6);
		}			
		for (unsigned int i = 1; i < list->count(); i++)
		{
			QPoint * pos = list->at(i);
			p->drawLine(first->x()*gridX+gridX/2, first->y()*gridY+gridY/2,
			 			pos->x()*gridX+gridX/2, pos->y()*gridY+gridY/2);
			 			
			first = pos;
		}
	}
}

void WireSV::drawBound(QPainter *)
{
}

// Search new routes
void WireSV::route()
{
	updateSheetMap(false);
	getComponent()->getSheetMap()->findRoute(getComponent()->getConnList(), routeList);
	updateSheetMap(true);
}	

/** if insert = true, insert compview to sheet map
if insert = true, delete compview to sheet map */
void WireSV::updateSheetMap(bool insert)
{
	if (getComponent()->getContainer()->isVisible())
	{
		mapItem value;
		if (insert)
		{
			value = getComponent()->getSheetMap()->costWire;
		}
		else
		{
			value = -getComponent()->getSheetMap()->costWire;
		}
	
		getComponent()->getSheetMap()->addRouteList(routeList, value);
	}
}
	
/** Hit point x,y the component ?
	NO_HIT
	NORMAL_HIT   - component is hit, KSimEditor controls mouse action
	CONNCTOR_HIT - connector of a component is hit
	WIRE_HIT     - wire is hit, KSimEditor controls mouse action
	SPECIAL_HIT  - component is hit, component controls mouse action */
eHitType WireSV::isHit(int x, int y) const
{
	x /= gridX;
	y /= gridY;
	
	
	for (QListIterator<CPointList> it(*routeList); it.current(); ++it)
	{
        if (isWireHit(it.current(),x,y))
        {
			// Remember "a" connector
			CompView::lastHitConnector = getComponent()->getConnList()->first();
			return WIRE_HIT;        	
		}
	}
	return NO_HIT;
}

/** New position of the wire view - not used !!! */
void WireSV::setPos(const QPoint &)
{
}
/** save WireSV properties - not used !!! */
void WireSV::save(KSimData &) const
{
}

/** Load WireSV properties - not used !!!
	Returns true if successful */
bool WireSV::load(KSimData &)
{
	return true;
}

int WireSV::isWireHit(CPointList * list, int x, int y)
{
	const QPoint * pos;
	const QPoint * prePos;
	
	for (unsigned int i = 1; i < list->count(); i++)
	{
		pos = list->at(i);
		if ((pos->x() == x) || (pos->y() == y))
		{
			prePos = list->at(i-1);
			if ((pos->x() == x)
				&& (   (( pos->y() >= y ) && ( y >= prePos->y()))
					|| (( pos->y() <= y ) && ( y <= prePos->y())) ) )
			{
				return i;
			}
			else
			if ((pos->y() == y)
				&& (   (( pos->x() >= x ) && ( x >= prePos->x()))
					|| (( pos->x() <= x ) && ( x <= prePos->x())) ) )
			{
				return i;
			}
		}
		else
		{
			// A hit from here to next edge isn't possible
			i++;
		}
	}
	return 0;
}


/**  */
const CPointListList * WireSV::getRouteList() const
{
	return routeList;
}


//###############################################################

Wire::Wire(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_wireProperty(0),
		m_dataCache(0)
{
	m_isWire = true;
	new WireSV(this);
	getConnList()->setAutoDelete (false);
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);

}

Wire::~Wire()
{
	ConnectorBase * conn;
	while (0 != (conn = getConnList()->first()))
	{
		delConnector(conn);
	}
	if (m_wireProperty)
		delete m_wireProperty;
}

void Wire::addConnector(ConnectorBase * conn)
{
	if (-1 == getConnList()->findRef(conn))
	{
		// Outputs first
		if (conn->isOutput() || conn->isTriState())
		{
			unsigned int i = 0;
			while ((getConnList()->count()>i) && !(getConnList()->at(i)->isInput()))
			{
				i++;
			};
			getConnList()->insert(i,conn);
		}
		else
		{
			//Inputs last
			getConnList()->append(conn);
		}
		conn->setWire(this);
	}
	else
	{
		KSIMDEBUG("Connector exists in connector list");
	}
}

void Wire::delConnector(ConnectorBase * conn)
{
	if (-1 != getConnList()->findRef(conn))
	{
		if (conn->getWire() == this)
			conn->setWire(0);
		getConnList()->remove(conn);
	}
	else
	{
		KSIMDEBUG("Connector doesn't exist in connector list");
	}
}

/** save wire properties */
void Wire::save(KSimData & file) const
{
	ConnectorBase * conn;
	QString baseGroup;
	QString connGroup;
	
	Component::save(file);
	
	baseGroup = file.group();
	
	// Save connections
	file.setGroup(baseGroup + "Connections/");
	file.writeEntry("Number", getConnList()->count());
	connGroup = file.group();
	for (unsigned int i=0; i < getConnList()->count(); i++)
	{
		QString dir;
		conn = getConnList()->at(i);
		ASSERT (conn->getComponent()->getSerialNumber() != 0);
		dir.sprintf("Connection %u/",i);
		file.setGroup(connGroup + dir );
		file.writeEntry("Component", conn->getComponent()->getSerialNumber());
		file.writeEntry("Connector", conn->getWireName());
	}
	
	// Save property
	if(m_wireProperty)
	{
		
		file.setGroup(baseGroup + "Property/");
		file.writeEntry("Type", m_wireProperty->getInfo()->getLibName());
		
		m_wireProperty->save(file);
		
	}
		
	// Restore group
	file.setGroup(baseGroup);
}

/** Load wire properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Wire::load(KSimData & file, bool copyLoad)
{
	bool ok;
	QString str;
	QString connName;
	Component * comp;
	ConnectorBase * conn;
	unsigned int compNo;
	unsigned int count;
	bool found;
	QString baseGroup;
	QString connGroup;
	QString dir;

#define LOAD_DEBUG	
	
	
	ok = Component::load(file, copyLoad);
    	
    baseGroup = file.group();

	// Load connections
	file.setGroup(baseGroup + "Connections/");
	count = file.readUnsignedNumEntry("Number", 0);
	connGroup = file.group();
	for (unsigned int i=0; i < count; i++)
	{
		dir.sprintf("Connection %u/",i);
		file.setGroup(connGroup + dir );
		
		compNo = file.readUnsignedNumEntry("Component", 0);
		ASSERT (compNo != 0);
		
		connName = file.readEntry("Connector", 0);

		// Search component
		found = false;
		comp = getContainer()->searchComponentBySerialNumber(compNo);
		if (!comp)
		{
			#ifdef LOAD_DEBUG			
				KSIMDEBUG_VAR("Component not found", compNo);
			#endif
		}
		// Fail, if connector not exist
		else if (!(conn = comp->searchConnector(connName)))
		{
			#ifdef LOAD_DEBUG			
				KSIMDEBUG_VAR("Connector not exist", connName);
			#endif
		}
		// TODO: Type test !!!
		
		// Fail, if connector is used
		else if (0 != conn->getWire())
		{
			#ifdef LOAD_DEBUG			
				KSIMDEBUG_VAR("Connector is already connected", connName);
			#endif
		}
		else
		{
			addConnector(conn);
		}
	}		
		
	// (re)load wire property
	file.setGroup(baseGroup + "Property/");
		
	WireProperty * newWireProperty;
	QString type = file.readEntry("Type");
	
	g_library->getWirePropertyLib()->create(&newWireProperty,this,type);
	
	if(newWireProperty)
	{
		ok &= newWireProperty->load(file);
	}
	else
	{
		KSIMDEBUG_VAR("Create WireProperty failed", type);
		const WirePropertyInfo * info = findWireProperty(getConnList());
		if (info)
		{
			newWireProperty = info->create(this);
			KSIMDEBUG_VAR("Create Default WireProperty", info->getDataType());
		}
		else
		{
			KSIMDEBUG("Load Wire faild");
			ASSERT(0);
			ok = false;
		}
	}
	
	if(m_wireProperty)
	{
		delete m_wireProperty;
	}
	m_wireProperty = newWireProperty;
	file.setGroup(baseGroup);
	
	return ok;
}

/** Find the suitable wire property class for the given connectors
*   Returns a null pointer, if no suitable wire property class is found
*/
const WirePropertyInfo * Wire::findWireProperty(const ConnectorList * list)
{
	const char * outputType = 0;
	const WirePropertyInfo * wirePropInfo = 0;
	bool searchFailed = false;
	
	CHECK_PTR(list);
	
	//##### 1. All outputs and tristate outputs must be the same data type
	FOR_EACH_CONNECTOR(it,*list)
	{
		if (it.current()->isOutput() || it.current()->isTriState())
		{
			if (outputType)
			{
				// Compare data types
				if (strcmp(outputType, it.current()->getConnInfo()->getDataType()))
				{
					// Different
					searchFailed = true;
					KSIMDEBUG("Different outputs");
					KSIMDEBUG_VAR("",outputType);
					KSIMDEBUG_VAR("",it.current()->getConnInfo()->getDataType());
				}
			}
			else
			{
				// Store first output
				outputType = it.current()->getConnInfo()->getDataType();
			}
		}
	}
	
	if (!outputType)
	{
		// No output found
		// First Input set dataType
		CHECK_PTR(list->getFirst());
		outputType = list->getFirst()->getConnInfo()->getDataType();
	}
	
	//##### 2. Inputs have to fit to dataType
	FOR_EACH_CONNECTOR(it,*list)
	{
		if (it.current()->isInput())
		{
			// Only same typedata at the moment !!!
			// Compare data types
			if (strcmp(outputType, it.current()->getConnInfo()->getDataType()))
			{
				// Different
				searchFailed = true;
				KSIMDEBUG("Different dataTypes");
				KSIMDEBUG_VAR("",outputType);
				KSIMDEBUG_VAR("",it.current()->getConnInfo()->getDataType());
			}
		}
	}
		
	// Search wirePropertyInfo
	if (outputType && !searchFailed)
	{
		// Output found
		wirePropInfo = g_library->getWirePropertyLib()->findDataType(outputType);
		
		if (!wirePropInfo)
		{
			KSIMDEBUG_VAR("dataType not found", outputType);
		}
	}
	return wirePropInfo;	
}
	

/** Wrapper function for const WirePropertyInfo * Wire::findWireProperty(const ConnectorList * list) */
const WirePropertyInfo * Wire::findWireProperty(const ConnectorBase * start, const ConnectorBase * end)
{
	// both connectors not wired
	if ((start->getWire() == 0) && (end->getWire() == 0))
	{
		ConnectorList list;
		list.append(start);
		list.append(end);
		return Wire::findWireProperty(&list);
	}
	// Both connectors are wired
	else if ((start->getWire() != 0) && (end->getWire() != 0))
	{
		if (start->getWire()->getConnList()->count() < end->getWire()->getConnList()->count())
		{
			// swap start and end
			const ConnectorBase * dummy = start;
			start = end;
			end = dummy;
		}
		
		ConnectorList list(*start->getWire()->getConnList());
		FOR_EACH_CONNECTOR(it,*end->getWire()->getConnList())
		{
			list.append(it.current());
		}
		return Wire::findWireProperty(&list);
	}
	// Only "start" connector wired
	else if ((start->getWire() != 0) && (end->getWire() == 0))
	{
		ConnectorList list(*start->getWire()->getConnList());
		list.append(end);
		return Wire::findWireProperty(&list);
	}
	// Only "end" connector wired
	else if ((start->getWire() == 0) && (end->getWire() != 0))
	{
		ConnectorList list(*end->getWire()->getConnList());
		list.append(start);
		return Wire::findWireProperty(&list);
	}
	else
	{
		ASSERT (0);
		return 0;
	}
}

/** Returns a pointer to the current data */	
const void * Wire::getCurrentData()
{
	if(m_dataCache)
	{
		// Data is cached
		return m_dataCache;
	}
		
	if(m_wireProperty)
	{
		m_dataCache = m_wireProperty->getCurrentData();
		return m_dataCache;
	}
	
	// Failed !!
	KSIMDEBUG_VAR("Wire has no wire property", getName());
	return 0;
}

/** Set a new wire property */
void Wire::setProperty(const WirePropertyInfo * wirePropInfo)
{
	if (!m_wireProperty)
	{
		m_wireProperty = wirePropInfo->create(this);
	}
	else if (wirePropInfo == m_wireProperty->getInfo())
	{
		// Same property, do nothing
	}
	else
	{
		delete m_wireProperty;
		m_wireProperty = wirePropInfo->create(this);
	}
}
		

/** Returns the current wire property */
const WirePropertyInfo * Wire::getProperty() const
{
	return m_wireProperty->getInfo();
}

/** Checks the component
*   eg. all required inputs are connected.
*   The implementation calls the checks functions of the wire property.
*   Returns the number of errors
*/
int Wire::checkCircuit()
{
	if (m_wireProperty)
	{
		return m_wireProperty->checkCircuit();
	}
	else
	{
		// No wire property set
		QString msg;
		logError(i18n("INTERNAL ERROR *** Wire has no property ***"));
		return 1;
	}		
}

/** Shift the result of calculation to output
*		The implementaion clears the data cache */
void Wire::updateOutput()
{
	m_dataCache = 0;
}

/** Reset all simulation variables
*		The implementaion clears the data cache */
void Wire::reset()
{
	Component::reset();
	m_dataCache = 0;
}
