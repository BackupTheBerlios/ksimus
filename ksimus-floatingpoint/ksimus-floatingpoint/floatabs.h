/***************************************************************************
                          floatabs.h  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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

#ifndef FLOATABS_H
#define FLOATABS_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

const ComponentInfo * getFloatAbsInfo();


//###############################################################

/**A floating point absolute gate.
	*
  * @author Rasmus Diekenbrock
  */

class FloatAbs : public Float1In1Out
{
public:
	/** Constructs a floating point ABS */
	FloatAbs(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point absolute gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatAbsView : public Float1In1OutView
{
public:
	FloatAbsView(FloatAbs * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
