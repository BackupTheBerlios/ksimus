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

class IntegerBitwiseAnd : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer AND */
	IntegerBitwiseAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer AND gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseAnd::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseAnd * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################


/**A bitwise integer NAND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerBitwiseNand : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer NAND */
	IntegerBitwiseNand(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer NAND gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseNand::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseNand * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer AND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalAnd : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer AND */
	IntegerLogicalAnd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical AND gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalAnd::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalAnd * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer NAND gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalNand : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer NAND */
	IntegerLogicalNand(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical NAND gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalNand::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalNand * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
