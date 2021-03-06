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

// Project-Includes
#include "flipflopbase.h"

// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ComponentLayoutSimple;


namespace KSimLibBoolean
{


//###############################################################
//###############################################################

/**A D FF.
  *@author Rasmus Diekenbrock
  */

class DFlipFlop : public KSimLibBoolean::FlipFlopBase
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

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorBoolIn * m_inD;
	ConnectorBoolInEdge * m_inEna;
};

//###############################################################
//###############################################################


class DFlipFlopView : public CompView
{
public:
	DFlipFlopView(DFlipFlop * comp, eViewType viewType, const char * name);
//	~DFlipFlopView();
//	virtual void draw(QPainter * p);

private:
	DFlipFlop * getDFF() const { return (DFlipFlop *) getComponent(); };

	ComponentLayoutSimple * m_layout;
};


//###############################################################



}; //namespace KSimLibBoolean

#endif
