/***************************************************************************
                          floatremainder.h  -  description
                             -------------------
    begin                : Fri Feb 1 2002
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

#ifndef FLOATREMAINDER_H
#define FLOATREMAINDER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float2in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{
// Forward declaration

/**This class computes the remainder from the division of numerator by denominator.
  *@author Rasmus Diekenbrock
  */

class FloatRemainder : public Float2In1Out
{
public:
	/** Constructs a floating point divider */
	FloatRemainder(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/** The view for the floating point remainder gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatRemainderView : public Float2In1OutView
{
public:
	FloatRemainderView(FloatRemainder * comp, eViewType viewType)
		: Float2In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
