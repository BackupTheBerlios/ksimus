/***************************************************************************
                          floatsubtractor.h  -  description
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

#ifndef FLOATSUBTRACTOR_H
#define FLOATSUBTRACTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float2in1out.h"
#include "ksimus/componentinfo.h"

// Forward declaration

namespace KSimLibFloatingPoint
{

/**A floating point subtractor gate with two inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatSubtractor : public Float2In1Out
{
public:
	/** Constructs a floating point subtractor */
	FloatSubtractor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point subtractor gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatSubtractorView : public Float2In1OutView
{
public:
	FloatSubtractorView(FloatSubtractor * comp, eViewType viewType)
		: Float2In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
