/***************************************************************************
                          integersubtractor.h  -  description
                             -------------------
    begin                : Fri Aug 1 2003
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

#ifndef INTEGERSUBTRACTOR_H
#define INTEGERSUBTRACTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer2in1out.h"

// Forward declaration

namespace KSimLibInteger
{

/**An integer subtractor gate with two inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerSubtractor : public Integer2In1Out
{
public:
	/** Constructs a integer subtractor */
	IntegerSubtractor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the integer subtractor gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerSubtractor::View : public Integer2In1OutView
{
public:
	View(IntegerSubtractor * comp, eViewType viewType)
		: Integer2In1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
