/***************************************************************************
                          booleanxin1out.h  -  description
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef BOOLEANXIN1OUT_H
#define BOOLEANXIN1OUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "boolean1out.h"
#include "component.h"
#include "compview.h"
#include "componentpropertygeneralwidget.h"


// Forward declaration
class ConnectorPack;



//###############################################################
//###############################################################




/** Base class for components with multiple boolean inputs and one boolean output.
	*
	* The class creates component with a connector pack with boolean input connectors (@ref ConnectorBoolIn)
	* and one boolean output connector (@ref ConnectorBoolOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class BooleanXIn1Out : public Boolean1Out
{

Q_OBJECT

public:
// ~BooleanXIn1Out();

	/** Returns the input connector pack.
	  */
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };


protected:
	BooleanXIn1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorPack * m_inPack;
};



//###############################################################
//###############################################################

/** Base class for component views with multiple boolean input connecotrs and one boolean output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Boolean1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class BooleanXIn1OutView : public CompView
{
public:
	BooleanXIn1OutView(BooleanXIn1Out * comp, eViewType viewType, const QString & text, const char * name);
//	~BooleanXIn1OutView();

protected:
};


//###############################################################
//###############################################################

/** The property widget for @ref BooleanXIn1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class BooleanXIn1OutPropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{

Q_OBJECT

public:
	BooleanXIn1OutPropertyGeneralWidget(BooleanXIn1Out * comp, QWidget *parent=0, const char *name=0)
		:	Boolean1OutPropertyGeneralWidget(comp, parent, name) {};
};

#endif
