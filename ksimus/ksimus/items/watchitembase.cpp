/***************************************************************************
                          watchitembase.cpp  -  description
                             -------------------
    begin                : Sun Jul 14 2002
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
#include <limits.h>

// QT-Includes
#include <qcheckbox.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qhbox.h>
#include <qvgroupbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "watchitembase.h"
#include "connectorbase.h"
#include "component.h"
#include "wireproperty.h"
#include "watchviewitem.h"
#include "watchwidget.h"
#include "wire.h"
#include "ksimdebug.h"
#include "ksimdialog.h"
#include "propertywidget.h"

// Forward declaration

WatchItemBase::WatchItemBase(ConnectorBase * connector)
	:	QObject(connector),
		m_wireProperty((WireProperty *)0),
		m_connector(connector),
		m_watchViewItem((WatchViewItem *)0)
{
	init();
	connect(connector, SIGNAL(signalSetName(const QString &)), SLOT(slotNameChanged()));
	connect(connector->getComponent(), SIGNAL(signalSetName(const QString &)), SLOT(slotNameChanged()));
}

WatchItemBase::WatchItemBase(WireProperty * wireProperty)
	:	QObject(wireProperty),
		m_wireProperty(wireProperty),
		m_connector((ConnectorBase *)0),
		m_watchViewItem((WatchViewItem *)0)
{
	init();
	connect(wireProperty->getWire(), SIGNAL(signalSetName(const QString &)), SLOT(slotNameChanged()));
}


WatchItemBase::~WatchItemBase()
{
	KSIMDEBUG("WatchItemBase::~WatchItemBase()");
	getWatchWidget()->delWatchItem(this);
	delete m_watchViewItem;
}

void WatchItemBase::init()
{
	setTraceSize(getWatchWidget()->getTraceSize());
	setBreaksRequired(1);
	
	m_traceSet.resize(getTraceSize());
	m_traceBreakCnt.fill(-1, getTraceSize());
	m_breakEnabled = false;
	
	reset();
}



void WatchItemBase::setTraceSize(unsigned int size)
{
	m_traceSize = size;
	m_traceSet.fill(false, size);

}

void WatchItemBase::reset()
{
	setBreaksActual(getBreaksRequired());
	m_traceSet.fill(false);
	m_traceBreakCnt.fill(-1, getTraceSize());
}

bool WatchItemBase::execute(unsigned int index, unsigned int stepCount)
{
	m_traceSet.clearBit(index);
	readData(index);
	if (isBreakEnabled() && testBreak(index, stepCount))
	{
		int breakAct = getBreaksActual()-1;
		setBreaksActual(breakAct);
		return (breakAct == 0);
	}
	m_traceBreakCnt[index] = getBreaksActual();
	return false;
}

void WatchItemBase::showData(unsigned int index)
{
	if (m_traceSet[index])
	{
		getViewItem()->setValueText(getDataText(index));
	}
	else
	{
		getViewItem()->setValueText(getUnsetText());
	}
}

void WatchItemBase::setBreaksRequired(unsigned int number)
{
	/*m_breaksActual = */m_breaksRequired = number;
	if (!getWatchWidget()->getStepCount())
		showBreakCount(number);
}

void WatchItemBase::setBreaksActual(unsigned int number)
{
	m_breaksActual = number;
	showBreakCount(number);
}

void WatchItemBase::showBreakCount(int cnt)
{
	if (!getViewItem()) return;  // No view item present
	

	if (isBreakEnabled())
	{
		getViewItem()->setBreakEvents(cnt);
	}
	else
	{
		getViewItem()->setBreakEvents(-1);
	}
}

void WatchItemBase::showBreakCountHistory(unsigned int index)
{
	showBreakCount(m_traceBreakCnt[index]);
}

void WatchItemBase::setBreakEnabled(bool ena)
{
	m_breakEnabled = ena;
	showBreakCount(getBreaksActual());
};

QString WatchItemBase::getWatchName() const
{
	if (getConnector())
	{
		return getConnector()->getFullName();
	}
		
	if (getWireProperty() && getWireProperty()->getWire())
	{
		return getWireProperty()->getWire()->getName();
	}
	
	return QString::fromLatin1("Undefined");
}

WatchWidget * WatchItemBase::getWatchWidget() const
{
	if (getConnector())
		return getConnector()->getWatchWidget();
		
	return getWireProperty()->getWatchWidget();
}

void WatchItemBase::setWatchViewItem(WatchViewItem * item)
{
	m_watchViewItem = item;
}

QString WatchItemBase::getUnsetText()
{
	static QString str(i18n("Unknown watchpoint value", "<Unknown>"));
	return str;
}

void WatchItemBase::slotNameChanged()
{
	getViewItem()->setName(getWatchName());
}

void WatchItemBase::executePropertyDialog(QWidget * parent)
{
	KSimDialog dia(i18n("Watchpoint dialog caption","Watchpoint property"), parent);

	QWidget * wid = dia.addVBoxPage(getWatchName());
	PropertyWidget * propWid = createPropertyWidget(wid);
	dia.connectSlots(propWid);

	dia.exec();
}


PropertyWidget * WatchItemBase::createPropertyWidget(QWidget * parent)
{
	PropertyWidget * wid = new PropertyWidget(1, parent, "Watchpoint Dialog");
	CHECK_PTR(wid);

	wid->setLeftColStretch(0);
	wid->setCenterColStretch(0);
	wid->setRightColStretch(1);
	
	wid->setTopRowStretch(0);
	wid->setCenterRowStretch(0);
	wid->setBottomRowStretch(1);

	m_propertyWidget_enaCheckBox = new QCheckBox(i18n("watchpoint break enable", "Enable break condition"), wid, "watchpoint break enable");
	CHECK_PTR(m_propertyWidget_enaCheckBox);
	PropertyWidget::addToolTip(i18n("Enables the break function of the watchpoint."),
	                           m_propertyWidget_enaCheckBox);
	PropertyWidget::addWhatsThis(i18n("Enables the break function of the watchpoint.\n\n"
	                                  "The trigger condition below is checked each simulation cycle if the box is checked.\n"
	                                  "The simulation is paused after a count of trigger events are occurred. Set the count "
	                                  "of trigger events in the spinbox below."),
	                             m_propertyWidget_enaCheckBox);
	

	QHBox * cntBox = new QHBox(wid);
	CHECK_PTR(cntBox);

	QLabel * cntLabel = new QLabel(i18n("watchpoint trigger count", "Trigger count:"), cntBox, "watchpoint count label");
	CHECK_PTR(cntLabel);
	
	m_propertyWidget_cntSpinBox = new QSpinBox(1, INT_MAX, 1, cntBox, "watchpoint count spinbox");
	CHECK_PTR(m_propertyWidget_cntSpinBox);
	PropertyWidget::addToolTip(i18n("Set the count of trigger events until simulation breaks."),
	                           m_propertyWidget_cntSpinBox, cntLabel);
	PropertyWidget::addWhatsThis(i18n("Enables the break function of the watchpoint.\n\n"
	                                  "The simulation is paused after a count of trigger events are occurred. Set the count "
	                                  "of trigger events in this spinbox."),
	                             m_propertyWidget_cntSpinBox, cntLabel);


	QVGroupBox * trigBox = new QVGroupBox(i18n("watchpoint trigger condition", "Trigger condition"), wid, "watchpoint trigger condition");
	CHECK_PTR(trigBox);

	connect(wid, SIGNAL(signalAccept()), this, SLOT(slotPropertyOk()));
	connect(wid, SIGNAL(signalDefault()), this, SLOT(slotPropertyDefault()));
	connect(wid, SIGNAL(signalCancel()), this, SLOT(slotPropertyCancel()));
	connect(m_propertyWidget_enaCheckBox, SIGNAL(toggled(bool)), cntBox, SLOT(setEnabled(bool)));
	connect(m_propertyWidget_enaCheckBox, SIGNAL(toggled(bool)), trigBox, SLOT(setEnabled(bool)));

	// Setup values
	m_propertyWidget_enaCheckBox->setChecked(isBreakEnabled());
	m_propertyWidget_cntSpinBox->setValue(getBreaksRequired());
	cntBox->setEnabled(isBreakEnabled());
	trigBox->setEnabled(isBreakEnabled());

	createTriggerPropertyWidget(trigBox);

	return wid;
}

void WatchItemBase::propertyOkPressed()
{
	setBreakEnabled(m_propertyWidget_enaCheckBox->isChecked());
	this->setBreaksRequired(m_propertyWidget_cntSpinBox->value());
	
	triggerPropertyOkPressed();
}

void WatchItemBase::propertyDefaultPressed()
{
	m_propertyWidget_enaCheckBox->setChecked(false);
	m_propertyWidget_cntSpinBox->setValue(1);

	triggerPropertyDefaultPressed();
}

void WatchItemBase::propertyCancelPressed()
{
	triggerPropertyCancelPressed();
}


void WatchItemBase::triggerPropertyOkPressed()
{
}
void WatchItemBase::triggerPropertyDefaultPressed()
{
}
void WatchItemBase::triggerPropertyCancelPressed()
{
}

void WatchItemBase::slotPropertyOk()
{
	propertyOkPressed();
}
void WatchItemBase::slotPropertyDefault()
{
	propertyDefaultPressed();
}
void WatchItemBase::slotPropertyCancel()
{
	propertyCancelPressed();
}
