/***************************************************************************
                          integerinverterarithmetic.h  -  description
                             -------------------
    begin                : Sun Aug 17 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef INTEGERINVERTERARITHMETIC_H
#define INTEGERINVERTERARITHMETIC_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1in1out.h"


namespace KSimLibInteger
{

// Forward declaration

/**An arithmetic integer inverter gate.
	*
  * @author Rasmus Diekenbrock
  */

class IntegerArithmeticInverter : public Integer1In1Out
{
public:
	/** Constructs an integer negate */
	IntegerArithmeticInverter(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the arithmetic integer inverter gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerArithmeticInverter::View : public Integer1In1OutView
{
public:
	View(IntegerArithmeticInverter * comp, eViewType viewType)
		: Integer1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
