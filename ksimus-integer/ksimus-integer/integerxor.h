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

class IntegerBitwiseXor : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer XOR */
	IntegerBitwiseXor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer XOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseXor::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseXor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################


/**A bitwise integer XNOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerBitwiseXnor : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer XNOR */
	IntegerBitwiseXnor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer XNOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseXnor::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseXnor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer XOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalXor : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer XOR */
	IntegerLogicalXor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical XOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalXor::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalXor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer XNOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalXnor : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer XNOR */
	IntegerLogicalXnor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical XNOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalXnor::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalXnor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
