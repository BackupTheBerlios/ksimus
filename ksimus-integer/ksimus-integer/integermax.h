/***************************************************************************
                          integermax.h  -  description
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

#ifndef INTEGERMAX_H
#define INTEGERMAX_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerxin1out.h"


namespace KSimLibInteger
{

// Forward declaration

/**An integer maximum gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerMax : public IntegerXIn1Out
{
public:
	/** Constructs a integer maximum gate */
	IntegerMax(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the integer maximum gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerMax::View : public IntegerXIn1OutView
{
public:
	View(IntegerMax * comp, eViewType viewType)
		: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
