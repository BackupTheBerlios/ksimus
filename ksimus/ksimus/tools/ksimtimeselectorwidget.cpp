/***************************************************************************
                          ksimtimeselectorwidget.cpp  -  description
                             -------------------
    begin                : Sat Nov 25 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#include <limits.h>

#include <qcombobox.h>
#include <qlayout.h>


#include "ksimtimeselectorwidget.h"
#include "ksimtimebase.h"
#include "ksimdoubleedit.h"
#include "ksimdebug.h"

KSimTimeSelectorWidget::KSimTimeSelectorWidget(const KSimTimeBase & time, QWidget *parent, const char *name )
	:	QWidget(parent,name)
{
	m_time = new KSimTimeBase(time);

	initView();
	initData();
	connect(m_de,SIGNAL(valueChanged(double)), this, SLOT(slotValueChanged(double)));
	connect(m_cb,SIGNAL(activated(const QString &)), this, SLOT(slotUnitChanged(const QString &)));
}

KSimTimeSelectorWidget::~KSimTimeSelectorWidget()
{
	delete m_time;
}

/** Sets the time */
void KSimTimeSelectorWidget::setTime(const KSimTimeBase & time)
{
	*m_time = time;
	initData();
}

void KSimTimeSelectorWidget::setTime(const double & time, eTimeUnit unit)
{
	if ((unit != unit_ticks) || (m_time->hasTimeServer()))
	{
		m_de->setValue(time);
		setUnit(unit);
	}
	else
	{
		KSIMDEBUG("Unit ticks is not allowed without a timeserver");
	}
}


int KSimTimeSelectorWidget::searchCbIdx(const QString & unitString)
{
	int idx = -1;
	int i;
	
	for(i=0; i<m_cb->count(); i++)
	{
		if (unitString == m_cb->text(i))
		{
			idx = i;
			break;
		}
	}
	return idx;
}


void KSimTimeSelectorWidget::setUnit(eTimeUnit unit)
{
	setUnit(QString::fromLatin1(KSimTimeBase::convertUnit(unit)));
}

void KSimTimeSelectorWidget::setUnit(const QString & unitString)
{
	m_cb->setCurrentItem(searchCbIdx(unitString));
}


/** Reads the time */
const KSimTimeBase & KSimTimeSelectorWidget::getTime() const
{
	return *m_time;
}



void KSimTimeSelectorWidget::initData()
{
	m_de->setValue(m_time->getValue());
	setUnit(m_time->getUnit());
}

void KSimTimeSelectorWidget::initView()
{
	m_de = new KSimDoubleEdit((double)0, (double)1000000,16,this, "Time Input");
	Q_CHECK_PTR(m_de);
	
	m_cb = new QComboBox(this, "Unit Select");
	Q_CHECK_PTR(m_cb);
	
	insertAllUnit();
	
	QBoxLayout * mainLayout = new QHBoxLayout( this );
	Q_CHECK_PTR(mainLayout);
	mainLayout->addWidget(m_de,1);
	mainLayout->addWidget(m_cb,0);
}

void KSimTimeSelectorWidget::slotValueChanged(double value)
{
	m_time->setValue(value);
	emit signalChanged();
}

void KSimTimeSelectorWidget::slotUnitChanged(const QString & unitStr)
{
	eTimeUnit unit = KSimTimeBase::convertUnit(unitStr);
	double value = m_time->getValue();
	m_time->setValue(value, unit);
	emit signalChanged();
}

/** Inserts a unit to the Selector */
void KSimTimeSelectorWidget::insertUnit(eTimeUnit unit)
{
	if ((unit != unit_ticks) || (m_time->hasTimeServer()))
	{
		m_cb->insertItem(QString::fromLatin1(KSimTimeBase::convertUnit(unit)));
	}
}

/** Inserts all units form the Selector
		Unit tick is only inserted, if time has a time server */
void KSimTimeSelectorWidget::insertAllUnit()
{
	for (int i = 0; i < KSIMTIME_UNITS;i++)
	{
		insertUnit(eTimeUnit(i));
	}	
}

/** Removes a unit from the Selector */
void KSimTimeSelectorWidget::removeUnit(eTimeUnit unit)
{
	int idx;
	idx = searchCbIdx(QString::fromLatin1(KSimTimeBase::convertUnit(unit)));
	if (idx != -1)
		m_cb->removeItem(idx);
}

/** Removes all units from the Selector */
void KSimTimeSelectorWidget::removeAllUnit()
{
	m_cb->clear();
}

#include "ksimtimeselectorwidget.moc"
