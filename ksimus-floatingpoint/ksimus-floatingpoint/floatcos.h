/***************************************************************************
                          floatcos.h  -  description
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

#ifndef FLOATCOS_H
#define FLOATCOS_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

/**A floating point cosinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatCos : public Float1In1Out
{
public:
	/** Constructs a floating point cosinus */
	FloatCos(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse cosinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatACos : public Float1In1Out
{
public:
	/** Constructs a floating point inverse cosinusoidal */
	FloatACos(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point hyperbolic cosinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatCosh : public Float1In1Out
{
public:
	/** Constructs a floating point cosinus */
	FloatCosh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A floating point inverse hyperbolic cosinusoidal gate.
  * @author Rasmus Diekenbrock
  */

class FloatACosh : public Float1In1Out
{
public:
	/** Constructs a floating point inverse cosinusoidal */
	FloatACosh(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
