/***************************************************************************
                          floatmultiplier.h  -  description
                             -------------------
    begin                : Wed Jan 02 2002
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

#ifndef FLOATMULITPILIER_H
#define FLOATMULITPILIER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "floatxin1out.h"
#include "ksimus/componentinfo.h"


// Forward declaration

namespace KSimLibFloatingPoint
{

extern const ComponentInfo FloatMultiplierInfo;


//###############################################################

/**A floating point multiplier gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatMultiplier : public FloatXIn1Out
{
public:
	/** Constructs a floating point multiplier */
	FloatMultiplier(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point multiplier gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatMultiplierView : public FloatXIn1OutView
{
public:
	FloatMultiplierView(FloatMultiplier * comp, eViewType viewType)
		: FloatXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
