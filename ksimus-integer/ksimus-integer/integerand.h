/***************************************************************************
                          integerand.h  -  description
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

#ifndef INTEGERAND_H
#define INTEGERAND_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerxin1out.h"

// Forward declaration

namespace KSimLibInteger
{

/**An bitwise integer AND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerAnd : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer AND */
	IntegerAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################
//###############################################################


/**A bitwise integer NAND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerNand : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer NAND */
	IntegerNand(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################


};  //namespace KSimLibInteger
#endif
