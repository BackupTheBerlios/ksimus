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

// Include QT header
#include <qpainter.h>
#include <qimage.h>
#include <qbitmap.h>

// Include KDE header
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
#include "connectorinfo.h"
#include "wirepropertylibrary.h"
#include "wirepropertyinfo.h"
#include "componentmap.h"
#include "wireproperty.h"
#include "implicitconverterlibrary.h"

// Forward declaration



// Helper class
// Creates immediatly a QBitmap from a XPM (used in static initializer)
class WireSV_Bitmap : public QBitmap
{
public:
	WireSV_Bitmap(const char * const xpm[])
	{
		*static_cast<QBitmap *>(this) = QImage(xpm);
		setMask(*this); // selfMask 
	}
};




static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Wire(container, ci);
}

const ComponentInfo * getWireInfo()
{
	static const ComponentInfo Info(QString::fromLatin1("Wire"),
	                                QString::fromLatin1("Wire/Wire"),
	                                QString::fromLatin1("Wire/Wire"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create);
	return &Info;
}

//###############################################################



WireColorScheme::WireColorScheme()
	:	m_foreground(),
		m_background()
{
}

WireColorScheme::WireColorScheme(QColor wireColor, unsigned int width)
	:	m_foreground(wireColor),
		m_background(),
		m_width(width)
{
}

WireColorScheme::WireColorScheme(QColor wireForegroundColor, QColor wireBackgroundColor, unsigned int width)
	:	m_foreground(wireForegroundColor),
		m_background(wireBackgroundColor),
		m_width(width)
{
}

bool WireColorScheme::isDualColor() const
{
	return m_background.isValid();
}

//###############################################################

WireSV::WireSV(Component * comp, const char * name)
	: CompView(comp, SHEET_VIEW, name)
{
	CompView::updateSheetMap(false);
	routeList = new CPointListList;
	Q_CHECK_PTR(routeList);
	routeList->setAutoDelete(true);
}
WireSV::~WireSV()
{
	updateSheetMap(false);
	CompView::updateSheetMap(true);
	delete routeList;
}

static inline void WireSV_drawLineSolid(QPainter * p, int x1, int y1, int x2, int y2, const QColor & color)
{
	// use rect
	p->setPen(color);
	p->setBrush(color);
	if (x1 > x2)
	{
		const int i = x1;
		x1  = x2;
		x2 = i;
	}
	if (y1 > y2)
	{
		const int i = y1;
		y1  = y2;
		y2 = i;
	}

	if (x1 == x2)
	{
		p->drawRect(x1-1, y1, 2, y2-y1);
	}
	else
	{
		p->drawRect(x1, y1-1, x2-x1, 2);
	}
}

static inline void WireSV_drawLineDot(QPainter * p, int x1, int y1, int x2, int y2, const QColor & color)
{
	// use lines
	p->setPen(QPen(color, 1, Qt::DotLine));
	if (x1 == x2)
	{
		p->drawLine(x1-1, y1, x1-1, y2);
		p->drawLine(x1,   y1, x1,   y2);
	}
	else
	{
		// y1 == y2 !!!
		p->drawLine(x1, y1-1, x2, y1-1);
		p->drawLine(x1, y1,   x2, y1);
	}
}


void WireSV::draw(QPainter * p)
{
	const Wire * const wire = (Wire*)getComponent();

	const WireColorScheme & colorScheme = (wire->m_wireProperty)
	                                    ? wire->getWireProperty()->getColorScheme()
	                                    : wire->getConnList()->at(0)->getColorScheme();

	for (unsigned int j = 0; j < routeList->count(); j++)
	{
		CPointList * list = routeList->at(j);
		int x2, y2;
		int x1 = list->at(0)->x()*gridX+gridX/2;
		int y1 = list->at(0)->y()*gridY+gridY/2;

		for (unsigned int i = 1; i < list->count(); i++)
		{
			x2 = list->at(i)->x()*gridX+gridX/2;
			y2 = list->at(i)->y()*gridY+gridY/2;

			if (colorScheme.isDualColor())
			{
				WireSV_drawLineSolid(p, x1, y1, x2, y2, colorScheme.getBackgroundColor());
				WireSV_drawLineDot(p, x1, y1, x2, y2, colorScheme.getForegroundColor());
			}
			else
			{
				WireSV_drawLineSolid(p, x1, y1, x2, y2, colorScheme.getColor());
			}

			x1 = x2;
			y1 = y2;
		}
	}

	if (routeList->count() > 1)
	{
		// The "outter" bubble
		static const char * const xpmOutter[] =
		{
			"6 6 2 1",
			" 	c #FFFFFF",
			".	c #000000",
			" .... ",
			"......",
			"......",
			"......",
			"......",
			" .... "
		};
		static WireSV_Bitmap mapOutter(xpmOutter);

		if (colorScheme.isDualColor())
		{
			// The "inner" bubble
			static const char * const xpmInner[] =
			{
				"6 6 2 1",
				" 	c #FFFFFF",
				".	c #000000",
				"  ..  ",
				"  ..  ",
				"..  ..",
				"..  ..",
				"  ..  ",
				"  ..  "
			};
			static WireSV_Bitmap mapInner(xpmInner);

			for (unsigned int j = 1; j < routeList->count(); j++)
			{
				const int x = routeList->at(j)->at(0)->x()*gridX+1;
				const int y = routeList->at(j)->at(0)->y()*gridY+1;
				p->setPen(colorScheme.getBackgroundColor());
				p->drawPixmap(x, y, mapOutter);
				p->setPen(colorScheme.getForegroundColor());
				p->drawPixmap(x, y, mapInner);
			}
		}
		else
		{
			p->setPen(colorScheme.getColor());
			for (unsigned int j = 1; j < routeList->count(); j++)
			{
				const int x = routeList->at(j)->at(0)->x()*gridX+1;
				const int y = routeList->at(j)->at(0)->y()*gridY+1;
				p->drawPixmap(x, y, mapOutter);
			}
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
	if (getComponent()->getContainer()->isVisible() && !isHidden())
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
	
	for (QPtrListIterator<CPointList> it(*routeList); it.current(); ++it)
	{
		if (isWireHit(it.current(),x,y))
		{
			// Remember "a" connector
			CompView::s_lastHitConnector = getComponent()->getConnList()->first();
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
		m_wireProperty(0)
{
//	m_isWire = true;
	setComponentType(eWire);
	new WireSV(this, "WireSV");
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
	delete m_wireProperty;
}

void Wire::addConnector(ConnectorBase * conn)
{
	if (-1 == getConnList()->findRef(conn))
	{
		// Outputs first
		if (conn->isOutput() || conn->isTristate())
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
	file.setGroup(baseGroup + QString::fromLatin1("Connections/"));
	file.writeEntry("Number", getConnList()->count());
	connGroup = file.group();
	for (unsigned int i=0; i < getConnList()->count(); i++)
	{
		QString dir;
		conn = getConnList()->at(i);
		Q_ASSERT (conn->getComponent()->getSerialNumber() != 0);
		dir.sprintf("Connection %u/",i);
		file.setGroup(connGroup + dir );
		file.writeEntry("Component", conn->getComponent()->getSerialNumber());
		file.writeEntry("Connector", conn->getWireName());
	}
	
	// Save property
	if(m_wireProperty)
	{
		
		file.setGroup(baseGroup + QString::fromLatin1("Property/"));
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

//#define LOAD_DEBUG	
	
	
	ok = Component::load(file, copyLoad);
	
	baseGroup = file.group();
	
	// Load connections
	file.setGroup(baseGroup + QString::fromLatin1("Connections/"));
	count = file.readUnsignedNumEntry("Number", 0);
	connGroup = file.group();
	for (unsigned int i=0; i < count; i++)
	{
		dir.sprintf("Connection %u/",i);
		file.setGroup(connGroup + dir );
		
		compNo = file.readUnsignedNumEntry("Component", 0);
		Q_ASSERT (compNo != 0);
		
		connName = file.readEntry("Connector", QString::null);

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
	file.setGroup(baseGroup + QString::fromLatin1("Property/"));
		
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
		const WirePropertyInfo * info = findWirePropertyInfo(getConnList());
		if (info)
		{
			newWireProperty = info->create(this);
			KSIMDEBUG_VAR("Create Default WireProperty", info->getDataType());
		}
		else
		{
			KSIMDEBUG("Load Wire faild");
			Q_ASSERT(0);
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
const WirePropertyInfo * Wire::findWirePropertyInfo(const ConnectorList * list)
{
	QString outputType;
	const WirePropertyInfo * wirePropInfo = 0;
	bool outputFound = false;
	
	Q_CHECK_PTR(list);
	
	//##### 1. All outputs and tristate outputs must be the same data type
	FOR_EACH_CONNECTOR(it,*list)
	{
		if (it.current()->isOutput() || it.current()->isTristate())
		{
			if (outputType.isNull())
			{
				// Store first output
				outputType = it.current()->getConnInfo()->getDataType();
				outputFound = true;
			}
			else
			{
				// Compare data types
				if (outputType != it.current()->getConnInfo()->getDataType())
				{
					// Different
/*					KSIMDEBUG("Different outputs");
					KSIMDEBUG_VAR("",outputType);
					KSIMDEBUG_VAR("",it.current()->getConnInfo()->getDataType());*/
					
					return getWirePropertyInvalidDifferentOutputsInfo();
				}
			}
		}
	}
	
	if (outputType.isNull())
	{
		// No output found
		// First Input set dataType
		Q_CHECK_PTR(list->getFirst());
		outputType = list->getFirst()->getConnInfo()->getDataType();
	}
	
	//##### 2. Inputs have to fit to dataType
	FOR_EACH_CONNECTOR(it,*list)
	{
		if (it.current()->isInput())
		{
			// Compare data types
			if (outputType != it.current()->getConnInfo()->getDataType())
			{
				// Different
/*				KSIMDEBUG("Different dataTypes");
				KSIMDEBUG_VAR("",outputType);
				KSIMDEBUG_VAR("",it.current()->getConnInfo()->getDataType());*/
				
				if (outputFound)
				{
					const ImplicitConverterInfo * ici = g_library->getImplicitConverterLib()
					              ->findDataType(outputType, it.current()->getConnInfo()->getDataType());
					if (!ici)
					{
						// No implicit converter found
						return getWirePropertyInvalidIncompatibleInputsInfo();
					}
				}
				else
				{
					return getWirePropertyInvalidDifferentInputsNoOutputInfo();
				}
			}
		}
	}
		
	// Search wirePropertyInfo
	if (!outputType.isNull())
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
const WirePropertyInfo * Wire::findWirePropertyInfo(const ConnectorBase * start, const ConnectorBase * end)
{
	if ((start->getWire() == 0) && (end->getWire() == 0))
	{
		// Both connectors not wired
		ConnectorList list;
		list.append(start);
		list.append(end);
		return Wire::findWirePropertyInfo(&list);
	}
	else if ((start->getWire() != 0) && (end->getWire() != 0))
	{
		// Both connectors are wired
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
		return Wire::findWirePropertyInfo(&list);
	}
	else if ((start->getWire() != 0) && (end->getWire() == 0))
	{
		// Only "start" connector wired
		ConnectorList list(*start->getWire()->getConnList());
		list.append(end);
		return Wire::findWirePropertyInfo(&list);
	}
	else if ((start->getWire() == 0) && (end->getWire() != 0))
	{
		// Only "end" connector wired
		ConnectorList list(*end->getWire()->getConnList());
		list.append(start);
		return Wire::findWirePropertyInfo(&list);
	}
	else
	{
		Q_ASSERT (0);
		return (const WirePropertyInfo *)0;
	}
}

/** Set a new wire property info. */
void Wire::setPropertyInfo(const WirePropertyInfo * wirePropInfo)
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


/** Returns the current wire property info. */
const WirePropertyInfo * Wire::getPropertyInfo() const
{
	return m_wireProperty->getInfo();
}

/** Returns the current wire property. */
/*WireProperty * Wire::getWireProperty()
{
	return m_wireProperty;
}*/

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

void Wire::checkProperty(QStringList & /*errorMsg*/)
{
	// Check nothing
}

void Wire::setupCircuit()
{
	if (m_wireProperty)
	{
		m_wireProperty->setupCircuit();
	}
}

#include "wire.moc"
