/***************************************************************************
                          floattan.h  -  description
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

#ifndef FLOATTAN_H
#define FLOATTAN_H

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

/**A floating point tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatTan : public Float1In1Out
{
public:
	/** Constructs a floating point tangent */
	FloatTan(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatATan : public Float1In1Out
{
public:
	/** Constructs a floating point inverse tangent */
	FloatATan(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse tangent gate with 2 inputs.
  * @author Rasmus Diekenbrock
  */

class FloatATan2 : public Float2In1Out
{
public:
	/** Constructs a floating point inverse tangent with 2 inputs */
	FloatATan2(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point hyperbolic tangent gate.
  * @author Rasmus Diekenbrock
  */

class FloatTanh : public Float1In1Out
{
public:
	/** Constructs a floating point tangential */
	FloatTanh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse hyperbolic tangential gate.
  * @author Rasmus Diekenbrock
  */

class FloatATanh : public Float1In1Out
{
public:
	/** Constructs a floating point inverse tangent */
	FloatATanh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
