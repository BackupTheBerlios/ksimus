/***************************************************************************
                          floatsin.h  -  description
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

#ifndef FLOATSIN_H
#define FLOATSIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

extern const ComponentInfo FloatSinInfo;
extern const ComponentInfo FloatASinInfo;


//###############################################################

/**A floating point sinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatSin : public Float1In1Out
{
public:
	/** Constructs a floating point sinus */
	FloatSin(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point sinusoidal gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatSinView : public Float1In1OutView
{
public:
	FloatSinView(FloatSin * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


//###############################################################

/**A floating point inverse sinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatASin : public Float1In1Out
{
public:
	/** Constructs a floating point inverse sinusoidal */
	FloatASin(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point inverse sinusoidal gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatASinView : public Float1In1OutView
{
public:
	FloatASinView(FloatASin * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
