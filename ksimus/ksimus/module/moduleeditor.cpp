/***************************************************************************
                          moduleeditor.cpp  -  description
                             -------------------
    begin                : Fri Jul 21 2000
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
#include <qpainter.h>
#include <qpixmap.h>
#include <qfontmetrics.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdebug.h"
#include "moduleeditor.h"
#include "module.h"
#include "moduledata.h"
#include "moduledialog.h"
#include "connectorbase.h"
#include "resource.h"
#include "displaywidget.h"
#include "externalconnector.h"

// Forward declaration


#define mdata (m_dialog->mdata)

ModuleEditor::ModuleEditor(ModuleDialog * dialog, QWidget * parent, const char *name)
	:	QWidget(parent,name),
		m_dialog (dialog),
		m_userView(0)
	
{
  	
	setBackgroundMode (NoBackground);
	m_drawMap = new QPixmap;
	CHECK_PTR(m_drawMap);
	updateDrawMap();
}
ModuleEditor::~ModuleEditor()
{
	delete m_drawMap;
	delete m_userView;
}


void ModuleEditor::mousePressEvent(QMouseEvent * ev)
{
	if (ev->button() & LeftButton)
	{
		m_lmbDown = true;
		m_rmbDown = false;
	}
	else if (ev->button() & RightButton)
	{
		m_lmbDown = false;
		m_rmbDown = true;
	}
	
	connMouseEvent(ev);
}
void ModuleEditor::mouseMoveEvent(QMouseEvent * ev)
{
	connMouseEvent(ev);
}
void ModuleEditor::mouseReleaseEvent(QMouseEvent * ev)
{
	connMouseEvent(ev);
}

void ModuleEditor::connMouseEvent(QMouseEvent * ev)
{
	ConnOrientationType orientation = CO_LEFT; // set because gcc warning
	bool found = false;
	
	if (m_leftArea.contains(ev->pos()))
	{
		found = true;
//		orientation = CON_LEFT;
	}
	else if (m_rightArea.contains(ev->pos()))
	{
		found = true;
		orientation = CO_RIGHT;
	}
	else if (m_topArea.contains(ev->pos()))
	{
		found = true;
		orientation = CO_TOP;
	}
	else if (m_bottomArea.contains(ev->pos()))
	{
		found = true;
		orientation = CO_BOTTOM;
	}
		
	if (found)
	{
		QPoint pos;
		pos.setX((ev->pos().x()-m_viewOffsetX)/gridX);
		pos.setY((ev->pos().y()-m_viewOffsetY)/gridY);

		if (m_lmbDown)
			m_dialog->posSelected(pos, true, orientation);
		else if (m_rmbDown)
			m_dialog->posSelected(pos, false, orientation);
	}
}

void ModuleEditor::paintEvent(QPaintEvent *)
{
	if (m_updateRequired)
	{
		m_updateRequired = false;
		redrawMap();
	}
	bitBlt(this, 0, 0, m_drawMap);
}	

void ModuleEditor::drawDragRect ()
{
	QPainter p (m_drawMap);

   	update();
}


/** Redraw the m_drawMap next paintEvent */
void ModuleEditor::updateDrawMap()
{
	m_updateRequired = true;
}

/** Redraw the m_drawMap immediatly */
void ModuleEditor::updateDrawMapNow()
{
	m_updateRequired = true;
	update();
}

/** redraw the module contents */
void ModuleEditor::redrawMap()
{
	CHECK_PTR(m_drawMap);

	switch(mdata->getModuleView())
	{
		case MV_NONE:
			if(m_userView)
			{
				delete m_userView;
				m_userView = 0;
			}
			updateDrawMapMessage(i18n("No view !"));
			break;
					
		case MV_GENERIC:
		{
			if(m_userView)
			{
				delete m_userView;
				m_userView = 0;
			}
			// calculate view layout
			calGeometric(mdata->getGenericSize());
			resize(m_viewSize);
			m_drawMap->resize(m_viewSize);
			m_drawMap->fill();			
			QPainter p (m_drawMap);
			QRect rect(QPoint(m_viewOffsetX,m_viewOffsetY), mdata->getGenericSize());
			ModuleSV::drawGeneric(&p, rect);
			drawConn(&p);
			break;
		}
		
		case MV_PIXMAP:
		{
			if(m_userView)
			{
				delete m_userView;
				m_userView = 0;
			}
			updateDrawMapPixmapView();
			break;
		}
		
		case MV_USERVIEW:
		{
			updateDrawMapUserView();
			break;
		}
		
		default:
			KSIMDEBUG_VAR("unknown viewType", (int)mdata->getModuleView());
			break;
	}
}
						
/** Setup the m_drawMap for the pixmap view */
void ModuleEditor::updateDrawMapPixmapView()
{
	if(mdata->isPixmapFileValid())
	{
		QSize pixSize = mdata->getPixmapSize();
	
		// calculate view layout
		calGeometric(pixSize);
							
		resize(m_viewSize);
		m_drawMap->resize(m_viewSize);
		m_drawMap->fill();			
		QPainter p (m_drawMap);
		p.drawPixmap(m_viewOffsetX+gridX, m_viewOffsetY+gridY, *(mdata->getPixmap()));
		drawConn(&p);
	}
	else
	{
		updateDrawMapMessage(i18n("Image file not valid !"));
	}
}	

/** Setup the m_drawMap for the user view */
void ModuleEditor::updateDrawMapUserView()
{
	QSize userViewSize = mdata->getUserViewSize();
	CompViewList * viewList = mdata->getUserViewList();
	
	// calculate view layout
    calGeometric(userViewSize + QSize(2*gridX, 2*gridY));
	
	if (!m_userView)
	{
		m_userView = new DisplayWidget(viewList, this);
		CHECK_PTR(m_userView);
	}
	
	m_userView->move(m_viewOffsetX+gridX, m_viewOffsetY+gridY);
	m_userView->setBackgroundMode(NoBackground);
	m_userView->resize( userViewSize );
	m_userView->show();
							
	resize(m_viewSize);
	m_drawMap->resize(m_viewSize);
	m_drawMap->fill();
	
	FOR_EACH_COMPVIEW(it, *viewList)
	{
		it.current()->makeWidget(m_userView);
	}
	QPainter p (m_drawMap);
	drawConn(&p);
}	

/** Setup the m_drawMap for the message view */
void ModuleEditor::updateDrawMapMessage(const QString & message)
{
	int width = fontMetrics().boundingRect(message).width();
	
	QSize size(width,width);
	
	// calculate view layout
	calGeometric(size);
							
	resize(m_viewSize);
	m_drawMap->resize(m_viewSize);
	m_drawMap->fill();			
	QPainter p (m_drawMap);
	p.drawText(rect(), AlignCenter, message);
}	

/** Draw the Connectors
	Called from updateDrawMap() */
void ModuleEditor::drawConn(QPainter *p)
{
	unsigned int i = 0;
	ExternalConnector * extConn /* = (ExternalConnector*)mdata->getExternalList()->at(i)*/;
	ConnOrientationType orient;
	QPoint pos;
	
	drawConnArea(p, m_topArea);
	drawConnArea(p, m_bottomArea);
	drawConnArea(p, m_leftArea);
	drawConnArea(p, m_rightArea);
	
	for(i=0; i< mdata->getExternalList()->count(); i++)
	{
		extConn = (ExternalConnector*)mdata->getExternalList()->at(i);
		
		switch (mdata->getModuleView())
		{
			default:
				KSIMDEBUG_VAR("unknown viewType", (int)mdata->getModuleView());
			case MV_GENERIC:
				orient = extConn->getExternalConn()->getOrientation();
				pos = *mdata->getGenericConnPos()->at(i);
				break;
			
			case MV_PIXMAP:
				orient = extConn->getPixmapOrientation();
				pos = extConn->getPixmapPos();
				break;
			
			case MV_USERVIEW:
				orient = extConn->getUserViewOrientation();
				pos = extConn->getUserViewPos();
				break;
		}
		// Connector positioned
		if (pos.x() != -1)
		{
			if (m_dialog->selectedConn == extConn)
			{
				// selected connector
				p->save();
				p->setPen(QPen(black,2));
				p->drawRect(pos.x()*gridX+m_viewOffsetX-1,
				            pos.y()*gridY+m_viewOffsetY-1,
				            gridX+2,gridY+2);
				p->restore();
			}
			extConn->getExternalConn()->draw(p, orient,
			              pos.x()*gridX+gridX/2+m_viewOffsetX,
			              pos.y()*gridY+gridY/2+m_viewOffsetY);
		}
	}
}

/** Draw a connectors area */
void ModuleEditor::drawConnArea(QPainter *p, const QRect & area)
{
	if (!area.isNull())
	{
		p->save();
		p->setPen(QPen(lightGray));
		int x = area.left();
		int y = area.top();
		
		if (area.width()>area.height())
		{
			//horizontal
			while (x < area.right())
			{
				p->drawRect(x,y,gridX,gridY);
				x += gridX;
			}
		}
		else
		{
			//vertical
			while (y < area.bottom())
			{
				p->drawRect(x,y,gridX,gridY);
				y += gridY;
			}
		}
		p->restore();
	}
}

/** Calculate window size and module offset
	@see m_viewSize,m_viewOffsetX,m_viewOffsetY
*/
void ModuleEditor::calGeometric(const QSize & moduleSize)
{
	QWidget * viewport = parentWidget();
	QWidget * scrollView = viewport->parentWidget();
	
	QSize winSize = scrollView->size();
	
	// Set widget size
	m_viewSize.setWidth (QMAX(winSize.width() ,moduleSize.width()  + 4*gridY));
	m_viewSize.setHeight(QMAX(winSize.height(),moduleSize.height() + 4*gridX));
	
	// Set module offset
	m_viewOffsetY = (m_viewSize.height() - moduleSize.height())/2;
	m_viewOffsetX = (m_viewSize.width()  - moduleSize.width() )/2;

	// Calculate connector areas
	
	m_topArea =    QRect(m_viewOffsetX+gridX, m_viewOffsetY,
	                     moduleSize.width()-2*gridX, gridY);
	m_bottomArea = QRect(m_viewOffsetX+gridX, m_viewOffsetY + moduleSize.height()-gridY,
	                     moduleSize.width()-2*gridX, gridY);
	m_leftArea =   QRect(m_viewOffsetX, m_viewOffsetY+gridY,
	                     gridX, moduleSize.height()-2*gridY);
	m_rightArea =  QRect(m_viewOffsetX + moduleSize.width()-gridX, m_viewOffsetY+gridY,
	                     gridX, moduleSize.height()-2*gridY);
}


