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
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

const ComponentInfo * getFloatCosInfo();
const ComponentInfo * getFloatACosInfo();


//###############################################################

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

};

//###############################################################

/** The view for the floating point cosinusoidal gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatCosView : public Float1In1OutView
{
public:
	FloatCosView(FloatCos * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


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

};

//###############################################################

/** The view for the floating point inverse cosinusoidal gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatACosView : public Float1In1OutView
{
public:
	FloatACosView(FloatACos * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
