/***************************************************************************
                          integerxin1out.h  -  description
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

#ifndef INTEGERXIN1OUT_H
#define INTEGERXIN1OUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1out.h"

// Forward declaration
class ConnectorPack;



//###############################################################
//###############################################################


namespace KSimLibInteger
{


/** Base class for components with multiple integer inputs and one integer output.
	*
	* The class creates component with a connector pack with integer input connectors (@ref ConnectorIntegerIn)
	* and one integer output connector (@ref ConnectorIntegerOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class IntegerXIn1Out : public Integer1Out
{

	Q_OBJECT

public: 
// ~IntegerXIn1Out();

	/** Returns the input connector pack.
	  */
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };

	
protected:
	IntegerXIn1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorPack * m_inPack;
};



//###############################################################
//###############################################################

/** Base class for component views with multiple integer input connecotrs and one integer output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Integer1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class IntegerXIn1OutView : public CompView
{
public:
	IntegerXIn1OutView(IntegerXIn1Out * comp, eViewType viewType, const QString & text);
//	~IntegerXIn1OutView();
};


//###############################################################
//###############################################################

/** The property widget for @ref IntegerXIn1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerXIn1OutPropertyGeneralWidget : public Integer1OutPropertyGeneralWidget
{

	Q_OBJECT

public:
	IntegerXIn1OutPropertyGeneralWidget(IntegerXIn1Out * comp, QWidget *parent=0, const char *name=0)
		:	Integer1OutPropertyGeneralWidget(comp, parent, name) {};
};


};  //namespace KSimLibInteger
#endif
