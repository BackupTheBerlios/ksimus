/***************************************************************************
                          ksimtimebase.cpp  -  description
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

#include "ksimtimebase.h"


#include <stdio.h>

#include <qdict.h>
#include <qstring.h>

#include "ksimdata.h"
#include "ksimtime.h"
#include "ksimdebug.h"
#include "ksimtimeserver.h"
#include "enumdict.h"

static EnumDict<eTimeUnit> timeUnitDict;

EnumDict<eTimeUnit>::tData EnumDict<eTimeUnit>::data[]
			= { {"Ticks",    unit_ticks},
          {"1/10nsec", unit_tenth_nsec},
					{"nsec",     unit_nsec},
					{"usec",     unit_usec},
					{"µsec",     unit_usec},
					{"msec",     unit_msec},
					{"sec",      unit_sec},
					{"min",      unit_min},
					{"hour",     unit_hour},
					{"day",      unit_day},
          {0,         (eTimeUnit)0}};


eTimeUnit KSimTimeBase::convertUnit(const char * unitString)
{
	return timeUnitDict.find(unitString, unit_msec);
}

const char * KSimTimeBase::convertUnit(eTimeUnit unit)
{
	return timeUnitDict.find(unit);
}

KSimTimeBase::KSimTimeBase(const KSimTimeServer & timeServer)
	:	m_time(0),
		m_unit(unit_tenth_nsec),
		m_timeServer(&timeServer)
{
}

KSimTimeBase::KSimTimeBase()
	:	m_time(0),
		m_unit(unit_tenth_nsec),
		m_timeServer(0)
{
}

KSimTimeBase::KSimTimeBase(const KSimTimeBase & time)
	:	m_time(time.m_time),
		m_unit(time.m_unit),
		m_timeServer(time.m_timeServer)
{
}

KSimTimeBase::KSimTimeBase(double time, eTimeUnit unit)
	:	m_timeServer(0)
{
	setValue(time,unit);
}

KSimTimeBase::KSimTimeBase(const KSimTimeServer & timeServer, double time, eTimeUnit unit)
	:	m_timeServer(&timeServer)
{
	setValue(time,unit);
}

KSimTimeBase::KSimTimeBase(KSimTimeType time)
	:	m_time(time),
		m_unit(unit_tenth_nsec),
		m_timeServer(0)
{
}

KSimTimeBase::KSimTimeBase(const KSimTimeServer & timeServer, KSimTimeType time)
	:	m_time(time),
		m_unit(unit_tenth_nsec),
		m_timeServer(&timeServer)
{
}



	
KSimTimeBase::~KSimTimeBase()
{
}

bool KSimTimeBase::load(KSimData & config)
{
	bool loadOk = true;
	
	KSimTimeType newTime;
	char currentTime[40];
	sprintf(currentTime,"%lli",m_time);
	
	// Load time (the number)
	QString tmp = config.readEntry("Time",currentTime);
	int res = sscanf(tmp,"%lli", &newTime);
	if (1 == res)
	{
		m_time = newTime;
	}
	else
	{
		loadOk = false;
		KSIMDEBUG_VAR("load failed", config.readEntry("Time"));
	}

	// Load unit
	const eTimeUnit * pUnit = timeUnitDict.load(config, "Unit");
	if (pUnit)
	{
		m_unit = *pUnit;
	}
	else
	{
		loadOk = false;
		KSIMDEBUG_VAR("load failed", config.readEntry("Unit"));
	}
	
	if (!loadOk)
	{
		//default
		setValue(10.0, unit_msec);
	}
	return loadOk;
}

void KSimTimeBase::save(KSimData & config) const
{
	char tmp[40];
	sprintf(tmp,"%lli",m_time);
	config.writeEntry("Time",tmp);

	timeUnitDict.save(config, "Unit", m_unit);
}

eTimeUnit KSimTimeBase::getUnit() const
{
	return m_unit;
}

void KSimTimeBase::setUnit(eTimeUnit unit)
{
	if (unit == m_unit) return;
	
	if (unit == unit_ticks)
	{
		if(hasTimeServer())
		{
			// Time server exist
			m_time /= getTimeServer().getTick().raw();
		}
		else
		{
			KSIMDEBUG("Unit ticks is not allowed without a timeserver");
			return;
		}
	}
	else if (m_unit == unit_ticks)
	{
		if(hasTimeServer())
		{
			// Time server exist
			m_time *= getTimeServer().getTick().raw();
		}
		else
		{
			KSIMDEBUG("Unit ticks is not allowed without a timeserver");
			return;
		}
	}
	m_unit = unit;
}

void KSimTimeBase::setUnit(const char * unitStr)
{
	const eTimeUnit * pUnit(timeUnitDict.find(unitStr));
	if (pUnit)
	{
		setUnit(*pUnit);
	}
	else
	{
		KSIMDEBUG(QString("Unknown unit %1. Skip!").arg(unitStr));
	}
}

const char * KSimTimeBase::getUnitStr() const
{
	return timeUnitDict.find(m_unit);
}

const QString KSimTimeBase::getValueString(eTimeUnit unit, int precision) const
{
	if (unit == unit_tenth_nsec) unit = unit_nsec;
	
	QString str;
	str.sprintf("%.*f %s",precision ,getValue(unit),convertUnit(unit));
	return str;
}

const QString KSimTimeBase::getAdjustValueString(int precision) const
{
	eTimeUnit unit = m_unit;
	
	if (m_time < KSIMTIME_SEC)
	{
		if (m_time < KSIMTIME_MSEC)
		{
			if (m_time < KSIMTIME_USEC)
			{
				unit = unit_nsec;
			}
			else
			{
				unit = unit_usec;
			}
		}
		else
		{
			unit = unit_msec;
		}
	}
	else
	{
		if (m_time < KSIMTIME_HOUR)
		{
			if (m_time < KSIMTIME_MIN)
			{
				unit = unit_sec;
			}
			else
			{
				unit = unit_min;
			}
		}
		else
		{
			if (m_time < KSIMTIME_DAY)
			{
				unit = unit_hour;
			}
			else
			{
				unit = unit_day;
			}
		}
	}
	
	return getValueString(unit, precision);
}

double KSimTimeBase::getValue(eTimeUnit unit) const
{
	double value;
		
	switch(unit)
	{
		case unit_ticks:
			if(hasTimeServer())
			{
				value = (double)raw() / m_timeServer->getTick().raw();
			}
			else
			{
				KSIMDEBUG("Unit ticks is not allowed without a timeserver");
				value = 0.0;
			}
			break;
			
		case unit_tenth_nsec:
			value = (double)raw();
			break;
			
		case unit_nsec:
			value = (double)raw() / KSIMTIME_NSEC;
			break;
			
		case unit_usec:
			value = (double)raw() / KSIMTIME_USEC;
			break;
			
		case unit_msec:
			value = (double)raw() / KSIMTIME_MSEC;
			break;
			
		case unit_sec:
			value = (double)raw() / KSIMTIME_SEC;
			break;
			
		case unit_min:
			value = (double)raw() / KSIMTIME_MIN;
			break;
			
		case unit_hour:
			value = (double)raw() / KSIMTIME_HOUR;
			break;
			
		case unit_day:
			value = (double)raw() / KSIMTIME_DAY;
			break;
	
		default:
			ASSERT(0);
			value = (double)raw();
			break;
	}
	return value;
}

void KSimTimeBase::setValue(double time, eTimeUnit unit)
{
	m_unit = unit;
	switch(unit)
	{
		case unit_ticks:
			if (hasTimeServer())
			{
				m_time = (KSimTimeType)time;
			}
			else
			{
				KSIMDEBUG("Unit ticks is not allowed without a timeserver");
				m_time = 0;
				m_unit = unit_tenth_nsec;
			}
			break;
			
		case unit_tenth_nsec:
			m_time = (KSimTimeType)(time);
			break;
			
		case unit_nsec:
			m_time = (KSimTimeType)(time * KSIMTIME_NSEC);
			break;
			
		case unit_usec:
			m_time = (KSimTimeType)(time * KSIMTIME_USEC);
			break;
			
		case unit_msec:
			m_time = (KSimTimeType)(time * KSIMTIME_MSEC);
			break;
			
		case unit_sec:
			m_time = (KSimTimeType)(time * KSIMTIME_SEC);
			break;
			
		case unit_min:
			m_time = (KSimTimeType)(time * KSIMTIME_MIN);
			break;
			
		case unit_hour:
			m_time = (KSimTimeType)(time * KSIMTIME_HOUR);
			break;
			
		case unit_day:
			m_time = (KSimTimeType)(time * KSIMTIME_DAY);
			break;
	
		default:
			ASSERT(0);
			m_time = (KSimTimeType)(time);
			break;
	}
}

void KSimTimeBase::setValue(double time, const char * unitStr)
{
	const eTimeUnit * pUnit(timeUnitDict.find(unitStr));
	if (pUnit)
	{
		setValue(time, *pUnit);
	}
	else
	{
		KSIMDEBUG(QString("Unknown unit %1. Skip!").arg(unitStr));
	}
}

int KSimTimeBase::tenth_nsec() const
{
	return (raw() % 10);
}

int KSimTimeBase::nsec() const
{
	return ((raw()/KSIMTIME_NSEC) % 1000);
}

int KSimTimeBase::usec() const
{
	return ((raw()/KSIMTIME_USEC) % 1000);
}

int KSimTimeBase::msec() const
{
	return ((raw()/KSIMTIME_MSEC) % 1000);
}

int KSimTimeBase::sec() const
{
	return ((raw()/KSIMTIME_SEC) % 60);
}

int KSimTimeBase::min() const
{
	return ((raw()/KSIMTIME_MIN) % 60);
}

int KSimTimeBase::hour() const
{
	return ((raw()/KSIMTIME_HOUR) % 24);
}

int KSimTimeBase::day() const
{
	return (raw()/KSIMTIME_DAY);
}

void KSimTimeBase::reset()
{
	m_time = 0;
}

const KSimTimeBase & KSimTimeBase::operator=(const KSimTimeBase & time)
{
	if (this != &time)
	{
		m_time = time.m_time;
		m_unit = time.m_unit;
		m_timeServer = time.m_timeServer;
	}
	return time;
}

void KSimTimeBase::setTimeServer(const KSimTimeServer & timeServer)
{
	m_timeServer = &timeServer;
}

void KSimTimeBase::clearTimeServer()
{
	m_timeServer = 0;
}

const KSimTimeServer & KSimTimeBase::getTimeServer() const
{
	return *m_timeServer;
}

bool KSimTimeBase::hasTimeServer() const
{
	return m_timeServer != 0;
}

KSimTimeType KSimTimeBase::raw() const
{
	if (m_unit == unit_ticks)
	{
		if (hasTimeServer())
		{
			return m_time * getTimeServer().getTick().raw();
		}
		else
		{
			KSIMDEBUG("Unit ticks is not allowed without a timeserver");
			return 0;
		}
	}
	else
	{
		return m_time;
	}
}

void KSimTimeBase::setRaw(const KSimTimeType & time)
{
	if (m_unit == unit_ticks)
	{
		if (hasTimeServer())
		{
			m_time = time / getTimeServer().getTick().raw();
		}
		else
		{
			KSIMDEBUG("Unit ticks is not allowed without a timeserver");
		}
	}
	else
	{
		m_time = time;
	}
}

