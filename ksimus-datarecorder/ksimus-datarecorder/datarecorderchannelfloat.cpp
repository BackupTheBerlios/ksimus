/***************************************************************************
                          datarecorderchannelfloat.cpp  -  description
                             -------------------
    begin                : Sun Feb 3 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include "ksimus/connectorfloatin.h"
#include "ksimus/ksimdebug.h"

// Project-Includes
#include "datarecorder.h"
#include "datarecorderchannelfloat.h"
#include "channelpositionwidget.h"

// Forward declaration


namespace KSimLibDataRecorder
{

#define DOUBLE_PER_ARRAY  256

class FloatStorage
{
	typedef struct
	{
		double array[DOUBLE_PER_ARRAY];
	}tData;

public:
	FloatStorage()
		:	m_count(0)
	{
		m_data.setAutoDelete(true);
	
	};
	~FloatStorage()
	{};
	
	void append(double data)
	{
		int index = m_count / DOUBLE_PER_ARRAY;
		int offset = m_count % DOUBLE_PER_ARRAY;

		if(offset == 0)
		{
			tData * newData = new tData;
			for (int i = 0; i < DOUBLE_PER_ARRAY; i++)
				newData->array[i] = 0.0;
			m_data.append(newData);
		}
		m_data.at(index)->array[offset] = data;
		m_count++;
	};
			
	double get(int number)
	{
		if(number >= m_count) return 0.0;
		
		int index = number / DOUBLE_PER_ARRAY;
		int offset = number % DOUBLE_PER_ARRAY;

		return m_data.at(index)->array[offset];
	}


	int count() const { return m_count; };
	
	void clear() { m_data.clear();
	               m_count = 0; };
	
private:
	int m_count;
	QList<tData> m_data;
	
};

#undef DOUBLE_PER_ARRAY


//#############################################################################
//#############################################################################

unsigned int DataRecorderChannelFloat::counter = 1;


DataRecorderChannelFloat::DataRecorderChannelFloat(DataRecorder * recorder)
	:	DataRecorderChannelBase(recorder),
		m_propertyWidget(0)
{
	setChannelType(CT_Float);
	m_input = new ConnectorFloatIn(recorder,QString::fromLatin1("Floating Point"),
	                             i18n("DataRecorder-Connector", "Floating Point"),
	                             QPoint());
	m_input->setErasable(true);
	m_input->getAction().disable(KSimAction::STORAGE);
	connect(m_input, SIGNAL(signalDeleteRequest(ConnectorBase *)),
	        recorder, SLOT(slotRemoveChannelConn(ConnectorBase *)));
	m_data = new FloatStorage();
	
	counter++;
	
	if (counter > 15) counter = 1;

	setVerticalGain(1);
	setVerticalOffset(0.5 * counter);
}

DataRecorderChannelFloat::~DataRecorderChannelFloat()
{
	if (m_propertyWidget)
		delete m_propertyWidget;
	delete m_data;
//	delete m_input;
}

void DataRecorderChannelFloat::fetchData()
{
	double data = ((ConnectorFloatIn*)getConnector())->getInput();
	
	m_data->append(data);
}
	
double DataRecorderChannelFloat::getData(unsigned int number)
{
	return m_data->get(number);
}	

/** Resets channel. Delete data. */
void DataRecorderChannelFloat::reset()
{
	m_data->clear();
}

int DataRecorderChannelFloat::drawData(QPaintDevice * paintDev,
                                       int startSample, int stopSample,
                                       int horizontalOffset, int height,
                                       double samplePerPixel, int verticalDivs)
{
	#define LIMIT 32767
	#define LIMITER(a) (QMIN(QMAX(a,-LIMIT),LIMIT))
	int index = startSample;
	int counter = 0;
	int horiPos = horizontalOffset;
	int lastHoriPos = horizontalOffset;
	int lastVertPos, vertPos;
	QPainter painter(paintDev);
	painter.setPen(getLineColor());
	
	double verticalOffset = (getVerticalOffset() * height) / verticalDivs;
	double verticalGain = (getVerticalGain() * height) / verticalDivs;

	if (index >= m_data->count())
		return m_data->count();	// Nothing to draw
		
	if (stopSample >= m_data->count())
		stopSample = m_data->count()-1; // Limit samples
		
	vertPos = lastVertPos = LIMITER(height - qRound(verticalGain * getData(index) + verticalOffset));
	
	while(index < stopSample)
	{
		index++;
		counter++;
		vertPos = LIMITER(height - qRound(verticalGain * getData(index) + verticalOffset));
		horiPos = horizontalOffset + qRound(counter / samplePerPixel);
//		if(lastVertPos != vertPos)		// Speed up static signals
		{
			painter.drawLine(lastHoriPos, lastVertPos, horiPos, vertPos);
			lastVertPos = vertPos;
			lastHoriPos = horiPos;
		}
	}
	// Draw final line
//	painter.drawLine(lastHoriPos, lastVertPos, horiPos, vertPos);
	return index-1;
}

QWidget * DataRecorderChannelFloat::getPropertyWidget(QWidget * parent, const char * name)
{
	if (!m_propertyWidget)
	{
		// Setup widget
		QVBox * widget = new QVBox(parent,name);
		CHECK_PTR(widget);
		widget->setMargin(KDialog::marginHint());
		widget->setSpacing(KDialog::spacingHint());
		
		QLabel * lab;
		lab = new QLabel(i18n("DataRecorder", "Channel:"), widget);
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

#include "datarecorderchannelfloat.moc"
