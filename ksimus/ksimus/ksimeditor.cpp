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
#include <qpainter.h>
#include <qpixmap.h>
#include <qscrollview.h>
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qbitmap.h>
#include <qpaintdevicemetrics.h>
#include <qcursor.h>

// include files for KDE
#include <kinstance.h>
#include <kapplication.h>
#include <kprinter.h>
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
#include "packageinfo.h"
#include "watchwidget.h"
#include "wire.h"
#include "wireproperty.h"


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


#include "wire.cur"
#include "wire_mask.cur"

class KSimEditor::Cursor
{
public:
	enum KSimCursorType { arrowCursor, upArrowCursor, crossCursor, waitCursor, ibeamCursor,
	                      sizeVerCursor, sizeHorCursor, sizeBDiagCursor, sizeFDiagCursor,
	                      sizeAllCursor, blankCursor, splitVCursor, splitHCursor,
	                      pointingHandCursor, forbiddenCursor, wireCursor };
	
	Cursor(KSimEditor * editor);
	~Cursor() {};
	
	void set(KSimCursorType newCursor);
	void setNow(KSimCursorType newCursor);
	KSimCursorType get() const { return m_current; };
		
		
private:
	KSimCursorType m_current;
	KSimEditor * m_editor;
	QCursor * m_wireCursor;
};

KSimEditor::Cursor::Cursor(KSimEditor * editor)
	:	m_editor(editor)
{
	// Wire cursor
	QBitmap bitmap(wire_width, wire_height, wire_bits, true);
	QBitmap bitmapMask(wire_mask_width, wire_mask_height, wire_mask_bits, true);
	m_wireCursor = new QCursor(bitmap, bitmapMask, wire_x_hot, wire_y_hot);
	Q_CHECK_PTR(m_wireCursor);

	setNow(arrowCursor);
};

void KSimEditor::Cursor::set(KSimCursorType newCursor)
{
	if(get() != newCursor)
	{
		setNow(newCursor);
	}
}

void KSimEditor::Cursor::setNow(KSimCursorType newCursor)
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
	Q_CHECK_PTR(drawMap);
	
	m_myCursor = new KSimEditor::Cursor(this);
	Q_CHECK_PTR(m_myCursor);
	
	setMouseTracking(true);
	setEditorMode(EM_SELECT);
	setEditorView(EV_SHEETVIEW);
	lmbDown = dragging = isMapResized = false;
	
	selected.setAutoDelete(false);
	
	setBackgroundMode(NoBackground);

	connect (&autoScrollTimer, SIGNAL(timeout()), SLOT(autoScroll()));
	connect (&delayedStatusHelpMsgTimer, SIGNAL(timeout()), SLOT(slotDelayedStatusHelpMsg()));
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

void KSimEditor::print(KPrinter *pPrinter)
{
	
#define PRINT_FACTOR   2
	
	QPainter p;
	p.begin(pPrinter);
	
/*	QPaintDeviceMetrics pdmEditor(this);
	KSIMDEBUG("-------- this ---------");
	KSIMDEBUG_VAR("",pdmEditor.width());
	KSIMDEBUG_VAR("",pdmEditor.widthMM());
	KSIMDEBUG_VAR("",pdmEditor.height());
	KSIMDEBUG_VAR("",pdmEditor.heightMM());
	KSIMDEBUG_VAR("",pdmEditor.logicalDpiX());
	KSIMDEBUG_VAR("",pdmEditor.logicalDpiY());*/

	
	QPaintDeviceMetrics pdmPrinter(pPrinter);
/*	KSIMDEBUG("-------- pPrinter ---------");
	KSIMDEBUG_VAR("",pdmPrinter.width());
	KSIMDEBUG_VAR("",pdmPrinter.widthMM());
	KSIMDEBUG_VAR("",pdmPrinter.height());
	KSIMDEBUG_VAR("",pdmPrinter.heightMM());
	KSIMDEBUG_VAR("",pdmPrinter.logicalDpiX());
	KSIMDEBUG_VAR("",pdmPrinter.logicalDpiY());*/
	
	int x = 0;
	int y = 0;
	
	do
	{
		p.save();
		
		p.setClipRect(0,0,pdmPrinter.width(),pdmPrinter.height());
		p.translate(-x,-y);
		p.scale(1.0 / PRINT_FACTOR, 1.0 / PRINT_FACTOR);
		
		p.setPen(black);
		p.setBrush(NoBrush);
		p.drawRect(0,0,getSize().width(), getSize().height());
		
		FOR_EACH_COMPVIEW(it, *viewList)
		{
			p.save();
		
			if (it.current()->isNormalRotationEnabled())
			{
				double rot = it.current()->getRotation();
				QRect rect(it.current()->getPlace());
			
					if((rot < 45.0) || (rot >= 315.0))
					{
						p.translate(rect.left(), rect.top());
	//					p.rotate(0.0);
					}
					else if(rot < 135.0)
					{
/*						p.translate(rect.right(), rect.top());
						p.rotate(90.0);*/
						QWMatrix m(0.0, 1.0, -1.0, 0.0, double(rect.right()), double(rect.top()));
						p.setWorldMatrix(m, true);
					}
					else if(rot < 225.0)
					{
/*						p->translate(rect.right(), rect.bottom());
						p->rotate(180.0);*/
						QWMatrix m(-1.0, 0.0, 0.0, -1.0, double(rect.right()), double(rect.bottom()));
						p.setWorldMatrix(m, true);
					}
					else
					{
/*						p->translate(rect.left(), rect.bottom());
						p->rotate(270.0);*/
						QWMatrix m(0.0, -1.0, 1.0, 0.0, double(rect.left()), double(rect.bottom()));
						p.setWorldMatrix(m, true);
					}
			}
			else
			{
				QPoint pos(it.current()->getPos());
				p.translate(pos.x(), pos.y());
			}

			it.current()->print(&p);
			
			p.restore();
		}
	
		p.restore();
		p.flush();
		
		y += pdmPrinter.height();
		if (y >= getSize().height() / PRINT_FACTOR)
		{
			x += pdmPrinter.width();
			if(x >= getSize().width() / PRINT_FACTOR)
			{
				// All pages printed
				break;
			}
			else
			{
				y = 0;
			}
		}
		pPrinter->newPage();
	}
	while(1);

	p.end();
}

/** Redraw the drawMap with all components, wires and selections
	updateDrawMap() doesn't update the widget. update() will do that.*/
void KSimEditor::updateDrawMap()
{
	Q_CHECK_PTR(drawMap);

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
		case EM_PAST_MOVE:
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
		select (selected.first(), false);

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
		lmbDown = true;
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
				if (g_library->getComponentLib()->createComponent(&newComp, getContainer(), m_insertCI->getLibName()))
				{
					KSimUndo * undo = getDoc()->getUndo();
					undo->begin(i18n("New Component"));
					getContainer()->addComponent(newComp);
					undo->end();
					getDoc()->setModified();
					setEditorMode(EM_INSERT_MOVE);

					if (newComp->isModule())
					{
						if (((Module*)newComp)->getModuleFile().isEmpty())
						{
							// Set module file name
							Q_ASSERT(!getApp()->getModuleFile().isEmpty());
						
							const ModuleInfo * mi =	ModuleData::makeModuleInfo(getApp()->getModuleFile());
							if (!mi)
							{
								// No valid module
								KMessageBox::error(getApp(),i18n("The file %1 contains no valid module!").arg(getApp()->getModuleFile()),
								                            i18n("Open Module"));

//								delete newComp;
								getContainer()->delComponent(newComp);
								undo->hiddenUndo();  // Remove last action from undo
								setEditorMode(EM_SELECT);
								lmbDown = false;
								break;
							}
							((ComponentLibrary*)g_library->getComponentLib())->insert(mi, 0);
							((Module*)newComp)->setModuleFile(getApp()->getModuleFile());
						}
					}
					
					newCompView = getSpecificCompView(newComp);
					
					// Position check
					QRect place(newCompView->getPlace());
					QPoint pos(newCompView->mapToGrid(QPoint(dragStart.x() - place.width()/2, dragStart.y() - place.height()/2)));
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
				KSimUndo * undo = getDoc()->getUndo();
				undo->begin(i18n("Copy Components"));
				getContainer()->pastComponent(&compList, pos);
				undo->end();
				unselectAll();
				getDoc()->setModified();
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

				dragNow = mousePos;
				setEditorMode(EM_PAST_MOVE);
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
					Q_ASSERT(0);
					break;
			}
		}
		else
		{
		
			switch(getContainer()->isCompViewHit(mousePos, viewList))
			{
				case SPECIAL_HIT:
					{
						QPainter p(drawMap);
						getContainer()->getFirstCompView()->mousePress(ev, &p);
					}
					break;
				
				default:
					break;
			}
		}
	}
}

static QString getComponentPartName(const Component * comp, const ConnectorBase * conn)
{
	if(conn)
	{
		return conn->getFullName();
	}
	if(comp)
	{
		return comp->getName();
	}
	return QString::null;
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
				break;
			
			case NORMAL_HIT:
			case WIRE_HIT:
			case SPECIAL_HIT:
			case COMP_RESIZE_F_HIT:
			case COMP_RESIZE_B_HIT:
				delayedStatusHelpMsg(getComponentPartName(getContainer()->getFirstCompView()->getComponent(),
				                                          (const ConnectorBase *) 0));
				break;
				
			case CONNECTOR_HIT:
			{
				delayedStatusHelpMsg(getComponentPartName(getContainer()->getFirstCompView()->getComponent(),
				                                          getContainer()->getFirstConnector()));
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
	
		QRect sheet(QPoint(0,0), size);
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

			// Falls Maus au�erhalb des Fensters, autoScroll aktivieren
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
 					if (selectArea.contains (it.current()->getPlace()) && !it.current()->isHidden())
						select(it.current(), TRUE);
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

// Wird vom Timer aufgerufen, wenn sich der Mauszeiger bei gedr�ckter
// Maustaste au�erhalb des Fensters befindet. Die Anzeigebereich wird
// ein St�ck in die Richtung verschoben, die scrollDir angibt.
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
				m_myCursor->set(KSimEditor::Cursor::crossCursor);
			break;
			
		case EM_RESIZE_MAP:
			if (   ((resizeDir ^ (RS_Left|RS_Up)) == 0)
				|| ((resizeDir ^ (RS_Right|RS_Down)) == 0) )
				m_myCursor->set(KSimEditor::Cursor::sizeFDiagCursor);
			else
				m_myCursor->set(KSimEditor::Cursor::sizeBDiagCursor);
			break;
			
		case EM_COMP_RESIZE_B:
				m_myCursor->set(KSimEditor::Cursor::sizeBDiagCursor);
			break;
		
		case EM_COMP_RESIZE_F:
				m_myCursor->set(KSimEditor::Cursor::sizeFDiagCursor);
			break;
		
		case EM_WIRE:
				m_myCursor->set(KSimEditor::Cursor::wireCursor);
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
						m_myCursor->set(KSimEditor::Cursor::sizeFDiagCursor);
					}
					else if ( ((myMousePos.x()<= RS_HANDLE_SIZE) && (myMousePos.y()>= size.height()-RS_HANDLE_SIZE))
					        ||((myMousePos.x()>= size.width()-RS_HANDLE_SIZE) && (myMousePos.y()<= RS_HANDLE_SIZE)))
					{
						m_myCursor->set(KSimEditor::Cursor::sizeBDiagCursor);
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
								m_myCursor->set(KSimEditor::Cursor::arrowCursor);
								break;
				
							case CONNECTOR_HIT:
							case WIRE_HIT:
								m_myCursor->set(KSimEditor::Cursor::wireCursor);
								break;
					
							case COMP_RESIZE_F_HIT:
								m_myCursor->set(KSimEditor::Cursor::sizeFDiagCursor);
								break;
					
							case COMP_RESIZE_B_HIT:
								m_myCursor->set(KSimEditor::Cursor::sizeBDiagCursor);
							break;
						}
					}
				}
				else
				{
					m_myCursor->set(KSimEditor::Cursor::arrowCursor);
				}
				break;
			}
	}
}




/**  */
void KSimEditor::refreshSimMode(CompViewList * sheetList, CompViewList * userList)
{
	QPainter p (drawMap);
	CompViewList cvList;
	
	if (EV_SHEETVIEW == editorView)
	{
		FOR_EACH_COMPVIEW(it, *sheetList)
		{
			if (it.current()->isViewChanged())
			{
				it.current()->setViewChanged(false);
				cvList.append(it.current());
			}
		}
	}
	else
	{
		FOR_EACH_COMPVIEW(it, *userList)
		{
			if (it.current()->isViewChanged())
			{
				it.current()->setViewChanged(false);
				cvList.append(it.current());
			}
		}
	}
	getContainer()->drawComponents(&p, &cvList);
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
				                        mapFromGlobal(((QWidget*)obj)->mapToGlobal(((QMouseEvent*)ev)->pos())),
				                        ((QMouseEvent*)ev)->button(),
				                        ((QMouseEvent*)ev)->state() );
				/*return*/ event(&mouseEv);  // FIX for KDE3
				return true;
			}
			break;
			
			case QEvent::KeyPress:
			case QEvent::KeyRelease:
			case QEvent::FocusIn:
			case QEvent::FocusOut:
			{
				/*return*/ event(ev);  // FIX for KDE3
				return true;
			}
			break;
			
			
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
	Q_CHECK_PTR(drawMap);*/

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
			getDoc()->setUserSize(size);
		}
		
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
	Q_CHECK_PTR(menu);
	int res;
	
	ComponentPopupMenu * compMenu = new ComponentPopupMenu(menu);
	connect(compMenu, SIGNAL(signalSelection(const ComponentInfo *)),
	        getApp(), SLOT(slotSelectCI(const ComponentInfo *)));

	getApp()->editDelete->plug(menu);
	getApp()->editCut->plug(menu);
	getApp()->editCopy->plug(menu);
	getApp()->editPaste->plug(menu);
	
	menu->insertSeparator();
	menu->insertItem(i18n("New component"), compMenu);

	res = menu->exec(QCursor::pos());
	
	delete menu;
}
	
/** Execute the component popup menu
	This menu is display, if one or more components are hit */
void KSimEditor::componentPopup(bool connectorHit)
{
	int idx, connIdx;
	int helpIdx = 0;
	int rot0Idx = 0;
	int rot90Idx = 0;
	int rot180Idx = 0;
	int rot270Idx = 0;
	int addWatchItemIdx = 0;
	KSimUndo * undo = getDoc()->getUndo();
	Q_CHECK_PTR(undo);
	QPopupMenu * menu = new QPopupMenu(0, "componentPopup");
	Q_CHECK_PTR(menu);
	int res;
	
	if ((!selected.containsRef(getContainer()->getFirstCompView())) || connectorHit)
	{
		// object is not selected
		// select it now
		unselectAll();
		select(getContainer()->getFirstCompView(), TRUE);
	}
	
	getApp()->editCut->plug(menu);
	getApp()->editCopy->plug(menu);
	getApp()->editPaste->plug(menu);
	getApp()->editDelete->plug(menu);
	
	if (selected.count()==1)
	{
		Component * comp = selected.first()->getComponent();
		CompView * compView = getSpecificCompView(comp);
		ConnectorBase * conn = (ConnectorBase *)0;
		
		
		// Init rotation
		if(compView && compView->isNormalRotationEnabled())
		{
			QPopupMenu * rotMenu = new QPopupMenu(menu, "RotationMenu");
			Q_CHECK_PTR(rotMenu);
			
			menu->insertSeparator();
			menu->insertItem(i18n("Ro&tation"), rotMenu);
			
			rot0Idx = rotMenu->insertItem(i18n("&0 Degree"));
			if ((compView->getRotation() < 45.0) || (compView->getRotation() >= 315.0))
			{
				rotMenu->setItemEnabled(rot0Idx, false);
			}
			
			rot90Idx = rotMenu->insertItem(i18n("&90 Degree"));
			if ((compView->getRotation() >= 45.0) && (compView->getRotation() < 135.0))
			{
				rotMenu->setItemEnabled(rot90Idx, false);
			}
			
			rot180Idx = rotMenu->insertItem(i18n("&180 Degree"));
			if ((compView->getRotation() >= 135.0) && (compView->getRotation() < 225.0))
			{
				rotMenu->setItemEnabled(rot180Idx, false);
			}
			
			rot270Idx = rotMenu->insertItem(i18n("&270 Degree"));
			if ((compView->getRotation() >= 225.0) && (compView->getRotation() < 315.0))
			{
				rotMenu->setItemEnabled(rot270Idx, false);
			}
		}
		
		if (connectorHit)
		{
			conn = getContainer()->getFirstConnector();
			conn->initPopupMenu(menu);
			menu->insertSeparator();
			addWatchItemIdx = menu->insertItem(i18n("&Add connector to watch view"));
		}
		
		comp->initPopupMenu(menu);

		menu->insertSeparator();
		idx = menu->insertItem(i18n("P&roperties"));
		connIdx = menu->insertItem(i18n("C&onnector Properties"));
		
		if (comp->getInfo()->getHTMLDescr() != QString::null)
		{
			menu->insertSeparator();
			helpIdx = menu->insertItem(i18n("&Help %1").arg(comp->getInfo()->getName()));
		}
		
		
		// Execute popup
		res = menu->exec(QCursor::pos());
		
		if (res == idx)
		{
			ComponentPropertyDialog::execute(comp, i18n("Component Properties"));
		}
		else if (res == connIdx)
		{
			ConnectorPropertyDialog::execute(comp->getConnList(), conn, i18n("Connector Properties"));
		}
		else if (res == rot0Idx)
		{
			undo->changeProperty(compView, i18n("Rotation 0 Degree"));
			compView->setRotation(0.0);
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
		else if (res == rot90Idx)
		{
			undo->changeProperty(compView, i18n("Rotation 90 Degree"));
			compView->setRotation(90.0);
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
		else if (res == rot180Idx)
		{
			undo->changeProperty(compView, i18n("Rotation 180 Degree"));
			compView->setRotation(180.0);
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
		else if (res == rot270Idx)
		{
			undo->changeProperty(compView, i18n("Rotation 270 Degree"));
			compView->setRotation(270.0);
			getDoc()->setModified();
			getContainer()->routeComponents();
			refresh();
		}
		else if (res == helpIdx)
		{
			const ComponentInfo * ci = comp->getInfo();
			const PackageInfo * pi = g_library->getComponentLib()->getPackageInfo(ci->getLibName());
			
			if (pi)
			{
				kapp->invokeHelp(ci->getHTMLDescr(), pi->getInstance()->instanceName());
			}
		}
		else if (res == addWatchItemIdx)
		{
			WatchItemBase * wi = conn->makeWatchItem();
			getApp()->getWatchWidget()->addWatchItem(wi);
		}

		// Not deleted ?
		if (getContainer()->getComponentList()->find(comp) != -1)
		{
			comp->menuExecuted();
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
	Q_CHECK_PTR(menu);
	KSimUndo * undo = getDoc()->getUndo();
	Q_CHECK_PTR(undo);
	Wire * wire = (Wire*)getContainer()->getFirstCompView()->getComponent();
	int res,idDelete, idTruncate, idProperty, addWatchItemIdx;
	WireProperty * wp = wire->getWireProperty();

	idDelete = menu->insertItem(i18n("&Delete wire"));
	idTruncate = menu->insertItem(i18n("&Truncate wire"));
	
	if (wire->getConnList()->count() <= 2)
	{
		// No truncate
		menu->setItemEnabled(idTruncate, false);
	}
		
	wire->initPopupMenu(menu);

	if (wp->inherits("WirePropertyInvalidBase"))
	{
		addWatchItemIdx = 0;
	}
	else
	{
		menu->insertSeparator();
		addWatchItemIdx = menu->insertItem(i18n("&Add wire to watch view"));
	}


	menu->insertSeparator();
	idProperty = menu->insertItem(i18n("P&roperties"));
	
	// Execute popup
	res = menu->exec(QCursor::pos());
	
	if(res == idDelete)
	{
		undo->begin(i18n("Delete Wire"));
		getContainer()->delComponent(wire);
		undo->end();
		getDoc()->setModified();
		getContainer()->routeComponents();
		refresh();
	}
	else if (res == idTruncate)
	{
		undo->begin(i18n("Truncate Wire"));
		getContainer()->truncateWire(wire, pos);
		undo->end();
		getDoc()->setModified();
		getContainer()->routeComponents();
		refresh();
	}
	else if (res == idProperty)
	{
		ComponentPropertyDialog::execute(wire, i18n("Component Properties"));
	}
	else if (res == addWatchItemIdx)
	{
		WireProperty * wp = wire->getWireProperty();

		if (wp)
		{
			getApp()->getWatchWidget()->addWatchItem(wp->makeWatchItem());
		}
		else
		{
			KMessageBox::sorry( getApp(),
			                    i18n("You can't add the wire to the watch view until \n"
			                         "the data type is not specified!\n"));
		}
	}
	else if (res == -1)
	{
		// Nothing
	}
	else
	{
		KSIMDEBUG_VAR("Unknown Result",res);
	}
	
	// Not deleted ?
	if (getContainer()->getComponentList()->find(wire) != -1)
	{
		wire->menuExecuted();
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

#include "ksimeditor.moc"

