/***************************************************************************
                          datarecorderwidget.h  -  description
                             -------------------
    begin                : Fri May 18 2001
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

#ifndef DATARECORDERWIDGET_H
#define DATARECORDERWIDGET_H

// C-Includes

// QT-Includes
#include <qwidget.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes

// Forward declaration


namespace KSimLibDataRecorder
{

// Forward declaration (namespace)
class DataRecorder;
class DataRecorderDataView;
class DataRecorderChannelBase;
class KSimGridWidget;
class ZoomWidget;


/**Widget class for the data recorder.
  *@author Rasmus Diekenbrock
  */




class DataRecorderWidget : public QWidget
{
   Q_OBJECT
public: 
	DataRecorderWidget(DataRecorder * recorder, QWidget *parent=0, const char *name=0);
	virtual ~DataRecorderWidget();

	DataRecorderDataView * getDataView() const;	
	DataRecorder * getDataRecorder() { return m_recorder; };
	const DataRecorder * getDataRecorder() const { return m_recorder; };
	KSimGridWidget * getButtonWidget();
	/** Creates a control button for the channel. */
	void createChannelButton(DataRecorderChannelBase * channel);

	ZoomWidget * getZoomWidget() { return m_zoom;};
	const ZoomWidget * getZoomWidget() const { return m_zoom;};

public slots:
	void redraw();

private:
	DataRecorder * m_recorder;
	DataRecorderDataView * m_dataView;
	KSimGridWidget * m_buttonWidget;
	ZoomWidget * m_zoom;
	
signals:
	void signalDeleted();
};


//#####################################################################################
//#####################################################################################


class KSimGridWidget : public QWidget
{
Q_OBJECT
public:
	KSimGridWidget(QWidget * parent = 0, const char * name = 0);
  ~KSimGridWidget();


  void addWidget(QWidget * wid);
  void setGeometry(bool horizontal, unsigned int rowCols = 1);
  void setAutoUpdate(bool enable);


private:
	void updateGeometry();
	QList<QWidget> * m_widgetList;
	QGridLayout * m_layout;
	bool m_horizontal;
	unsigned int m_rowCols;
	bool m_autoUpdate;
	bool m_updateRequired;
	
	
private slots:
	void slotDelWidget();


};


//#####################################################################################
//#####################################################################################



};  //namespace KSimLibDataRecorder

#endif
