/***************************************************************************
                          ksimunit.cpp  -  description
                             -------------------
    begin                : Mon Sep 10 2001
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

// KDE-Includes
#include <klocale.h>


// Project-Includes
#include "ksimunit.h"
#include "ksimtime.h"
#include <kdebug.h>

// Forward declaration

#ifndef KSIMDEBUG
#ifdef DEBUG
#define __DEBUG_INFO__        __FILE__ << "(" << __LINE__ << ") "
#define KSIMDEBUG(str) 	 	 {kdDebug() << __DEBUG_INFO__ << str << endl;}
#define KSIMDEBUG_VAR(str,x) {kdDebug() << __DEBUG_INFO__ << str << " " << #x << " = " << x << endl;}
#define KSIMDEBUG_FUNCTION   {kdDebug() << k_funcinfo << endl;}
#else
#define KSIMDEBUG(str) 	
#define KSIMDEBUG_VAR(str,x)
#define KSIMDEBUG_FUNCTION
#endif
#endif


KSimUnitBase::KSimUnitBase(const QString & unitString, double preferMin, double preferMax, bool hidden)
	:	m_unitStr(unitString),
		m_preferMin(preferMin),
		m_preferMax(preferMax),
		m_hidden(hidden)
{
}

KSimUnitBase::~KSimUnitBase()
{
}


const QString & KSimUnitBase::getUnitString() const
{
	return m_unitStr;
}
	
bool KSimUnitBase::isHidden() const
{
	return m_hidden;
}

void KSimUnitBase::setHidden(bool hidden)
{
	m_hidden = hidden;
}

void KSimUnitBase::setMinPrefered(double min)
{
	m_preferMin = min;
}

double KSimUnitBase::getMinPrefered() const
{
	return m_preferMin;
}

void KSimUnitBase::setMaxPrefered(double max)
{
	m_preferMax = max;
}

double KSimUnitBase::getMaxPrefered() const
{
	return m_preferMax;
}

void KSimUnitBase::setPrefered(double min, double max)
{
	setMinPrefered(min);
	setMaxPrefered(max);
}


bool KSimUnitBase::isPrefered (double value) const
{
	if (isHidden()) return false;
	
	double absVal = toUnit(QABS(value));
	
	return ((absVal >= getMinPrefered()) && (absVal < getMaxPrefered()));
}

//########################################################################################
//########################################################################################

KSimProportionalUnit::KSimProportionalUnit(const QString & unitString, double factor, bool hidden)
	:	KSimUnitBase(unitString, 1.0, 1000.0, hidden),
		m_factor(factor)
{
}
	

KSimProportionalUnit::KSimProportionalUnit(const QString & unitString, double factor, double preferMin, double preferMax)
	:	KSimUnitBase(unitString, preferMin, preferMax, false),
		m_factor(factor)
{
}

double KSimProportionalUnit::fromUnit (double value) const
{
	return value * getFactor();
}

double KSimProportionalUnit::toUnit (double value) const
{
	if (getFactor() != 0)
	{
		return value / getFactor();
	}
	else if (value == 0)
	{
		return 1;
	}
	else if (value > 0)
	{
		return DBL_MAX;
	}
	else
	{
		return -DBL_MAX;
	}
}

double KSimProportionalUnit::getFactor() const
{
	return m_factor;
}


//########################################################################################
//########################################################################################


KSimUnitTicks::KSimUnitTicks(const KSimTimeServer & timeServer, const QString & name, bool hidden)
	:	KSimUnitBase(name, -DBL_MAX, DBL_MAX, hidden),
		m_timeServer(timeServer)
{
}


double KSimUnitTicks::fromUnit(double value_with_unit) const
{
	KSimTime time(m_timeServer);
	
	time.setValue(value_with_unit, unit_ticks);
	
	double d =  time.getValue(unit_sec);
	
//	KSIMDEBUG(QString::fromLatin1("fromUnit %1 = %2").arg(value_with_unit).arg(d));
	
	return d;
}

double KSimUnitTicks::toUnit(double value_without_unit) const
{
	KSimTime time(m_timeServer);
	
	time.setValue(value_without_unit, unit_sec);
	
	double d = time.getValue(unit_ticks);
	
//	KSIMDEBUG(QString::fromLatin1("toUnit %1 ==> %2").arg(value_without_unit).arg(d));
	
	return d;
}


//########################################################################################
//########################################################################################


KSimUnitList::KSimUnitList(const QString & listName)
	:	m_listName(listName),
		m_defaultUnit(),
		m_isStatic(false)
{
	setAutoDelete(true);
}

void KSimUnitList::add(const KSimUnitBase * unit)
{
	inSort(unit);
}

void KSimUnitList::setDefaultUnit(const QString & unitStr)
{
	m_defaultUnit = unitStr;
}

const QString & KSimUnitList::getDefaultUnit() const
{
	return m_defaultUnit;
}

const KSimUnitBase * KSimUnitList::findPrefered(double reference) const
{
	QListIterator<KSimUnitBase> it(*this);
	
	for (; it.current(); ++it)
	{
		if (it.current()->isPrefered(reference))
		{
			return it.current();
		}
	}

	// Not found
	if (!getDefaultUnit().isEmpty())
	{
		return findUnit(getDefaultUnit());
	}
	
	return 0;
}

const KSimUnitBase * KSimUnitList::findUnit(const QString & unitStr) const
{
	QListIterator<KSimUnitBase> it(*this);
	
	while (it.current() && (unitStr != it.current()->getUnitString()))
	{
		++it;
	}
	return it.current();	
}

	
bool KSimUnitList::isStatic() const
{
	return m_isStatic;
}

int KSimUnitList::compareItems(QCollection::Item di1, QCollection::Item di2)
{
	double res1 = ((KSimUnitBase *)di1)->fromUnit(1);
	double res2 = ((KSimUnitBase *)di2)->fromUnit(1);
	
	if(res1 < res2)
	{
		return -1;
	}
	if(res1 > res2)
	{
		return 1;
	}
	return 0;
}
	
void KSimUnitList::setStatic(bool setStatic)
{
	m_isStatic = setStatic;
}
	
const QString & KSimUnitList::getListName() const
{
	return m_listName;
}

unsigned int KSimUnitList::countHidden() const
{
	QListIterator<KSimUnitBase> it(*this);
	unsigned int count = 0;
	
	while (it.current())
	{
		if (it.current()->isHidden())
		{
			count ++;
		}
		++it;
	}
	return count;	
}

unsigned int KSimUnitList::countVisible() const
{
	QListIterator<KSimUnitBase> it(*this);
	unsigned int count = 0;
	
	while (it.current())
	{
		if (!it.current()->isHidden())
		{
			count ++;
		}
		++it;
	}
	return count;	
}

	

//########################################################################################
//########################################################################################

KSimUnitListStatic::KSimUnitListStatic(const QString & listName)
	:	KSimUnitList(listName)
{
	setStatic(true);
}


const KSimUnitListStatic & KSimUnitListStatic::getTimeUnitList()
{
	static KSimUnitListStatic * timeUnitList = 0;
	if (!timeUnitList)
	{
		timeUnitList = new KSimUnitListStatic(i18n("Time Units"));
		
		*timeUnitList
									<< new KSimProportionalUnit(QString::fromLatin1("ps"),  1e-12, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("ns"),  1e-9, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("µs"),  1e-6, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("us"),  1e-6, true)
									<< new KSimProportionalUnit(QString::fromLatin1("ms"),  1e-3, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("s"),   1e0, 1.0, DBL_MAX)
									<< new KSimProportionalUnit(QString::fromLatin1("min"), 60, true)
									<< new KSimProportionalUnit(QString::fromLatin1("h"),   60*60, true)
									<< new KSimProportionalUnit(QString::fromLatin1("a"),   24*60*60, true);
		timeUnitList->setDefaultUnit(QString::fromLatin1("s"));
	}
	
	return *timeUnitList;
}

const KSimUnitListStatic & KSimUnitListStatic::getLengthUnitList()
{
	static KSimUnitListStatic * lengthUnitList = 0;
	if (!lengthUnitList)
	{
		lengthUnitList = new KSimUnitListStatic(i18n("Length Units"));
		
		*lengthUnitList
									<< new KSimProportionalUnit(QString::fromLatin1("am"), 1e-18, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("fm"), 1e-15, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("pm"), 1e-12, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("nm"), 1e-9, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("µm"), 1e-6, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("um"), 1e-6, true)
									<< new KSimProportionalUnit(QString::fromLatin1("mm"), 1e-3, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("cm"), 1e-2, true)
									<< new KSimProportionalUnit(QString::fromLatin1("dm"), 1e-1, true)
									<< new KSimProportionalUnit(QString::fromLatin1("m"),  1e0, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("km"), 1e3, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("Mm"), 1e6, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("Gm"), 1e9, 1.0, 1000.0)
									<< new KSimProportionalUnit(QString::fromLatin1("Tm"), 1e12, 1.0, DBL_MAX/1e12);
		lengthUnitList->setDefaultUnit(QString::fromLatin1("m"));
	}
	
	return *lengthUnitList;
}

//########################################################################################
//########################################################################################

KSimMultiUnitList::KSimMultiUnitList()
{
}

KSimMultiUnitList::~KSimMultiUnitList()
{
	clear();
}
	
void KSimMultiUnitList::clear()
{
	QListIterator<KSimUnitList> it(m_list);
	
	for (; it.current(); ++it)
	{
		if (it.current()->isStatic())
		{
			m_list.removeRef(it.current());
		}
		else
		{
			delete it.current();
		}
	}
}

const KSimUnitList * KSimMultiUnitList::findUnitList(const QString & listName) const
{
	QListIterator<KSimUnitList> it(m_list);
	
	while (it.current() && (it.current()->getListName() != listName))
	{
		++it;
	}
	
	return it.current();
}
	
const KSimUnitBase * KSimMultiUnitList::findUnit(const QString & unitStr, QString * listName) const
{
	const KSimUnitBase * unit = 0;
	const KSimUnitList * list = 0;
	QListIterator<KSimUnitList> it(m_list);
	
	for (; it.current(); ++it)
	{
		unit = it.current()->findUnit(unitStr);
		if (unit)
		{
			list = it.current();
			break;
		}
	}
	
	if (listName)
	{
		*listName = list ? list->getListName() : QString::fromLatin1("");
	}
	
	return unit;
}

const KSimUnitBase * KSimMultiUnitList::findPrefered(double reference, QString * listName) const
{
	const KSimUnitBase * unit = 0;
	const KSimUnitList * list = 0;
	
	if(listName && !listName->isEmpty())
	{
		list = findUnitList(*listName);
		
		if(!list)
		{
			KSIMDEBUG_VAR("List not found", *listName);
		}
		else
		{
			unit = list->findPrefered(reference);
//			KSIMDEBUG_VAR("No unit found", *listName);
//			KSIMDEBUG(QString("reference = %1").arg(reference,'e'));
		}
	}
	
	if(!unit)
	{
		QListIterator<KSimUnitList> it(m_list);
	
		for (; it.current(); ++it)
		{
			unit = it.current()->findPrefered(reference);
			if (unit)
			{
				list = it.current();
				break;
			}
		}
	}
	
	if (listName)
	{
		*listName = list ? list->getListName() : QString::fromLatin1("");
	}
	
	return unit;
}

void KSimMultiUnitList::append(const KSimUnitList & unitList)
{
	m_list.append(&unitList);
}




