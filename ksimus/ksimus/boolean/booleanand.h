/***************************************************************************
                          booleanand.h  -  description
                             -------------------
    begin                : Sat Feb 17 2001
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

#ifndef BOOLEANAND_H
#define BOOLEANAND_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "componentinfo.h"

// Forward declaration
class ConnectorPack;
class ConnectorBoolOut;

/**A boolean and gate
  *@author Rasmus Diekenbrock
  */
extern "C" const ComponentInfoList BooleanAndList;

class BooleanAnd : public Component
{
public:
	/** Constructs a boolean AND/NAND */
	BooleanAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Shift the result of calculation to output */
	virtual void updateOutput();
	/** Reset all simulation variables */
	virtual void reset();

	/** Returns the output connector.
	  */
	ConnectorBoolOut * getOutputConnector() const { return m_out; };
	/** Returns the input connector pack.
	  */
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };
	

protected:
	bool m_result;
	ConnectorBoolOut * m_out;
	ConnectorPack * m_inPack;

};

//###############################################################

class BooleanAndView : public CompView
{
public:
	BooleanAndView(BooleanAnd * comp, eViewType viewType);
	~BooleanAndView();
	virtual void draw(QPainter * p);
};

//###############################################################

#endif
