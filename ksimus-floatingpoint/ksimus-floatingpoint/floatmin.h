/***************************************************************************
                          floatmin.h  -  description
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

#ifndef FLOATMIN_H
#define FLOATMIN_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "floatxin1out.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

/**A floating point minimum gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatMin : public FloatXIn1Out
{
public:
	/** Constructs a floating point minimum gate */
	FloatMin(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

};  //namespace KSimLibFloatingPoint
#endif
