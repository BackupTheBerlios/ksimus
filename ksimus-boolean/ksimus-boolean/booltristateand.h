/***************************************************************************
                      booltristateand.h  -  description
                             -------------------
    begin                : Sun Nov 17 2002
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

#ifndef BOOLTRISTATEAND_H
#define BOOLTRISTATEAND_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "booltristate1out.h"

// Forward declaration


//###############################################################

namespace KSimLibBoolean
{

/**A boolean AND /NAND gate with multiple inputs and a tristate output
	*
  * @author Rasmus Diekenbrock
  */

class BoolTristateAnd : public BoolTristateXIn1Out1Ena
{
	
	Q_OBJECT

public:
	/** Constructs a boolean AND/NAND with a tristate output */
	BoolTristateAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticAndInfo();
	static const ComponentInfo * getStaticNandInfo();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
