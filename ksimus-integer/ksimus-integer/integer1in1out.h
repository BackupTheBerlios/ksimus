/***************************************************************************
                          integer1in1out.h  -  description
                             -------------------
    begin                : Sat Jul 12 2003
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

#ifndef INTEGER1IN1OUT_H
#define INTEGER1IN1OUT_H

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


/** Base class for components with two integer inputs and one inputs output.
	*
	* The class creates component with two integer input connectors (@ref ConnectorIntegerIn)
	* and one inputs output connector (@ref ConnectorIntegerOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class Integer1In1Out : public Integer1Out
{

	Q_OBJECT

public: 
// ~Integer1In1Out();

	/** Returns the input connector.
	  */
	ConnectorIntegerIn * getInputConnector() const { return m_input; };

	
protected:
	Integer1In1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorIntegerIn * m_input;
};



//###############################################################
//###############################################################

/** Base class for component views with one integer input connecotrs and one integer output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Integer1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class Integer1In1OutView : public CompView
{

	Q_OBJECT

public:
	Integer1In1OutView(Integer1In1Out * comp, eViewType viewType, const QString & text, const char * name);
//	~Integer1In1OutView();
};


//###############################################################
//###############################################################

/** The property widget for @ref Integer1In1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class Integer1In1OutPropertyGeneralWidget : public Integer1OutPropertyGeneralWidget
{

	Q_OBJECT

public:
	Integer1In1OutPropertyGeneralWidget(Integer1In1Out * comp, QWidget *parent=0, const char *name=0)
		:	Integer1OutPropertyGeneralWidget(comp, parent, name) {};
};


};  //namespace KSimLibInteger
#endif
