/***************************************************************************
                          rsflipflop.h  -  description
                             -------------------
    begin                : Sun Aug 19 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef RSFLIPFLOP_H
#define RSFLIPFLOP_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "flipflopbase.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"

// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolIn;


/**A simple RS FF
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{

extern const ComponentInfo RSFlipFlopInfo;

class RSFlipFlop : public FlipFlopBase
{
Q_OBJECT

public:
	RSFlipFlop(CompContainer * container, const ComponentInfo * ci);
//	~RSFlipFlop();
	
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################
//###############################################################


class RSFlipFlopView : public CompView
{
public:
	RSFlipFlopView(RSFlipFlop * comp, eViewType viewType);
//	~RSFlipFlopView();
	virtual void draw(QPainter * p);
};


//###############################################################



}; //namespace KSimLibBoolean

#endif

