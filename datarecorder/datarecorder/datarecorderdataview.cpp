/***************************************************************************
                          datarecorderdataview.cpp  -  description
                             -------------------
    begin                : Thu May 24 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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
#include <limits.h>

// QT-Includes
#include <qpainter.h>
#include <qtimer.h>
#include <qpixmap.h>

// KDE-Includes

// Project-Includes
#include "datarecorderdataview.h"
#include "datarecorder.h"
#include "datarecorderchannelbase.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimeserver.h"

#include <ksimus/ksimunit.h>
// Forward declaration


// Debug switch
//#define VIEW_DEBUG_ENABLE


#ifdef VIEW_DEBUG_ENABLE
#define VIEW_DEBUG(str)					KSIMDEBUG(str)
#define VIEW_DEBUG_VAR(str,x)		KSIMDEBUG_VAR(str,x)
#else
#define VIEW_DEBUG(str)
#define VIEW_DEBUG_VAR(str,x)
#endif




#define RESIZE_DELAY	0
#define OVER_SCAN			2


class DataRecorderPixmap
{
public:
	DataRecorderPixmap(DataRecorderDataView * view, unsigned int overScan)
		:	m_view(view),
			m_overScan(overScan),
			m_pixmapOffset(0),
			m_lastHorizontalPosition(0)
	{};

//	~DataRecorderPixmap();
	QPixmap & getPixmap() { return m_map; };
	void setOffset(int offset) { m_pixmapOffset = offset; };
	int getOffset() const { return m_pixmapOffset; };
	int getWidth() const { return m_map.width(); };
	int getEndPos() const { return getOffset() + getWidth() - 1; };
	bool isPositionValid() const { return (m_view->m_horizontalOffset >= getOffset())
															&&(m_view->m_horizontalSize + m_view->m_horizontalOffset <= getEndPos());};
	void resize() { m_map.resize(m_view->m_horizontalSize * m_overScan, m_view->visibleHeight()); };
	void setHorizontalPosition(int lastPos) { m_lastHorizontalPosition = lastPos; };
	int getHorizontalPosition() const { return m_lastHorizontalPosition; };
	

private:
	DataRecorderDataView * m_view;
	int m_overScan;
	QPixmap m_map;
	int m_pixmapOffset;
	int m_lastHorizontalPosition;

};
















DataRecorderDataView::DataRecorderDataView(DataRecorderWidget *parent, const char *name)
	: QScrollView(parent,name,WRepaintNoErase | WResizeNoErase),
		m_horizontalOffset(0),
		m_horizontalSize(0),
		m_pixmapOffset(0),
		m_mapUpdateRequired(true),
		m_samplePerPixel(1.0),
		m_lastDrawnIndex(0),
		m_snapHorizontal(horizontalScrollBar()->lineStep())
{
	viewport()->setFocusPolicy(QWidget::WheelFocus);
	m_map = new DataRecorderPixmap(this, OVER_SCAN);
	m_horizontalOffset = contentsX();
	m_horizontalSize = visibleWidth();
	resizeContents(OVER_SCAN * visibleWidth(), visibleHeight());
	m_map->resize();
	setVScrollBarMode(QScrollView::AlwaysOff);
	setHScrollBarMode(QScrollView::AlwaysOn);
	viewport()->setBackgroundMode(NoBackground);
	
	m_resizeTimer = new QTimer(this);
	connect(m_resizeTimer,SIGNAL(timeout()),this,SLOT(delayedResizeEvent()));
//	connect(this, SIGNAL(contentsMoving(int,int)),this,SLOT(slotContentsMove()));

	m_itemList = new DataRecorderDataViewItemList;
	
	m_divGrid = new DataRecorderDataViewDivGrid(this,this);

	
	
	VIEW_DEBUG_VAR("",m_snapHorizontal);
};

DataRecorderDataView::~DataRecorderDataView()
{
	delete m_itemList;
	delete m_resizeTimer;
	delete m_map;
}

void DataRecorderDataView::reset()
{
	m_lastDrawnIndex = 0;
	setContentsPos(0, 0);
	resizeContents(OVER_SCAN * visibleWidth(), visibleHeight());
	m_mapUpdateRequired = true;
	completeUpdate();
}

void DataRecorderDataView::setZoomSample(double samplePerPixel)
{
	if (samplePerPixel > 0)
	{
		// Position in the middle of the view
		int middle = contentsX() + visibleHeight() / 2;
		middle = qRound(middle * m_samplePerPixel / samplePerPixel);
		middle -= visibleHeight() / 2;
		
		// set zoom
		m_samplePerPixel = samplePerPixel;
		
		// Set Scrollview width
		int completeHorizontalSize = qRound(getDataRecorder()->getDataCount() / m_samplePerPixel);
		resizeContents(completeHorizontalSize, visibleHeight());
		
		setContentsPos(middle,0);
		
		// Draw
		m_mapUpdateRequired = true;
		completeUpdate();
	}
}

double DataRecorderDataView::getZoomSample() const
{
	return m_samplePerPixel;
}

/** Returns the number of vertical divisions (grids).
	* Returns 1, if the view has no grid. */
int DataRecorderDataView::getVerticalDiv() const
{
	if (m_divGrid)
	{
		return m_divGrid->getVerticalDiv();
	}
	return 1;
}

int DataRecorderDataView::getHorizontalPixelPerDiv() const
{
	if (m_divGrid)
	{
		return m_divGrid->getHorizontalPixelPerDiv();
	}
	return m_horizontalSize;
}
		
void DataRecorderDataView::viewportResizeEvent(QResizeEvent * ev)
{
	QScrollView::viewportResizeEvent(ev);
	m_resizeTimer->start(RESIZE_DELAY, true);	
	m_mapUpdateRequired = true;
	VIEW_DEBUG("viewportResizeEvent");
}	

void DataRecorderDataView::viewportPaintEvent(QPaintEvent * ev)
{
	QScrollView::viewportPaintEvent(ev);
	m_resizeTimer->start(0, true);	
	VIEW_DEBUG("viewportPaintEvent");
}	

void DataRecorderDataView::viewportWheelEvent(QWheelEvent * ev)
{
	scrollBy(ev->delta() / 12, 0);
}


void DataRecorderDataView::slotContentsMove()
{
	
	m_mapUpdateRequired = true;
	VIEW_DEBUG("slotContentsMove");
}

void DataRecorderDataView::delayedResizeEvent()
{
	m_horizontalOffset = contentsX();
	m_horizontalSize = visibleWidth();
	m_map->resize();
	completeUpdate();
}


void DataRecorderDataView::completeUpdate(bool left)
{
	int completeHorizontalSize;

	if(m_mapUpdateRequired || !m_map->isPositionValid())
	{
		m_mapUpdateRequired = false;
		VIEW_DEBUG("completeUpdate UPDATE PIXMAP");
		
		//*** Optimal pixmap position ***
		completeHorizontalSize = qRound(getDataRecorder()->getDataCount() / m_samplePerPixel);
		
		if (left)
		{
			// Viewable area on the left edge
			m_map->setOffset(m_horizontalOffset);
		}
		else
		{
			// Viewable area in the middle of the pixmap
			m_map->setOffset(m_horizontalOffset - (OVER_SCAN - 1)*m_horizontalSize/2);
		
			// Limit right edge
			if(m_map->getOffset() > completeHorizontalSize - m_horizontalSize * OVER_SCAN)
			{
				m_map->setOffset(completeHorizontalSize - m_horizontalSize * OVER_SCAN);
			}
		}

		// Limit left edge
		if(m_map->getOffset() < 0)
		{
			m_map->setOffset(0);
		}
	
		// Blank map
		m_map->getPixmap().fill();
		
		// pre draws
		DataRecorderDataViewInfo viewInfo(m_map->getOffset(),
																			m_map->getWidth(),
																			visibleHeight(),
																			contentsX(),
																			visibleWidth(),
																			m_samplePerPixel);
		
		FOR_EACH_VIEWITEM(it, *getItemList())
		{
			it.current()->preDraw(&m_map->getPixmap(), viewInfo);
		}
		
		// Draw channels
		int start = qRound(m_map->getOffset() * m_samplePerPixel);
		int stop = qRound(((m_map->getOffset() + (m_horizontalSize * OVER_SCAN)) * m_samplePerPixel) + 1);
		int offset = qRound((start / m_samplePerPixel) - m_map->getOffset());
		
		drawViewArea( start, stop, offset );
		
//		VIEW_DEBUG_VAR("completeUpdate",m_horizontalOffset);
//		VIEW_DEBUG_VAR("completeUpdate",m_horizontalSize);
		// VIEW_DEBUG_VAR("completeUpdate",m_map->getOffset());
		// VIEW_DEBUG_VAR("completeUpdate",m_map->getEndPos());
	}
	
	bitBlt(viewport(),0,0,&m_map->getPixmap(), m_horizontalOffset - m_map->getOffset(), 0, visibleWidth(), visibleHeight(), CopyROP);

	
	
	// post draws
	DataRecorderDataViewInfo viewInfo(m_map->getOffset(),
																		m_map->getWidth(),
																		visibleHeight(),
																		contentsX(),
																		visibleWidth(),
																		m_samplePerPixel);

	FOR_EACH_VIEWITEM(it, *getItemList())
	{
		it.current()->postDraw(&m_map->getPixmap(), viewInfo);
	}
	
}

void DataRecorderDataView::newData()
{
	int completeHorizontalSize;
	int newOffset;
	bool completeDraw = false;
	bool partialDraw = false;
	bool nonScrolling = false;

	// Limit to int
	double horSize = getDataRecorder()->getDataCount() / m_samplePerPixel;
	if (horSize < (INT_MAX - OVER_SCAN * m_horizontalSize))
	{
		completeHorizontalSize = qRound(horSize);
	}
	else
	{
		completeHorizontalSize = INT_MAX - OVER_SCAN * m_horizontalSize;
	}
	
	// Set Scrollview width
	resizeContents(completeHorizontalSize, visibleHeight());

	//*** Calculate new Offset ***
	newOffset = QMAX(completeHorizontalSize - visibleWidth(),0);
	// Non scrolling situation?
	if ((completeHorizontalSize - contentsX() - visibleWidth()) > m_snapHorizontal)
	{
		nonScrolling = true;
	}
	
	//*** Set new Offset
	if(!nonScrolling)
	{
		setContentsPos(newOffset,0);
		m_horizontalOffset = contentsX();
	}	
	
	//*** Complete draw required ? ***
	if ((!m_map->isPositionValid() && !nonScrolling) || (m_lastDrawnIndex == 0))
	{
		completeDraw = true;
	}
	
	//*** Partial draw required ? ***
	if (completeHorizontalSize <= m_map->getEndPos())
	{
		partialDraw = true;
	}
	
	//*** Draw
	if(completeDraw)
	{
		VIEW_DEBUG("newData completeDraw");
		m_mapUpdateRequired = true;
		completeUpdate(true);
	}
	else if(partialDraw)
	{
		VIEW_DEBUG("newData partialDraw");
		drawViewArea( m_lastDrawnIndex,
								 	getDataRecorder()->getDataCount()-1,
								 	qRound((m_lastDrawnIndex / m_samplePerPixel) - m_map->getOffset()));
	}
	else
	{
		VIEW_DEBUG("newData no draw");
	}
		
	m_lastDrawnIndex = getDataRecorder()->getDataCount() - 1;		
}

void DataRecorderDataView::drawViewArea(int startSample, int stopSample, int offset)
{
//	KSIMDEBUG(QString("start %1;stop %2; offset %3").arg(startSample).arg(stopSample).arg(offset));
	
	FOR_EACH_CHANNEL(it, *getDataRecorder()->getChannelList())
	{
		it.current()->drawData(&m_map->getPixmap(), startSample, stopSample,
													 offset, visibleHeight(), m_samplePerPixel, getVerticalDiv());
	}
}

/** Updates the complete view immediatly. */
void DataRecorderDataView::updateNow()
{
	m_mapUpdateRequired = true;
	completeUpdate();
}
	
/** Next view update is a complete update instead a (possible) partial update. */
void DataRecorderDataView::updateNext()
{
	m_mapUpdateRequired = true;
}

//#############################################################################
//#############################################################################

DataRecorderDataViewItem::DataRecorderDataViewItem(DataRecorderDataView * dataView, QObject * parent, const char *name)
	: QObject(parent,name),
		m_dataView(dataView)
{
	if(getDataView())
	{
		getDataView()->getItemList()->append(this);
	}
}

DataRecorderDataViewItem::~DataRecorderDataViewItem()
{
	if(getDataView())
	{
		getDataView()->getItemList()->removeRef(this);
	}
}
	

void DataRecorderDataViewItem::preDraw(QPaintDevice * , const DataRecorderDataViewInfo & )
{
}	

void DataRecorderDataViewItem::postDraw(QPaintDevice * , const DataRecorderDataViewInfo & )
{
}


//#############################################################################
//#############################################################################

DataRecorderDataViewDivGrid::DataRecorderDataViewDivGrid(DataRecorderDataView * dataView, QObject * parent, const char *name)
	:	DataRecorderDataViewItem(dataView, parent, name),
		m_verticalDiv(DEFAULT_VERTICAL_DIV),
		m_horizontalPixelPerDiv(DEFAULT_HORIZONTAL_PIXEL_PER_DIV),
		m_timeStampEna(true),
		m_pixelSpace(5)
{
}

void DataRecorderDataViewDivGrid::preDraw(QPaintDevice * paintDev, const DataRecorderDataViewInfo & info)
{
	int start;
	int stop;
	int interval;
	int i,j;
	int height;
	int offset;
	bool drawTimeStamp = isTimeStampEnabled();
	
	QPainter paint(paintDev);
	paint.setPen(QPen(lightGray, 0, SolidLine));
	
	start = getHorizontalPixelPerDiv() - (info.getPixmapOffset() % getHorizontalPixelPerDiv());
	if (start == getHorizontalPixelPerDiv()) start = 0;
	offset = info.getPixmapOffset();
	stop = info.getPixmapWidth();
	height = paint.fontMetrics().height();
	
	// Dont draw time stamp, if not fit into 1 horizontal division
	if (height > (info.getPixmapHeight() / getVerticalDiv()))
	{
		drawTimeStamp = false;
	}
	
/*	KSIMDEBUG_VAR("",info.getPixmapOffset());
	KSIMDEBUG_VAR("",getHorizontalPixelPerDiv());
	KSIMDEBUG_VAR("",start);
	KSIMDEBUG_VAR("",offset);
	KSIMDEBUG_VAR("",stop);
	KSIMDEBUG_VAR("",height);*/
	
	
	for (i = start; i <= stop; i += getHorizontalPixelPerDiv())
	{
		if (!drawTimeStamp || (((i+offset) / getHorizontalPixelPerDiv()) & 1))
		{
			// Draw only a line
//			paint.drawLine(i,0,i,info.getPixmapHeight());
			for(j=0; j < info.getPixmapHeight(); j += m_pixelSpace)
			{
				paint.drawPoint(i,j);
			}
		}
		else
		{
			// Draw line and time stamp
//			paint.drawLine(i,0,i,info.getPixmapHeight()-height);
			for(j=0; j < info.getPixmapHeight()-height; j += m_pixelSpace)
			{
				paint.drawPoint(i,j);
			}
			
			double time = (double)(i+offset)*info.getSamplePerPixel() * getDataRecorder()->getTimeServer().getTick().getValue(unit_sec);
			const KSimUnitBase * unit = KSimUnitListStatic::getTimeUnitList().findPrefered(time);
			
			QString str = "%1 %2";
			str = str.arg(unit->toUnit(time)).arg(unit->getUnitString());
			
			paint.drawText(i-100,info.getPixmapHeight()-height,200,height,
										AlignCenter, str);
		}
	}

	// Horizontal lines
	for (i = 1; i < getVerticalDiv(); i++)
	{
		interval = info.getPixmapHeight() * i / getVerticalDiv();
//		paint.drawLine(0,interval,info.getPixmapWidth(),interval);
		for(j=0; j < info.getPixmapWidth(); j += m_pixelSpace)
		{
			paint.drawPoint(j,interval);
		}
	
	}

}
	

void DataRecorderDataViewDivGrid::setVerticalDiv(int divNo)
{
	if(divNo > 0)
	{
		m_verticalDiv = divNo;
	}
	else
	{
		KSIMDEBUG_VAR("DataRecorderDataViewDivGrid::setVerticalDiv out of range", divNo);
	}
}

void DataRecorderDataViewDivGrid::setHorizontalPixelPerDiv(int pixel)
{
	if(pixel > 1)
	{
		m_horizontalPixelPerDiv = pixel;
	}
	else
	{
		KSIMDEBUG_VAR("DataRecorderDataViewDivGrid::setHorizontalPixelPerDiv out of range", m_horizontalPixelPerDiv);
	}
}


void DataRecorderDataViewDivGrid::setTimeStampEnabled(bool ena)
{
	m_timeStampEna = ena;	
}

//#############################################################################
//#############################################################################





