/***************************************************************************
                          ksimeditor.h  -  description
                             -------------------
    begin                : Sun May 7 2000
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

#ifndef KSIMEDITOR_H
#define KSIMEDITOR_H

// include files for Qt
#include <qwidget.h>
#include <qtimer.h>

// project includes
#include "component.h"
#include "compview.h"

class QPixmap;
class QPopupMenu;
class KSimusDoc;
class KSimusView;
class KSimusApp;
class CompContainer;
class ConnectorBase;
class Wire;
class ComponentInfo;
class KPrinter;

/** The KSimEditor class provides the view widget for the KSimusApp instance.	
 * The View instance inherits QWidget as a base class and represents the view object of a KTMainWindow. As KSimEditor is part of the
 * docuement-view model, it needs a reference to the document object connected with it by the KSimusApp class to manipulate and display
 * the document structure provided by the KSimusDoc class.
 * 	
 * @author Source Framework Automatically Generated by KDevelop, (c) The KDevelop Team.
 * @version KDevelop version 0.4 code generation
 */

enum EditorModeType { EM_SELECT = 0, EM_INSERT, EM_INSERT_MOVE,
                      EM_PAST, EM_PAST_MOVE,
                      EM_MOVE_OR_SELECT, EM_MOVE, EM_MOVE_COPY,
                      EM_RECT_SELECT, EM_SINGLE_SELECT,
                      EM_SPECIAL, EM_COMP_RESIZE_B, EM_COMP_RESIZE_F, EM_WIRE, EM_RESIZE_MAP };

enum EditorViewType { EV_NOTVALID = 0,
                      EV_SHEETVIEW,
                      EV_USERVIEW };


class KSimEditor : public QWidget
{
class Cursor;
	Q_OBJECT

	friend class KSimUndo;

public:
	
	/** Constructor for the main view */
	KSimEditor(QWidget *parent = 0, const char *name=0);
	/** Destructor for the main view */
	~KSimEditor();

	/** returns a pointer to the document connected to the view instance. Mind that this method requires a KSimusApp instance as a parent
	  * widget to get to the window document pointer by calling the KSimusApp::getDocument() method.
	  *
	  * @see KSimusApp#getDocument
	  */
	KSimusDoc *getDoc() const;
	KSimusApp *getApp() const;
	KSimusView *getView() const;
	CompContainer * getContainer() const;

	/** contains the implementation for printing functionality */
	void print(KPrinter *pPrinter);
	/** Redraw the drawMap with all components, wires and selections
		updateDrawMap() doesn't update the widget. update() will do that.*/
	void updateDrawMap();
	
	/** All views of the document redraw and show the drawMap */
	void refresh();
	void setEditorMode(EditorModeType newMode);
	
	/** Sets the type of view */
	void setEditorView(EditorViewType viewType);
	/** Gets the type of view */
	EditorViewType getEditorView() const { return editorView; };
	
	/** The event filter has to be installed by compViews based on widgets */
	virtual bool eventFilter( QObject * obj, QEvent * ev);
	
	/** Returns the size of the drawing area */
	const QSize & getSize() const;
	/** Sets the size of the drawing area */
	void setSize(const QSize & newSize);
	
	void select (CompView *compView, bool sel);
	void select (Component *comp, bool sel);
	void unselectAll ();

	/** Cuts selected components */
	void cutSelection();

	/** Copies selected components */
	void copySelection();
	
	/** Deletes selected components */
	void deleteSelection();



protected: // Protected methods
	
	virtual void paintEvent(QPaintEvent * ev );
	
	void drawDragRect ();
	
	virtual void mouseDoubleClickEvent (QMouseEvent *ev)
		{mousePressEvent (ev);}
	virtual void mouseMoveEvent (QMouseEvent *ev);
	virtual void mouseReleaseEvent (QMouseEvent *ev);
	virtual void mousePressEvent (QMouseEvent *ev);

	/** Returns the component view in order of the selected  view type */
	CompView* getSpecificCompView(Component * comp) const;
	/** Convert a list of component to a list of compoennt views in order of the selected  view type */
	void convertComponentToCompView(ComponentList * compList, CompViewList * compViewList);
	/** Manages map resizing
		Called from mouseMoveEvent */
	QPoint resizingMap(const QPoint & mousePos, bool increaseOnly);
	
	/** Execute the "background" popup menu
		This menu is display, if the left mouse button is pressed
		and no component, wire, or connector is hit */
	void backgroundPopup();
	/** Execute the component popup menu
		This menu is display, if one or more components are hit */
	void componentPopup(bool connectorHit);
	/** Execute the wire popup menu
		This menu is display, if a wire is hit */
	void wirePopup(const QPoint & pos);
  /** Sets a proper cursor. */
	void setEditorCursor(QPoint * pMousePos, eHitType hit) const;
	
private:
	/** Displays the status message msg delayed */
	void delayedStatusHelpMsg(const QString & msg);
	
	QPoint dragStart;
	QPoint dragNow;
	bool lmbDown;
	bool dragging;
	bool isMapResized;

	CompViewList * viewList;
	
	CompViewList selected;
	CompView * special;
	
	ConnectorBase * startConn;
	
	EditorModeType editorMode;
	EditorViewType editorView;
	
	/** Contains the drawing */
	QPixmap * drawMap;
	
	QSize size;

	QTimer autoScrollTimer;
	QTimer delayedStatusHelpMsgTimer;
	QString delayedStatusHelpMsgString;
	
	int scrollDir;
	int resizeDir;
	
	const ComponentInfo * m_insertCI;
	Cursor * m_myCursor;
	

public slots: // Public slots
	void refreshSimMode(CompViewList * sheetList, CompViewList * userList);
	/** Receives the component info for inserting component */
	void slotSelectCI(const ComponentInfo *ci);
	/** Called, if component is deleting */
	void slotDelete(Component * comp);
	
private slots: // Private slots
	void autoScroll();
	void slotDelayedStatusHelpMsg();

signals:
	void editorModeChanged(int);
	void editorViewChanged(int);
	void deleteAllowed(bool);
	void cutAllowed(bool);
	void copyAllowed(bool);
	void signalStatusMsg(const QString &);
	void signalStatusHelpMsg(const QString &);
};

class KSimEditorList : public QList<KSimEditor> {};

#endif // KSIMEDITOR_H
