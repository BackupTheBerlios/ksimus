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
#include "componentinfo.h"

// Forward declaration
class ConnectorFloatOut;

/**A component which returns the current simulation time.
  *@author Rasmus Diekenbrock
  */


const ComponentInfo * getSimTimeInputInfo();

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
	
protected:
	ConnectorFloatOut * m_out;
};

//###############################################################

class SimTimeInputView : public CompView
{
public:
	SimTimeInputView(SimTimeInput * comp, eViewType viewType);
	
	virtual void draw(QPainter * p);
};


//###############################################################
//###############################################################


/**A component which returns the duration between two simulation cycles.
  *@author Rasmus Diekenbrock
  */


const ComponentInfo * getTickTimeInputInfo();

class TickTimeInput : public Component
{

Q_OBJECT

public:
	TickTimeInput(CompContainer * container, const ComponentInfo * ci);
	~TickTimeInput();
	/** Reset all simulation variables */
	virtual void reset();
	
protected:
	ConnectorFloatOut * m_out;
};

//###############################################################

class TickTimeInputView : public CompView
{
public:
	TickTimeInputView(TickTimeInput * comp, eViewType viewType);
	
	virtual void draw(QPainter * p);
};


#endif
