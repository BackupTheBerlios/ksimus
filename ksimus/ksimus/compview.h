/***************************************************************************
                          compview.h  -  description
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

#ifndef COMPVIEW_H
#define COMPVIEW_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// C-Includes

// QT-Includes
#include <qobject.h>
#include <qpoint.h>
#include <qrect.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "types.h"
#include "componentitem.h"

// Forward declaration
class QStringList;
class KSimWidgetList;
class Component;
class ComponentMap;
class ConnectorBase;
class KSimEditor;
class KSimData;
class QMouseEvent;
class QPainter;

/**Widget for components
  *@author Rasmus Diekenbrock
  */

#define CV_TOP		0x01
#define CV_BOTTOM	0x02
#define CV_LEFT		0x04
#define CV_RIGHT	0x08

#define HANDLE_SIZE 6


class CompView : public QObject, public ComponentItem
{
class CompViewPrivate;

friend class CompViewSize;
friend class Module;

	Q_OBJECT

public:
	CompView(Component * comp, eViewType viewType);
	virtual ~CompView();
	
	/** Manage the mouse pressed event if required  */
	virtual void mousePress(QMouseEvent *ev, QPainter *p);
	/** Manage the mouse move event if required  */
	virtual void mouseMove(QMouseEvent *ev, QPainter *p);
	/** Manage the mouse released event if required  */
	virtual void mouseRelease(QMouseEvent *ev, QPainter *p);
	
	/** Draw the component view */
	virtual void draw (QPainter *p);
	/** Draws the CompView to a printer. Widget are also drawn. */
	virtual void print(QPainter * paint);
	/** Draw the boundary of the component */
	virtual void drawBound(QPainter * p);
	virtual void drawBound(QPainter * p, QPoint & tempPos);
	
	/** Hit point x,y the component ?
		NO_HIT
		NORMAL_HIT        - component is hit, KSimEditor controls mouse action
		CONNCTOR_HIT      - connector of a component is hit
		WIRE_HIT          - wire is hit, KSimEditor controls mouse action
		SPECIAL_HIT       - component is hit, component controls mouse action
	*/
	virtual eHitType isHit(int x, int y) const;
	
	/** New position of the component view */
	virtual void setPos(const QPoint & pos);
	/** Give the position of the component view */
	QPoint getPos() const;
	/** Changes the Place of the view. If degree0 is true the place is given without
	  * rotation.  */
	void setPlace(const QRect & newPlace, bool degree0 = false);
	/** Give the place  */
	QRect getPlace() const;
	QRect getWidgetPlace() const;
  /** Returns the drawing area. The connector space is evaluated.
    * @see isConnectorSpacingTop
    * @see isConnectorSpacingRight
    * @see isConnectorSpacingBottom
    * @see isConnectorSpacingLeft */
	QRect getDrawingPlace() const;	
	/** Set the minimum size of the view. */
	void setMinSize(int width, int height);
	/** Set the minimum size of the view. */
	void setMinSize(const QSize & size);
	/**  Returns the minimum size of the view. */
	QSize getMinSize() const;
	/** Return last connector that was hit */
	ConnectorBase * getLastHitConnector() const;
	/** if insert = true, insert compview to sheet map
		if insert = true, delete compview to sheet map */
	
	virtual void updateSheetMap(bool insert);
	
	/** Returns true, if last simulation changes the view */
	bool isViewChanged() const { return m_viewChanged; };
	void setViewChanged(bool changed) { m_viewChanged = changed;};
	
	/** Creates a new compView widget
		Overload this function if the component uses a widget as view
		Returns a pointer to the widget, or 0 if no widget is created
	*/
	virtual QWidget * createCompViewWidget(QWidget * parent);
	
	/** The function is called if a new compView widget is required.
		It calls createCompViewWidget(QWidget * parent) and setup some other things
	*/
	bool makeWidget(QWidget * parent);

		
	/** Map pos to grid */
	QPoint mapToGrid(QPoint pos);

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
		Returns true if successful */
	virtual bool load(KSimData & file);

    /** Returns the type of view
    	@see enum eViewType */
	eViewType getViewType() const;
	
	/** Move the view to the best place off the map */
	void moveToBestPlace();
	
	/** Hide the component view */
	void setHide(bool hide);
	/** Returns true, if component view is hidden */
	bool isHidden() const;
	
	/** Returns true, if component view can be resized */
	bool isResizeable() const;
	
	/** Returns a list of all widgets of the component */
	KSimWidgetList * getWidgetList();
		
	/** If enable is true, reserve space for connectors at the top of the component.
		If false, reserve no space.
	*/
	void enableConnectorSpacingTop(bool enable = true);
	/** If enable is true, reserve space for connectors at the right of the component.
		If false, reserve no space.
	*/
	void enableConnectorSpacingRight(bool enable = true);
	/** If enable is true, reserve space for connectors at the bottom of the component.
		If false, reserve no space.
	*/
	void enableConnectorSpacingBottom(bool enable = true);
	/** If enable is true, reserve space for connectors at the left of the component.
		If false, reserve no space.
	*/
	void enableConnectorSpacingLeft(bool enable = true);

	/** True, is space resserved at the top of the component
	*/
	bool isConnectorSpacingTop() const;
	/** True, is space resserved at the right of the component
	*/
	bool isConnectorSpacingRight() const;
	/** True, is space resserved at the bottom of the component
	*/
	bool isConnectorSpacingBottom() const;
	/** True, is space resserved at the left of the component
	*/
	bool isConnectorSpacingLeft() const;
	
	/** Enables the grid snap functionality.
		If enabled, the component position and size are fixed to the grid positions */
	void enableGridSnap(bool enable = true);
	
	/** Enables the grid snap functionality.
		If enabled, the component position and size are fixed to the grid positions */
	bool isGridSnapEnabled() const;
	
	//#####   Rotation   ######
	
	/** Enables compView rotation. The default is disable. */
	void enableRotation(bool enable = true);
	/** Returns true if rotation is enabled. The default is disable. */
	bool isRotationEnabled() const;
	/** Enables rotation for compView which handles the rotation by it's own.
	  *  The rotation should also be enabled. The default is disable. */
	void enableSpecialRotation(bool enable = true);
	/** Returns true if rotation for compView which handles the rotation by it's
	  * own is enabled. The rotation should also be enabled. The default is disable. */
	bool isSpecialRotationEnabled() const;
	/** Returns true if rotations is enabled and rotation special is disabled. */
	bool isNormalRotationEnabled() const;
	/** Sets the current rotation position in degree. If normal rotation is enabled the values
	  * 0, 90, 180 and 270 are allowed. */
	void setRotation(double rotation);	
	/** Sets the current rotation position in degree. If normal rotation is enabled the values
	  * 0, 90, 180 and 270 are allowed. */
	double getRotation() const;
	/** Sets the current rotation position more 90 degree clock wise. The values
	  * 0, 90, 180 and 270 are used. */
	void stepRotationCW();	
	/** Sets the current rotation position more 90 degree counter clock wise. The values
	  * 0, 90, 180 and 270 are used. */
	void stepRotationCCW();
	/** Returns a position which is converted to the rotation position. The function dont work with
	  * compView specialized rotation. */
	QPoint mapToRotation(const QPoint & pos) const;
	/** Returns a position which is converted from the rotation position. The function dont work with
	  * compView specialized rotation. */
	QPoint mapFromRotation(const QPoint & rotPos) const;



protected: // Protected attributes

	/** Returns the current Component Map */
	ComponentMap * getComponentMap() const;

    /** Set true, if component view is resizeable */
    void setResizeable(bool enable);

    /** Change the type of view
    	@see enum eViewType */
	void setViewType(eViewType newType);
	
	virtual void resize();

	static ConnectorBase * lastHitConnector;
	
	/** Draws a rect a round the component body (excluding the connector space).
	  * Color black, thick: 2 pixel
	  */
	void drawFrame(QPainter * p) const;
	
		
public slots:
	/** Toogle the hide flag */
	void slotToggleHide();
	
private:
	CompViewPrivate * m_p;
	bool m_viewChanged;


signals: // Signals
	/** This signal is emitted, if view is moved */
	void signalMove(const QPoint & newPos);
	/** This signal is emitted, if view is resized */
	void signalResize(const QSize & newSize);
	/** This signal is emitted, if view is moved
	    newPos is the position without connector space */
	void signalMoveWidget(const QPoint & newPos);
	/** This signal is emitted, if view is resized
	    newSize is the size without connector space */
	void signalResizeWidget(const QSize & newSize);
	/** This signal is emitted, if view should hide */
	void signalHide();
	/** This signal is emitted, if view should show */
	void signalShow();

};

//#############################################################################
//#############################################################################

class CompViewSize : public CompView
{
class CompViewSizePrivate;

	Q_OBJECT

public:
	CompViewSize(Component * comp, eViewType viewType);
	virtual ~CompViewSize();
	
	/** Manage the mouse pressed event if required  */
	virtual void mousePress(QMouseEvent *ev, QPainter *p);
	/** Manage the mouse move event if required  */
	virtual void mouseMove(QMouseEvent *ev, QPainter *p);
	/** Manage the mouse released event if required  */
	virtual void mouseRelease(QMouseEvent *ev, QPainter *p);
	
	/** Draw the boundary of the component and handles */
	virtual void drawBound(QPainter * p);
	/** Hit point x,y the component ?
		NO_HIT
		NORMAL_HIT        - component is hit, KSimEditor controls mouse action
		CONNCTOR_HIT      - connector of a component is hit
		WIRE_HIT          - wire is hit, KSimEditor controls mouse action
		SPECIAL_HIT       - component is hit, component controls mouse action
		COMP_RESIZE_F_HIT - Resize component, component controls mouse action
		COMP_RESIZE_B_HIT - Resize component, component controls mouse action
	*/
	virtual eHitType isHit(int x, int y) const;
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
		Returns true if successful */
	virtual bool load(KSimData & file);
	

private:
	CompViewSizePrivate * m_ps;
};


//#############################################################################
//#############################################################################

class CompViewList : public QList<CompView>
{
public:
	/** Returns the rect which contains all component views (excluding wires). */
	QRect getRect() const;

};


#define FOR_EACH_COMPVIEW(_it_,_viewList_)	\
		for(QListIterator<CompView> _it_(_viewList_);_it_.current();++_it_)

//#define REMOVE_ALL_COMPVIEW(_viewList_) while ((_viewList_)->removeFirst ());
		


#endif
