/***************************************************************************
                          integerabs.h  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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

#ifndef INTEGERABS_H
#define INTEGERABS_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1in1out.h"


namespace KSimLibInteger
{

// Forward declaration

/**An integer absolute gate.
	*
  * @author Rasmus Diekenbrock
  */

class IntegerAbs : public Integer1In1Out
{
public:
	/** Constructs an integer ABS */
	IntegerAbs(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibInteger
#endif
