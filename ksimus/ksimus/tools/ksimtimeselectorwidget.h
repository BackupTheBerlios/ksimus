/***************************************************************************
                          ksimtimeselectorwidget.h  -  description
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

#ifndef KSIMTIMESELECTORWIDGET_H
#define KSIMTIMESELECTORWIDGET_H

#include <qwidget.h>
#include "ksimtimebase.h"

/**Time selector widget
  *@author Rasmus Diekenbrock
  */

class KSimDoubleEdit;
class QComboBox;


class KSimTimeSelectorWidget : public QWidget
{
   Q_OBJECT
public:
	KSimTimeSelectorWidget(const KSimTimeBase & time, QWidget *parent=0, const char *name=0);
	~KSimTimeSelectorWidget();

	/** Sets the time */
	void setTime(const KSimTimeBase & time);
	void setTime(const double & time, eTimeUnit unit);
	/** Sets the unit */
	void setUnit(eTimeUnit unit);
	void setUnit(const QString & unitString);
	
	/** Reads the time */
	const KSimTimeBase & getTime() const;
	
	/** Inserts a unit to the Selector */
	void insertUnit(eTimeUnit unit);
	/** Inserts all units form the Selector
			Unit tick is only inserted, if time has a time server */
	void insertAllUnit();
	/** Removes a unit from the Selector */
	void removeUnit(eTimeUnit unit);
	/** Removes all units from the Selector */
	void removeAllUnit();

protected:
	void initData();	
	void initView();
	int searchCbIdx(const QString & unitString);
	
	KSimTimeBase * m_time;
	
	KSimDoubleEdit * m_de;
	QComboBox * m_cb;
	
public slots:


protected slots:
	void slotValueChanged(double value);
	void slotUnitChanged(const QString & unitStr);
	
signals:
	void signalChanged();
};

#endif
