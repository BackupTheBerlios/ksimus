/***************************************************************************
                          ksimeditor.cpp  -  description
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

// include std headerfiles
#include <limits.h>
#include <stdlib.h>


// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qscrollview.h>
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qbitmap.h>

// include files for KDE
#include <klocale.h>
#include <kmessagebox.h>

// application specific includes
#include "globals.h"
#include "resource.h"
#include "ksimeditor.h"
#include "ksimusdoc.h"
#include "ksimusview.h"
#include "ksimundo.h"
#include "ksimus.h"
#include "library.h"
#include "componentlibrary.h"
#include "componentpopupmenu.h"
#include "moduleinfo.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "connectorpropertydialog.h"
#include "compcontainer.h"
#include "module.h"
#include "moduledata.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"


// Scroll directions
#define SC_NoScroll	0
#define SC_Left		1
#define SC_Right	2
#define SC_Up		4
#define SC_Down		8

// Resize directions
#define RS_NoResize	0
#define RS_Left		1
#define RS_Right	2
#define RS_Up		4
#define RS_Down		8

#define RS_HANDLE_SIZE 8


//###################################################################
//###################################################################

class DynamicTip: public QToolTip
{
public:	
	DynamicTip(QWidget * parent)
		:	QToolTip(parent),
			m_text(QString::null)
	{
    // no explicit initialization needed
	};
	
	~DynamicTip() {};

	virtual void maybeTip(const QPoint &pos);
	
	QString m_text;
};



void DynamicTip::maybeTip( const QPoint &pos )
{
	if (m_text.isNull())
		return;
	QRect r( pos - QPoint(3,3), pos + QPoint(3,3));

	tip( r, m_text );
}


//###################################################################
//###################################################################


#include "wire.cur"
#include "wire_mask.cur"

class KSimEditorCursor
{
public:
	enum KSimCursorType { arrowCursor, upArrowCursor, crossCursor, waitCursor, ibeamCursor,
	                      sizeVerCursor, sizeHorCursor, sizeBDiagCursor, sizeFDiagCursor,
	                      sizeAllCursor, blankCursor, splitVCursor, splitHCursor,
	                      pointingHandCursor, forbiddenCursor, wireCursor };
	
	KSimEditorCursor(KSimEditor * editor);
	~KSimEditorCursor() {};
	
	void set(KSimCursorType newCursor);
	void setNow(KSimCursorType newCursor);
	KSimCursorType get() const { return m_current; };
		
		
private:
	KSimCursorType m_current;
	KSimEditor * m_editor;
	QCursor * m_wireCursor;
	
};

KSimEditorCursor::KSimEditorCursor(KSimEditor * editor)
	:	m_editor(editor)
{
	// Wire cursor
	QBitmap bitmap(wire_width, wire_height, wire_bits, true);
	QBitmap bitmapMask(wire_mask_width, wire_mask_height, wire_mask_bits, true);
	m_wireCursor = new QCursor(bitmap, bitmapMask, wire_x_hot, wire_y_hot);
	CHECK_PTR(m_wireCursor);

	setNow(arrowCursor);
};

void KSimEditorCursor::set(KSimCursorType newCursor)
{
	if(get() != newCursor)
	{
		setNow(newCursor);
	}
}

void KSimEditorCursor::setNow(KSimCursorType newCursor)
{
	m_current = newCursor;
	
	switch(newCursor)
	{
		// Std-Cursors
		case arrowCursor       : m_editor->setCursor(Qt::arrowCursor);        break;
		case upArrowCursor     : m_editor->setCursor(Qt::upArrowCursor);      break;
		case crossCursor       : m_editor->setCursor(Qt::crossCursor);        break;
		case waitCursor        : m_editor->setCursor(Qt::waitCursor);         break;
		case ibeamCursor       : m_editor->setCursor(Qt::ibeamCursor);        break;
		case sizeVerCursor     : m_editor->setCursor(Qt::sizeVerCursor);      break;
		case sizeHorCursor     : m_editor->setCursor(Qt::sizeHorCursor);      break;
		case sizeBDiagCursor   : m_editor->setCursor(Qt::sizeBDiagCursor);    break;
		case sizeFDiagCursor   : m_editor->setCursor(Qt::sizeFDiagCursor);    break;
		case sizeAllCursor     : m_editor->setCursor(Qt::sizeAllCursor);      break;
		case blankCursor       : m_editor->setCursor(Qt::blankCursor);        break;
		case splitVCursor      : m_editor->setCursor(Qt::splitVCursor);       break;
		case splitHCursor      : m_editor->setCursor(Qt::splitHCursor);       break;
		case pointingHandCursor: m_editor->setCursor(Qt::pointingHandCursor); break;
		case forbiddenCursor   : m_editor->setCursor(Qt::forbiddenCursor);    break;
		
		// KSimus Cursors
		case wireCursor:
			m_editor->setCursor(*m_wireCursor);    break;
			break;
	}
}







//###################################################################
//###################################################################


KSimEditor::KSimEditor(QWidget *parent, const char *name)
	:	QWidget(parent, name),
		delayedStatusHelpMsgString(),
		m_insertCI(0)
{
	if (!g_editorList)
		g_editorList = new KSimEditorList;
	g_editorList->append(this);
	
	drawMap = new QPixmap;
	CHECK_PTR(drawMap);
  	
	m_myTip = new DynamicTip(this);
	CHECK_PTR(m_myTip);
	m_myCursor = new KSimEditorCursor(this);
	CHECK_PTR(m_myCursor);
	
	setMouseTracking(true);
	setEditorMode(EM_SELECT);
	setEditorView(EV_SHEETVIEW);
	lmbDown = dragging = isMapResized = false;
   	
	selected.setAutoDelete(false);
   	
	setBackgroundMode(NoBackground);

	connect (&autoScrollTimer, SIGNAL(timeout()), SLOT(autoScroll()));
	connect (&delayedStatusHelpMsgTimer, SIGNAL(timeout()), SLOT(slotDelayedStatusHelpMsg()));
	
	
	QFont newFont("helvetica",12);
	setFont(newFont);
	
}

KSimEditor::~KSimEditor()
{
	delete drawMap;
	g_editorList->remove(this);
}

KSimusApp *KSimEditor::getApp() const
{
  return getView()->getApp();
}

KSimusDoc *KSimEditor::getDoc() const
{
  return getView()->getDocument();
}

CompContainer *KSimEditor::getContainer() const
{
  return getDoc()->getContainer();
}

KSimusView *KSimEditor::getView() const
{
  KSimusView *theView=(KSimusView *) parentWidget()->parentWidget();

  return theView;
}

void KSimEditor::print(QPrinter *pPrinter)
{
	QPainter printpainter;
	printpainter.begin(pPrinter);
//	printpainter.scale(0.5,0.5);
	FOR_EACH_COMPVIEW(it, *viewList)
	{
		it.current()->print(&printpainter);
	}
	
	
	// TODO: add your printing code here

	printpainter.end();
}

/** Redraw the drawMap with all components, wires and selections
	updateDrawMap() doesn't update the widget. update() will do that.*/
void KSimEditor::updateDrawMap()
{
	CHECK_PTR(drawMap);

	QPainter p (drawMap);
	switch (editorView)
	{
		case EV_SHEETVIEW:
			drawMap->fill(white);
			break;
		
		case EV_USERVIEW:
//			drawMap->fill(lightGray);
			drawMap->fill(backgroundColor());
			break;
		
		default:
			KSIMDEBUG_VAR("unknown viewType", (int)editorView);
			break;
	}
	
	p.setPen(black);
	p.setBrush(black);
	p.drawRect(0,            0,              RS_HANDLE_SIZE,  RS_HANDLE_SIZE);
	p.drawRect(0,            size.height(),  RS_HANDLE_SIZE, -RS_HANDLE_SIZE);
	p.drawRect(size.width(), 0,             -RS_HANDLE_SIZE,  RS_HANDLE_SIZE);
	p.drawRect(size.width(), size.height(), -RS_HANDLE_SIZE, -RS_HANDLE_SIZE);
	
	
	switch (editorView)
	{
		case EV_SHEETVIEW:
			getContainer()->drawSheetView(&p);
			break;
		
		case EV_USERVIEW:
			getContainer()->drawUserView(&p);
			break;
		
		default:
			break;
	}

	if (!getContainer()->isRunning())
	{
		// Umrandung markierter Objekte zeichnen
		FOR_EACH_COMPVIEW(it, selected)
		{
			if (!it.current()->isHidden())
				it.current()->drawBound (&p);
		}
	}
}	
		


void KSimEditor::paintEvent(QPaintEvent *)
{
	bitBlt(this, 0, 0, drawMap, 0, 0, -1, -1);
}	

/** All views of the document redraw and show the drawMap */
void KSimEditor::refresh()
{
	getDoc()->slotUpdateAllViews(0);
}

void KSimEditor::drawDragRect ()
{
	QPainter p (drawMap);

	switch (editorMode)
	{
		case EM_MOVE:	
		case EM_MOVE_COPY:	
		case EM_INSERT_MOVE:	
			FOR_EACH_COMPVIEW(it, selected)
			{
				QPoint oldPos = it.current()->getPos();
				QPoint newPos (oldPos - dragStart + dragNow);
	      		it.current()->drawBound (&p, newPos);
			}
			break;

		case EM_RECT_SELECT:
		case EM_RESIZE_MAP:
			p.setRasterOp (NotROP);
			p.setPen (QPen (white, 0, SolidLine));
			p.setBrush (NoBrush);
			p.drawRect (QRect (dragStart, dragNow).normalize());
			break;
			
		case EM_WIRE:
			p.setRasterOp (NotROP);
			p.setPen(QPen(black, 2, DashLine));
			p.drawLine (dragStart /*startConn->getPos()*/, dragNow);
			break;
			
		default:
			break;
	}
   	update();
}


void KSimEditor::select (CompView * compView, bool sel)
{
	if(!viewList->containsRef(compView))
		return;
	if ((selected.containsRef (compView) == 0) == sel)
	{
		QPainter p (drawMap);
		compView->drawBound (&p);
		update();

		if (sel)
			selected.append (compView);
		else
			selected.removeRef (compView);
	}
	
	if (selected.count())
	{
		emit deleteAllowed(true);
		emit cutAllowed(true);
		emit copyAllowed(true);
	}
	else
	{
		emit deleteAllowed(false);
		emit cutAllowed(false);
		emit copyAllowed(false);
	}
}

void KSimEditor::select (Component *comp, bool sel)
{
	if(getContainer()->getComponentList()->containsRef(comp))
		select(getSpecificCompView(comp), sel);
}

// Hebt die Auswahl auf.
void KSimEditor::unselectAll()
{
	while (!selected.isEmpty())
		select (selected.first(), FALSE);

	emit deleteAllowed(false);
	emit cutAllowed(false);
	emit copyAllowed(false);
}


void KSimEditor::mousePressEvent (QMouseEvent *ev)
{
	eHitType hit;
	getDoc()->setActiveView(getView());
	QPoint mousePos = mapFromGlobal(ev->globalPos());
  	// Left mouse button
	if (ev->button() == LeftButton)
	{
		dragStart = dragNow = mousePos;
		lmbDown = TRUE;
		dragging = isMapResized = false;

		switch (editorMode)
		{
			case EM_SELECT:
				// Test resizehandle
				if ( ((mousePos.x()<= RS_HANDLE_SIZE) || (mousePos.x()>= size.width()-RS_HANDLE_SIZE))
				  && ((mousePos.y()<= RS_HANDLE_SIZE) || (mousePos.y()>= size.height()-RS_HANDLE_SIZE))
				  && (!getContainer()->isRunning()))
				{
					resizeDir = RS_NoResize;
					// Horizontal
					if (mousePos.x()<= RS_HANDLE_SIZE)
						resizeDir |= RS_Left;
					else
						resizeDir |= RS_Right;
					// Vertical
					if (mousePos.y()<= RS_HANDLE_SIZE)
						resizeDir |= RS_Up;
					else
						resizeDir |= RS_Down;
						
					setEditorMode(EM_RESIZE_MAP);
					
					return;
				}
				
				// First check selected object
				hit = getContainer()->isCompViewHit(mousePos, &selected);
				{
					switch (hit)
					{
						case NORMAL_HIT:
							if (!getContainer()->isRunning())
							{
								setEditorMode(EM_MOVE_OR_SELECT);
							}
							return;
							break;
							
						case CONNECTOR_HIT:
						case WIRE_HIT:
							if (!getContainer()->isRunning())
							{
								setEditorMode(EM_WIRE);
								// Get the connector
								startConn = getContainer()->getFirstConnector();
							}
							return;
							break;
							
						case SPECIAL_HIT:
						case COMP_RESIZE_F_HIT:
						case COMP_RESIZE_B_HIT:
							{
								// Set editor mode
								if(hit == COMP_RESIZE_B_HIT) setEditorMode(EM_COMP_RESIZE_B);
								else if(hit == COMP_RESIZE_F_HIT) setEditorMode(EM_COMP_RESIZE_F);
								else setEditorMode(EM_SPECIAL);
								
								special = getContainer()->getFirstCompView();
								QPainter p(drawMap);
								special->mousePress(ev, &p);
								p.end();
								refresh();
							}
							return;
							break;
							
						default:
							break;
					}
				}
				// Then check all displayed objects
				hit = getContainer()->isCompViewHit(mousePos, viewList);
				switch (hit)
				{
					case NORMAL_HIT:
						if (!getContainer()->isRunning())
						{
							setEditorMode(EM_SINGLE_SELECT);
						}
						return;
						break;
							
					case CONNECTOR_HIT:
					case WIRE_HIT:
						if (!getContainer()->isRunning())
						{
							setEditorMode(EM_WIRE);
							// Get the connector
							startConn = getContainer()->getFirstConnector();
							dragStart.setX((dragStart.x()/gridX)*gridX+gridX/2);
							dragStart.setY((dragStart.y()/gridY)*gridY+gridY/2);
						}
						return;
						break;
							
					case SPECIAL_HIT:
					case COMP_RESIZE_F_HIT:
					case COMP_RESIZE_B_HIT:
						{
							// Set editor mode
							if(hit == COMP_RESIZE_B_HIT) setEditorMode(EM_COMP_RESIZE_B);
							else if(hit == COMP_RESIZE_F_HIT) setEditorMode(EM_COMP_RESIZE_F);
							else setEditorMode(EM_SPECIAL);
							QPainter p(drawMap);
							special = getContainer()->getFirstCompView();
							special->mousePress(ev, &p);
							p.end();
							refresh();
						}
						return;
						break;
						
					default:
						break;
				}
				if (!getContainer()->isRunning())
				{
					setEditorMode(EM_RECT_SELECT);
				}
				break;
			
			case EM_INSERT:
			{
				Component * newComp;
				CompView * newCompView;
				QRect place;
				QPoint pos;
				if (g_library->getComponentLib()->createComponent(&newComp, getDoc()->getContainer(), m_insertCI->getLibName()))
				{
					if (newComp->isModule())
					{
						if (((Module*)newComp)->getModuleFile().isEmpty())
						{
							// Set module file name
							ASSERT(!getApp()->getModuleFile().isEmpty());
						
							const ModuleInfo * mi =	ModuleData::makeModuleInfo(getApp()->getModuleFile());
              if (!mi)
              {
              	// No valid module
           			KMessageBox::error(0,i18n("The file %1 contains no valid module!").arg(getApp()->getModuleFile()),
           													i18n("Open Module"));

              	delete newComp;
              	setEditorMode(EM_SELECT);
              	break;
              }
							((ComponentLibrary*)g_library->getComponentLib())->insert(mi, 0);
							((Module*)newComp)->setModuleFile(getApp()->getModuleFile());
						}
					}
					
					KSimUndo * undo = getDoc()->getUndo();
					undo->begin(i18n("New Component"));
					getContainer()->addComponent(newComp);
					undo->end();
					getDoc()->setModified();
					
					
					setEditorMode(EM_INSERT_MOVE);
					newCompView = getSpecificCompView(newComp);
					
					// Position check
					place = newCompView->getPlace();
					pos.setX(dragStart.x() - place.width()/2);
					pos.setY(dragStart.y() - place.height()/2);
					pos = newCompView->mapToGrid(pos);
					if (pos.x() < 0)
						pos.setX(0);
					if (pos.x() >= size.width() - place.width())
						pos.setX(size.width() - place.width());
					if (pos.y() < 0)
						pos.setY(0);
					if (pos.y() >= size.height() - place.height())
						pos.setY(size.height() - place.height());
					newCompView->setPos(pos);
					getDoc()->addComponentToEditor(newComp);

					
					// Update selection
					unselectAll();
					select(newCompView, true);
					refresh();
					
					mouseMoveEvent(ev);
				}
				else
				{
					KSIMDEBUG("View mouse pressed EM_INSERT");
					setEditorMode(EM_SELECT);
				}
			}
			break;
			
			case EM_PAST:
			{
				QRect sheet = QRect(QPoint(0,0), size);
				QPoint pos(0,0);
				ComponentList compList;
				int x = 0;
				int y = 0;
				int minX = INT_MAX;
				int minY = INT_MAX;
				int maxX = INT_MIN;
				int maxY = INT_MIN;
				unselectAll();
				getDoc()->setModified();
				KSimUndo * undo = getDoc()->getUndo();
				undo->begin(i18n("Copy Components"));
				getContainer()->pastComponent(&compList, pos);
				undo->end();
				convertComponentToCompView(&compList, &selected);
				// Calculate dimension and the middle of all components
				FOR_EACH_COMPVIEW(it, selected)
				{
					QRect rec(it.current()->getPlace());
					
					x += rec.left();
					if (minX > rec.left()) minX = rec.left();
					x += rec.right();
					if (maxX < rec.right())	maxX = rec.right();
					y += rec.top();
					if (minY > rec.top())	minY = rec.top();
					y += rec.bottom();
					if (maxY < rec.bottom()) maxY = rec.bottom();
				};
				pos.setX( x / (selected.count()*2));
				pos.setY( y / (selected.count()*2));
				pos = dragStart - pos;				
				// Components have to fit in sheet
				if (pos.x() < -minX)
					pos.setX(-minX);
				if (pos.y() < -minY)
					pos.setY(-minY);
				if (pos.x() > sheet.right()-maxX)
					pos.setX(sheet.right()-maxX);
				if (pos.y() > sheet.bottom()-maxY)
					pos.setY(sheet.bottom()-maxY);
				
				getContainer()->moveComponent(&selected, pos);
				select(selected.first(),true);
				getContainer()->routeComponents();
				refresh();
				setEditorMode(EM_PAST_MOVE);

				dragNow = mousePos;
				mouseMoveEvent(ev);
			}
			break;
			
			default:
				break;
		}
	}
  	// Right mouse button
	else if ((ev->button() == RightButton) && (!lmbDown))
	{
		if ((editorMode == EM_INSERT) || (editorMode == EM_PAST))
		{
			setEditorMode(EM_SELECT);
			return;
		}
		
		if (!getContainer()->isRunning())
		{
			switch(getContainer()->isCompViewHit(mousePos, viewList))
			{
				case NO_HIT:
					backgroundPopup();
					break;
				
				case CONNECTOR_HIT:
					componentPopup(true);
					break;
				
				case WIRE_HIT:
					wirePopup(mousePos);
					break;
				
				case NORMAL_HIT:
				case SPECIAL_HIT:
				case COMP_RESIZE_F_HIT:
				case COMP_RESIZE_B_HIT:
					componentPopup(false);
					break;
			
				default:
					ASSERT(0);
					break;
			}
		}
	}
}

static QString getComponentPartName(const Component * comp, const ConnectorBase * conn)
{
	QString s(comp->getInfo()->getName());

	if (conn)
	{
		s += ":" + conn->getName();
	}
		
	if (comp->getName() != comp->getInfo()->getName())	
	{
		s += " (" + comp->getName() + ")";
	}
	
	return s;
}

void KSimEditor::mouseMoveEvent (QMouseEvent *ev)
{
	static int lastMsgX = -1, lastMsgY = -1;
	QPoint dragPos;
	QPoint mousePos = mapFromGlobal(ev->globalPos());
	eHitType hit = getContainer()->isCompViewHit(mousePos, viewList);
	// New component info
	if (((mousePos.x()+gridX)/2 != lastMsgX)
		|| ((mousePos.y()+gridY)/2 != lastMsgY))
	{
		 // Store pos
		 lastMsgX = (mousePos.x()+gridX)/2;
		 lastMsgY = (mousePos.y()+gridY)/2;
		
		switch(hit)
		{
			case INVALID_HIT:
			case NO_HIT:
				delayedStatusHelpMsg(QString::null);
				m_myTip->m_text = QString::null;
				break;
			
			case NORMAL_HIT:
			case WIRE_HIT:
			case SPECIAL_HIT:
			case COMP_RESIZE_F_HIT:
			case COMP_RESIZE_B_HIT:
				delayedStatusHelpMsg(getComponentPartName(getContainer()->getFirstCompView()->getComponent(), 0));
				m_myTip->m_text = getComponentPartName(getContainer()->getFirstCompView()->getComponent(), 0);
				break;
				
			case CONNECTOR_HIT:
			{
				delayedStatusHelpMsg(getComponentPartName(getContainer()->getFirstCompView()->getComponent(),
				                                      getContainer()->getFirstConnector()));
				m_myTip->m_text = getComponentPartName(getContainer()->getFirstCompView()->getComponent(),
				                                      getContainer()->getFirstConnector());
				break;
			}
		}
	}
	
	if (lmbDown)
	{
		if ((editorMode == EM_SPECIAL)
		 || (editorMode == EM_COMP_RESIZE_B)
		 || (editorMode == EM_COMP_RESIZE_F))
		{
			QPainter p(drawMap);
			special->mouseMove(ev, &p);
			p.end();
			drawDragRect();
//			refresh();	No refresh because during resizeing the connectors will be moved also!
			return;
		}
	
		QRect sheet = QRect(QPoint(0,0), size);
		if ( dragging  ||
			(abs(mousePos.x()-dragStart.x())>=4) ||
			(abs(mousePos.y()-dragStart.y())>=4) )
		{
			switch (editorMode)
			{
				case EM_SINGLE_SELECT:
				{
					eHitType hit;

					if ((ev->state() & ControlButton) == 0)
						unselectAll();
					hit = getContainer()->isCompViewHit(dragStart, viewList);
					if (hit == NORMAL_HIT)
					{
						select (getContainer()->getFirstCompView(),
								selected.containsRef (getContainer()->getFirstCompView()) == 0);
					}
				}
				// no break here !!!!!
					
				case EM_MOVE_OR_SELECT:
					setEditorMode(EM_MOVE);
					// no break here !!!!!
					
				case EM_MOVE:
				case EM_MOVE_COPY:
				case EM_INSERT_MOVE:
				case EM_PAST_MOVE:
				{
					if ((editorMode == EM_MOVE) && (ev->state() & ControlButton))
					{
						setEditorMode(EM_MOVE_COPY);
					}
					else if ((editorMode == EM_MOVE_COPY) && !(ev->state() & ControlButton))
					{
						setEditorMode(EM_MOVE);
					}
					
					dragPos = mousePos;
					// Components fits into sheet?
					FOR_EACH_COMPVIEW (it, selected)
					{
						QRect newPlace(it.current()->getPlace());
						newPlace.moveTopLeft(newPlace.topLeft()+ dragPos - dragStart);
				
						if (0 < (sheet.top()- newPlace.top()))
							dragPos.setY(dragPos.y() + (sheet.top() - newPlace.top()));
						if (0 > (sheet.bottom()- newPlace.bottom()))
							dragPos.setY(dragPos.y() + (sheet.bottom() - newPlace.bottom()));
						if (0 < (sheet.left()- newPlace.left()))
							dragPos.setX(dragPos.x() + (sheet.left() - newPlace.left()));
						if (0 > (sheet.right()- newPlace.right()))
							dragPos.setX(dragPos.x() + (sheet.right() - newPlace.right()));
					}
				}
				break;
					
				case EM_RECT_SELECT:
				case EM_WIRE:
					dragPos = mousePos;
					if (sheet.top() > dragPos.y())
						dragPos.setY(sheet.top());
					if (sheet.bottom() < dragPos.y())
						dragPos.setY(sheet.bottom());
					if (sheet.left() > dragPos.x())
						dragPos.setX(sheet.left());
					if (sheet.right() < dragPos.x())
						dragPos.setX(sheet.right());
				break;

				case EM_RESIZE_MAP:
					dragPos = resizingMap(mousePos, true);
				break;
				
				default:
				break;			
			}

			// Falls Maus außerhalb des Fensters, autoScroll aktivieren
			scrollDir = SC_NoScroll;
			QPoint pos = getView()->viewport()->mapToParent(mapToParent(ev->pos()));
			// Scrollview pos
			if (!getView()->viewport()->rect().contains(pos))
			{
				// horizontal
				if (pos.x() <= 0)				
					scrollDir |= SC_Left;
				else if (pos.x() >= getView()->viewport()->width())
					scrollDir |= SC_Right;
				// vertical
				if (pos.y() <= 0)
					scrollDir |= SC_Up;
				else if (pos.y() >= getView()->viewport()->height())
					scrollDir |= SC_Down;
			}
			// Editor pos
			if (!rect().contains(mousePos))
			{
				// horizontal
				if (mousePos.x() <= 0)
					scrollDir |= SC_Left;
				else if (mousePos.x() >= size.width())
					scrollDir |= SC_Right;
				// vertical
				if (mousePos.y() <= 0)
					scrollDir |= SC_Up;
				else if (mousePos.y() >= size.height())
					scrollDir |= SC_Down;
			}
			if (scrollDir == SC_NoScroll)
			{
				autoScrollTimer.stop();
			}
			else
			{
				if (!autoScrollTimer.isActive())
				{
					if (editorMode == EM_RESIZE_MAP)
					{
						autoScrollTimer.start (30);
					}
					else
					{
						autoScrollTimer.start (100);
					}
				}
			}
			
			if (dragging)
			{
				// Remove old boundings or rects
				drawDragRect();
			}
			else
			{
				dragging = true;
			}       	
		    // Save new position
			dragNow = dragPos;
			
			drawDragRect();
		}		
	}
	else  // (!lmb)
	{
		setEditorCursor(&mousePos, hit);
	}
}

static int evButton, evState;

void KSimEditor::mouseReleaseEvent (QMouseEvent *ev)
{
	eHitType hit;
	
	evButton = ev->button();
	evState = ev->state();
	
	autoScrollTimer.stop();
	// Left mouse button
	if (ev->button() == LeftButton)
	{
		lmbDown = FALSE;
		if ((editorMode == EM_SPECIAL)
		 || (editorMode == EM_COMP_RESIZE_B)
		 || (editorMode == EM_COMP_RESIZE_F))
		{
			QPainter p(drawMap);
			special->mouseRelease(ev, &p);
			setEditorMode(EM_SELECT);
			p.end();
			refresh();
			return;
		}
	
		if (dragging)
			drawDragRect();
        dragging = false;

		if ((editorMode == EM_MOVE) && (ev->state() & ControlButton))
		{
			setEditorMode(EM_MOVE_COPY);
		}
		else if ((editorMode == EM_MOVE_COPY) && !(ev->state() & ControlButton))
		{
			setEditorMode(EM_MOVE);
		}
		
		switch (editorMode)
		{
			case EM_MOVE_OR_SELECT:
			case EM_SINGLE_SELECT:
				if ((ev->state() & ControlButton) == 0)
					unselectAll();
				hit = getContainer()->isCompViewHit(dragNow, viewList);
				if (hit == NORMAL_HIT)
				{
					select (getContainer()->getFirstCompView(),
							selected.containsRef (getContainer()->getFirstCompView()) == 0);
				}
				setEditorMode(EM_SELECT);
				break;
				
			case EM_RECT_SELECT:
			{
				if ((ev->state() & ControlButton) == 0)
					unselectAll();
				QRect selectArea = QRect (dragStart, dragNow).normalize();
				FOR_EACH_COMPVIEW(it, *viewList)
				{
 					if (selectArea.contains (it.current()->getPlace()))
						select (it.current(), TRUE);
				}
				setEditorMode(EM_SELECT);
				break;
			}			
			case EM_MOVE:
			{
				KSimUndo * undo = getDoc()->getUndo();
				QPoint diff(dragNow - dragStart);
				undo->begin(i18n("Move Components"));
				undo->reload(&selected);
				undo->end();
				getContainer()->moveComponent(&selected, diff);
				getContainer()->setModified();
				getContainer()->routeComponents();
				refresh();
				setEditorMode(EM_SELECT);
				break;
			}
				
			case EM_MOVE_COPY:
			{
				KSimUndo * undo = getDoc()->getUndo();
				QPoint diff(dragNow - dragStart);
				undo->begin(i18n("Copy Components"));
				ComponentList compList;
				getContainer()->copyComponent(&selected);
				selected.clear();
				getContainer()->pastComponent(&compList, diff);
				convertComponentToCompView(&compList, &selected);
				undo->end();
				getContainer()->setModified();
				getContainer()->routeComponents();
				refresh();
				setEditorMode(EM_SELECT);
				break;
			}
				
			case EM_INSERT_MOVE:
			case EM_PAST_MOVE:
			{
				QPoint diff(dragNow - dragStart);
				getContainer()->moveComponent(&selected, diff);
				getDoc()->setModified();
				getContainer()->routeComponents();
				refresh();
				if (editorMode == EM_INSERT_MOVE)
					setEditorMode(EM_INSERT);
				else
					setEditorMode(EM_PAST);
				break;
			}
				
			case EM_WIRE:
				hit = getContainer()->isCompViewHit(dragNow, viewList);
				if ((hit == CONNECTOR_HIT) || (hit == WIRE_HIT))
				{
					KSimUndo * undo = getDoc()->getUndo();
					undo->begin(i18n("Add connection"));
					getContainer()->addConnection(startConn, getContainer()->getFirstConnector());
					undo->end();
					getDoc()->setModified();
					getContainer()->routeComponents();
				}
				refresh();
				setEditorMode(EM_SELECT);
				break;
			
			case EM_RESIZE_MAP:
				resizingMap(dragNow, false);
				if(isMapResized)
				{
					getDoc()->getUndo()->end();
					isMapResized = false;
				}
				setEditorMode(EM_SELECT);
				break;
			
			default:
				break;
		}
	}
}

// Wird vom Timer aufgerufen, wenn sich der Mauszeiger bei gedrückter
// Maustaste außerhalb des Fensters befindet. Die Anzeigebereich wird
// ein Stück in die Richtung verschoben, die scrollDir angibt.
void KSimEditor::autoScroll ()
{
/*	if (dragging) drawDragRect();
	dragging = TRUE;*/
	
	if (scrollDir & SC_Left)
		getView()->horizontalScrollBar()->subtractLine();
	
	if (scrollDir & SC_Right)
		getView()->horizontalScrollBar()->addLine();
	
	if (scrollDir & SC_Up)
		getView()->verticalScrollBar()->subtractLine();
	
	if (scrollDir & SC_Down)
		getView()->verticalScrollBar()->addLine();
	
	// Call mouseMoveEvent for position calculating
	QMouseEvent ev = QMouseEvent( QEvent::MouseMove,
								  mapFromGlobal(QCursor::pos()),
								  evButton, evState);
	mouseMoveEvent(&ev);
}

void KSimEditor::setEditorMode(EditorModeType newMode)
{
	editorMode = newMode;
	
	setEditorCursor((QPoint*)0, INVALID_HIT);
	
	emit editorModeChanged(editorMode);
}

void KSimEditor::setEditorCursor(QPoint * pMousePos, eHitType hit) const
{
	// set cursor type
	switch (editorMode)
	{
		case EM_INSERT:
		case EM_PAST:
				m_myCursor->set(KSimEditorCursor::crossCursor);
			break;
			
		case EM_RESIZE_MAP:
			if (   ((resizeDir ^ (RS_Left|RS_Up)) == 0)
				|| ((resizeDir ^ (RS_Right|RS_Down)) == 0) )
				m_myCursor->set(KSimEditorCursor::sizeFDiagCursor);
			else
				m_myCursor->set(KSimEditorCursor::sizeBDiagCursor);
			break;
			
		case EM_COMP_RESIZE_B:
				m_myCursor->set(KSimEditorCursor::sizeBDiagCursor);
			break;
		
		case EM_COMP_RESIZE_F:
				m_myCursor->set(KSimEditorCursor::sizeFDiagCursor);
			break;
		
		case EM_WIRE:
				m_myCursor->set(KSimEditorCursor::wireCursor);
			break;
		
		default:
			{
				if (!getContainer()->isRunning() /* && (editorMode == EM_SELECT)*/)
				{
					QPoint myMousePos;
					if (pMousePos)
					{
						myMousePos = *pMousePos;
					}
					else
					{
						myMousePos = mapFromGlobal(QCursor::pos());
					}
						
					if ( ((myMousePos.x()<= RS_HANDLE_SIZE) && (myMousePos.y()<= RS_HANDLE_SIZE))
					   ||((myMousePos.x()>= size.width()-RS_HANDLE_SIZE) && (myMousePos.y()>= size.height()-RS_HANDLE_SIZE)) )
					{
						m_myCursor->set(KSimEditorCursor::sizeFDiagCursor);
					}
					else if ( ((myMousePos.x()<= RS_HANDLE_SIZE) && (myMousePos.y()>= size.height()-RS_HANDLE_SIZE))
					        ||((myMousePos.x()>= size.width()-RS_HANDLE_SIZE) && (myMousePos.y()<= RS_HANDLE_SIZE)))
					{
						m_myCursor->set(KSimEditorCursor::sizeBDiagCursor);
					}
					else
					{
						if (hit == INVALID_HIT)
						{
							hit = getContainer()->isCompViewHit(myMousePos, viewList);
						}
						switch(hit)
						{
							case INVALID_HIT:
							case NO_HIT:
							case SPECIAL_HIT:
							case NORMAL_HIT:
								m_myCursor->set(KSimEditorCursor::arrowCursor);
								break;
				
							case CONNECTOR_HIT:
							case WIRE_HIT:
								m_myCursor->set(KSimEditorCursor::wireCursor);
								break;
					
							case COMP_RESIZE_F_HIT:
								m_myCursor->set(KSimEditorCursor::sizeFDiagCursor);
								break;
					
							case COMP_RESIZE_B_HIT:
								m_myCursor->set(KSimEditorCursor::sizeBDiagCursor);
							break;
						}
					}
				}
				else
				{
					m_myCursor->set(KSimEditorCursor::arrowCursor);
				}
				break;
			}
	}
}




/**  */
void KSimEditor::refreshSimMode(CompViewList * sheetList, CompViewList * userList)
{
	QPainter p (drawMap);
	if (EV_SHEETVIEW == editorView)
	{
		FOR_EACH_COMPVIEW(it, *sheetList)
		{
			if (it.current()->isViewChanged())
			{
				it.current()->draw(&p);
			}
		}
	}
	else
	{
		FOR_EACH_COMPVIEW(it, *userList)
		{
			if (it.current()->isViewChanged())
			{
				it.current()->draw(&p);
			}
		}
	}
}
/**  */
bool KSimEditor::eventFilter( QObject * obj, QEvent * ev)
{
	if (getContainer()->isRunning())
	{
		return false;
	}
	else
	{
		switch (ev->type())
		{
			case QEvent::MouseButtonPress:
			case QEvent::MouseButtonRelease:
			case QEvent::MouseButtonDblClick:
			case QEvent::MouseMove:
			{
				QMouseEvent mouseEv = QMouseEvent(  ev->type(),
//								 	 				((QWidget*)obj)->mapToParent(((QMouseEvent*)ev)->pos()),
								 	 				mapFromGlobal(((QWidget*)obj)->mapToGlobal(((QMouseEvent*)ev)->pos())),
								  					((QMouseEvent*)ev)->button(),
								  					((QMouseEvent*)ev)->state()
								  				 );
				return event(&mouseEv);
			}
			
			default:
				return false;
		}
	}
}
/** Returns the component view in order of the selected  view type */
CompView* KSimEditor::getSpecificCompView(Component * comp) const
{
	if (editorView == EV_SHEETVIEW)
	{
		return comp->getSheetView();
	}
	else
	{
		return comp->getUserView();
	}
}

/** Convert a list of component to a list of compoennt views in order of the selected  view type */
void KSimEditor::convertComponentToCompView(ComponentList * compList, CompViewList * compViewList)
{
//	REMOVE_ALL_COMPVIEW(compViewList);
	compViewList->clear();
	FOR_EACH_COMP(it, *compList)
	{
		if (editorView == EV_SHEETVIEW)
		{
			if (it.current()->getSheetView())
				compViewList->append(it.current()->getSheetView());
		}
		else
		{
			if (it.current()->getUserView())
				compViewList->append(it.current()->getUserView());
		}
	}
}

/** Set the type of view */
void KSimEditor::setEditorView(EditorViewType viewType)
{
	CompViewList oldSel = selected;
	
	if (viewType != EV_NOTVALID)
	{
		editorView = viewType;
	
		// Delete all childrens
		emit destroyed();
	
		unselectAll();
		if (viewType == EV_SHEETVIEW)
		{
			setSize(getContainer()->getSheetSize());
/*			getView()->resizeContents(  size.width()+100,
										size.height()+100);
			move(50,50);*/
			viewList = getContainer()->getSheetViewList();
			
			FOR_EACH_COMPVIEW(it, oldSel)
			{
				if (it.current()->getComponent()->getSheetView())
					selected.append(it.current()->getComponent()->getSheetView());
			}
		}
		else
		{
			setSize(getContainer()->getUserSize());
/*			getView()->resizeContents(  size.width()+2,	size.height()+2);
			move(1,1);*/
			viewList = getContainer()->getUserViewList();
			FOR_EACH_COMPVIEW(it, oldSel)
			{
				if (it.current()->getComponent()->getUserView())
					selected.append(it.current()->getComponent()->getUserView());
			}
		}
		
		updateDrawMap();
				
		// Create new childrens
		FOR_EACH_COMPVIEW(it, *viewList)
		{
			it.current()->makeWidget(this);
		}
		refresh();
		
		emit editorViewChanged(editorView);
	}	
}
/** Returns the size of the drawing area */
const QSize & KSimEditor::getSize() const
{
	return size;
}
					
/** Sets the size of the drawing area */
void KSimEditor::setSize(const QSize & newSize)
{
	size = newSize;
/*	delete drawMap;
	drawMap = new QPixmap(size);
	CHECK_PTR(drawMap);*/

	drawMap->resize(size);
	setFixedSize(size);
	getView()->resizeContents(size.width(),size.height());
	getView()->moveChild(this,0,0);
}

/** Manages map resizing
	Called from mouseMoveEvent */
QPoint KSimEditor::resizingMap(const QPoint & mousePos, bool increaseOnly)
{
	QPoint dragPos = mousePos;
	QPoint move = QPoint(0,0);
	QSize newSize = size;
	QRect usedRect = CompContainer::getContainRect(viewList);
	if (usedRect.isNull())
	{
		usedRect = QRect(QPoint(size.width()-gridX,size.height()-gridY),QPoint(gridX,gridY));
	}
	
	if (resizeDir & RS_Up)
	{
		// increase ?
		if (mousePos.y() < -gridY)
		{
			newSize.setHeight(size.height() + gridY);
			move.setY(gridY);
			dragPos.setY(0);
			dragStart.setY(size.height() + gridY - 1);
		}
		else if (mousePos.y() > 0)
		{
			dragStart.setY(size.height() - 1);
			dragPos.setY(QMIN(mousePos.y(), QMAX(0,usedRect.top()/* - gridY*/)));
			if (!increaseOnly)
			{
				int rel = dragPos.y();
				newSize.setHeight(size.height() - rel);
				move.setY(-rel);
			}
		}
	}
	
	if (resizeDir & RS_Left)
	{
		// increase ?
		if (mousePos.x() < -gridX)
		{
			newSize.setWidth(size.width() + gridX);
			move.setX(gridX);
			dragPos.setX(0);
			dragStart.setX(size.width() + gridX - 1);
		}
		else if (mousePos.x() > 0)
		{
			dragStart.setX(size.width() - 1);
			dragPos.setX(QMIN(mousePos.x(), QMAX(0,usedRect.left()/* - gridX*/)));
			if (!increaseOnly)
			{
				int rel = dragPos.x();
				newSize.setWidth(size.width() - rel);
				move.setX(-rel);
			}
		}
	}
	
	if (resizeDir & RS_Down)
	{
		dragStart.setY(0);
		// increase ?
		if (mousePos.y() > size.height() + gridY)
		{
			newSize.setHeight(size.height() + gridY);
			dragPos.setY(size.height() + gridY -1);
		}
		else if (mousePos.y() < size.height())
		{
			dragPos.setY(QMAX(mousePos.y(), QMIN(size.height(),usedRect.bottom() /*+ gridY*/)));
			if (!increaseOnly)
				newSize.setHeight(dragPos.y());
		}
	}
	
	if (resizeDir & RS_Right)
	{
		dragStart.setX(0);
		// increase ?
		if (mousePos.x() > size.width() + gridX)
		{
			newSize.setWidth(size.width() + gridX);
			dragPos.setX(size.width() + gridX -1);
		}
		else if (mousePos.x() < size.width())
		{
			dragPos.setX(QMAX(mousePos.x(), QMIN(size.width(),usedRect.right() /*+ gridX*/)));
			if (!increaseOnly)
				newSize.setWidth(dragPos.x());
		}
	}

	// Round dragPos
	dragPos.setX(((dragPos.x()+gridX/2)/gridX)*gridX);
	dragPos.setY(((dragPos.y()+gridY/2)/gridY)*gridY);
	
	QString msg;
	msg.sprintf(i18n("Resize Map: %i,%i"),
				((QABS(dragPos.x()-dragStart.x())+gridX/2)/gridX)*gridX,
				((QABS(dragPos.y()-dragStart.y())+gridY/2)/gridY)*gridY);
	getApp()->slotStatusMsg(msg);

		
	if (newSize != size)
	{
		size = newSize;
		if (editorView == EV_SHEETVIEW)
		{
			if(!isMapResized)
			{
				getDoc()->getUndo()->begin(i18n("Resizing schematic map"));
				isMapResized = true;
				getDoc()->setModified();
			}
/*			getContainer()->setSheetSize(size);
			size = getContainer()->getSheetSize();
			getView()->resizeContents(size.width(),	size.height()/);*/
			getDoc()->setSheetSize(size);
		}
		else
		{
			if(!isMapResized)
			{
				getDoc()->getUndo()->begin(i18n("Resizing user interface map"));
				isMapResized = true;
				getDoc()->setModified();
			}
/*			getContainer()->setUserSize(size);
			size = getContainer()->getUserSize();
			getView()->resizeContents(size.width(), size.height());*/
			getDoc()->setUserSize(size);
		}
/*		setFixedSize( size );
		drawMap->resize(size);*/
		
		
		if (!move.isNull())
		{
			getDoc()->getUndo()->reload(viewList);
			getContainer()->moveComponent(viewList,move);
			getContainer()->routeComponents();
		}
		updateDrawMap();
		update();
		dragging = false;
	}
	
	return dragPos;
}
			
		
/** Execute the "background" popup menu
	This menu is display, if the left mouse button is pressed
	and no component, wire, or connector is hit */
void KSimEditor::backgroundPopup()
{
	QPopupMenu * menu = new QPopupMenu(0, "backgroundPopup");
	CHECK_PTR(menu);
	int res;
	
	ComponentPopupMenu * compMenu = new ComponentPopupMenu(menu);
	connect(compMenu, SIGNAL(signalSelection(const ComponentInfo *)),
			getApp(), SLOT(slotSelectCI(const ComponentInfo *)));

    getApp()->editDelete->plug(menu);
    getApp()->editCut->plug(menu);
    getApp()->editCopy->plug(menu);
    getApp()->editPaste->plug(menu);
	
	menu->insertSeparator();
	menu->insertItem(i18n("New component"), compMenu, ID_COMP_MENU);

//	connect(menu, SIGNAL(highlighted(int)), getApp(), SLOT(statusCallback(int)));
	
	res = menu->exec(QCursor::pos());
	
//	getApp()->commandCallback(res);
	
	delete menu;
}
	
/** Execute the component popup menu
	This menu is display, if one or more components are hit */
void KSimEditor::componentPopup(bool connectorHit)
{
	int idx, connIdx;
	int rotCWIdx = 0;
	int rotCCWIdx = 0;
	KSimUndo * undo = getDoc()->getUndo();
	CHECK_PTR(undo);
	QPopupMenu * menu = new QPopupMenu(0, "componentPopup");
	CHECK_PTR(menu);
	int res;
	
	if ((!selected.containsRef(getContainer()->getFirstCompView())) || connectorHit)
	{
		// object is not selected
		// select it now
		unselectAll();
		select(getContainer()->getFirstCompView(), TRUE);
	}
	
	getApp()->editDelete->plug(menu);
	getApp()->editCut->plug(menu);
	getApp()->editCopy->plug(menu);
	getApp()->editPaste->plug(menu);
	
	if (selected.count()==1)
	{
		Component * comp = selected.first()->getComponent();
		CompView * compView = getSpecificCompView(comp);
		ConnectorBase * conn = 0;
		
		
		// Init rotation
		if(compView && compView->isNormalRotationEnabled())
		{
			menu->insertSeparator();
			rotCWIdx = menu->insertItem(i18n("Rotate &CW"));
			rotCCWIdx = menu->insertItem(i18n("Rotate CC&W"));
		}
		
		if (connectorHit)
		{
			conn = getContainer()->getFirstConnector();
			conn->initPopupMenu(menu);
		}
		
		comp->initPopupMenu(menu);

		menu->insertSeparator();
		idx = menu->insertItem(i18n("P&roperties"));
		connIdx = menu->insertItem(i18n("C&onnector Properties"));
		// Execute popup
		res = menu->exec(QCursor::pos());
		
		if (res == idx)
		{
			ComponentPropertyDialog * dia;
			dia = new ComponentPropertyDialog(comp);
			comp->initPropertyDialog(dia);
			dia->exec();
			delete dia;
		}
		else if (res == connIdx)
		{
			ConnectorPropertyDialog * dia = new ConnectorPropertyDialog(comp->getConnList(), conn);
			dia->exec();
			delete dia;
		}
		else if (res == rotCWIdx)
		{
			undo->changeProperty(compView, i18n("Rotate CW"));
			compView->stepRotationCW();
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
		else if (res == rotCCWIdx)
		{
			undo->changeProperty(compView, i18n("Rotate CCW"));
			compView->stepRotationCCW();
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
	}
	else
	{
		// Multiple selection
		res = menu->exec(QCursor::pos());
	}
		
	refresh();
	delete menu;
}

/** Execute the wire popup menu
	This menu is display, if a wire is hit */
void KSimEditor::wirePopup(const QPoint & pos)
{
	QPopupMenu * menu = new QPopupMenu(0, "wirePopup");
	CHECK_PTR(menu);
	KSimUndo * undo = getDoc()->getUndo();
	CHECK_PTR(undo);
	Component * comp = getContainer()->getFirstCompView()->getComponent();
	int res,idDelete, idTruncate, idProperty;

	idDelete = menu->insertItem(i18n("&Delete wire"));
	idTruncate = menu->insertItem(i18n("&Truncate wire"));
	
	if (comp->getConnList()->count() <= 2)
	{
		// No truncate
		menu->setItemEnabled(idTruncate, false);
	}
		
	comp->initPopupMenu(menu);

	menu->insertSeparator();
	idProperty = menu->insertItem(i18n("P&roperties"));
	
	// Execute popup
	res = menu->exec(QCursor::pos());
	
	if(res == idDelete)
	{
		undo->begin(i18n("Delete Wire"));
		getContainer()->delComponent(comp);
		undo->end();
		getDoc()->setModified();
		getContainer()->routeComponents();
		refresh();
	}
	else if (res == idTruncate)
	{
		undo->begin(i18n("Truncate Wire"));
		getContainer()->truncateWire((Wire*)comp, pos);
		undo->end();
		getDoc()->setModified();
		getContainer()->routeComponents();
		refresh();
	}
	else if (res == idProperty)
	{
		ComponentPropertyDialog * dia;
		dia = new ComponentPropertyDialog(comp);
		comp->initPropertyDialog(dia);
		dia->exec();
		delete dia;
	}
	else if (res == -1)
	{
		// Nothing
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Result",res);
	}
	
	delete menu;
}

/** Receives the component info for inseting component */
void KSimEditor::slotSelectCI(const ComponentInfo *ci)
{
	if(ci)
	{
		m_insertCI = ci;
		setEditorMode(EM_INSERT);
	}
	else
	{
		setEditorMode(EM_SELECT);
	}
}

/** Cuts selected components */
void KSimEditor::cutSelection()
{
	KSimUndo * undo = getDoc()->getUndo();
	undo->begin(i18n("Cut Components"));
	getContainer()->cutComponent(&selected);
	undo->end();
	unselectAll();
	getContainer()->setModified();
	getContainer()->routeComponents();
	getDoc()->slotUpdateAllViews(0);
}

/** Deletes selected components */
void KSimEditor::deleteSelection()
{
	KSimUndo * undo = getDoc()->getUndo();
	undo->begin(i18n("Delete Components"));
	getContainer()->delComponent(&selected);
	undo->end();
	unselectAll();
	getContainer()->setModified();
	getContainer()->routeComponents();
	getDoc()->slotUpdateAllViews(0);
}

/** Copies selected components */
void KSimEditor::copySelection()
{
	getContainer()->copyComponent(&selected);
}

void KSimEditor::slotDelete(Component * comp)
{
	// Delete components (views!) form selection
	if (comp->getSheetView())
		selected.removeRef(comp->getSheetView());
	if (comp->getUserView())
		selected.removeRef(comp->getUserView());
		
}

void KSimEditor::delayedStatusHelpMsg(const QString & msg)
{
	if (msg != delayedStatusHelpMsgString)
	{
		delayedStatusHelpMsgString = msg;
/*		if (msg.isEmpty())
		{
			delayedStatusHelpMsgTimer.stop();
		}
		else*/
		{
			delayedStatusHelpMsgTimer.start(30, true);
		}
	}
}

void KSimEditor::slotDelayedStatusHelpMsg()
{
	if (delayedStatusHelpMsgString.isEmpty())
	{
//		emit signalStatusHelpMsg(i18n("Ready."));
	}
	else
	{
		emit signalStatusHelpMsg(delayedStatusHelpMsgString);
	}
}
