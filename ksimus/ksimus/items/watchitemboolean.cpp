/***************************************************************************
                          watchitemboolean.cpp  -  description
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

// QT-Includes
#include <qcombobox.h>
#include <qlayout.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "watchitemboolean.h"
#include "wirepropertyboolean.h"
#include "connectorbase.h"

// Forward declaration


WatchItemBooleanBase::WatchItemBooleanBase(ConnectorBase * connector)
	:	WatchItemBase(connector)
{
	init();
}

WatchItemBooleanBase::WatchItemBooleanBase(WireProperty * wireProperty)
	:	WatchItemBase(wireProperty)
{
	init();
}

WatchItemBooleanBase::~WatchItemBooleanBase()
{
}

void WatchItemBooleanBase::init()
{
	m_traceBuffer.resize(getTraceSize());
	m_traceBuffer.fill(false);
	m_triggerType = False2True;
	m_triggerBox = (QComboBox*) 0;
}


/** Resets the item. */
void WatchItemBooleanBase::reset()
{
	WatchItemBase::reset();
	m_traceBuffer.fill(false);
}


void WatchItemBooleanBase::setTraceSize(unsigned int size)
{
	WatchItemBase::setTraceSize(size);

	m_traceBuffer.resize(getTraceSize());
	m_traceBuffer.fill(false);

}

QString WatchItemBooleanBase::getDataText(unsigned int index)
{
	return WirePropertyBoolean::getI18nTextValue(m_traceBuffer[index]);
}

bool WatchItemBooleanBase::testBreak(unsigned int index, unsigned int stepCount)
{
	switch(m_triggerType)
	{
		case False2True:
			if ((m_traceBuffer[index] == true) && (stepCount != 0))
			{
				unsigned int previous = (index != 0) ? index-1 : getTraceSize()-1;
				return !m_traceBuffer[previous];
			}
			break;
			
		case True2False:
			if ((m_traceBuffer[index] == false) && (stepCount != 0))
			{
				unsigned int previous = (index != 0) ? index-1 : getTraceSize()-1;
				return m_traceBuffer[previous];
			}
			break;
			
		case FalseState:
			return !m_traceBuffer[index];
			break;
			
		case TrueState:
			return m_traceBuffer[index];
			break;
	}
	return false;
}

void WatchItemBooleanBase::createTriggerPropertyWidget(QWidget * parent)
{
	ASSERT(m_triggerBox == 0);
	
	m_triggerBox = new QComboBox(parent, "m_triggerBox");
	CHECK_PTR(m_triggerBox);

	m_triggerBox->insertItem(i18n("Boolean Trigger", "False to True transition"), (int)False2True);
	m_triggerBox->insertItem(i18n("Boolean Trigger", "True to False transition"), (int)True2False);
	m_triggerBox->insertItem(i18n("Boolean Trigger", "False state"), (int)FalseState);
	m_triggerBox->insertItem(i18n("Boolean Trigger", "True state"), (int)TrueState);

	m_triggerBox->setCurrentItem((int)m_triggerType);
}

void WatchItemBooleanBase::triggerPropertyOkPressed()
{
	ASSERT(m_triggerBox != 0);

	m_triggerType = (eTriggerType)m_triggerBox->currentItem();

	m_triggerBox = 0;    // Will be deleted now
}
void WatchItemBooleanBase::triggerPropertyDefaultPressed()
{
	ASSERT(m_triggerBox != 0);
	m_triggerBox->setCurrentItem((int)False2True);
}
void WatchItemBooleanBase::triggerPropertyCancelPressed()
{
	ASSERT(m_triggerBox != 0);

	m_triggerBox = 0;    // Will be deleted now
}

//############################################################################################
//############################################################################################

WatchItemBooleanConnector::WatchItemBooleanConnector(ConnectorBase * connector)
	:	WatchItemBooleanBase(connector)
{
}

WatchItemBooleanConnector::~WatchItemBooleanConnector()
{
}

void WatchItemBooleanConnector::readData(unsigned int index)
{
	const bool * pData = (const bool *)getConnector()->readoutData();
	if (pData)
	{
		m_traceBuffer.setBit(index, *pData);
		setIndexUsed(index);
	}
}

//############################################################################################
//############################################################################################

WatchItemBooleanWireProperty::WatchItemBooleanWireProperty(WireProperty * wireProperty)
	:	WatchItemBooleanBase(wireProperty)
{
}

WatchItemBooleanWireProperty::~WatchItemBooleanWireProperty()
{
}

void WatchItemBooleanWireProperty::readData(unsigned int index)
{
	const bool * pData = (const bool *)getWireProperty()->readoutData();
	if (pData)
	{
		m_traceBuffer.setBit(index, *pData);
		setIndexUsed(index);
	}
}

#include "watchitemboolean.moc"

