/***************************************************************************
                          dflipflop.h  -  description
                             -------------------
    begin                : Tue Dec 11 2001
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

#ifndef DFLIPFLOP_H
#define DFLIPFLOP_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"

// Project-Includes
#include "flipflopbase.h"

// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ComponentLayout;


namespace KSimLibBoolean
{

extern const ComponentInfo DFlipFlopInfo;


//###############################################################
//###############################################################

/**A D FF.
  *@author Rasmus Diekenbrock
  */

class DFlipFlop : public FlipFlopBase
{
	Q_OBJECT

public:
	DFlipFlop(CompContainer * container, const ComponentInfo * ci);
//	~DFlipFlop();


	/** Executes the simulation of this component */
	virtual void calculate();

	/** Returns the "D" input connector.
	  */
	ConnectorBoolIn * getDataInputConnector() const { return m_inD; };
	/** Returns the "Clk" input connector.
	  */
	ConnectorBoolInEdge * getEnableInputConnector() const { return m_inEna; };

private:
	ConnectorBoolIn * m_inD;
	ConnectorBoolInEdge * m_inEna;
};

//###############################################################
//###############################################################


class DFlipFlopView : public CompView
{
public:
	DFlipFlopView(DFlipFlop * comp, eViewType viewType);
//	~DFlipFlopView();
	virtual void draw(QPainter * p);

private:
	ComponentLayout * m_layout;
};


//###############################################################



}; //namespace KSimLibBoolean

#endif
