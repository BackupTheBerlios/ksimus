/***************************************************************************
                          datarecorderdataview.h  -  description
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

#ifndef DATARECORDERDATAVIEW_H
#define DATARECORDERDATAVIEW_H

// C-Includes

// QT-Includes
#include <qscrollview.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "datarecorderwidget.h"

// Forward declaration
class QPixmap;
class QTimer;

namespace KSimLibDataRecorder
{

// Forward declaration (namespace)
class DataRecorder;
class DataRecorderWidget;
class DataRecorderPixmap;
class DataRecorderDataViewItem;
class DataRecorderDataViewItemList;
class DataRecorderDataViewDivGrid;

/**Data aera of the data recorder view.
  *@author Rasmus Diekenbrock
  */

class DataRecorderDataViewInfo
{

public:
	DataRecorderDataViewInfo(	int pixmapOffset,
														int pixmapWidth,
														int pixmapHeight,
														int viewOffset,
														int viewWidth,
														double samplePerPixel)
	:	m_pixmapOffset(pixmapOffset),
		m_pixmapWidth(pixmapWidth),
		m_pixmapHeight(pixmapHeight),
		m_viewOffset(viewOffset),
		m_viewWidth(viewWidth),
		m_samplePerPixel(samplePerPixel)
	{};
	~DataRecorderDataViewInfo() {};
	
	int getPixmapOffset() const { return m_pixmapOffset; };
	int getPixmapWidth() const { return m_pixmapWidth; };
	int getPixmapHeight() const { return m_pixmapHeight; };
	int getViewOffset() const { return m_viewOffset; };
	int getViewWidth() const { return m_viewWidth; };
	double getSamplePerPixel() const { return m_samplePerPixel; };


private:
	int m_pixmapOffset;
	int m_pixmapWidth;
	int m_pixmapHeight;
	int m_viewOffset;
	int m_viewWidth;
	double m_samplePerPixel;
};

//#############################################################################
//#############################################################################

class DataRecorderDataView : public QScrollView
{
friend class DataRecorderPixmap;

	Q_OBJECT

public:
	DataRecorderDataView(DataRecorderWidget * parent, const char *name=0);
	~DataRecorderDataView();
	
	DataRecorderWidget * getDataRecorderWidget() const { return (DataRecorderWidget *)parent(); };
	DataRecorder * getDataRecorder() const { return getDataRecorderWidget()->getDataRecorder(); };

	/** Resets view */
	void reset();
	double getZoomSample() const;
	DataRecorderDataViewItemList * getItemList() { return m_itemList; };

	/** Returns the number of vertical divisions (grids).
		* Returns 1, if the view has no grid. */
	int getVerticalDiv() const;
	/** Returns the number ofhorizonatl pixels per div.
			Returns the width if no grid is present. */
	int getHorizontalPixelPerDiv() const;

public slots:
	void newData();
	void setZoomSample(double samplePerPixel);
	/** Updates the complete view immediatly. */
	void updateNow();
	/** Next view update is a complete update instead a (possible) partial update. */
	void updateNext();
	

protected:
	void completeUpdate(bool left = false);
	virtual void viewportResizeEvent(QResizeEvent * ev);
	virtual void viewportPaintEvent(QPaintEvent * ev);
	virtual void viewportWheelEvent(QWheelEvent * ev);
	void drawViewArea(int startSample, int stopSample, int offset);
	
	DataRecorderPixmap * m_map;
	QTimer * m_resizeTimer;
	int m_horizontalOffset;
	int m_horizontalSize;
	int m_pixmapOffset;
	bool m_mapUpdateRequired;
	double m_samplePerPixel;		// Zoom
	int m_lastDrawnIndex;
	int m_snapHorizontal;
	DataRecorderDataViewItemList * m_itemList;
	DataRecorderDataViewDivGrid * m_divGrid;

protected slots:
	void delayedResizeEvent();
	void slotContentsMove();

};

//#############################################################################
//#############################################################################


class DataRecorderDataViewItem : public QObject
{
	Q_OBJECT

public:
	DataRecorderDataViewItem(DataRecorderDataView * dataView, QObject * parent, const char *name=0);
	~DataRecorderDataViewItem();
	

	virtual void preDraw(QPaintDevice * paintDev, const DataRecorderDataViewInfo & info);	
	virtual void postDraw(QPaintDevice * paintDev, const DataRecorderDataViewInfo & info);	

	DataRecorderDataView * getDataView() const { return m_dataView; };
	DataRecorder * getDataRecorder() const { return m_dataView->getDataRecorder(); };

private:
	DataRecorderDataView * m_dataView;

};

//#############################################################################
//#############################################################################


class DataRecorderDataViewDivGrid : public DataRecorderDataViewItem
{
#define DEFAULT_VERTICAL_DIV	8
#define DEFAULT_HORIZONTAL_PIXEL_PER_DIV 50
	
	Q_OBJECT

public:
	DataRecorderDataViewDivGrid(DataRecorderDataView * dataView, QObject * parent, const char *name=0);
//	~DataRecorderDataViewDivGrid();

	virtual void preDraw(QPaintDevice * paintDev, const DataRecorderDataViewInfo & info);	
	
	int getVerticalDiv() const { return m_verticalDiv; };
	int getHorizontalPixelPerDiv() const { return m_horizontalPixelPerDiv; };
	bool isTimeStampEnabled() const { return m_timeStampEna; };



public slots:
	void setVerticalDiv(int divNo);
	void setHorizontalPixelPerDiv(int pixel);
	void setTimeStampEnabled(bool ena);


private:
	int m_verticalDiv;
	int m_horizontalPixelPerDiv;
	bool m_timeStampEna;
	int m_pixelSpace;

};	
	
//#############################################################################
//#############################################################################



class DataRecorderDataViewItemList : public QPtrList<DataRecorderDataViewItem>
{
};

#define FOR_EACH_VIEWITEM(_it_,_itemList_)	\
		for(QPtrListIterator<DataRecorderDataViewItem> _it_(_itemList_);_it_.current();++_it_)




};  //namespace KSimLibDataRecorder


#endif
