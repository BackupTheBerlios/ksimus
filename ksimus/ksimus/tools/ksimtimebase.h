/***************************************************************************
                          ksimtimebase.h  -  description
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

#ifndef KSIMTIMEBASE_H
#define KSIMTIMEBASE_H

#include <qstring.h>
#include <qstringlist.h>

/**Base class for all timing classes
  *@author Rasmus Diekenbrock
  */

#define KSIMTIME_NSEC	10LL
#define KSIMTIME_USEC	(KSIMTIME_NSEC * 1000)
#define KSIMTIME_MSEC	(KSIMTIME_USEC * 1000)
#define KSIMTIME_SEC	(KSIMTIME_MSEC * 1000)
#define KSIMTIME_MIN	(KSIMTIME_SEC  * 60)
#define KSIMTIME_HOUR	(KSIMTIME_MIN  * 60)
#define KSIMTIME_DAY	(KSIMTIME_HOUR * 24)

#define KSIMTIME_UNITS	9

class QString;
class KSimData;
class KSimTimeServer;

typedef signed long long KSimTimeType;

enum eTimeUnit { unit_ticks = 0, unit_tenth_nsec, unit_nsec, unit_usec,
				 unit_msec, unit_sec, unit_min, unit_hour, unit_day };


class KSimTimeBase
{
public:
	KSimTimeBase();
	KSimTimeBase(const KSimTimeServer & timeServer);
	KSimTimeBase(const KSimTimeBase & time);
	KSimTimeBase(double time, eTimeUnit unit);
	KSimTimeBase(const KSimTimeServer & timeServer, double time, eTimeUnit unit);
	KSimTimeBase(KSimTimeType time);
	KSimTimeBase(const KSimTimeServer & timeServer, KSimTimeType time);
	
	~KSimTimeBase();

	bool operator==(const KSimTimeBase & time) const { return ((raw() == time.raw()) && (getUnit() == time.getUnit()));};
	bool operator!=(const KSimTimeBase & time) const { return !(*this == time);};
	bool operator>(const KSimTimeBase & time)  const { return (raw() >  time.raw());};
	bool operator>=(const KSimTimeBase & time) const { return (raw() >= time.raw());};
	bool operator<(const KSimTimeBase & time)  const { return (raw() <  time.raw());};
	bool operator<=(const KSimTimeBase & time) const { return (raw() <= time.raw());};
	const KSimTimeBase & operator+=(KSimTimeType time){ setRaw(raw() + time);return (*this);};
	const KSimTimeBase & operator+=(const KSimTimeBase & time){ setRaw(raw() + time.raw());return (*this);};
	const KSimTimeBase & operator-=(KSimTimeType time){ setRaw(raw() - time);return (*this);};
	const KSimTimeBase & operator-=(const KSimTimeBase & time){ setRaw(raw() - time.raw());return (*this);};

	const KSimTimeBase & operator=(const KSimTimeBase & time);
	
	bool load(KSimData & config);
	void save(KSimData & config) const;

	int tenth_nsec() const;
	int nsec() const;
	int usec() const;
	int msec() const;
	int sec() const;
	int min() const;
	int hour() const;
	int day() const;

	KSimTimeType raw() const;
	void setRaw(const KSimTimeType & time);
	
	const char * getUnitStr() const;
	
	void reset();
	
	eTimeUnit getUnit() const;
	void setUnit(eTimeUnit unit);
	void setUnit(const char * unitStr);
	
	QString getValueString(eTimeUnit unit, int precision = 2) const;
	QString getValueString(int precision = 2) const { return getValueString(m_unit, precision); };
	QString getAdjustValueString(int precision = 2) const;
	double getValue(eTimeUnit unit) const;
	double getValue() const { return getValue(m_unit);};
	void setValue(double time, eTimeUnit unit);
	void setValue(double time) { setValue(time,m_unit);};
	void setValue(double time, const char * unitStr);
	void setTimeServer(const KSimTimeServer & timeServer);
	void clearTimeServer();
	const KSimTimeServer & getTimeServer() const;
	bool hasTimeServer() const;

	/** Converts a unit string to a eTimeUnit
	  * Returns unit_msec if the string is not valid
	  */
	static eTimeUnit convertUnit(const char * unitString);
	static const char * convertUnit(eTimeUnit unit);
	
protected:
	KSimTimeType m_time;
	eTimeUnit m_unit;
	const KSimTimeServer * m_timeServer;
};
#endif
