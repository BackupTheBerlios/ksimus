/***************************************************************************
                          integer2in1out.h  -  description
                             -------------------
    begin                : Fri Aug 1 2003
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

#ifndef INTEGER2IN1OUT_H
#define INTEGER2IN1OUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1out.h"

// Forward declaration
class ConnectorIntegerIn;


//###############################################################
//###############################################################


namespace KSimLibInteger
{


/** Base class for components with two integer inputs and one integer output.
	*
	* The class creates component with two integer input connectors (@ref ConnectorIntegerIn)
	* and one integer output connector (@ref ConnectorIntegerOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class Integer2In1Out : public Integer1Out
{
public: 
// ~Integer2In1Out();

	/** Returns the first input connector.
	  */
	ConnectorIntegerIn * getInputA() const { return m_inputA; };
	/** Returns the second input connector.
	  */
	ConnectorIntegerIn * getInputB() const { return m_inputB; };

	
protected:
	Integer2In1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorIntegerIn * m_inputA;
	ConnectorIntegerIn * m_inputB;
};



//###############################################################
//###############################################################

/** Base class for component views with two integer input connecotrs and one integer output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Integer1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class Integer2In1OutView : public Integer1OutView
{

public:
//	~Integer2In1OutView();
	
protected:
	Integer2In1OutView(Integer2In1Out * comp, eViewType viewType);

};


//###############################################################
//###############################################################

/** The property widget for @ref Integer2In1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class Integer2In1OutPropertyGeneralWidget : public Integer1OutPropertyGeneralWidget
{
public:
	Integer2In1OutPropertyGeneralWidget(Integer2In1Out * comp, QWidget *parent=0, const char *name=0)
		:	Integer1OutPropertyGeneralWidget(comp, parent, name) {};
};


};  //namespace KSimLibInteger
#endif
