/***************************************************************************
                          integerinverter.h  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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

#ifndef INTEGERINVERTER_H
#define INTEGERINVERTER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1in1out.h"


namespace KSimLibInteger
{

// Forward declaration

/**A bitwise integer inverter gate.
	*
  * @author Rasmus Diekenbrock
  */

class IntegerInverter : public Integer1In1Out
{
public:
	/** Constructs a bitwise integer inverter */
	IntegerInverter(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibInteger
#endif
