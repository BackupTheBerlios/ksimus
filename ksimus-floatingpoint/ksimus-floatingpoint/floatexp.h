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
#include "float2in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

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

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
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
		: Float1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point power gate.
  * @author Rasmus Diekenbrock
  */

class FloatPow : public Float2In1Out
{
public:
	/** Constructs a floating point power gate */
	FloatPow(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point power gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatPowView : public Float2In1OutView
{
public:
	FloatPowView(FloatPow * comp, eViewType viewType)
		: Float2In1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point square root gate.
  * @author Rasmus Diekenbrock
  */

class FloatSqrt : public Float1In1Out
{
public:
	/** Constructs a floating point square root */
	FloatSqrt(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point square root gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatSqrtView : public Float1In1OutView
{
public:
	FloatSqrtView(FloatSqrt * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};

	virtual void draw(QPainter * p);
};

//###############################################################



};  //namespace KSimLibFloatingPoint
#endif
