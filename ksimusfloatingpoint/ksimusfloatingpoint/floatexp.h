/***************************************************************************
                          floatexp.h  -  description
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

#ifndef FLOATEXP_H
#define FLOATEXP_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

extern const ComponentInfo FloatExpInfo;


//###############################################################

/**A floating point exponent gate.
  * @author Rasmus Diekenbrock
  */

class FloatExp : public Float1In1Out
{
public:
	/** Constructs a floating point Exp */
	FloatExp(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point exponent gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatExpView : public Float1In1OutView
{
public:
	FloatExpView(FloatExp * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
