/***************************************************************************
                          watchitemfloatingpoint.cpp  -  description
                             -------------------
    begin                : Tue Jul 16 2002
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
#include <math.h>  //isnan(), HUGE_VAL

// QT-Includes
#include <qcombobox.h>
#include <qhbox.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "watchitemfloatingpoint.h"
#include "wirepropertyboolean.h"
#include "connectorbase.h"
#include "ksimdoubleedit.h"

// Forward declaration


WatchItemFloatingPointBase::WatchItemFloatingPointBase(ConnectorBase * connector)
	:	WatchItemBase(connector)
{
	init();
}

WatchItemFloatingPointBase::WatchItemFloatingPointBase(WireProperty * wireProperty)
	:	WatchItemBase(wireProperty)
{
	init();
}

WatchItemFloatingPointBase::~WatchItemFloatingPointBase()
{
}

void WatchItemFloatingPointBase::init()
{
	m_traceBuffer.fill(0.0, getTraceSize());
	m_triggerType = eRising;
	m_limit = 0.0;
	m_triggerBox = (QComboBox*) 0;
	m_limitEdit = (KSimDoubleEdit*) 0;
}


/** Resets the item. */
void WatchItemFloatingPointBase::reset()
{
	WatchItemBase::reset();
	m_traceBuffer.fill(false);
}


void WatchItemFloatingPointBase::setTraceSize(unsigned int size)
{
	WatchItemBase::setTraceSize(size);

	m_traceBuffer.fill(0.0, getTraceSize());
}

QString WatchItemFloatingPointBase::getDataText(unsigned int index)
{
	return QString::number(m_traceBuffer[index]);
}

bool WatchItemFloatingPointBase::testBreak(unsigned int index, unsigned int stepCount)
{
	switch(m_triggerType)
	{
		case eRising:
			if ((m_traceBuffer[index] >= m_limit) && (stepCount != 0))
			{
				unsigned int previous = ((index != 0) ? index : getTraceSize())-1;
				return (m_traceBuffer[previous] < m_limit);
			}
			break;

		case eFalling:
			if ((m_traceBuffer[index] <= m_limit) && (stepCount != 0))
			{
				unsigned int previous = ((index != 0) ? index : getTraceSize())-1;
				return (m_traceBuffer[previous] > m_limit);
			}
			break;

		case eNaN:
			return isnan(m_traceBuffer[index]);
			break;
			
		case ePosInf:
			return (m_traceBuffer[index] == HUGE_VAL);
			break;

		case eNegInf:
			return (m_traceBuffer[index] == -HUGE_VAL);
			break;
	}
	return false;
}

void WatchItemFloatingPointBase::createTriggerPropertyWidget(QWidget * parent)
{
	ASSERT(m_triggerBox == 0);
	ASSERT(m_limitEdit == 0);
	
	m_triggerBox = new QComboBox(parent, "m_triggerBox");
	CHECK_PTR(m_triggerBox);

	m_triggerBox->insertItem(i18n("Floating Point Trigger", "Rising edge"), (int)eRising);
	m_triggerBox->insertItem(i18n("Floating Point Trigger", "Falling edge"), (int)eFalling);
	m_triggerBox->insertItem(i18n("Floating Point Trigger", "Not a number (NaN)"), (int)eNaN);
	m_triggerBox->insertItem(i18n("Floating Point Trigger", "Positive infinity"), (int)ePosInf);
	m_triggerBox->insertItem(i18n("Floating Point Trigger", "Negative infinity"), (int)eNegInf);

	m_triggerBox->setCurrentItem((int)m_triggerType);

	QHBox * limitBox = new QHBox(parent);
	CHECK_PTR(limitBox);

	QLabel * limiLabel = new QLabel(i18n("Floating Point Trigger", "Threshold:"), limitBox, "watchpoint treshold label");
	CHECK_PTR(limiLabel);

	m_limitEdit = new KSimDoubleEdit(limitBox, "m_limitEdit");
	CHECK_PTR(m_limitEdit);
	m_limitEdit->setValue(m_limit);


	slotTriggerBoxChanged();   // Dis-/enabled m_limitEdit

	connect(m_triggerBox, SIGNAL(activated(int)), SLOT(slotTriggerBoxChanged()));
}


void WatchItemFloatingPointBase::slotTriggerBoxChanged()
{
	ASSERT(m_triggerBox != 0);
	ASSERT(m_limitEdit != 0);

	switch((eTriggerType)m_triggerBox->currentItem())
	{
		case eRising:
		case eFalling:
			m_limitEdit->setEnabled(true);
			break;
		case eNaN:
		case ePosInf:
		case eNegInf:
			m_limitEdit->setEnabled(false);
			break;
	}
}

void WatchItemFloatingPointBase::triggerPropertyOkPressed()
{
	ASSERT(m_triggerBox != 0);
	ASSERT(m_limitEdit != 0);

	m_triggerType = (eTriggerType)m_triggerBox->currentItem();
	m_limit = m_limitEdit->value();

	m_triggerBox = (QComboBox *) 0;    // Will be deleted now
	m_limitEdit = (KSimDoubleEdit *) 0;
}
void WatchItemFloatingPointBase::triggerPropertyDefaultPressed()
{
	ASSERT(m_triggerBox != 0);
	ASSERT(m_limitEdit != 0);
	m_triggerBox->setCurrentItem((int)eRising);
	m_limitEdit->setValue(0.0);
}
void WatchItemFloatingPointBase::triggerPropertyCancelPressed()
{
	ASSERT(m_triggerBox != 0);
	ASSERT(m_limitEdit != 0);

	m_triggerBox = (QComboBox *) 0;    // Will be deleted now
	m_limitEdit = (KSimDoubleEdit *) 0;
}

//############################################################################################
//############################################################################################

WatchItemFloatingPointConnector::WatchItemFloatingPointConnector(ConnectorBase * connector)
	:	WatchItemFloatingPointBase(connector)
{
}

WatchItemFloatingPointConnector::~WatchItemFloatingPointConnector()
{
}

void WatchItemFloatingPointConnector::readData(unsigned int index)
{
	const double * pData = (const double *)getConnector()->readoutData();
	if (pData)
	{
		m_traceBuffer[index] = *pData;
		setIndexUsed(index);
	}
}

//############################################################################################
//############################################################################################

WatchItemFloatingPointWireProperty::WatchItemFloatingPointWireProperty(WireProperty * wireProperty)
	:	WatchItemFloatingPointBase(wireProperty)
{
}

WatchItemFloatingPointWireProperty::~WatchItemFloatingPointWireProperty()
{
}

void WatchItemFloatingPointWireProperty::readData(unsigned int index)
{
	const double * pData = (const double *)getWireProperty()->readoutData();
	if (pData)
	{
		m_traceBuffer[index] = *pData;
		setIndexUsed(index);
	}
}

#include "watchitemfloatingpoint.moc"
