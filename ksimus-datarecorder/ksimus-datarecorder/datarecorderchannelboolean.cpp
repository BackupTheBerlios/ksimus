/***************************************************************************
                          datarecorderchannelboolean.cpp  -  description
                             -------------------
    begin                : Sun Aug 19 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qpoint.h>
#include <qlist.h>
#include <qlabel.h>
#include <qvbox.h>

// KDE-Includes
#include <kdialogbase.h>
#include <kcolorbtn.h>
#include <klocale.h>

// KSIMUS-Includes
#include "ksimus/ksimdata.h"
#include <ksimus/ksimlineedit.h>
#include "ksimus/connectorboolin.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "datarecorder.h"
#include "datarecorderchannelboolean.h"
#include "channelpositionwidget.h"

// Forward declaration


namespace KSimLibDataRecorder
{

static int counter = 1;




//#############################################################################
//#############################################################################

class BoolStorage
{
	#define BITS_PER_BYTE		8
	#define INTS_PER_ARRAY	16
	#define BITS_PER_INT		(BITS_PER_BYTE * sizeof(unsigned int))
	#define BITS_PER_TDATA	(BITS_PER_INT * INTS_PER_ARRAY)
	typedef struct
	{
		unsigned int array[INTS_PER_ARRAY];
	}tData;

public:
	BoolStorage()
		:	m_count(0)
	{
		m_data.setAutoDelete(true);
	
	};
	~BoolStorage()
	{};
	
	void append(bool data)
	{
		int index = m_count / BITS_PER_TDATA;
		int arrayOffset = (m_count % BITS_PER_TDATA) / BITS_PER_INT;
		int uintOffset = m_count % BITS_PER_INT;

		if((arrayOffset == 0) && (uintOffset == 0))
		{
			tData * newData = new tData;
			for (int i = 0; i < INTS_PER_ARRAY; i++)
				newData->array[i] = 0;
			m_data.append(newData);
		}
		if(data)
			m_data.at(index)->array[arrayOffset] |= 1 << uintOffset;
		m_count++;
	};
			
	bool get(int number)
	{
  	if(number >= m_count) return false;
  	
		int index = number / BITS_PER_TDATA;
		int arrayOffset = (number % BITS_PER_TDATA) / BITS_PER_INT;
		int uintOffset = number % BITS_PER_INT;

 		return (m_data.at(index)->array[arrayOffset] & (1 << uintOffset));
 	}
		
				
	int count() const { return m_count; };
	
	void clear() { m_data.clear();
								 m_count = 0; };
	
private:
	int m_count;
	QList<tData> m_data;
	
};



//#############################################################################
//#############################################################################

DataRecorderChannelBoolean::DataRecorderChannelBoolean(DataRecorder * recorder)
	:	DataRecorderChannelBase(recorder),
		m_propertyWidget(0)
{
	setChannelType(CT_Boolean);
	m_input = new ConnectorBoolIn(recorder,QString::fromLatin1("Boolean"),
	                             i18n("DataRecorder-Connector", "Boolean"),
	                             QPoint());
	m_input->setErasable(true);
	m_input->getAction().disable(KSimAction::STORAGE);
	connect(m_input, SIGNAL(signalDeleteRequest(ConnectorBase *)),
	        recorder, SLOT(slotRemoveChannelConn(ConnectorBase *)));
	m_data = new BoolStorage();
	
	counter++;
	
	if (counter > 15) counter = 1;

	setVerticalGain(1);
	setVerticalOffset(0.5 * counter);
	
}

DataRecorderChannelBoolean::~DataRecorderChannelBoolean()
{
	if (m_propertyWidget)
		delete m_propertyWidget;
	delete m_data;
//	delete m_input;
}

void DataRecorderChannelBoolean::fetchData()
{
	bool data = ((ConnectorBoolIn*)getConnector())->getInput();
	
	m_data->append(data);
}
	
bool DataRecorderChannelBoolean::getData(unsigned int number)
{
	return m_data->get(number);
}	

/** Resets channel. Delete data. */
void DataRecorderChannelBoolean::reset()
{
	m_data->clear();
}

int DataRecorderChannelBoolean::drawData(QPaintDevice * paintDev,
											 int startSample, int stopSample,
											 int horizontalOffset, int height,
											 double samplePerPixel, int verticalDivs)
{
	int index = startSample;
	int counter = 0;
	int horiPos = horizontalOffset;
	int lastHoriPos = horizontalOffset;
	int lastVertPos, vertPos;
	QPainter painter(paintDev);
	painter.setPen(getLineColor());
	
	int verticalOffset = qRound((getVerticalOffset() * height) / verticalDivs);
	int verticalGain = qRound((getVerticalGain() * height) / verticalDivs);

	if (index >= m_data->count())
		return m_data->count();	// Nothing to draw
		
	if (stopSample >= m_data->count())
		stopSample = m_data->count()-1; // Limit samples
		
	lastVertPos = height - (verticalGain * getData(index) + verticalOffset);
	
	while(index <= stopSample)
	{
		vertPos = height - (verticalGain * getData(index) + verticalOffset);
		if(lastVertPos != vertPos)
		{
			painter.drawLine(lastHoriPos, lastVertPos, horiPos, lastVertPos);
			painter.drawLine(horiPos, lastVertPos, horiPos, vertPos);
			lastVertPos = vertPos;
			lastHoriPos = horiPos;
		}
		horiPos = horizontalOffset + qRound(counter / samplePerPixel);
		index++;
		counter++;
	}
	// Draw final line
	painter.drawLine(lastHoriPos, lastVertPos, horiPos, lastVertPos);
	return index-1;
}

QWidget * DataRecorderChannelBoolean::getPropertyWidget(QWidget * parent, const char * name)
{
	if (!m_propertyWidget)
	{
		// Setup widget
		QVBox * widget = new QVBox(parent,name);
		CHECK_PTR(widget);
		widget->setMargin(KDialog::marginHint());
		widget->setSpacing(KDialog::spacingHint());
		
		QLabel * lab;
		lab = new QLabel(i18n("Channel:"), widget);
		CHECK_PTR(lab);
		
		KSimLineEdit * conName;
		conName = new KSimLineEdit(widget);
		CHECK_PTR(conName);
		conName->setText(getConnector()->getName());
	  connect(getConnector(), SIGNAL(signalSetName(const QString &)), conName, SLOT(setText(const QString &)));
	  connect(conName, SIGNAL(changed(const QString &)), SLOT(setChannelName(const QString &)));
		
		ChannelPositionWidget * pos;
		pos = new ChannelPositionWidget(this, widget);
		CHECK_PTR(pos);
		pos->setGainValue(getVerticalGain());
		pos->setOffsetValue(getVerticalOffset());
		connect(pos, SIGNAL(gainChanged(double)), SLOT(setVerticalGain(double)));
		connect(pos, SIGNAL(offsetChanged(double)), SLOT(setVerticalOffset(double)));
		
		
		KColorButton * color = new KColorButton(widget);
		CHECK_PTR(color);
		color->setColor(getLineColor());
		connect(color, SIGNAL(changed(const QColor &)), SLOT(setLineColor(const QColor &)));
		
		
		m_propertyWidget = widget;
	}
	else
	{
		if (parent)
		{
			KSIMDEBUG("Only one widget allowed");
		}
	}
		
	return m_propertyWidget;	
}

};  //namespace KSimLibDataRecorder

