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
	/** Draws the CompView to a printer */
	virtual void print(QPainter * paint);
	/** Draw the boundary of the component */
	virtual void drawBound(QPainter * p);
	virtual void drawBound(QPainter * p, QPoint & tempPos);
	
	/** Hit point x,y the component ?
		NO_HIT
		NORMAL_HIT   - component is hit, KSimEditor controls mouse action
		CONNCTOR_HIT - connector of a component is hit
		WIRE_HIT     - wire is hit, KSimEditor controls mouse action
		SPECIAL_HIT  - component is hit, component controls mouse action */
	virtual eHitType isHit(int x, int y) const;
	
	/** New position of the component view */
	virtual void setPos(const QPoint & pos);
	/** Give the position of the component view */
	QPoint getPos() const;
	/** Changes the Place of the view */
	void setPlace(const QRect & newPlace);
	/** Give the place  */
	QRect getPlace() const;
	QRect getWidgetPlace() const;
	
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
		NORMAL_HIT - component is hit, KSimView controls mouse action
		SPECIAL_HIT - component is hit, component controls mouse action */
	virtual eHitType isHit(int x, int y) const;
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
		Returns true if successful */
	virtual bool load(KSimData & file);
	/** Set the minimum size of the view */
	void setMinSize(int width, int height);
	/** Set the minimum size of the view*/
	void setMinSize(const QSize & size) { setMinSize(size.width(),size.height()); };
	

private:
	CompViewSizePrivate * m_ps;
};


//#############################################################################
//#############################################################################

class CompViewList : public QList<CompView>
{
};


#define FOR_EACH_COMPVIEW(_it_,_viewList_)	\
		for(QListIterator<CompView> _it_(_viewList_);_it_.current();++_it_)

//#define REMOVE_ALL_COMPVIEW(_viewList_) while ((_viewList_)->removeFirst ());
		


#endif
