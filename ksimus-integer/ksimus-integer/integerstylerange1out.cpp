/***************************************************************************
                          integerstylerange1out.cpp  -  description
                             -------------------
    begin                : Mon Jul 14 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include <limits.h>

// QT-Includes
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbaseintedit.h"
#include "integerstylerange1out.h"

// Forward declaration


namespace KSimLibInteger
{


//############################################################################
//############################################################################

IntegerStyleRange1Out::IntegerStyleRange1Out(CompContainer * container, const ComponentInfo * ci)
	: IntegerStyle1Out(container, ci),
		m_min(KSimBaseInt(INT_MIN, KSimBaseInt::Decimal)),
		m_max(KSimBaseInt(INT_MAX, KSimBaseInt::Decimal)),
		m_initMin(KSimBaseInt(INT_MIN, KSimBaseInt::Decimal)),
		m_initMax(KSimBaseInt(INT_MAX, KSimBaseInt::Decimal))
{
	m_flags.swapRangeAllowed = 0;
}

IntegerStyleRange1Out::~IntegerStyleRange1Out()
{
}

void IntegerStyleRange1Out::setMaxValue(const KSimBaseInt & max)
{
	m_max = max;
	emit maxValueChanged(m_max.value());
}

void IntegerStyleRange1Out::setMinValue(const KSimBaseInt & min)
{
	m_min = min;
	emit minValueChanged(min.value());
}

void IntegerStyleRange1Out::setInitMaxValue(const KSimBaseInt & max)
{
	m_initMax = max;
	setMaxValue(max);
}

void IntegerStyleRange1Out::setInitMinValue(const KSimBaseInt & min)
{
	m_initMin = min;
	setMinValue(min);
}

void IntegerStyleRange1Out::setMinMaxSwapAllowed(bool swap)
{
	m_flags.swapRangeAllowed = swap;
}

bool IntegerStyleRange1Out::isMinMaxSwapAllowed() const
{
	return m_flags.swapRangeAllowed;
}


/** save component properties */
void IntegerStyleRange1Out::save(KSimData & file) const
{
	IntegerStyle1Out::save(file);
	
	if (m_min != m_initMin)
	{
		m_min.save(file, "Min Value");
	}
	if (m_max != m_initMax)
	{
		m_max.save(file, "Max Value");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerStyleRange1Out::load(KSimData & file, bool copyLoad)
{
	if (!m_min.load(file, "Min Value"))
	{
		m_min = m_initMin;
	}
	
	if (!m_max.load(file, "Max Value"))
	{
		m_max = m_initMax;
	}

	return IntegerStyle1Out::load(file, copyLoad);
}

void IntegerStyleRange1Out::checkProperty(QStringList & errorMsg)
{
	IntegerStyle1Out::checkProperty(errorMsg);
	
	if (isMinMaxSwapAllowed())
	{
		if (((getMinValue().value() <= getResetValue().value()) && (getResetValue().value() <= getMaxValue().value()))
		 || ((getMinValue().value() >= getResetValue().value()) && (getResetValue().value() >= getMaxValue().value())))
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
		if ((getMinValue().value() <= getResetValue().value()) && (getResetValue().value() <= getMaxValue().value()))
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

ComponentPropertyBaseWidget * IntegerStyleRange1Out::createGeneralProperty(QWidget *parent)
{
	IntegerStyleRange1OutPropertyGeneralWidget * wid;
	wid = new IntegerStyleRange1OutPropertyGeneralWidget(this, parent);
	Q_CHECK_PTR(wid);
	
	return wid;
}

//##########################################################################################
//##########################################################################################

IntegerStyleRange1OutPropertyGeneralWidget::IntegerStyleRange1OutPropertyGeneralWidget(IntegerStyleRange1Out * comp, QWidget *parent, const char *name)
	:	IntegerStyle1OutPropertyGeneralWidget(comp, parent, name)
{
	m_maxValueLabel = new QLabel(i18n("Maximum value: "), this, "MaxValueLabel");
	Q_CHECK_PTR(m_maxValueLabel);
	
	m_maxValue = new KSimBaseIntEdit(this, "MaxValue");
	Q_CHECK_PTR(m_maxValue);
	QString tip(i18n("The maximum value of the component."));
	addToolTip(tip, m_maxValue, m_maxValueLabel);
	addWhatsThis(tip, m_maxValue, m_maxValueLabel);
	m_maxValueLabel->setBuddy(m_maxValue);
	
	
	m_minValueLabel = new QLabel(i18n("Minimum value: "), this, "MinValueLabel");
	Q_CHECK_PTR(m_minValueLabel);
	
	m_minValue = new KSimBaseIntEdit(this, "MinValue");
	Q_CHECK_PTR(m_minValue);
	tip = i18n("The minimum value of the component.");
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
void IntegerStyleRange1OutPropertyGeneralWidget::acceptPressed()
{
	IntegerStyle1OutPropertyGeneralWidget::acceptPressed();
	
	KSimBaseInt max(getMaxValueEdit()->value());
	if (getIntegerStyleRange1Out()->getMaxValue() != max)
	{
		changeData();
		getIntegerStyleRange1Out()->setMaxValue(max);
	}

	KSimBaseInt min(getMinValueEdit()->value());
	if (getIntegerStyleRange1Out()->getMinValue() != min)
	{
		changeData();
		getIntegerStyleRange1Out()->setMinValue(min);
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerStyleRange1OutPropertyGeneralWidget::defaultPressed()
{
	IntegerStyle1OutPropertyGeneralWidget::defaultPressed();

	getMaxValueEdit()->setValue(getIntegerStyleRange1Out()->getInitMaxValue());
	getMinValueEdit()->setValue(getIntegerStyleRange1Out()->getInitMinValue());
}


//###############################################################
//###############################################################


};  //namespace KSimLibInteger

#include "integerstylerange1out.moc"
