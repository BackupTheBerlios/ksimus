/***************************************************************************
                          simulationtiming.cpp  -  description
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

// C-Includes
#include <stdlib.h>

// QT-Includes
#include <qtimer.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "simulationtiming.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "loglist.h"
#include "ksimtimebase.h"
#include "ksimtimeserver.h"
#include "ksimdebug.h"

// Forward declaration
static const char * sExecTime       = "Execution/";
static const char * sSimulationTime = "Simulation/";
static const char * sUpdateTime     = "Update/";
static const char * sFastest        = "Fastest";
static const char * sSynchronized   = "Sychronized";


SimulationTiming::SimulationTiming(KSimusDoc *parent, const char *name )
	:	QObject(parent,name),
		m_doc(parent)
{
	m_execTimer = new QTimer(this);
	CHECK_PTR(m_execTimer);
	connect(m_execTimer, SIGNAL(timeout()), this, SLOT(slotExecute()));
	m_updateTimer = new QTimer(this);
	CHECK_PTR(m_updateTimer);
	connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(slotUpdate()));
	
	m_execTime = new KSimTimeBase();
	CHECK_PTR(m_execTime);
	m_updateTime = new KSimTimeBase();
	CHECK_PTR(m_updateTime);
	m_tickTime = new KSimTimeBase();
	CHECK_PTR(m_tickTime);

	m_simulationTime = new KSimTimeServer();
	CHECK_PTR(m_simulationTime);

	setDefault();
}

SimulationTiming::~SimulationTiming()
{
	delete m_simulationTime;
	delete m_tickTime;
	delete m_updateTime;
	delete m_execTime;
}

void SimulationTiming::setDefault()
{
	m_execTime->setValue(10.0, unit_msec);
	m_updateTime->setValue(50.0, unit_msec);
	m_tickTime->setValue(10.0, unit_msec);
	m_execFastest = false;
	m_running = false;
	m_updateNow = false;
	m_updateSynchronized = false;
	m_tickSynchronized = true;
	updateTickTime();
	reset();
}


void SimulationTiming::reset()
{
	m_simulationTime->reset();
}

void SimulationTiming::load(KSimData & config)
{
	if (config.hasGroupRel(sExecTime))
	{
	  config.pushGroupRel(sExecTime);
  	m_execTime->load(config);
		m_execFastest = config.readBoolEntry(sFastest,false);
	  config.popGroup();
	}

	if (config.hasGroupRel(sSimulationTime))
	{
	  config.pushGroupRel(sSimulationTime);
  	m_tickTime->load(config);
		m_tickSynchronized = config.readBoolEntry(sSynchronized,false);
	  config.popGroup();
	}

	if (config.hasGroupRel(sUpdateTime))
	{
	  config.pushGroupRel(sUpdateTime);
  	m_updateTime->load(config);
		m_updateSynchronized = config.readBoolEntry(sSynchronized,false);
	  config.popGroup();
	}

  updateTickTime();
}

void SimulationTiming::save(KSimData & config) const
{
  config.pushGroupRel(sExecTime);
  m_execTime->save(config);
  if(m_execFastest)
  	config.writeEntry(sFastest,true);
  config.popGroup();

  config.pushGroupRel(sSimulationTime);
  m_tickTime->save(config);
  if(m_tickSynchronized)
  	config.writeEntry(sSynchronized,true);
  config.popGroup();

  config.pushGroupRel(sUpdateTime);
  m_updateTime->save(config);
  if(m_updateSynchronized)
  	config.writeEntry(sSynchronized,true);
  config.popGroup();

}


void SimulationTiming::slotStart()
{
	int time;
	
	m_running = true;
	if(m_execFastest)
	{
		m_execTimer->start(0, true);
	}
	else
	{
		m_execTimeTick = (int)m_execTime->getValue(unit_msec);
//		KSIMDEBUG_VAR("m_execTimer->start(m_execTimeTick)", m_execTimeTick);
		
		m_execTimer->start(m_execTimeTick, true);
		m_startTime.start();
		m_timeSum = m_execTimeTick;		// Set to next tick
	}
	
  updateTickTime();
	
	if(!m_updateSynchronized)
	{
		time = (int)m_updateTime->getValue(unit_msec);
		m_updateTimer->start(time);
//		KSIMDEBUG_VAR("m_updateTimer->start(time)", time);
	}		
}

void SimulationTiming::slotStop()
{
	m_running = false;
	m_execTimer->stop();
	m_updateTimer->stop();
	m_doc->simulateUpdate();
}

void SimulationTiming::slotExecute()
{
	m_doc->simulateExecute();
	if(m_updateNow || m_updateSynchronized)
	{
		m_updateNow = false;
		m_doc->simulateUpdate();
	}
	m_simulationTime->addTick();

	m_timeSum += m_execTimeTick;
	int next = m_timeSum - m_startTime.elapsed();
	
	if(m_execFastest)
	{
		m_execTimer->start(0, true);
	}
	else
	{
		// Limit values
		if (abs(next) > 1000)
		{
			if (abs(next) > 2000)
			{
				// Difference > 2 sec
				// Drop the difference (warp a round or time adjust)
				next = m_execTimeTick;
				m_timeSum = m_startTime.elapsed();
	
			}
			else
			{
				m_doc->getApp()->getLogList()->error(i18n("Timing difference simulation time - real time = %1 ms").arg(next));
			}
		}
		
		
		if (next > (m_execTimeTick * 2))
		{
			// Upper Limit
			next = (m_execTimeTick * 2);
		}
		else if (next < 0)
		{
			// Lower Limit
			next = 0;
		}
		
		m_execTimer->start(next, true);
	}
}

void SimulationTiming::slotUpdate()
{
	m_updateNow = true;
}

void SimulationTiming::updateTickTime()
{
  if(m_tickSynchronized)
  {
  	m_simulationTime->setTick(*m_execTime);
  }
  else
  {
  	m_simulationTime->setTick(*m_tickTime);
	}  	
}

	
