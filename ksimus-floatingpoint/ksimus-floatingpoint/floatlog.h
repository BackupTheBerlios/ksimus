/***************************************************************************
                          floatlog.h  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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

#ifndef FLOATLOG_H
#define FLOATLOG_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1in1out.h"
#include "ksimus/componentinfo.h"


namespace KSimLibFloatingPoint
{

// Forward declaration

const ComponentInfo * getFloatLogInfo();
const ComponentInfo * getFloatLog10Info();


//###############################################################

/**A floating point natural logarithm gate.
  * @author Rasmus Diekenbrock
  */

class FloatLog : public Float1In1Out
{
public:
	/** Constructs a floating point natural logarithm gate */
	FloatLog(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point natural logarithm gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatLogView : public Float1In1OutView
{
public:
	FloatLogView(FloatLog * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};
	
	virtual void draw(QPainter * p);
};

//###############################################################

//###############################################################

/**A floating point natural logarithm gate.
  * @author Rasmus Diekenbrock
  */

class FloatLog10 : public Float1In1Out
{
public:
	/** Constructs a floating point base-10 logarithm gate */
	FloatLog10(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();

};

//###############################################################

/** The view for the floating point base-10 logarithm gate.
	*
  * @author Rasmus Diekenbrock
  */
class FloatLog10View : public Float1In1OutView
{
public:
	FloatLog10View(FloatLog10 * comp, eViewType viewType)
		: Float1In1OutView(comp, viewType, 5, 5) {};

	virtual void draw(QPainter * p);
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
