/***************************************************************************
                          ksimtimespinbox.cpp  -  description
                             -------------------
    begin                : Sun Oct 14 2001
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

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimtimespinbox.h"
#include "ksimtimeserver.h"

// Forward declaration


KSimTimeSpinBox::KSimTimeSpinBox(const KSimTimeBase & time, QWidget * parent, const char * name)
	:	KSimDoubleUnitSpinBox(parent, name),
		m_time(time)
{
	setMinValue(1e-10);
	setMaxValue(1e6);
	setLineStep(1e-3);
	
	init();
}

KSimTimeSpinBox::KSimTimeSpinBox(const KSimTimeBase & time, double min, double max, double steps, QWidget * parent, const char * name)
	:	KSimDoubleUnitSpinBox(min,max,steps,parent, name),
		m_time(time)
{
	init();
}

KSimTimeSpinBox::~KSimTimeSpinBox()
{
}
	
void KSimTimeSpinBox::init()
{
	getMultiUnitList().append(KSimUnitListStatic::getTimeUnitList());
		
	if (m_time.hasTimeServer())
	{
		KSimUnitList * tickList = new KSimUnitList("Ticks");
		CHECK_PTR(tickList);
		
		// Add some ticks
		KSimUnitTicks * ticks;
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "ticks");
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "Ticks", true);
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "tick", true);
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "Tick", true);
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "T", true);
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		ticks = new KSimUnitTicks(m_time.getTimeServer(), "t", true);
		CHECK_PTR(ticks);
		tickList->add(ticks);
		
		getMultiUnitList().append(*tickList);
	}

	setNewExponentSteps(1.0,2.0,5.0);
	
	setValue(m_time);
	
	connect(this, SIGNAL(valueChanged(double)), this, SLOT(slotChanged(double)));
}


void KSimTimeSpinBox::setValue(const KSimTimeBase & time)
{
	if (!fixedUnit())
	{
		if (time.getUnit() == unit_ticks)
		{
			setUnitListString("Ticks");
		}
		else
		{
			setUnitListString(KSimUnitListStatic::getTimeUnitList().getListName());
		}
	}
		
	m_time = time;
	
	KSimDoubleUnitSpinBox::setValue(m_time.getValue(unit_sec));
}

void KSimTimeSpinBox::slotChanged(double sec)
{
	m_time.setValue(sec, unit_sec);
	
	if (getUnitListString() == QString("Ticks"))
	{
		m_time.setUnit(unit_ticks);
	}
	else
	{
		m_time.setUnit(unit_sec);
	}
	
	emit changed();
	emit changed(m_time);
}

const KSimTimeBase & KSimTimeSpinBox::value() const
{
	return m_time;
}


