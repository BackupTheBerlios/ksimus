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

// QT-Includes
#include <qtimer.h>

// KDE-Includes
#include <kconfigbase.h>

// Project-Includes
#include "simulationtiming.h"
#include "ksimusdoc.h"
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
		m_doc(parent),
		m_execFastest(false),
		m_running(false),
		m_updateNow(false),
		m_updateSynchronized(false),
		m_tickSynchronized(true)
{
	m_execTimer = new QTimer(this);
	connect(m_execTimer,SIGNAL(timeout()),
					this,SLOT(slotExecute()));
	m_updateTimer = new QTimer(this);
	connect(m_updateTimer,SIGNAL(timeout()),
					this,SLOT(slotUpdate()));
	
	
	m_execTime = new KSimTimeBase(10.0, unit_msec);
	m_updateTime = new KSimTimeBase(50.0, unit_msec);
	m_tickTime = new KSimTimeBase(10.0, unit_msec);

	m_simulationTime = new KSimTimeServer();

  updateTickTime();
}

SimulationTiming::~SimulationTiming()
{
	delete m_simulationTime;
	delete m_tickTime;
	delete m_updateTime;
	delete m_execTime;
}

void SimulationTiming::reset()
{
	m_simulationTime->reset();
}

void SimulationTiming::load(KConfigBase & config)
{
	QString group;

  group = config.group();

	if (config.hasGroup(group + sExecTime))
	{
	  config.setGroup(group + sExecTime);
  	m_execTime->load(config);
		m_execFastest = config.readBoolEntry(sFastest,false);
	}

	if (config.hasGroup(group + sSimulationTime))
	{
	  config.setGroup(group + sSimulationTime);
  	m_tickTime->load(config);
		m_tickSynchronized = config.readBoolEntry(sSynchronized,false);
	}

	if (config.hasGroup(group + sUpdateTime))
	{
	  config.setGroup(group + sUpdateTime);
  	m_updateTime->load(config);
		m_updateSynchronized = config.readBoolEntry(sSynchronized,false);
	}

  config.setGroup(group);

  updateTickTime();
}

void SimulationTiming::save(KConfigBase & config) const
{
	QString group;

  group = config.group();

  config.setGroup(group + sExecTime);
  m_execTime->save(config);
  if(m_execFastest)
  	config.writeEntry(sFastest,true);

  config.setGroup(group + sSimulationTime);
  m_tickTime->save(config);
  if(m_tickSynchronized)
  	config.writeEntry(sSynchronized,true);

  config.setGroup(group + sUpdateTime);
  m_updateTime->save(config);
  if(m_updateSynchronized)
  	config.writeEntry(sSynchronized,true);

  config.setGroup(group);
  	
}


void SimulationTiming::slotStart()
{
	int time;
	
	m_running = true;
	if(m_execFastest)
	{
		m_execTimer->start(0);
	}
	else
	{
		time = (int)m_execTime->getValue(unit_msec);
		m_execTimer->start(time);
//		KSIMDEBUG_VAR("m_execTimer->start(time)", time);
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
	m_doc->slotSimulateUpdate();
}

void SimulationTiming::slotExecute()
{
	m_doc->slotSimulateExecute();
	if(m_updateNow || m_updateSynchronized)
	{
		m_updateNow = false;
		m_doc->slotSimulateUpdate();
	}
	m_simulationTime->addTick();
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

	
