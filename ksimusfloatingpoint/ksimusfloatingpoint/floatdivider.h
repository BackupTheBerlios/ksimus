/***************************************************************************
                          floatdivider.h  -  description
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

#ifndef FLOATDIVIDER_H
#define FLOATDIVIDER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float2in1out.h"
#include "ksimus/componentinfo.h"

namespace KSimLibFloatingPoint
{
// Forward declaration

extern const ComponentInfo FloatDividerInfo;


//###############################################################

/**A floating point divider gate with two inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatDivider : public Float2In1Out
{
public:
	/** Constructs a floating point divider */
	FloatDivider(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point divider gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatDividerView : public Float2In1OutView
{
public:
	FloatDividerView(FloatDivider * comp, eViewType viewType)
		: Float2In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
