/***************************************************************************
                          floatstylerange1out.cpp  -  description
                             -------------------
    begin                : Sat Feb 2 2002
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "floatstylerange1out.h"

// Forward declaration


namespace KSimLibFloatingPoint
{

#define DEFAULT_MIN_VALUE    0.0
#define DEFAULT_MAX_VALUE    1.0

#define MAX_SLIDER_VAL   1000
#define MIN_SLIDER_VAL   0

#define FLAGS_SWAP_RANGE_ALLOWED   0x0001


//############################################################################
//############################################################################


FloatStyleRange1Out::FloatStyleRange1Out(CompContainer * container, const ComponentInfo * ci)
	: FloatStyle1Out(container, ci),
		m_initMax(DEFAULT_MAX_VALUE),
		m_initMin(DEFAULT_MIN_VALUE),
		m_max(DEFAULT_MAX_VALUE),
		m_min(DEFAULT_MIN_VALUE),
		m_flags(0)
{
}

void FloatStyleRange1Out::setMaxValue(double max, bool init)
{
	m_max = max;
	emit maxChanged(max);
	if (init)
	{
		m_initMax = max;
	}
}

void FloatStyleRange1Out::setMinValue(double min, bool init)
{
	m_min = min;
	emit minChanged(min);
	if (init)
	{
		m_initMin = min;
	}
}

void FloatStyleRange1Out::setMinMaxSwapAllowed(bool swap)
{
	if (swap)
	{
		m_flags |= FLAGS_SWAP_RANGE_ALLOWED;
	}
	else
	{
		m_flags &= ~FLAGS_SWAP_RANGE_ALLOWED;
	}
}

bool FloatStyleRange1Out::isMinMaxSwapAllowed() const
{
	return m_flags & FLAGS_SWAP_RANGE_ALLOWED;
}


/** save component properties */
void FloatStyleRange1Out::save(KSimData & file) const
{
	FloatStyle1Out::save(file);
	
	if (getMinValue() != getInitMinValue())
	{
		file.writeEntry("Min Value", getMinValue());
	}
	if (getMaxValue() != getInitMaxValue())
	{
		file.writeEntry("Max Value", getMaxValue());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FloatStyleRange1Out::load(KSimData & file, bool copyLoad)
{
	setMinValue( file.readDoubleNumEntry("Min Value", getInitMinValue()) );
	setMaxValue( file.readDoubleNumEntry("Max Value", getInitMaxValue()) );
	
	return FloatStyle1Out::load(file, copyLoad);
}

void FloatStyleRange1Out::checkProperty(QStringList & errorMsg)
{
	FloatStyle1Out::checkProperty(errorMsg);
	
	if (isMinMaxSwapAllowed())
	{
		if (((getMinValue() <= getResetValue()) && (getResetValue() <= getMaxValue()))
		 || ((getMinValue() >= getResetValue()) && (getResetValue() >= getMaxValue())))
		{
			// Okay
		}
		else
		{
			errorMsg.append(i18n("The reset, minimum and maximum values do not match!\n"
			                     "The values have to match following conditions:\n"
			                     "\tMinimum Value <= Reset Value <= Maximum Value\n"
			                     "or\n"
			                     "\tMinimum Value >= Reset Value >= Maximum Value\n"));
		}
	}
	else
	{
		if ((getMinValue() <= getResetValue()) && (getResetValue() <= getMaxValue()))
		{
			// Okay
		}
		else
		{
			errorMsg.append(i18n("The reset, minimum and maximum values do not match!\n"
			                     "The values have to match following condition:\n"
			                     "\tMinimum Value <= Reset Value <= Maximum Value\n"));
		}
	}
}

ComponentPropertyBaseWidget * FloatStyleRange1Out::createGeneralProperty(QWidget *parent)
{
	FloatStyleRange1OutPropertyGeneralWidget * wid;
	wid = new FloatStyleRange1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//##########################################################################################
//##########################################################################################

FloatStyleRange1OutPropertyGeneralWidget::FloatStyleRange1OutPropertyGeneralWidget(FloatStyleRange1Out * comp, QWidget *parent, const char *name)
	:	FloatStyle1OutPropertyGeneralWidget(comp, parent, name)
{
	m_maxValueLabel = new QLabel(i18n("Maximum value: "), this, "MaxValueLabel");
	CHECK_PTR(m_maxValueLabel);
	
	m_maxValue = new KSimDoubleEdit(this, "MaxValue");
	CHECK_PTR(m_maxValue);
	QString tip(i18n("The maximum value of the component."));
	addToolTip(tip, m_maxValue, m_maxValueLabel);
	addWhatsThis(tip, m_maxValue, m_maxValueLabel);
	
	
	m_minValueLabel = new QLabel(i18n("Minimum value: "), this, "MinValueLabel");
	CHECK_PTR(m_minValueLabel);
	
	m_minValue = new KSimDoubleEdit(this, "MinValue");
	CHECK_PTR(m_minValue);
	tip = i18n("The minimum value of the component.");
	addToolTip(tip, m_minValue, m_minValueLabel);
	addWhatsThis(tip, m_minValue, m_minValueLabel);
	
	// Setup value
	m_maxValue->setValue(comp->getMaxValue());
	m_minValue->setValue(comp->getMinValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void FloatStyleRange1OutPropertyGeneralWidget::acceptPressed()
{
	FloatStyle1OutPropertyGeneralWidget::acceptPressed();
	
	double max = getMaxValueEdit()->value();
	if (getComponent()->getMaxValue() != max)
	{
		changeData();
		getComponent()->setMaxValue(max);
	}

	double min = getMinValueEdit()->value();
	if (getComponent()->getMinValue() != min)
	{
		changeData();
		getComponent()->setMinValue(min);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void FloatStyleRange1OutPropertyGeneralWidget::defaultPressed()
{
	FloatStyle1OutPropertyGeneralWidget::defaultPressed();

	getMaxValueEdit()->setValue(getComponent()->getInitMaxValue());
	getMinValueEdit()->setValue(getComponent()->getInitMinValue());
}


//###############################################################
//###############################################################

#undef DEFAULT_MIN_VALUE
#undef DEFAULT_MAX_VALUE

#undef MAX_SLIDER_VAL
#undef MIN_SLIDER_VAL

#undef FLAGS_SWAP_RANGE_ALLOWED

};  //namespace KSimLibFloatingPoint

#include "floatstylerange1out.moc"
