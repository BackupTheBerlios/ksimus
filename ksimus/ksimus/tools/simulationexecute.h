/***************************************************************************
                          simulationexecute.h  -  description
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

#ifndef SIMULATIONEXECUTE_H
#define SIMULATIONEXECUTE_H

// C-Includes

// QT-Includes
#include <qobject.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class KSimusDoc;
class KSimTime;
class ComponentList;
class Component;
class CompContainer;
class TimedExecuteList;
class WireProperty;
class WirePropertyList;

/**The execution unit.
  *@author Rasmus Diekenbrock
  */

class SimulationExecute : public QObject
{
	Q_OBJECT

public:
	SimulationExecute(KSimusDoc * parent);
	~SimulationExecute();


	
public:
	/** Resets the execution unit. */
	void reset();
	
	/** Executes components which are ready to run. */
	void execute();
	
	/** Adds a component in the list for execute next cycle. */
	void executeComponentNext(Component * comp);
	
	/** Adds a component in the timed list. */
	void executeComponentAt(Component * comp, unsigned int timerNo, const KSimTime & time);
	/** Adds a component in the timed list. */
	void executeComponentAfter(Component * comp, unsigned int timerNo, const KSimTime & diffTime);
	
	/** Adds a wire property in the list for execute next cycle. */
	void executeWirePropertyNext(WireProperty * wireProperty);

private:
	/** Adds all components of a container and sub container (Modules) to the add list. */
	void addComponents(CompContainer * container);

  KSimusDoc * m_parent;
	
	ComponentList * m_nextComponent;
	ComponentList * m_execList;
	ComponentList * m_addList;
	unsigned int m_nextCompIndex;
	TimedExecuteList * m_timedList;
	WirePropertyList * m_wirePropertyList;
	WirePropertyList * m_execWirePropertyList;
	WirePropertyList * m_addWirePropertyList;
};

#endif
