/***************************************************************************
                          watchiteminteger.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
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

// QT-Includes
#include <qcombobox.h>
#include <qhbox.h>
#include <qvgroupbox.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "wireproperty.h"
#include "watchiteminteger.h"
#include "connectorbase.h"
#include "propertywidget.h"

// Forward declaration


WatchItemIntegerBase::WatchItemIntegerBase(ConnectorBase * connector)
	:	WatchItemBase(connector)
{
	init();
}

WatchItemIntegerBase::WatchItemIntegerBase(WireProperty * wireProperty)
	:	WatchItemBase(wireProperty)
{
	init();
}

WatchItemIntegerBase::~WatchItemIntegerBase()
{
}

void WatchItemIntegerBase::init()
{
	m_traceBuffer.fill(0, getTraceSize());
	m_triggerType = eRising;
	m_limit = 0;
	m_triggerBox = (QComboBox*) 0;
	m_limitEdit = (KSimBaseIntEdit*) 0;
	m_viewBase = KSimBaseInt::Decimal;
}


/** Resets the item. */
void WatchItemIntegerBase::reset()
{
	WatchItemBase::reset();
	m_traceBuffer.fill(false);
}


void WatchItemIntegerBase::setTraceSize(unsigned int size)
{
	WatchItemBase::setTraceSize(size);

	m_traceBuffer.fill(0, getTraceSize());
}

QString WatchItemIntegerBase::getDataText(unsigned int index)
{
	KSimBaseInt value(m_traceBuffer[index], m_viewBase);
	return value.text();
}

bool WatchItemIntegerBase::testBreak(unsigned int index, unsigned int stepCount)
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
//			return isnan(m_traceBuffer[index]);
			break;
			
		case ePosInf:
//			return (m_traceBuffer[index] == HUGE_VAL);
			break;

		case eNegInf:
//			return (m_traceBuffer[index] == -HUGE_VAL);
			break;
	}
	return false;
}

PropertyWidget * WatchItemIntegerBase::createPropertyWidget(QWidget * parent)
{
	PropertyWidget * wid = WatchItemBase::createPropertyWidget(parent);

	QVGroupBox * baseBox = new QVGroupBox(i18n("INT base", "Base selection"), wid, "Base selection");
	Q_CHECK_PTR(baseBox);

	m_baseSelector = new KSimBaseIntBaseSelection(m_viewBase, baseBox, "m_baseSelector");
	Q_CHECK_PTR(m_baseSelector);
	
	return wid;
}

void WatchItemIntegerBase::propertyOkPressed()
{
	WatchItemBase::propertyOkPressed();
	m_viewBase = m_baseSelector->base();
}

void WatchItemIntegerBase::propertyDefaultPressed()
{
	WatchItemBase::propertyDefaultPressed();
	m_viewBase = KSimBaseInt::Decimal;
}

void WatchItemIntegerBase::propertyCancelPressed()
{
	WatchItemBase::propertyCancelPressed();
}

void WatchItemIntegerBase::createTriggerPropertyWidget(QWidget * parent)
{
	Q_ASSERT(m_triggerBox == 0);
	Q_ASSERT(m_limitEdit == 0);
	
	m_triggerBox = new QComboBox(parent, "m_triggerBox");
	Q_CHECK_PTR(m_triggerBox);

	m_triggerBox->insertItem(i18n("Integer Trigger", "Rising edge"), (int)eRising);
	m_triggerBox->insertItem(i18n("Integer Trigger", "Falling edge"), (int)eFalling);
	m_triggerBox->insertItem(i18n("Integer Trigger", "Not a number (NaN)"), (int)eNaN);
	m_triggerBox->insertItem(i18n("Integer", "Positive infinity"), (int)ePosInf);
	m_triggerBox->insertItem(i18n("Integer", "Negative infinity"), (int)eNegInf);

	m_triggerBox->setCurrentItem((int)m_triggerType);

	QHBox * limitBox = new QHBox(parent);
	Q_CHECK_PTR(limitBox);

	QLabel * limiLabel = new QLabel(i18n("Integer Trigger", "Threshold:"), limitBox, "watchpoint treshold label");
	Q_CHECK_PTR(limiLabel);

	m_limitEdit = new KSimBaseIntEdit(limitBox, "m_limitEdit");
	Q_CHECK_PTR(m_limitEdit);
	m_limitEdit->setValue(m_limit);
	
	limiLabel->setBuddy(m_limitEdit);
	//TODO add addToolTip


	slotTriggerBoxChanged();   // Dis-/enabled m_limitEdit

	connect(m_triggerBox, SIGNAL(activated(int)), SLOT(slotTriggerBoxChanged()));
}


void WatchItemIntegerBase::slotTriggerBoxChanged()
{
	Q_ASSERT(m_triggerBox != 0);
	Q_ASSERT(m_limitEdit != 0);

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

void WatchItemIntegerBase::triggerPropertyOkPressed()
{
	Q_ASSERT(m_triggerBox != 0);
	Q_ASSERT(m_limitEdit != 0);

	m_triggerType = (eTriggerType)m_triggerBox->currentItem();
	m_limit = m_limitEdit->value();

	m_triggerBox = (QComboBox *) 0;    // Will be deleted now
	m_limitEdit = (KSimBaseIntEdit *) 0;
}
void WatchItemIntegerBase::triggerPropertyDefaultPressed()
{
	Q_ASSERT(m_triggerBox != 0);
	Q_ASSERT(m_limitEdit != 0);
	m_triggerBox->setCurrentItem((int)eRising);
	m_limitEdit->setValue(0);
}
void WatchItemIntegerBase::triggerPropertyCancelPressed()
{
	Q_ASSERT(m_triggerBox != 0);
	Q_ASSERT(m_limitEdit != 0);

	m_triggerBox = (QComboBox *) 0;    // Will be deleted now
	m_limitEdit = (KSimBaseIntEdit *) 0;
}

//############################################################################################
//############################################################################################

WatchItemIntegerConnector::WatchItemIntegerConnector(ConnectorBase * connector)
	:	WatchItemIntegerBase(connector)
{
}

WatchItemIntegerConnector::~WatchItemIntegerConnector()
{
}

void WatchItemIntegerConnector::readData(unsigned int index)
{
	const int * pData = (const int *)getConnector()->readoutData();
	if (pData)
	{
		m_traceBuffer[index] = *pData;
		setIndexUsed(index);
	}
}

//############################################################################################
//############################################################################################

WatchItemIntegerWireProperty::WatchItemIntegerWireProperty(WireProperty * wireProperty)
	:	WatchItemIntegerBase(wireProperty)
{
}

WatchItemIntegerWireProperty::~WatchItemIntegerWireProperty()
{
}

void WatchItemIntegerWireProperty::readData(unsigned int index)
{
	const int * pData = (const int *)getWireProperty()->readoutData();
	if (pData)
	{
		m_traceBuffer[index] = *pData;
		setIndexUsed(index);
	}
}

#include "watchiteminteger.moc"

