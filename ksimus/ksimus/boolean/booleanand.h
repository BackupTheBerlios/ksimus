/***************************************************************************
                          booleanand.h  -  description
                             -------------------
    begin                : Sat Feb 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef BOOLEANAND_H
#define BOOLEANAND_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "booleanxin1out.h"

// Forward declaration

extern const ComponentInfoList BooleanAndList;


//###############################################################

/**A boolean AND /NAND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class BooleanAnd : public BooleanXIn1Out
{
public:
	/** Constructs a boolean AND/NAND */
	BooleanAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the boolean AND/NAND gates.
	*
  * @author Rasmus Diekenbrock
  */
class BooleanAndView : public BooleanXIn1OutView
{
public:
	BooleanAndView(BooleanAnd * comp, eViewType viewType)
		: BooleanXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################

#endif
