/***************************************************************************
                          simulationtiming.h  -  description
                             -------------------
    begin                : Sat Mar 10 2001
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

#ifndef SIMULATIONTIMING_H
#define SIMULATIONTIMING_H

// C-Includes

// QT-Includes
#include <qobject.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class QTimer;
class KSimData;
class KSimusDoc;
class KSimTimeServer;
class KSimTimeBase;

/**Controls the simulation
  *@author Rasmus Diekenbrock
  */

class SimulationTiming : public QObject
{
   Q_OBJECT

friend class SimulationTimingWidget;

public:
	SimulationTiming(KSimusDoc *parent, const char *name=0);
	~SimulationTiming();

	KSimTimeServer & getTimeServer() { return *m_simulationTime; };
	bool running() const { return m_running; };
	void reset();

	void load(KSimData & config);
	void save(KSimData & config) const;
	
	void updateTickTime();
	
public slots:
	void slotStart();
	void slotStop();
  void slotExecute();
  void slotUpdate();

protected:
	

private:
	KSimusDoc * m_doc;
	QTimer * m_execTimer;
	QTimer * m_updateTimer;
	KSimTimeBase * m_execTime;
	KSimTimeBase * m_updateTime;
	KSimTimeBase * m_tickTime;
	KSimTimeServer * m_simulationTime;
	bool m_execFastest;
	bool m_running;
	bool m_updateNow;
	bool m_updateSynchronized;
	bool m_tickSynchronized;
	
signals:
	
};

#endif
