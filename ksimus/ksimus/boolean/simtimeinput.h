/***************************************************************************
                          simtimeinput.h  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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

#ifndef SIMTIMEINPUT_H
#define SIMTIMEINPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"

// Forward declaration
class ConnectorFloatOut;

/**A component which returns the current simulation time.
  *@author Rasmus Diekenbrock
  */


class SimTimeInput : public Component
{

Q_OBJECT

public:
	SimTimeInput(CompContainer * container, const ComponentInfo * ci);
	~SimTimeInput();
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	ConnectorFloatOut * m_out;
};

//###############################################################

class SimTimeInputView : public CompView
{
public:
	SimTimeInputView(SimTimeInput * comp, eViewType viewType, const char * name);
	
	virtual void draw(QPainter * p);
};


//###############################################################
//###############################################################


/**A component which returns the duration between two simulation cycles.
  *@author Rasmus Diekenbrock
  */


class TickTimeInput : public Component
{

Q_OBJECT

public:
	TickTimeInput(CompContainer * container, const ComponentInfo * ci);
	~TickTimeInput();
	/** Reset all simulation variables */
	virtual void reset();
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	ConnectorFloatOut * m_out;
};

//###############################################################

class TickTimeInputView : public CompView
{
public:
	TickTimeInputView(TickTimeInput * comp, eViewType viewType, const char * name);
	
	virtual void draw(QPainter * p);
};


#endif
