/***************************************************************************
                          integermultiplier.h  -  description
                             -------------------
    begin                : Fri Aug 1 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef INTEGERMULITPILIER_H
#define INTEGERMULITPILIER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerxin1out.h"

// Forward declaration

namespace KSimLibInteger
{

/**An integer multiplier gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerMultiplier : public IntegerXIn1Out
{
public:
	/** Constructs a integer multiplier */
	IntegerMultiplier(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the integer multiplier gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerMultiplier::View : public IntegerXIn1OutView
{
public:
	View(IntegerMultiplier * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
