/***************************************************************************
                          compview.cpp  -  description
                             -------------------
    begin                : Thu Mar 16 2000
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

// C-Includes

// QT-Includes
#include <qevent.h>
#include <qpainter.h>
#include <qobjectlist.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "compview.h"
#include "component.h"
#include "componentaddon.h"
#include "compcontainer.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "componentmap.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimwidget.h"
#include "ksimeditor.h"
#include "ksimundo.h"
#include "ksimdebug.h"
#include "resource.h"

// Forward declaration





static const char * sPos = "Pos";
static const char * sSize = "Size";
static const char * sHide = "Hidden";

static const int sheetGridX = gridX;
static const int sheetGridY = gridY;


// Used for m_flags
//#define FLAGS_VIEW_CHANGED			0x0001
#define FLAGS_HIDDEN							0x0002
#define FLAGS_RESIZEABLE					0x0004
#define FLAGS_CONN_SPACING_TOP		0x0008
#define FLAGS_CONN_SPACING_RIGHT	0x0010
#define FLAGS_CONN_SPACING_BOTTOM	0x0020
#define FLAGS_CONN_SPACING_LEFT		0x0040
#define FLAGS_ENABLE_GRID_SNAP		0x0080
// 0x01xx - 0xFFxx Reserved for CompViewSize



static void installGlobalEventFilter(QWidget * main, QWidget * eventSource)
{
	main->installEventFilter(eventSource);
	
	const QObjectList * list = main->children();
	
    if (list)
    {
		QObject * obj;
	    for ( obj = ((QObjectList*)list)->first(); obj; obj = ((QObjectList*)list)->next() )
    	{
    		if (obj->isWidgetType())
	    	{
				installGlobalEventFilter((QWidget*)obj, eventSource);
	    	}
	    }
    }
}	

ConnectorBase * CompView::lastHitConnector;

class CompView::CompViewPrivate
{
public:
	CompViewPrivate(eViewType vt)
		:	place(0,0,0,0),
			posGridX(&sheetGridX),
			posGridY(&sheetGridY),
			viewType (vt),
			widgetList(0)
	{
		if (vt == SHEET_VIEW)
			flags =	FLAGS_CONN_SPACING_TOP | FLAGS_CONN_SPACING_RIGHT | FLAGS_CONN_SPACING_BOTTOM
					| FLAGS_CONN_SPACING_LEFT | FLAGS_ENABLE_GRID_SNAP;
		else
			flags =	FLAGS_ENABLE_GRID_SNAP;
	};
	~CompViewPrivate()
	{
		if (widgetList)
			delete widgetList;
		
	};
	
	/** Position and dimension of the component view */
	QRect place;
	// Some flags
	int flags;
	// Snaps
	const int * posGridX;
	const int * posGridY;
	
	eViewType viewType;
	KSimWidgetList * widgetList;
};

CompView::CompView(Component * comp, eViewType viewType)
	:	QObject(),
		ComponentItem(comp),
		m_viewChanged(false)
{
	m_p = new CompViewPrivate(viewType);
	
	switch(viewType)
	{
		case SHEET_VIEW:
			if (comp->m_sheetView)
			{
				KSIMDEBUG_VAR("m_sheetView is not null", comp->m_sheetView);
			}
			comp->m_sheetView = this;
			break;
				
		case USER_VIEW:
			if (comp->m_userView)
			{
				KSIMDEBUG_VAR("m_userView is not null", comp->m_userView);
			}
			comp->m_userView = this;
			break;
			
		default:
			KSIMDEBUG_VAR("Unknown viewType",(int)viewType);
			break;
	}
				
	updateSheetMap(true);
}

CompView::~CompView()
{
	updateSheetMap(false);
	delete m_p;
}

/** Returns the type of view
  	@see enum eViewType */
eViewType CompView::getViewType() const
{
	return m_p->viewType;
};

/** Change the type of view
   	@see enum eViewType */
void CompView::setViewType(eViewType newType)
{
	m_p->viewType = newType;
};


/** Manage the mouse pressed event if required */
void CompView::mousePress(QMouseEvent *, QPainter *)
{
}

/** Manage the mouse move event if required  */
void CompView::mouseMove(QMouseEvent *, QPainter *)
{
}
	
/** Manage the mouse released event if required  */
void CompView::mouseRelease(QMouseEvent *, QPainter *)
{
}
/** New position of the comoponent view */
void CompView::setPos(const QPoint & pos)
{
	QPoint newPos(pos);
	
	// Limit position
	QSize mapSize;
	if (getViewType() == SHEET_VIEW)
	{
		mapSize = getComponent()->getContainer()->getSheetSize();
	}
	else
	{
		mapSize = getComponent()->getContainer()->getUserSize();
	}
	KSIMDEBUG_VAR("", mapSize.width());
	KSIMDEBUG_VAR("", mapSize.height());
	if (newPos.x() > (mapSize.width() - getPlace().width()))
	{
		newPos.setX(mapSize.width() - getPlace().width());
	}
	if (newPos.y() > (mapSize.height() - getPlace().height()))
	{
		newPos.setY(mapSize.height() - getPlace().height());
	}
	if (newPos.x() < 0)
	{
		newPos.setX(0);
	}
	if (newPos.y() < 0)
	{
		newPos.setY(0);
	}
	
	// now map to grid
	if (isGridSnapEnabled())
	{
		newPos = mapToGrid(newPos);
	}
	
	// Remove Object from sheet map
	updateSheetMap(false);
	m_p->place.moveTopLeft(newPos);
	// Insert Object to sheet map
	updateSheetMap(true);

	emit signalMove(getPlace().topLeft());
	emit signalMoveWidget(getWidgetPlace().topLeft());
}

/** Changes the place of the view */
void CompView::setPlace(const QRect & place)
{
	QRect newPlace;
	
	if (isGridSnapEnabled())
	{
		newPlace = QRect(mapToGrid(place.topLeft()),mapToGrid(place.bottomRight()) + QPoint(-1,-1));
	}
	else
	{
		newPlace = place;
	}
	
	// Remove Object from sheet map
	updateSheetMap(false);
	m_p->place = newPlace;
	// Insert Object to sheet map
	updateSheetMap(true);

	emit signalMove(getPlace().topLeft());
	emit signalResize(getPlace().size());
	emit signalMoveWidget(getWidgetPlace().topLeft());
	emit signalResizeWidget(getWidgetPlace().size());
	
	resize();
}

void CompView::resize()
{
}

/** Give the position of the component view */
QPoint CompView::getPos() const
{
//	return getPlace().topLeft();
	return m_p->place.topLeft();
}
/**  */
QRect CompView::getPlace() const
{
	return m_p->place;
}

/**  */
QRect CompView::getWidgetPlace() const
{
	//Default complete place
	QRect widgetPlace(getPlace());
	
	if (isConnectorSpacingTop())
	{
		widgetPlace.rTop() += gridY;
	}

	if (isConnectorSpacingRight())
	{
		widgetPlace.rRight() -= gridX;
	}

	if (isConnectorSpacingBottom())
	{
		widgetPlace.rBottom() -= gridY;
	}

	if (isConnectorSpacingLeft())
	{
		widgetPlace.rLeft() += gridX;
	}
		
	return widgetPlace;
}


/** Hit point x,y the component ?
NO_HIT      - component is not hit
NORMAL_HIT  - component is hit, KSimView controls mouse action
SPECIAL_HIT - component is hit, component controls mouse action */

eHitType CompView::isHit(int x, int y) const
{
	QRect rect = getPlace().normalize ();
	if (x < rect.left() ||
		x > rect.right() ||
		y < rect.top() ||
		y > rect.bottom() ||
		isHidden())
	{
		return NO_HIT;
	}
	// connector hit
	// Exist List?
	if (getComponent()->getConnList())
	{
		ConnectorBase * conn;
		for (	conn = getComponent()->getConnList()->first();
				conn;
				conn = getComponent()->getConnList()->next())
		{
			if (CONNECTOR_HIT == conn->isHit(x,y))
			{
				// Remember connector
				lastHitConnector = conn;
				return CONNECTOR_HIT;
			}
				
		}
	}
	return NORMAL_HIT;

}

/** Draw the component view */
void CompView::draw (QPainter *p)
{
	if (getViewType() == SHEET_VIEW)
	{
		// Draw connectors
		// Exist List?
		if (getComponent()->getConnList())
		{
			ConnectorBase * conn;
			for (	conn = getComponent()->getConnList()->first();
					conn;
					conn = getComponent()->getConnList()->next())
			{
				if (!conn->isHidden())
				{
					conn->draw(p);
				}
			}
		}
		// Draw sheet view addons
		// Exist List?
		if (getComponent()->getAddOnList())
		{
			getComponent()->getAddOnList()->drawSheetView(p);
		}
	}
	else //USER_VIEW
	{
		// Draw user view addons
		// Exist List?
		if (getComponent()->getAddOnList())
		{
			getComponent()->getAddOnList()->drawUserView(p);
		}
	}
}

/** Draws the CompView to a printer */
void CompView::print(QPainter * paint)
{
	draw(paint);
}

/** Draw the boundary of the component
	Draw handles if required */
void CompView::drawBound(QPainter * p)
{
	p->save();
	p->setRasterOp (NotROP);
	p->setPen (DashLine);
	p->setBrush (NoBrush);
	p->drawRect (getPlace());
	p->restore();
}

void CompView::drawBound(QPainter * p, QPoint & tempPos)
{
	QRect actualPlace(getPlace());
	
	if(isGridSnapEnabled())
	{
		m_p->place.moveTopLeft(mapToGrid(tempPos));
	}
	else
	{
		m_p->place.moveTopLeft(tempPos);
	}
	
	drawBound(p);
	
	m_p->place = actualPlace;
}
	
/** Return last connector that was hit */
ConnectorBase * CompView::getLastHitConnector() const
{
	return lastHitConnector;
}
/** if insert = true, insert compview to sheet map
if insert = true, delete compview to sheet map */
void CompView::updateSheetMap(bool insert)
{
	QRect mapPlace;
	
	if (getComponent()->getContainer()->isVisible())
	{
		mapPlace.setLeft(getPlace().left()/gridX);
		mapPlace.setTop(getPlace().top()/gridY);
		mapPlace.setRight((getPlace().right())/gridX);
		mapPlace.setBottom((getPlace().bottom())/gridY);
		
//		ComponentMap * map = getComponent()->getContainer()->getSheetMap();
//		map->addPlace(mapPlace, insert ? map->costComponent : -map->costComponent );
		getComponentMap()->addPlace(mapPlace, insert
									?  getComponentMap()->costComponent
									: -getComponentMap()->costComponent );
	}
}
/** Creates a new Widget  */
QWidget * CompView::createCompViewWidget(QWidget *)
{
	return (QWidget *)0;
}

bool CompView::makeWidget(QWidget * parent)
{
	QWidget * widget;
	
	widget = createCompViewWidget(parent);
	
	if (widget)
	{
		// widget positioning
		widget->setGeometry(getWidgetPlace());
		resize();
		if(isHidden())
		{
			emit signalHide();
		}
		else
		{
			emit signalShow();
		}
		
		installGlobalEventFilter(widget, parent);

		return true;
	}
	else
		return false;
}

/**  */
QPoint CompView::mapToGrid(QPoint pos)
{
	QPoint res;
	
	res.setX(((pos.x() + *m_p->posGridX/2) / *m_p->posGridX)* *m_p->posGridX);
	res.setY(((pos.y() + *m_p->posGridY/2) / *m_p->posGridY)* *m_p->posGridY);
	
	return res;
}

/** save CompView properties */
void CompView::save(KSimData & file) const
{
	file.writeEntry(sPos, getPos());
	if (isHidden())
		file.writeEntry(sHide,true);
}

/** Load CompView properties
	Returns true if successful */
bool CompView::load(KSimData & file)
{
	setPos(file.readPointEntry(sPos));
	setHide(file.readBoolEntry(sHide, false));
	return true;
}

/** Returns the current Component Map */
ComponentMap * CompView::getComponentMap() const
{
	ComponentMap * map;
	
	switch(m_p->viewType)
	{
		case SHEET_VIEW:
			map = getComponent()->getSheetMap();
			break;
		
		case USER_VIEW:
			map = getComponent()->getUserMap();
			break;
			
		default:
			KSIMDEBUG_VAR("Unknown viewType",(int)m_p->viewType);
			map = 0;
			break;
	}
	return map;
}

/** Move the view to the best place off the map */
void CompView::moveToBestPlace()
{
	QSize gridSize;
	gridSize.setWidth((getPlace().width() + gridX - 1) / gridX);
	gridSize.setHeight((getPlace().height() + gridY - 1) / gridY);
	
	QPoint newPos = getComponentMap()->findBestPlace(gridSize);
	newPos.setX(newPos.x() * gridX);
	newPos.setY(newPos.y() * gridY);
	
	setPos(newPos);
}

/** Returns true, if last simulation changes the view */
/*bool CompView::isViewChanged() const
{
	return (m_p->flags & FLAGS_VIEW_CHANGED);
};*/

/*void CompView::setViewChanged(bool changed)
{
	if (changed)
		m_p->flags |= FLAGS_VIEW_CHANGED;
	else
		m_p->flags &= ~FLAGS_VIEW_CHANGED;
};*/

/** Hide the component view */
void CompView::setHide(bool hide)
{
	if(hide != isHidden())
	{
		if(hide)
		{
			m_p->flags |= FLAGS_HIDDEN;
			// Remove object from sheet map
			updateSheetMap(false);
			emit signalHide();
		}
		else
		{
			m_p->flags &= ~FLAGS_HIDDEN;
			// Insert object to sheet map
			updateSheetMap(true);
			emit signalShow();
		}
	}
}
	
/** Toogle the hide flag */
void CompView::slotToggleHide()
{
	setHide(!isHidden());
}

/** Returns true, if component view is hidden */
bool CompView::isHidden() const
{
	return (m_p->flags & FLAGS_HIDDEN);
}

/** Returns true, if component view can be resized */
bool CompView::isResizeable() const
{
	return (m_p->flags & FLAGS_RESIZEABLE);
}

/** Set true, if component view is resizeable */
void CompView::setResizeable(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_RESIZEABLE;
	else
		m_p->flags &= ~FLAGS_RESIZEABLE;
}

/** If enable is true, reserve space for connectors at the top of the component.
	If false, reserve no space.
*/
void CompView::enableConnectorSpacingTop(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_CONN_SPACING_TOP;
	else
		m_p->flags &= ~FLAGS_CONN_SPACING_TOP;
}

/** If enable is true, reserve space for connectors at the right of the component.
	If false, reserve no space.
*/
void CompView::enableConnectorSpacingRight(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_CONN_SPACING_RIGHT;
	else
		m_p->flags &= ~FLAGS_CONN_SPACING_RIGHT;
}

/** If enable is true, reserve space for connectors at the bottom of the component.
	If false, reserve no space.
*/
void CompView::enableConnectorSpacingBottom(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_CONN_SPACING_BOTTOM;
	else
		m_p->flags &= ~FLAGS_CONN_SPACING_BOTTOM;
}

/** If enable is true, reserve space for connectors at the left of the component.
	If false, reserve no space.
*/
void CompView::enableConnectorSpacingLeft(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_CONN_SPACING_LEFT;
	else
		m_p->flags &= ~FLAGS_CONN_SPACING_LEFT;
}

/** True, is space resserved at the top of the component
*/
bool CompView::isConnectorSpacingTop() const
{
	return m_p->flags & FLAGS_CONN_SPACING_TOP;
}

/** True, is space resserved at the right of the component
*/
bool CompView::isConnectorSpacingRight() const
{
	return m_p->flags & FLAGS_CONN_SPACING_RIGHT;
}

/** True, is space resserved at the bottom of the component
*/
bool CompView::isConnectorSpacingBottom() const
{
	return m_p->flags & FLAGS_CONN_SPACING_BOTTOM;
}

/** True, is space resserved at the left of the component
*/
bool CompView::isConnectorSpacingLeft() const
{
	return m_p->flags & FLAGS_CONN_SPACING_LEFT;
}

/** Enables the grid snap functionality.
	If enabled, the component position and size are fixed to the grid positions */
void CompView::enableGridSnap(bool enable)
{
	if (enable)
		m_p->flags |= FLAGS_ENABLE_GRID_SNAP;
	else
		m_p->flags &= ~FLAGS_ENABLE_GRID_SNAP;
}

/** Enables the grid snap functionality.
	If enabled, the component position and size are fixed to the grid positions */
bool CompView::isGridSnapEnabled() const
{
	return m_p->flags & FLAGS_ENABLE_GRID_SNAP;
}

/** Returns a list of all widgets of the component */
KSimWidgetList * CompView::getWidgetList()
{
	if (!m_p->widgetList)
		m_p->widgetList = new KSimWidgetList();
	
	return m_p->widgetList;
}



//#############################################################################
//#############################################################################



class CompViewSize::CompViewSizePrivate
{
public:	
	CompViewSizePrivate()
		:	oldPlace(),
			minSize(2*gridX,2*gridY),
			resizeMode(0),
			lmbDown(0)
	{};
	
	~CompViewSizePrivate() {};
			
	QRect oldPlace;
	QSize minSize;
	int resizeMode;
	bool lmbDown;
};

CompViewSize::CompViewSize(Component * comp, eViewType viewType)
	: 	CompView(comp, viewType)
{
	m_ps = new CompViewSizePrivate();
	CHECK_PTR(m_ps);
	setResizeable(true);
}

CompViewSize::~CompViewSize()
{
	delete m_ps;
}
	
/** Manage the mouse pressed event */
void CompViewSize::mousePress(QMouseEvent *ev, QPainter *)
{
  	// Left mouse button
	if ((ev->button() == LeftButton) && (!getComponent()->getContainer()->isRunning()))
	{
		QPoint mousePos = ev->pos();
		m_ps->lmbDown = true;

		m_ps->oldPlace = getPlace();
		m_ps->resizeMode = 0;

		// A handle on left side
		if (mousePos.x() <= getPlace().left()+HANDLE_SIZE)
		{
			// Top left handle
			if (mousePos.y() <= getPlace().top()+HANDLE_SIZE)
				m_ps->resizeMode = CV_LEFT | CV_TOP;

			// Bottom left handle
			if (mousePos.y() >= getPlace().bottom()-HANDLE_SIZE)
				m_ps->resizeMode = CV_LEFT | CV_BOTTOM;
		}
		// A handle on right side
		else if (mousePos.x() >= getPlace().right()-HANDLE_SIZE)
		{
			// Top right handle
			if (mousePos.y() <= getPlace().top()+HANDLE_SIZE)
				m_ps->resizeMode = CV_RIGHT | CV_TOP;

			// Bottom right handle
			if (mousePos.y() >= getPlace().bottom()-HANDLE_SIZE)
				m_ps->resizeMode = CV_RIGHT | CV_BOTTOM;
		}
	}
	else
		m_ps->lmbDown = false;
}

/** Manage the mouse move event if required  */
void CompViewSize::mouseMove(QMouseEvent *ev, QPainter *p)
{
	if (m_ps->lmbDown)
	{
		KSimusDoc * doc = getComponent()->getDoc();
		CHECK_PTR(doc);
		QSize maxSize = doc->getActiveEditor()->getSize();
		
		QRect newPlace = getPlace();
		QPoint mousePos;
		if(isGridSnapEnabled())
		{
			mousePos = mapToGrid(ev->pos());
		}
		else
		{
			mousePos = ev->pos();
		}
		
		
		if (m_ps->resizeMode & CV_LEFT)
		{
			int left = mousePos.x();// - HANDLE_SIZE / 2;
			if (left < 0)
				left = 0;
			newPlace.setLeft(left);
			if (newPlace.width() < m_ps->minSize.width())
				newPlace.setLeft(newPlace.right()-m_ps->minSize.width()+1);
		}
		if (m_ps->resizeMode & CV_RIGHT)
		{
			int right = mousePos.x()-1;// + HANDLE_SIZE / 2;
			if (right >= maxSize.width())
				right = maxSize.width()-1;
			newPlace.setRight(right);
			if (newPlace.width() < m_ps->minSize.width())
				newPlace.setRight(newPlace.left()+m_ps->minSize.width()-1);
		}
		if (m_ps->resizeMode & CV_TOP)
		{
			int top = mousePos.y();// - HANDLE_SIZE / 2;
			if (top < 0) top = 0;
			newPlace.setTop(top);
			if (newPlace.height() < m_ps->minSize.height())
				newPlace.setTop(newPlace.bottom()-m_ps->minSize.height()+1);
		}
		if (m_ps->resizeMode & CV_BOTTOM)
		{
			int bottom = mousePos.y()-1;// + HANDLE_SIZE / 2;
			if (bottom >= maxSize.height())
				bottom = maxSize.height()-1;
			newPlace.setBottom(bottom);
			if (newPlace.height() < m_ps->minSize.height())
				newPlace.setBottom(newPlace.top()+m_ps->minSize.height()-1);
		}
		drawBound(p);
		m_p->place = newPlace;
		drawBound(p);
	}		
}

/** Manage the mouse released event if required  */
void CompViewSize::mouseRelease(QMouseEvent *, QPainter *)
{
	if (m_ps->lmbDown)
	{
		m_ps->lmbDown = false;
		
		if (getPlace() != m_ps->oldPlace)
		{
			KSimusDoc * doc = getComponent()->getDoc();
			CHECK_PTR(doc);
		
			QRect helpPlace = getPlace();
			m_p->place = m_ps->oldPlace;
			doc->getUndo()->changeProperty(this, i18n("Resize"));
			setPlace(helpPlace);
    	
			getComponent()->getContainer()->routeComponents();
			doc->setModified();
		}
	}
}
	
/** Draw the boundary of the component and handles */
void CompViewSize::drawBound(QPainter * p)
{
	CompView::drawBound(p);
	p->setRasterOp(NotROP);
//	p->setPen(NoPen);
//	p->setBrush(SolidPattern);
	
//	p->drawRect(getPlace().left(), getPlace().top(), HANDLE_SIZE, HANDLE_SIZE);
//	p->drawRect(getPlace().left(), getPlace().bottom(), HANDLE_SIZE, -HANDLE_SIZE);
//	p->drawRect(getPlace().right(), getPlace().top(), -HANDLE_SIZE, HANDLE_SIZE);
//	p->drawRect(getPlace().right(), getPlace().bottom(), -HANDLE_SIZE, -HANDLE_SIZE);

  #define HANDLE_THICK 2
	
	p->setPen(QPen(black, HANDLE_THICK));
	QRect place(getPlace().normalize());
	place.rLeft() -= 1;
	place.rRight() += HANDLE_THICK;
	place.rTop() -= 1;
	place.rBottom() += HANDLE_THICK;
	
	// Top Left Handle
	
	p->drawLine(place.left(),               place.top(),
	            place.left() + HANDLE_SIZE, place.top());
	p->drawLine(place.left(),               place.top(),
	            place.left(),               place.top() + HANDLE_SIZE);
	
	// Bottom Left Handle
	p->drawLine(place.left(),               place.bottom(),
	            place.left() + HANDLE_SIZE, place.bottom());
	p->drawLine(place.left(),               place.bottom(),
	            place.left(),               place.bottom() - HANDLE_SIZE);

	// Top Right Handle
	p->drawLine(place.right(),               place.top(),
	            place.right() - HANDLE_SIZE, place.top());
	p->drawLine(place.right(),               place.top(),
	            place.right(),               place.top() + HANDLE_SIZE);
	
	// Bottom Right Handle
	p->drawLine(place.right(),               place.bottom(),
	            place.right() - HANDLE_SIZE, place.bottom());
	p->drawLine(place.right(),               place.bottom(),
	            place.right(),               place.bottom() - HANDLE_SIZE);
}

eHitType CompViewSize::isHit(int x, int y) const
{
	eHitType hitRes(NO_HIT);
	QRect place(getPlace().normalize());
	place.rLeft() -= 1;
	place.rRight() += HANDLE_THICK;
	place.rTop() -= 1;
	place.rBottom() += HANDLE_THICK;

	if (place.contains(x,y))
	{
		if (((x <= place.left()  + HANDLE_SIZE) && (y <= place.top()    + HANDLE_SIZE)) //top left
		 || ((x >= place.right() - HANDLE_SIZE) && (y >= place.bottom() - HANDLE_SIZE)))//bottom right
		{
			hitRes = COMP_RESIZE_F_HIT;
		}
		else
		if (((x <= place.left()  + HANDLE_SIZE) && (y >= place.bottom() - HANDLE_SIZE))  //top right
		 || ((x >= place.right() - HANDLE_SIZE) && (y <= place.top()    + HANDLE_SIZE))) //bottom left
		{
			hitRes = COMP_RESIZE_B_HIT;
		}
		else
		{
			hitRes = CompView::isHit(x,y);
		}
	}
	return hitRes;
}

/** save CompViewSize properties */
void CompViewSize::save(KSimData & file) const
{
	file.writeEntry(sSize, getPlace().size());
	CompView::save(file);
}

/** Load CompViewSize properties
	Returns true if successful */
bool CompViewSize::load(KSimData & file)
{
	bool ok;
	// Load size first, then load pos!!!
	setPlace(QRect(getPos(),file.readSizeEntry(sSize,&m_ps->minSize)));
	ok = CompView::load(file);
	resize();
	return ok;
}

/** Set the minimum size of the view */
void CompViewSize::setMinSize(int width, int height)
{
	m_ps->minSize = QSize(width,height);
}



//#############################################################################
//#############################################################################

/** the returned rect contains all components excluding wires */
QRect CompViewList::getRect() const
{
	bool empty = true;
	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = INT_MIN;
	int maxY = INT_MIN;
	// Calculate the dimension of all components
	FOR_EACH_COMPVIEW(it, *this)
	{
		// do not if wire
		if (!it.current()->getComponent()->isWire())
		{
			empty = false;
			if (minX > it.current()->getPlace().left())
				minX = it.current()->getPlace().left();
			if (maxX < it.current()->getPlace().right())
				maxX = it.current()->getPlace().right();
			if (minY > it.current()->getPlace().top())
				minY = it.current()->getPlace().top();
			if (maxY < it.current()->getPlace().bottom())
				maxY = it.current()->getPlace().bottom();
		}
	};
	if (empty)
		return QRect(0,0,0,0);
	else		
		return QRect(QPoint(minX,minY),QPoint(maxX,maxY));
}

//#############################################################################
//#############################################################################
