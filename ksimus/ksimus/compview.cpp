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
#include <math.h>

// QT-Includes
#include <qevent.h>
#include <qpainter.h>
#include <qobjectlist.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "compview.h"
#include "componentaddon.h"
#include "compcontainer.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "componentmap.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimwidget.h"
#include "ksimeditor.h"
#include "ksimusview.h"
#include "ksimundo.h"
#include "ksimdebug.h"
#include "resource.h"
#include "module.h"

// Forward declaration





static const char * sPos      = "Pos";
static const char * sSize     = "Size";
static const char * sHide     = "Hidden";
static const char * sRotation = "Rotation";

static const int sheetGridX = gridX;
static const int sheetGridY = gridY;


// Used for m_flags
//#define FLAGS_VIEW_CHANGED			0x00000001
#define FLAGS_HIDDEN							0x00000002
#define FLAGS_RESIZEABLE					0x00000004
#define FLAGS_CONN_SPACING_TOP		0x00000008
#define FLAGS_CONN_SPACING_RIGHT	0x00000010
#define FLAGS_CONN_SPACING_BOTTOM	0x00000020
#define FLAGS_CONN_SPACING_LEFT		0x00000040
#define FLAGS_ENABLE_GRID_SNAP		0x00000080
#define FLAGS_ROTATE_ENABLE       0x00000100
#define FLAGS_ROTATE_SPECIAL      0x00000200
// 0x0001xxxx - 0xFFFFxxxx Reserved for CompViewSize



#define ROT_0_DEG	  0
#define ROT_90_DEG	1
#define ROT_180_DEG	2
#define ROT_270_DEG	3


static int convertRotToInt(double degree)
{
	if((degree < 45.0) || (degree >= 315.0))
	{
		return ROT_0_DEG;
	}
	else if(degree < 135.0)
	{
		return ROT_90_DEG;
	}
	else if(degree < 225.0)
	{
		return ROT_180_DEG;
	}
	else
	{
		return ROT_270_DEG;
	}
}	

/* not needed yet
static double convertIntToRot(int step)
{
	double res;
	
	switch(step)
	{
		default:
			KSIMDEBUG_VAR("Unknown rot step",step);
			//No break!
			
		case ROT_0_DEG:
			res = 0.0;
			break;
			
		case ROT_90_DEG:
			res = 90.0;
			break;
			
		case ROT_180_DEG:
			res = 180.0;
			break;
			
		case ROT_270_DEG:
			res = 270.0;
			break;
	}
	return res;
}	*/

static QSize convertSize(const QSize & size, int fromRotStep, int toRotStep)
{
	if ((fromRotStep & 1) == (toRotStep & 1))
	{
		return size;
	}
	else
	{
		//Swap size
		return QSize(size.height(), size.width());
	}
}


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

static void setGlobalMouseTracking(QWidget * main, bool tracking)
{
	if (!main)
		return;
	
	main->setMouseTracking(tracking);
	
	const QObjectList * list = main->children();
	
	if (list)
	{
		QObject * obj;
		for ( obj = ((QObjectList*)list)->first(); obj; obj = ((QObjectList*)list)->next() )
		{
			if (obj->isWidgetType())
			{
				setGlobalMouseTracking((QWidget*)obj, tracking);
			}
		}
	}
}	

ConnectorBase * CompView::lastHitConnector = 0;


//############################################################################
//############################################################################

class CompView::CompViewPrivate
{
public:
	CompViewPrivate(eViewType vt)
		:	place(0,0,0,0),
			posGridX(&sheetGridX),
			posGridY(&sheetGridY),
			viewType (vt),
			widgetList(0),
			minSize(2*gridX,2*gridY),
			rotation(0.0)
	{
		if (vt == SHEET_VIEW)
			flags =	FLAGS_CONN_SPACING_TOP | FLAGS_CONN_SPACING_RIGHT | FLAGS_CONN_SPACING_BOTTOM
					| FLAGS_CONN_SPACING_LEFT | FLAGS_ENABLE_GRID_SNAP;
		else
			flags =	FLAGS_ENABLE_GRID_SNAP;
	};
	~CompViewPrivate()
	{
		delete widgetList;
	};
	
	/** Position and dimension of the component view */
	QRect place;
	// Some flags
	Q_UINT32 flags;
	// Snaps
	const int * posGridX;
	const int * posGridY;
	
	eViewType viewType;
	KSimWidgetList * widgetList;
	QSize minSize;
	float rotation;
};

//############################################################################
//############################################################################


CompView::CompView(Component * comp, eViewType viewType)
	:	QObject(),
		ComponentItem(comp),
		m_viewChanged(false)
{
	m_p = new CompViewPrivate(viewType);
	CHECK_PTR(m_p);
	
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
	
	if (getComponent()->getContainer()->isVisible() && !isHidden())
	{	
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
	
		if(m_p->place.topLeft() != newPos)
		{
			// Remove Object from sheet map
			updateSheetMap(false);
			m_p->place.moveTopLeft(newPos);
			// Insert Object to sheet map
			updateSheetMap(true);

			emit signalMove(getPlace().topLeft());
			emit signalMoveWidget(getWidgetPlace().topLeft());
		}
	}
}

/** Changes the place of the view */
void CompView::setPlace(const QRect & place, bool degree0)
{
	QRect newPlace(place);
	
	// Limit size
	if(newPlace.width()  < getMinSize().width())  newPlace.setWidth(getMinSize().width());
	if(newPlace.height() < getMinSize().height()) newPlace.setHeight(getMinSize().height());
	
	if (degree0 && isNormalRotationEnabled())
	{
		newPlace.setSize(convertSize(newPlace.size(), ROT_0_DEG, convertRotToInt(getRotation())));
	}
	
	
	if (getComponent()->getContainer()->isVisible())
	{	
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
		if (newPlace.bottom() > mapSize.height())
			newPlace.moveBy(0, mapSize.height() - newPlace.bottom());
		
		if (newPlace.right() > mapSize.width())
			newPlace.moveBy(mapSize.width() - newPlace.right(), 0);
		
		if (newPlace.top() < 0)
			newPlace.moveBy(0, - newPlace.top());
		
		if (newPlace.left() < 0)
			newPlace.moveBy( -newPlace.left(), 0);
	}
	
	if (isGridSnapEnabled())
	{
		newPlace = QRect(mapToGrid(newPlace.topLeft()),mapToGrid(newPlace.bottomRight()) + QPoint(-1,-1));
	}
	
	if (newPlace != m_p->place)
	{
		// Remove Object from sheet map
		updateSheetMap(false);
		m_p->place = newPlace;
		// Insert Object to sheet map
		updateSheetMap(true);

		// Don't forget setRotation !!
		emit signalMove(getPlace().topLeft());
		emit signalResize(getPlace().size());
		emit signalMoveWidget(getWidgetPlace().topLeft());
		emit signalResizeWidget(getWidgetPlace().size());
	
		resize();
	}
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

QRect CompView::getWidgetPlace() const
{
	QRect widgetPlace(getPlace());
	
	int rotStep = ROT_0_DEG;
	
	if(isNormalRotationEnabled())
	{
		rotStep = convertRotToInt(getRotation());
	}
		
	switch(rotStep)
	{
		case ROT_0_DEG:
			if (isConnectorSpacingTop())    widgetPlace.rTop()    += gridY;
			if (isConnectorSpacingRight())  widgetPlace.rRight()  -= gridX;
			if (isConnectorSpacingBottom())	widgetPlace.rBottom() -= gridY;
			if (isConnectorSpacingLeft())   widgetPlace.rLeft()   += gridX;
			break;
			
		case ROT_90_DEG:
			if (isConnectorSpacingTop())    widgetPlace.rRight()  -= gridX;
			if (isConnectorSpacingRight())  widgetPlace.rBottom() -= gridY;
			if (isConnectorSpacingBottom())	widgetPlace.rLeft()   += gridX;
			if (isConnectorSpacingLeft())   widgetPlace.rTop()    += gridY;
			break;
				
		case ROT_180_DEG:
			if (isConnectorSpacingTop())    widgetPlace.rBottom() -= gridY;
			if (isConnectorSpacingRight())  widgetPlace.rLeft()   += gridX;
			if (isConnectorSpacingBottom())	widgetPlace.rTop()    += gridY;
			if (isConnectorSpacingLeft())   widgetPlace.rRight()  -= gridX;
			break;
				
		case ROT_270_DEG:
			if (isConnectorSpacingTop())    widgetPlace.rLeft()   += gridX;
			if (isConnectorSpacingRight())  widgetPlace.rTop()    += gridY;
			if (isConnectorSpacingBottom())	widgetPlace.rRight()  -= gridX;
			if (isConnectorSpacingLeft())   widgetPlace.rBottom() -= gridY;
			break;
			
		default:
			KSIMDEBUG_VAR("Unkown",rotStep);
			break;
	}
	
	return widgetPlace;
}

QRect CompView::getDrawingPlace() const
{
	QSize size(getPlace().size());
	
	if(isNormalRotationEnabled())
	{
		size = convertSize(size, ROT_0_DEG, convertRotToInt(getRotation()));
	}
	
	int left = (isConnectorSpacingLeft() ? gridX : 0);
	int top = (isConnectorSpacingTop()  ? gridY+1 : 0);
	int right = size.width() - (isConnectorSpacingRight()  ? gridX : 0);
	int bottom = size.height() - (isConnectorSpacingBottom()  ? gridY : 0);
	
	return QRect(left, top, right - left, bottom - top);
}

void CompView::setMinSize(const QSize & size)
{
	m_p->minSize = size;
}

void CompView::setMinSize(int width, int height)
{
	setMinSize(QSize(width,height));
}

QSize CompView::getMinSize() const
{
	return m_p->minSize;
}



/** Hit point x,y the component ?
NO_HIT      - component is not hit
NORMAL_HIT  - component is hit, KSimView controls mouse action
SPECIAL_HIT - component is hit, component controls mouse action */

eHitType CompView::isHit(int x, int y) const
{
	
	QRect rect = getPlace();
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
		QPoint p(x,y);
		if(isNormalRotationEnabled())
		{
			p = mapFromRotation(p);
		}
		
		for (conn = getComponent()->getConnList()->first();
		     conn;
		     conn = getComponent()->getConnList()->next())
		{
			if (CONNECTOR_HIT == conn->isHit(p.x(),p.y()))
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


void CompView::print(QPainter * paint)
{
	draw(paint);

  QWidget * pWid;
	if (getWidgetList() && (pWid = getWidgetList()->getFirstWidget()))
	{
		// Reset the compView translation (a little bit ugly :)
		paint->restore();
		paint->drawPixmap(pWid->pos(), QPixmap::grabWidget(pWid));
		// Push one
		paint->save();
	}
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
	
void CompView::drawFrame(QPainter * p) const
{
	QRect rect(getDrawingPlace());
	rect.rLeft()++;
	rect.rTop()++;
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
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
	if (getComponent()->getContainer()->isVisible() && !isHidden())
	{
		QRect mapPlace;
	
		mapPlace.setLeft(getPlace().left() / gridX);
		mapPlace.setTop(getPlace().top() / gridY);
		mapPlace.setRight(getPlace().right() / gridX);
		mapPlace.setBottom(getPlace().bottom() / gridY);
		
		getComponentMap()->addPlace(mapPlace, insert
									?  getComponentMap()->costComponent
									: -getComponentMap()->costComponent );
	}
}

void CompView::setViewChanged(bool changed)
{
	m_viewChanged = changed;
	if (changed && getComponent()->getContainer()->isParentComponent())
	{
		// Module !!!
		Module * module((Module *)getComponent()->getContainer()->getParentComponent());
		if (module->getSheetView())
		{
			module->getSheetView()->setViewChanged(true);
		}
		if (module->getUserView())
		{
			module->getUserView()->setViewChanged(true);
		}
	}
};


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
		setGlobalMouseTracking(widget, true);
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
	if (isRotationEnabled() && (getRotation() != 0))
	{
		file.writeEntry(sRotation, getRotation());
	}
		
}

/** Load CompView properties
	Returns true if successful */
bool CompView::load(KSimData & file)
{
	setPos(file.readPointEntry(sPos));
	setHide(file.readBoolEntry(sHide, false));
	if (isRotationEnabled())
	{
		setRotation(file.readDoubleNumEntry(sRotation, 0.0));
	}

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

/** Hide the component view */
void CompView::setHide(bool hide)
{
	if(hide != isHidden())
	{
		if(hide)
		{
			// Remove object from sheet map (before set hidden!)
			updateSheetMap(false);
			// set hidden;
			m_p->flags |= FLAGS_HIDDEN;
			// Unselect view
			for(QListIterator<KSimusView> it(*getComponent()->getDoc()->getViewList());it.current();++it)
			{
				KSimEditor * ed = it.current()->getEditor();
				if (ed) ed->select(this, false);
			}
			emit signalHide();
		}
		else
		{
			m_p->flags &= ~FLAGS_HIDDEN;
			// Insert object to sheet map (after set not hidden)
			updateSheetMap(true);
			moveToBestPlace();
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

Component::eComponentType CompView::getComponentType() const
{
	return getComponent()->getComponentType();
}

	
/** Returns a list of all widgets of the component */
KSimWidgetList * CompView::getWidgetList()
{
	if (!m_p->widgetList)
		m_p->widgetList = new KSimWidgetList();
	
	return m_p->widgetList;
}

void CompView::enableRotation(bool enable)
{
	if(enable)
	{
		m_p->flags |= FLAGS_ROTATE_ENABLE;
	}
	else
	{
		m_p->flags &= ~FLAGS_ROTATE_ENABLE;
	}
}

bool CompView::isRotationEnabled() const
{
	return m_p->flags & FLAGS_ROTATE_ENABLE;
}

void CompView::enableSpecialRotation(bool enable)
{
	if(enable)
	{
		m_p->flags |= FLAGS_ROTATE_SPECIAL;
	}
	else
	{
		m_p->flags &= ~FLAGS_ROTATE_SPECIAL;
	}
}

bool CompView::isSpecialRotationEnabled() const
{
	return m_p->flags & FLAGS_ROTATE_SPECIAL;
}

bool CompView::isNormalRotationEnabled() const
{
	return ((m_p->flags & (FLAGS_ROTATE_ENABLE|FLAGS_ROTATE_SPECIAL)) == FLAGS_ROTATE_ENABLE);
}

void CompView::setRotation(double rotation)
{
	// Normalize
	rotation = fmod(rotation, 360.0);
	if(rotation<0.0) rotation += 360.0;
	
	if(isNormalRotationEnabled())
	{
		int curRot = convertRotToInt(getRotation());
		if((rotation < 45.0) || (rotation >= 315.0))
		{
			m_p->rotation = 0.0;
		}
		else if(rotation < 135.0)
		{
			m_p->rotation = 90.0;
		}
		else if(rotation < 225.0)
		{
			m_p->rotation = 180.0;
		}
		else
		{
			m_p->rotation = 270.0;
		}
		// Update widget size and widget min size
		setMinSize(convertSize(getMinSize(), curRot, convertRotToInt(m_p->rotation)));
		QSize newSize(convertSize(getPlace().size(), curRot, convertRotToInt(m_p->rotation)));
		if (newSize == getPlace().size())
		{
			// Manually
			emit signalMove(getPlace().topLeft());
			emit signalResize(getPlace().size());
			emit signalMoveWidget(getWidgetPlace().topLeft());
			emit signalResizeWidget(getWidgetPlace().size());
	
			resize();
		}
		else
		{
			setPlace(QRect(getPlace().topLeft(), newSize));
		}
	}
	else
	{
		m_p->rotation = rotation;
	}
}
	
double CompView::getRotation() const
{
	return m_p->rotation;
}
	
void CompView::stepRotationCW()
{
	setRotation(getRotation() + 90.0);
}

void CompView::stepRotationCCW()
{
	setRotation(getRotation() - 90.0);
}

QPoint CompView::mapToRotation(const QPoint & pos) const
{
	if (isNormalRotationEnabled())
	{
		if((getRotation() < 45.0) || (getRotation() >= 315.0))
		{
			return pos;
		}
		else if(getRotation() < 135.0)
		{
			QPoint p(pos - getPos());
			return QPoint(getPlace().width() - p.y(), p.x()) + getPos();
		}
		else if(getRotation() < 225.0)
		{
			QPoint p(pos - getPos());
			return QPoint(getPlace().width() - p.x(), getPlace().height() - p.y()) + getPos();
		}
		else
		{
			QPoint p(pos - getPos());
			return QPoint(p.y(), getPlace().height() - p.x()) + getPos();
		}
	}
	else
	{
		return pos;
	}
}

QPoint CompView::mapFromRotation(const QPoint & rotPos) const
{
	if (isNormalRotationEnabled())
	{
		if((getRotation() < 45.0) || (getRotation() >= 315.0))
		{
			return rotPos;
		}
		else if(getRotation() < 135.0)
		{
			QPoint p(rotPos - getPos());
			return QPoint(p.y(), getPlace().width() - p.x()) + getPos();
		}
		else if(getRotation() < 225.0)
		{
			QPoint p(rotPos - getPos());
			return QPoint(getPlace().width() - p.x(), getPlace().height() - p.y()) + getPos();
		}
		else
		{
			QPoint p(rotPos - getPos());
			return QPoint(getPlace().height() - p.y(), p.x()) + getPos();
		}
	}
	else
	{
		return rotPos;
	}
}




//#############################################################################
//#############################################################################



class CompViewSize::CompViewSizePrivate
{
public:	
	CompViewSizePrivate()
		:	oldPlace(),
			resizeMode(0),
			lmbDown(false)
	{};
	
//	~CompViewSizePrivate() {};
			
	QRect oldPlace;
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
			if (newPlace.width() < getMinSize().width())
				newPlace.setLeft(newPlace.right()-getMinSize().width()+1);
		}
		if (m_ps->resizeMode & CV_RIGHT)
		{
			int right = mousePos.x()-1;// + HANDLE_SIZE / 2;
			if (right >= maxSize.width())
				right = maxSize.width()-1;
			newPlace.setRight(right);
			if (newPlace.width() < getMinSize().width())
				newPlace.setRight(newPlace.left()+getMinSize().width()-1);
		}
		if (m_ps->resizeMode & CV_TOP)
		{
			int top = mousePos.y();// - HANDLE_SIZE / 2;
			if (top < 0) top = 0;
			newPlace.setTop(top);
			if (newPlace.height() < getMinSize().height())
				newPlace.setTop(newPlace.bottom()-getMinSize().height()+1);
		}
		if (m_ps->resizeMode & CV_BOTTOM)
		{
			int bottom = mousePos.y()-1;// + HANDLE_SIZE / 2;
			if (bottom >= maxSize.height())
				bottom = maxSize.height()-1;
			newPlace.setBottom(bottom);
			if (newPlace.height() < getMinSize().height())
				newPlace.setBottom(newPlace.top()+getMinSize().height()-1);
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

	if (place.contains(x,y) && !isHidden())
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
	ok = CompView::load(file);
	QSize tmpSize(getMinSize());
	setPlace(QRect(getPos(),file.readSizeEntry(sSize,&tmpSize)));
//	ok = CompView::load(file);
	resize();
	return ok;
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
		if (!it.current()->getComponent()->isWire() && !it.current()->isHidden())
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


void CompViewList::insertCompView(CompView * cv)
{
	Component::eComponentType type = cv->getComponentType();
	unsigned int step = count() / 2;
	unsigned int i = step;
	
	// Step fast to the position
	while(step >= 2)
	{
		step /= 2;
		if (type < at(i)->getComponentType())
		{
			i -= step;
		}
		else
		{
			i += step;
		}
	}
	
	while((i > 0) && (type < at(i)->getComponentType()))
	{
		i --;
	}
	
	while((i < count()) && !(type < at(i)->getComponentType()))
	{
		i ++;
	}
	insert(i,cv);
}




//#############################################################################
//#############################################################################
