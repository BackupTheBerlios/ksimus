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


namespace KSimLibFloatingPoint
{

// Forward declaration

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

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

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

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point hyperbolic sinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatSinh : public Float1In1Out
{
public:
	/** Constructs a floating point sinus */
	FloatSinh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse hyperbolic sinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatASinh : public Float1In1Out
{
public:
	/** Constructs a floating point sinus */
	FloatASinh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
