/***************************************************************************
                          integeror.h  -  description
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

#ifndef INTEGEROR_H
#define INTEGEROR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integerxin1out.h"

// Forward declaration

namespace KSimLibInteger
{

/**A bitwise integer OR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerBitwiseOr : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer OR */
	IntegerBitwiseOr(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer OR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseOr::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseOr * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################


/**An bitwise integer NOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerBitwiseNor : public IntegerXIn1Out
{
public:
	/** Constructs a bitwise integer NOR */
	IntegerBitwiseNor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise integer NOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerBitwiseNor::View : public IntegerXIn1OutView
{
public:
	View(IntegerBitwiseNor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer OR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalOr : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer OR */
	IntegerLogicalOr(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical OR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalOr::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalOr * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################
//###############################################################

/**A logical integer NOR gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class IntegerLogicalNor : public IntegerXIn1Out
{
public:
	/** Constructs a logical integer NOR */
	IntegerLogicalNor(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
	class View;
};

//###############################################################

/** The view for the bitwise logical NOR gate.
	*
  * @author Rasmus Diekenbrock
  */
class IntegerLogicalNor::View : public IntegerXIn1OutView
{
public:
	View(IntegerLogicalNor * comp, eViewType viewType)
	: IntegerXIn1OutView(comp, viewType) {};

	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibInteger
#endif
