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

// KSimus-Includes
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"

// Project-Includes
#include "flipflopbase.h"

// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolIn;


/**A simple RS FF
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{

class RSFlipFlop : public KSimLibBoolean::FlipFlopBase
{
Q_OBJECT

public:
	RSFlipFlop(CompContainer * container, const ComponentInfo * ci);
//	~RSFlipFlop();
	
	/** Executes the simulation of this component */
	virtual void calculate();


	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################
//###############################################################


class RSFlipFlopView : public CompView
{
public:
	RSFlipFlopView(RSFlipFlop * comp, eViewType viewType);
//	~RSFlipFlopView();
	virtual void draw(QPainter * p);

	RSFlipFlop* getRSFF() { return (RSFlipFlop *) getComponent(); };
};


//###############################################################



}; //namespace KSimLibBoolean

#endif

