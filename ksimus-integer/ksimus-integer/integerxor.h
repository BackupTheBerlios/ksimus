/***************************************************************************
                          integerxor.h  -  description
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

#ifndef INTEGERXOR_H
#define INTEGERXOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerxin1out.h"

// Forward declaration

namespace KSimLibInteger
{

/**A bitwise integer XOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerXor : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer XOR */
	IntegerXor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################
//###############################################################


/**A bitwise integer XNOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerXnor : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer XNOR */
	IntegerXnor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibInteger
#endif
