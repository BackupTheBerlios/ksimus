/***************************************************************************
                          floatmax.h  -  description
                             -------------------
    begin                : Sat Feb 23 2002
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

#ifndef FLOATMAX_H
#define FLOATMAX_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "floatxin1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

extern const ComponentInfo FloatMaxInfo;


//###############################################################

/**A floating point maximum gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatMax : public FloatXIn1Out
{
public:
	/** Constructs a floating point maximum gate */
	FloatMax(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point maximum gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatMaxView : public FloatXIn1OutView
{
public:
	FloatMaxView(FloatMax * comp, eViewType viewType)
		: FloatXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
