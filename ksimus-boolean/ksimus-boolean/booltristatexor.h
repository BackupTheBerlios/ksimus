/***************************************************************************
                      booltristatexor.h  -  description
                             -------------------
    begin                : Tue Nov 27 2002
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

#ifndef BOOLTRISTATEXOR_H
#define BOOLTRISTATEXOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "booltristate1out.h"

// Forward declaration


//###############################################################

namespace KSimLibBoolean
{

/**A boolean XOR/XNOR gate with multiple inputs and a tristate output
	*
  * @author Rasmus Diekenbrock
  */

class BoolTristateXor : public BoolTristateXIn1Out1Ena
{
	
	Q_OBJECT

public:
	/** Constructs a boolean XOR/XNOR with a tristate output */
	BoolTristateXor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticXorInfo();
	static const ComponentInfo * getStaticXnorInfo();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
