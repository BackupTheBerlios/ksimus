/***************************************************************************
                          datarecorderwidget.cpp  -  description
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

// C-Includes

// QT-Includes
//#include <qpainter.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qvbox.h>

// KDE-Includes
#include <kdialogbase.h>
#include <klocale.h>

// KSimus-Includes
#include "ksimus/ksimdoublespinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorbase.h"

// Project-Includes
#include "datarecorder.h"
#include "datarecorderwidget.h"
#include "datarecorderdataview.h"
#include "datarecorderchannelbase.h"
#include "channelpositionwidget.h"
#include "widgetcontrolbutton.h"
#include "zoomwidget.h"

// Forward declaration



//#####################################################################################



KSimGridWidget::KSimGridWidget(QWidget * parent, const char * name)
	: QWidget(parent, name),
		m_widgetList(0),
		m_layout(0),
		m_horizontal(false),
		m_rowCols(1),
		m_autoUpdate(true),
		m_updateRequired(false)
{
}

KSimGridWidget::~KSimGridWidget()
{
	setAutoUpdate(false);
	if(m_widgetList)
	{
		m_widgetList->setAutoDelete(true);
		delete m_widgetList;
	}
	if(m_layout)
		delete m_layout;
}

void KSimGridWidget::addWidget(QWidget * wid)
{
	if (wid)
	{
		// Create variables
		if(!m_widgetList)
			m_widgetList = new QList<QWidget>;
			
		connect(wid,SIGNAL(destroyed()), this, SLOT(slotDelWidget()));
		m_widgetList->append(wid);
		updateGeometry();
		wid->show();
//		KSIMDEBUG_VAR("",m_widgetList->count());
			
	}
}

void KSimGridWidget::slotDelWidget()
{
	QWidget * wid = (QWidget *) sender();
	m_widgetList->removeRef(wid);
	KSIMDEBUG_VAR("",m_widgetList->count());
	updateGeometry();
}

void KSimGridWidget::updateGeometry()
{
	if (!m_autoUpdate)
	{
		m_updateRequired = true;
		return;
	}
	
	m_updateRequired = false;
	
	if(m_layout)
		delete m_layout;

	m_layout = new QGridLayout(this);
	CHECK_PTR(m_layout);
	
	unsigned int i;
	if (m_horizontal)
	{
		for (i=0; i<m_widgetList->count(); i++)
		{
			m_layout->addWidget(m_widgetList->at(i),i % m_rowCols,i / m_rowCols);
		}
	}
	else
	{
		for (i=0; i<m_widgetList->count(); i++)
		{
			m_layout->addWidget(m_widgetList->at(i),i / m_rowCols,i % m_rowCols);
		}
	}
	m_layout->activate();

}

void KSimGridWidget::setGeometry(bool horizontal, unsigned int rowCols)
{
	if (rowCols)
	{
		m_horizontal = horizontal;
		m_rowCols = rowCols;
		updateGeometry();
	}
	else
	{
		KSIMDEBUG("rowCols == 0 not allowed. Skipped.");
	}
}

void KSimGridWidget::setAutoUpdate(bool enable)
{
	m_autoUpdate = enable;
	if (m_autoUpdate && m_updateRequired)
		updateGeometry();	
}

//#####################################################################################

DataRecorderWidget::DataRecorderWidget(DataRecorder * recorder, QWidget *parent, const char *name )
	:	QWidget(parent,name),
		m_recorder(recorder)
{
	m_dataView = new DataRecorderDataView(this);
	CHECK_PTR(m_dataView); 	

	QVBox * box = new QVBox(this);
	CHECK_PTR(box); 	
	box->setMargin(KDialog::marginHint());
	box->setSpacing(KDialog::spacingHint());
	
	QLabel * lab = new QLabel("Channel:",box);
	CHECK_PTR(lab); 	
	m_buttonWidget = new KSimGridWidget (box);
	CHECK_PTR(m_buttonWidget); 	
	
	lab = new QLabel("&Zoom:",box);
	CHECK_PTR(lab); 	
	m_zoom = new ZoomWidget(m_dataView, box);
	CHECK_PTR(m_zoom);
	lab->setBuddy(m_zoom); 	
	connect(m_zoom, SIGNAL(changedSamplePixel(double)),m_dataView,SLOT(setZoomSample(double)));
	connect(m_zoom, SIGNAL(undoRequest()), getDataRecorder(), SLOT(undoZoom()));

	QHBoxLayout * horiLayout = new QHBoxLayout(this);
	CHECK_PTR(horiLayout);
	horiLayout->addWidget(box);
	horiLayout->addWidget(m_dataView,1);

	FOR_EACH_CHANNEL(it, *getDataRecorder()->getChannelList())
	{
		createChannelButton(it.current());
	}
	
	getDataRecorder()->getZoomVar().copyTo(*m_zoom);
}

DataRecorderWidget::~DataRecorderWidget()
{
//	KSIMDEBUG("DataRecorderWidget::~DataRecorderWidget()");
	emit signalDeleted();
}
	
DataRecorderDataView * DataRecorderWidget::getDataView() const
{
	return m_dataView;
}

void DataRecorderWidget::redraw()
{
//	update();
	m_dataView->viewport()->update();
}

KSimGridWidget * DataRecorderWidget::getButtonWidget()
{
	return m_buttonWidget;	
}

/** Creates a control button for the channel. */
void DataRecorderWidget::createChannelButton(DataRecorderChannelBase * channel)
{
	QWidget * wid = channel->getPropertyWidget(0);

	
  WidgetControlButton * but = new WidgetControlButton(wid,getButtonWidget());
  CHECK_PTR(but);
  getButtonWidget()->addWidget(but);


  but->setText(channel->getConnector()->getName());
	but->setColor(channel->getLineColor());
  connect(channel->getConnector(), SIGNAL(signalSetName(const QString &)), but, SLOT(setText(const QString &)));
  connect(channel, SIGNAL(lineColorChanged(const QColor &)), but, SLOT(setColor(const QColor &)));
}

