/***************************************************************************
                          float1in1out.h  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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

#ifndef FLOAT1IN1OUT_H
#define FLOAT1IN1OUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1out.h"

// Forward declaration
class ConnectorFloatIn;


//###############################################################
//###############################################################


namespace KSimLibFloatingPoint
{


/** Base class for components with two floating point inputs and one floating point output.
	*
	* The class creates component with two floating point input connectors (@ref ConnectorFloatIn)
	* and one floating point output connector (@ref ConnectorFloatOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class Float1In1Out : public Float1Out
{
public: 
// ~Float1In1Out();

	/** Returns the input connector.
	  */
	ConnectorFloatIn * getInput() const { return m_input; };

	
protected:
	Float1In1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorFloatIn * m_input;
};



//###############################################################
//###############################################################

/** Base class for component views with one floating point input connecotrs and one floating point output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Float1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class Float1In1OutView : public Float1OutView
{

public:
//	~Float1In1OutView();
	
protected:
	Float1In1OutView(Float1In1Out * comp, eViewType viewType, unsigned int width = 5, unsigned int height = 3);

};


//###############################################################
//###############################################################

/** The property widget for @ref Float1In1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class Float1In1OutPropertyGeneralWidget : public Float1OutPropertyGeneralWidget
{
public:
	Float1In1OutPropertyGeneralWidget(Float1In1Out * comp, QWidget *parent=0, const char *name=0)
		:	Float1OutPropertyGeneralWidget(comp, parent, name) {};
};


};  //namespace KSimLibFloatingPoint
#endif
