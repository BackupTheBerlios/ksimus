/***************************************************************************
                          simulationexecute.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
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

// QT-Includes
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compcontainer.h"
#include "module.h"
#include "ksimusdoc.h"
#include "simulationexecute.h"
#include "ksimtime.h"
#include "ksimtimeserver.h"
#include "wire.h"
#include "wireproperty.h"
#include "ksimdebug.h"

// Forward declaration


// #define MYDEBUG_ENABLE 1

#ifdef MYDEBUG_ENABLE
#define MYDEBUG(a)       KSIMDEBUG(a)
#define MYDEBUG_VAR(a,b) KSIMDEBUG_VAR(a,b)
#else
#define MYDEBUG(a)
#define MYDEBUG_VAR(a,b)
#endif


//##########################################################
//##########################################################

class TimedExecute
{
public:
	TimedExecute(Component * comp, unsigned int timerNo, const KSimTime & time)
		:	m_component(comp),
			m_timerNo(timerNo),
			m_time(time)
	{};
	
//	~TimedExecute();

	Component * getComponent() { return m_component; };
	const Component * getComponent() const { return m_component; };
	
	const KSimTime & getTime() const { return m_time; };
	
	unsigned int getTimerNo() const { return m_timerNo; };


private:
	Component * m_component;
	unsigned int m_timerNo;
	KSimTime m_time;
};



class TimedExecuteList : public QList<TimedExecute>
{
public:
	TimedExecuteList()
	{
		setAutoDelete(true);
	};
	

	void addTimer(Component * comp, unsigned int timerNo, const KSimTime & time);
	void removeTimer(Component * comp, unsigned int timerNo);


protected:
	virtual int compareItems(QCollection::Item di1, QCollection::Item di2);
	
};

// Compares the time
int TimedExecuteList::compareItems(QCollection::Item cdiA, QCollection::Item cdiB)
{
	if (((TimedExecute*)cdiA)->getTime() > ((TimedExecute*)cdiB)->getTime())
	{
		return 1;
	}
	else if (((TimedExecute*)cdiA)->getTime() < ((TimedExecute*)cdiB)->getTime())
	{
		return -1;
	}
	
	return 0;
}


// Removes a timer
void TimedExecuteList::removeTimer(Component * comp, unsigned int timerNo)
{
	unsigned int idx;
	for(idx = 0; idx < count(); idx++)
	{
		TimedExecute * item = at(idx);
		if((item->getComponent() == comp) && (item->getTimerNo() == timerNo))
		{
			remove(); // current item == at(idx);
			break;
		}
	}
}

// Add or update a timer
void TimedExecuteList::addTimer(Component * comp, unsigned int timerNo, const KSimTime & time)
{
	removeTimer(comp, timerNo);
	
	TimedExecute * newItem = new TimedExecute(comp, timerNo, time);
	CHECK_PTR(newItem);
	
	inSort(newItem);
}


//###############################################################################
//###############################################################################

SimulationExecute::SimulationExecute(KSimusDoc * parent)
	:	QObject(parent),
		m_parent(parent)
{
	m_nextComponent = new ComponentList[2];
	CHECK_PTR(m_nextComponent);
	m_nextComponent[0].setAutoDelete(false);
	m_nextComponent[1].setAutoDelete(false);
	
	m_timedList = new TimedExecuteList();
	CHECK_PTR(m_timedList);
	
	
	m_wirePropertyList = new WirePropertyList[2];
	CHECK_PTR(m_wirePropertyList);
	
	
	// init m_execList, m_addList & m_nextCompIndex
	reset();
	

}
	

SimulationExecute::~SimulationExecute()
{
	delete[] m_nextComponent;
	delete m_timedList;
	delete[] m_wirePropertyList;
}
	
	
	
	
void SimulationExecute::reset()
{
	m_nextCompIndex = 0;
	m_execList = &m_nextComponent[0];
	m_nextComponent[0].clear();
	
	m_addList = &m_nextComponent[1];
	m_nextComponent[1].clear();
	
	m_timedList->clear();
	
	m_execWirePropertyList = &m_wirePropertyList[0];
	m_wirePropertyList[0].clear();
	
	m_addWirePropertyList = &m_wirePropertyList[1];
	m_wirePropertyList[1].clear();
	
	addComponents(m_parent->getContainer());
	

}
	
void SimulationExecute::execute()
{
	// Check components from timed list
	// and psuh it to the execution list
	KSimTime time = m_parent->getTimeServer().getTime();
	MYDEBUG_VAR("Execute Time:", time.getAdjustValueString());
	
	while(m_timedList->first() && (m_timedList->first()->getTime() <= time))
	{
		MYDEBUG_VAR("Timed Execute", m_timedList->first()->getComponent()->getName());
		executeComponentNext(m_timedList->first()->getComponent());
		m_timedList->remove();
	};
			
	
	// Toggle index
	m_nextCompIndex ^= 1;
	
	m_execWirePropertyList = &m_wirePropertyList[m_nextCompIndex ? 1 : 0];
	m_addWirePropertyList = &m_wirePropertyList[m_nextCompIndex ? 0 : 1];
	m_addWirePropertyList->clear();
	
	// Execute WirePropertys
	FOR_EACH_WIREPROPERTY(itWireProperty, *m_execWirePropertyList)
	{
		MYDEBUG(QString::fromLatin1("WireProp Execute %1").arg(itWireProperty.current()->getComponent()->getName()));
		itWireProperty.current()->execute();
	}
	m_execWirePropertyList->clear();
	
	
	
	m_execList = &m_nextComponent[m_nextCompIndex ? 1 : 0];
	m_addList = &m_nextComponent[m_nextCompIndex ? 0 : 1];
	m_addList->clear();
	
	// Calulate
	FOR_EACH_COMP(it,*m_execList)
	{
		MYDEBUG_VAR("Comp Execute", it.current()->getName());
		it.current()->calculate();
	}
}
	
void SimulationExecute::executeComponentNext(Component * comp)
{
	if (-1 == m_addList->findRef(comp))
	{
		// Component does not exist in list
		m_addList->append(comp);
		MYDEBUG(QString::fromLatin1("executeComponentNext %1").arg(comp->getName()));
	}
	else
	{
		MYDEBUG(QString::fromLatin1("executeComponentNext already in list %1").arg(comp->getName()));
	}
	
}


void SimulationExecute::addComponents(CompContainer * container)
{
	FOR_EACH_COMP(it,*container->getComponentList())
	{
		if (it.current()->isWire())
		{
			if (((Wire *)it.current())->getWireProperty())
			{
				executeWirePropertyNext(((Wire *)it.current())->getWireProperty());
			}
		}
		else if (it.current()->isModule())
		{
			Module * module = (Module*)it.current();
			addComponents(module->getModuleContainer());
		}
		else
		{
			executeComponentNext(it.current());
		}
	}
}


void SimulationExecute::executeComponentAt(Component * comp, unsigned int timerNo, const KSimTime & time)
{
	m_timedList->addTimer(comp, timerNo, time);	
	MYDEBUG(QString::fromLatin1("executeComponentAt %1 %2 %3").arg(comp->getName()).arg(time.getAdjustValueString()).arg(timerNo));
}


void SimulationExecute::executeComponentAfter(Component * comp, unsigned int timerNo, const KSimTime & diffTime)
{
	KSimTime t(diffTime);
	t += m_parent->getTimeServer().getTime();
	m_timedList->addTimer(comp, timerNo, t);	
	MYDEBUG(QString::fromLatin1("executeComponentAfter %1 %2 %3").arg(comp->getName()).arg(diffTime.getAdjustValueString()).arg(timerNo));
}

/** Adds a wire property in the list for execute next cycle. */
void SimulationExecute::executeWirePropertyNext(WireProperty * wireProperty)
{
	if (-1 == m_addWirePropertyList->findRef(wireProperty))
	{
		// WireProperty does not exist in list
		m_addWirePropertyList->append(wireProperty);
		MYDEBUG(QString::fromLatin1("executeWirePropertyNext %1").arg(wireProperty->getComponent()->getName()));
	}
	else
	{
		MYDEBUG(QString::fromLatin1("executeWirePropertyNext already in list %1").arg(wireProperty->getComponent()->getName()));
	}

}
