/***************************************************************************
                          zoomwidget.cpp  -  description
                             -------------------
    begin                : Fri Oct 5 2001
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
#include <float.h>

// QT-Includes
#include <qlabel.h>

// KDE-Includes

// KSimus-Includes
#include "ksimus/ksimdata.h"
#include <ksimus/ksimdoublespinbox.h>
#include <ksimus/ksimunit.h>

// Project-Includes
#include "datarecorderdataview.h"
#include "datarecorder.h"

#include "zoomwidget.h"

// Forward declaration



static const KSimUnitList & getTimeDivList()
{
	static KSimUnitList * list = 0;
	
	if (!list)
	{
		list = new KSimUnitListStatic("Time/Div");
		
		*list << new KSimProportionalUnit("ns/Div", 1e-9, 0.0, 1000.0)
		      << new KSimProportionalUnit("µs/Div", 1e-6, 1.0, 1000.0)
		      << new KSimProportionalUnit("ms/Div", 1e-3, 1.0, 1000.0)
		      << new KSimProportionalUnit("s/Div",  1e-0, 1.0, DBL_MAX)
		      // Some Shorties / malformed
		      << new KSimProportionalUnit("us/Div", 1e-6, true)
		      << new KSimProportionalUnit("ns/D",   1e-9, true)
		      << new KSimProportionalUnit("µs/D",   1e-6, true)
		      << new KSimProportionalUnit("us/D",   1e-6, true)
		      << new KSimProportionalUnit("ms/D",   1e-3, true)
		      << new KSimProportionalUnit("s/D",    1e-0, true)
		      << new KSimProportionalUnit("ns",     1e-9, true)
		      << new KSimProportionalUnit("µs",     1e-6, true)
		      << new KSimProportionalUnit("us",     1e-6, true)
		      << new KSimProportionalUnit("ms",     1e-3, true)
		      << new KSimProportionalUnit("s",      1e-0, true)
		      << new KSimProportionalUnit("n",      1e-9, true)
		      << new KSimProportionalUnit("µ",      1e-6, true)
		      << new KSimProportionalUnit("u",      1e-6, true)
		      << new KSimProportionalUnit("m",      1e-3, true);
		list->setDefaultUnit("s/Div");
	}
	return *list;
}

static const KSimUnitList & getSampleDivList()
{
	static KSimUnitList * list = 0;
	
	if (!list)
	{
		list = new KSimUnitListStatic("Sample/Div");
		
		*list << new KSimProportionalUnit("Sample/Div",  1e0, 0.0, 1000.0)
		      << new KSimProportionalUnit("kSample/Div", 1e3, 1.0, 1000.0)
		      << new KSimProportionalUnit("MSample/Div", 1e6, 1.0, DBL_MAX / 1e6)
		      // Some Shorties / malformed
		      << new KSimProportionalUnit("S/Div",  1e0, true)
		      << new KSimProportionalUnit("kS/Div", 1e3, true)
		      << new KSimProportionalUnit("MS/Div", 1e6, true)
		      << new KSimProportionalUnit("S/D",    1e0, true)
		      << new KSimProportionalUnit("kS/D",   1e3, true)
		      << new KSimProportionalUnit("MS/D",   1e6, true)
		      << new KSimProportionalUnit("S",      1e0, true)
		      << new KSimProportionalUnit("kS",     1e3, true)
		      << new KSimProportionalUnit("MS",     1e6, true)
		      << new KSimProportionalUnit("k",      1e3, true)
		      << new KSimProportionalUnit("M",      1e6, true);
		list->setDefaultUnit("Sample/Div");
	}
	return *list;
}

static const KSimUnitList & getSamplePixelList()
{
	static KSimUnitList * list = 0;
	
	if (!list)
	{
		list = new KSimUnitListStatic("Sample/Pixel");
		
		*list << new KSimProportionalUnit("Sample/Pixel",  1e0, 0.0, 1000.0)
		      << new KSimProportionalUnit("kSample/Pixel", 1e3, 1.0, 1000.0)
		      << new KSimProportionalUnit("MSample/Pixel", 1e6, 1.0, DBL_MAX / 1e6)
		      // Some Shorties / malformed
		      << new KSimProportionalUnit("S/Pixel",  1e0, true)
		      << new KSimProportionalUnit("kS/Pixel", 1e3, true)
		      << new KSimProportionalUnit("MS/Pixel", 1e6, true)
		      << new KSimProportionalUnit("S/P",    1e0, true)
		      << new KSimProportionalUnit("kS/P",   1e3, true)
		      << new KSimProportionalUnit("MS/P",   1e6, true)
		      << new KSimProportionalUnit("S",      1e0, true)
		      << new KSimProportionalUnit("kS",     1e3, true)
		      << new KSimProportionalUnit("MS",     1e6, true)
		      << new KSimProportionalUnit("k",      1e3, true)
		      << new KSimProportionalUnit("M",      1e6, true);
		list->setDefaultUnit("Sample/Pixel");
	}
	return  *list;
}



//#########################################################################################

ZoomWidget::ZoomWidget(DataRecorderDataView * recorderDV, QWidget *parent, const char *name )
	:	QVBox(parent,name),
		m_recorderDataView(recorderDV)
{
	m_time = new KSimDoubleUnitSpinBox(this,"zoom spinbox");
	CHECK_PTR(m_time);
	m_time->getMultiUnitList().append(getTimeDivList());
	m_time->getMultiUnitList().append(getSampleDivList());
	m_time->getMultiUnitList().append(getSamplePixelList());
	m_time->setMinValue(1e-9);
	m_time->setMaxValue(1e9);
	m_time->setNewExponentSteps(1.0,2.0,5.0);
	m_time->setValue(/*getRecorderDataView()->getZoomSample() */ getRecorderDataView()->getHorizontalPixelPerDiv()
	                  * getRecorderDataView()->getDataRecorder()->getSampleTime());
	connect(m_time, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
	connect(m_time, SIGNAL(undoRequest()), this, SLOT(slotUndoRequest()));
	
  m_label = new QLabel("----", this, "zoom label");
	CHECK_PTR(m_label);

	updateMinZoom();

}

ZoomWidget::~ZoomWidget()
{
}

const QString & ZoomWidget::getUnit() const
{
	return m_time->getUnitString();
}
double ZoomWidget::getRawValue() const
{
	return m_time->value();
}
void ZoomWidget::setUnitList(const QString & unitListStr)
{
	m_time->setUnitListString(unitListStr);
}
void ZoomWidget::setRawValue(double rawValue)
{
	m_time->setValue(rawValue);
}

const QString & ZoomWidget::getFixedUnit() const
{
	const KSimUnitBase * fixedUnit = m_time->fixedUnit();
	
	if (fixedUnit)
		return fixedUnit->getUnitString();
	else
		return QString::null;
}

void ZoomWidget::setFixedUnit(const QString & fixedUnit)
{
	m_time->setFixedUnit(fixedUnit);
}

bool ZoomWidget::getExponentEnabled() const
{
	return m_time->exponentStepEnabled();
}
void ZoomWidget::setExponentEnabled(bool ena)
{
	m_time->setExponentStepEnabled(ena);
}

void ZoomWidget::slotChanged()
{
	m_unitList = m_time->getUnitListString();
	
	
	double time = getTimePerPixel();
	double sample = getSamplePerPixel();
	
	emit changedTimePixel(time);
	emit changedSamplePixel(sample);
	
	if (m_unitList == getTimeDivList().getListName())
	{
		m_label->setText(QString("%1 Sample/Pixel").arg(sample));
	}
	else
	{
		m_label->setText(QString("%1 s/div").arg(time*getRecorderDataView()->getHorizontalPixelPerDiv()));
	}
}

double ZoomWidget::getTimePerPixel() const
{
	int pixelDiv = getRecorderDataView()->getHorizontalPixelPerDiv();
	double res = m_time->value();
	double sampleTime = getRecorderDataView()->getDataRecorder()->getSampleTime();

	if (getUnitList() == getTimeDivList().getListName())
	{
		// Unit from list time / Div
		res /= pixelDiv;
	
	}
	else if (getUnitList() == getSampleDivList().getListName())
	{
		// Unit from list sample / Div
		res = res * sampleTime / pixelDiv;
	}
	else
	{
		// Unit from list sample / pixel
		res = res * sampleTime;
	}
	
	return res;
}

double ZoomWidget::getSamplePerPixel() const
{
	double sampleTime = getRecorderDataView()->getDataRecorder()->getSampleTime();
	return getTimePerPixel() / sampleTime;
}

void ZoomWidget::slotUndoRequest()
{
	emit undoRequest();
}

double ZoomWidget::getMinZoom() const
{
	return m_time->minValue();
}

double ZoomWidget::getMaxZoom() const
{
	return m_time->maxValue();
}

void ZoomWidget::setMinZoom(double val)
{
	m_time->setMinValue(val);
}

void ZoomWidget::setMaxZoom(double val)
{
	m_time->setMaxValue(val);
}

void ZoomWidget::updateMinZoom()
{
	// Hint: minZoom[time/div]
	//       div = 50 pixels
	//       m_sampleTime[ms/sample]
	// ==> minZoom = x / pixel/div] * m_sampleTime
	//       x = max pixel/sample
	setMinZoom(getRecorderDataView()->getDataRecorder()->getSampleTime()
						 * (getRecorderDataView()->getHorizontalPixelPerDiv() / 10.0));
}

	
//#######################################################################################
//#######################################################################################

static const char * sGroup = "Zoom/";
static const char * sValue = "Value";
static const char * sUnitList = "Unit List";
static const char * sFixedUnit = "Fixed Unit";
static const char * sExpEna = "Use exponent Steps";

ZoomWidgetVar::ZoomWidgetVar(const double & sampleTime)
	:	m_rawValue(sampleTime),
		m_unitListString(QString()),
		m_fixedUnitString(QString()),
		m_exponentSteps(true)
{
}

ZoomWidgetVar::ZoomWidgetVar(const ZoomWidget & zoom)
{
	copyFrom(zoom);
}

ZoomWidgetVar::~ZoomWidgetVar()
{
}

bool ZoomWidgetVar::operator==(const ZoomWidgetVar & var) const
{
	if (&var == this)	return true;
	
	return (
			(m_rawValue == var.m_rawValue)                  &&
			(m_unitListString == var.m_unitListString)      &&
			(m_fixedUnitString == var.m_fixedUnitString)    &&
			(m_exponentSteps == var.m_exponentSteps)
		);
}


void ZoomWidgetVar::copyFrom(const ZoomWidget & zoom)
{
	m_rawValue = zoom.getRawValue();
	m_unitListString = zoom.getUnitList();
	m_fixedUnitString = zoom.getFixedUnit();
	m_exponentSteps = zoom.getExponentEnabled();
}

void ZoomWidgetVar::copyTo(ZoomWidget & zoom) const
{
	zoom.setRawValue(m_rawValue);
	zoom.setUnitList(m_unitListString);
	zoom.setFixedUnit(m_fixedUnitString);
	zoom.setExponentEnabled(m_exponentSteps);
}

void ZoomWidgetVar::save(KSimData & file) const
{
	QString oldGroup(file.group());
	
	QString group(oldGroup + sGroup);
	
	file.setGroup(group);
	
	file.writeEntry(sValue, m_rawValue);
	file.writeEntry(sUnitList, m_unitListString);

	if (!m_fixedUnitString.isEmpty())
	{
		file.writeEntry(sFixedUnit, m_fixedUnitString);
	}
	
	if (!m_exponentSteps)
	{
		file.writeEntry(sExpEna, false);
	}

	file.setGroup(oldGroup);
}
	
bool ZoomWidgetVar::load(KSimData & file, bool /*copyLoad*/)
{
	QString oldGroup(file.group());
	
	QString group(oldGroup + sGroup);
	
	file.setGroup(group);
	
	m_rawValue = file.readDoubleNumEntry(sValue, 1e-3);
	m_unitListString = file.readEntry(sUnitList, QString());

	m_fixedUnitString = file.readEntry(sFixedUnit, QString());
	
	m_exponentSteps = file.readBoolEntry(sExpEna, true);
	
	file.setGroup(oldGroup);

	return true;
}
	
	


