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

class BoolTriStateXor : public BoolTriStateXIn1Out1Ena
{
	
	Q_OBJECT

public:
	/** Constructs a boolean XOR/XNOR with a tristate output */
	BoolTriStateXor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticXorInfo();
	static const ComponentInfo * getStaticXnorInfo();

};

//###############################################################

/** The view for the boolean XOR/XNOR gates with a tristate output.
	*
  * @author Rasmus Diekenbrock
  */
class BoolTriStateXorView : public BoolTriStateXIn1Out1EnaView
{

	Q_OBJECT

public:
	BoolTriStateXorView(BoolTriStateXor * comp, eViewType viewType)
		: BoolTriStateXIn1Out1EnaView(QString::fromLatin1("=1"), comp, viewType) {};
};

//###############################################################

}; //namespace KSimLibBoolean

#endif