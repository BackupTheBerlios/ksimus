/***************************************************************************
                          floatinputslider.cpp  -  description
                             -------------------
    begin                : Sun Dec 30 2001
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
#include <qslider.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/componentlayout.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "floatinputslider.h"

// Forward declaration


namespace KSimLibFloatingPoint
{

#define DEFAULT_MIN_VALUE    0.0
#define DEFAULT_MAX_VALUE    1.0

#define MAX_SLIDER_VAL   1000
#define MIN_SLIDER_VAL   0

//#######################################################################
//#######################################################################

Component * FloatInputSlider::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatInputSlider(container, ci);
}

const ComponentInfo * FloatInputSlider::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Input Slider"),
	                                QString::fromLatin1("Floating Point/Input/Slider"),
	                                i18n("Component", "Floating Point/Input/Slider"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-input-slider"));
	return &Info;
}


//############################################################################
//############################################################################


FloatInputSlider::FloatInputSlider(CompContainer * container, const ComponentInfo * ci)
	: FloatStyle1Out(container, ci),
		m_max(DEFAULT_MAX_VALUE),
		m_min(DEFAULT_MIN_VALUE)
{
	
//	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
//	setFontAdjustmentEnabled(true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatInputSliderView(this, SHEET_VIEW, "FloatInputSliderSV");
	}
	// Initializes the user view
	if (getUserMap())
	{
		new FloatInputSliderView(this, USER_VIEW, "FloatInputSliderSV");
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}

void FloatInputSlider::reset()
{
	FloatStyle1Out::reset();
	
	emit valueChanged(toSlider(getValue()));
}

void FloatInputSlider::newValue(int i)
{
	double d = fromSlider(i);
	
	if (d != getValue())
	{
		setValue(d);
		emit valueChanged(toSlider(d));
		executeNext();
	}
}

double FloatInputSlider::fromSlider(int i) const
{
	double d = ((double)i)/MAX_SLIDER_VAL;
	// Result: 0 >= d >= 1
	
	return (d * (getMaxValue()-getMinValue())) + getMinValue();
}

int FloatInputSlider::toSlider(double d) const
{
	d = (d - getMinValue()) / (getMaxValue()-getMinValue());
	
//	return MAX_SLIDER_VAL - qRound(d * MAX_SLIDER_VAL);
	return qRound(d * MAX_SLIDER_VAL);
}

void FloatInputSlider::setMaxValue(double max)
{
	m_max = max;
}

void FloatInputSlider::setMinValue(double min)
{
	m_min = min;
}

/** save component properties */
void FloatInputSlider::save(KSimData & file) const
{
	FloatStyle1Out::save(file);
	
	if (getMinValue() != DEFAULT_MIN_VALUE)
	{
		file.writeEntry("Min Value", getMinValue());
	}
	if (getMaxValue() != DEFAULT_MAX_VALUE)
	{
		file.writeEntry("Max Value", getMaxValue());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FloatInputSlider::load(KSimData & file, bool copyLoad)
{
	setMinValue( file.readDoubleNumEntry("Min Value", DEFAULT_MIN_VALUE) );
	setMaxValue( file.readDoubleNumEntry("Max Value", DEFAULT_MAX_VALUE) );
	
	return FloatStyle1Out::load(file, copyLoad);
}

ComponentPropertyBaseWidget * FloatInputSlider::createGeneralProperty(QWidget *parent)
{
	FloatInputSliderPropertyGeneralWidget * wid;
	wid = new FloatInputSliderPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}
//############################################################################
//############################################################################



FloatInputSliderView::FloatInputSliderView(FloatInputSlider * comp, eViewType viewType, const char * name)
	: CompViewSize(comp,viewType, name)
{
	enableRotation(false);
	
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 5*gridX, 15*gridY));
		setMinSize(5*gridX, 4*gridY);

		ComponentLayoutFixed * layout = new ComponentLayoutFixed(this, false);
		CHECK_PTR(layout);
		
		ComponentLayoutBlock * block = new ComponentLayoutBlock(layout);
		CHECK_PTR(layout);
	
		block->getRight()->addStretch(2);
		block->getRight()->addConnector(comp->getOutputConnector(),0);
		block->getRight()->addStretch(2);
	}
	else
	{
		setPlace(QRect(0, 0, 4*gridX, 15*gridY));
		setMinSize(4*gridX, 4*gridY);
	}	
}


QWidget * FloatInputSliderView::createCompViewWidget(QWidget * parent)
{
	FloatInputSliderWidgetView * wid = new FloatInputSliderWidgetView(this, parent,"Slider");
	
	/* Specific signals */
	// Number changed (Component->Widget)
	connect(getComponent(), SIGNAL(valueChanged(int)), wid, SLOT(setValue(int)));
	// Number changed (Widget->Component)
	connect(wid, SIGNAL(valueChanged(int)), getComponent(), SLOT(newValue(int)));
	// Number changed (Slider->Widget)
	connect(wid->m_slider, SIGNAL(valueChanged(int)), wid, SLOT(silderValueChanged(int)));
	// Number changed (CompView->widget)
	connect(this, SIGNAL(signalOrientation(Orientation)), wid, SLOT(setOrientation(Orientation)));
	
	
	makeOrientation();

	return wid;
}

void FloatInputSliderView::resize()
{
	CompViewSize::resize();	

	makeOrientation();
}


/*FloatInputSliderView::~FloatInputSliderView()
{
} */

void FloatInputSliderView::makeOrientation()
{
	if (getPlace().width()-gridX > getPlace().height())
	{
		emit signalOrientation(Horizontal);
	}
	else
	{
		emit signalOrientation(Vertical);
	}
}	

//##########################################################################################
//##########################################################################################

FloatInputSliderWidgetView::FloatInputSliderWidgetView(FloatInputSliderView * cv, QWidget *parent, const char *name)
	:	CompViewVBox(cv,parent,name)
{
	m_slider = new QSlider(this);
	CHECK_PTR(m_slider);
	m_slider->setMinValue(MIN_SLIDER_VAL);
	m_slider->setMaxValue(MAX_SLIDER_VAL);
	m_slider->setValue(MIN_SLIDER_VAL);
	m_slider->setTickmarks(QSlider::Right);
	m_slider->setTickInterval(50);
	
}

/*FloatInputSliderWidgetView::~FloatInputSliderWidgetView()
{
} */

void FloatInputSliderWidgetView::setOrientation(Orientation orient)
{
	m_slider->setOrientation(orient);
	m_slider->updateGeometry();
}


void FloatInputSliderWidgetView::setValue(int i)
{
	if (m_slider->orientation() == Vertical)
	{
		i = m_slider->maxValue() - i;
	}
	m_slider->setValue(i);
}

void FloatInputSliderWidgetView::silderValueChanged(int i)
{
	if (m_slider->orientation() == Vertical)
	{
		i = m_slider->maxValue() - i;
	}
	emit valueChanged(i);
}

//##########################################################################################
//##########################################################################################

FloatInputSliderPropertyGeneralWidget::FloatInputSliderPropertyGeneralWidget(FloatInputSlider * comp, QWidget *parent, const char *name)
	:	FloatStyle1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip(i18n("FloatingPoint", "The reset value of the slider."));
	addToolTip(tip, getResetValueEdit(), getResetValueLabel());
	addWhatsThis(tip, getResetValueEdit(), getResetValueLabel());
	
	
	
	m_maxValueLabel = new QLabel(i18n("FloatingPoint", "Top/Left value: "), this, "MaxValueLabel");
	CHECK_PTR(m_maxValueLabel);
	
	m_maxValue = new KSimDoubleEdit(this, "MaxValue");
	CHECK_PTR(m_maxValue);
	tip = i18n("FloatingPoint", "The value of the slider at the top or left position.");
	addToolTip(tip, m_maxValue, m_maxValueLabel);
	addWhatsThis(tip, m_maxValue, m_maxValueLabel);
	m_maxValueLabel->setBuddy(m_maxValue);
	
	
	m_minValueLabel = new QLabel(i18n("FloatingPoint", "Bottom/Right value: "), this, "MinValueLabel");
	CHECK_PTR(m_minValueLabel);
	
	m_minValue = new KSimDoubleEdit(this, "MinValue");
	CHECK_PTR(m_minValue);
	tip = i18n("FloatingPoint", "The value of the slider at the bottom or right position.");
	addToolTip(tip, m_minValue, m_minValueLabel);
	addWhatsThis(tip, m_minValue, m_minValueLabel);
	m_minValueLabel->setBuddy(m_minValue);
	
	// Setup value
	m_maxValue->setValue(comp->getMaxValue());
	m_minValue->setValue(comp->getMinValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void FloatInputSliderPropertyGeneralWidget::acceptPressed()
{
	FloatStyle1OutPropertyGeneralWidget::acceptPressed();
	
	double max = m_maxValue->value();
	if (getSlider()->getMaxValue() != max)
	{
		changeData();
		getSlider()->setMaxValue(max);
	}

	double min = m_minValue->value();
	if (getSlider()->getMinValue() != min)
	{
		changeData();
		getSlider()->setMinValue(min);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void FloatInputSliderPropertyGeneralWidget::defaultPressed()
{
	FloatStyle1OutPropertyGeneralWidget::defaultPressed();

	m_maxValue->setValue(DEFAULT_MAX_VALUE);
	m_minValue->setValue(DEFAULT_MIN_VALUE);
}


//###############################################################
//###############################################################

#undef DEFAULT_MIN_VALUE
#undef DEFAULT_MAX_VALUE
#undef MAX_SLIDER_VAL
#undef MIN_SLIDER_VAL

};  //namespace KSimLibFloatingPoint

#include "floatinputslider.moc"

