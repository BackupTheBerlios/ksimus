/***************************************************************************
                          floatxin1out.h  -  description
                             -------------------
    begin                : Wen Jan 3 2002
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

#ifndef FLOATXIN1OUT_H
#define FLOATXIN1OUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1out.h"
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"

// Forward declaration
class ConnectorPack;



//###############################################################
//###############################################################


namespace KSimLibFloatingPoint
{


/** Base class for components with multiple floating point inputs and one floating point output.
	*
	* The class creates component with a connector pack with floating point input connectors (@ref ConnectorFloatIn)
	* and one floating point output connector (@ref ConnectorFloatOut).
	*
	* You have to derive this class and in most cases reimplement the function calculate().
	*
  * @author Rasmus Diekenbrock
  */

class FloatXIn1Out : public Float1Out
{
public: 
// ~FloatXIn1Out();

	/** Returns the input connector pack.
	  */
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };

	
protected:
	FloatXIn1Out(CompContainer * container, const ComponentInfo * ci);


private:
	ConnectorPack * m_inPack;
};



//###############################################################
//###############################################################

/** Base class for component views with multiple floating point input connecotrs and one floating point output.
  * The component layout (@ref ComponentLayout) is used to place the inputs on the left side and the
  * output connector in the middle of the right side. The @ref Float1OutView::draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */

class FloatXIn1OutView : public Float1OutView
{

public:
//	~FloatXIn1OutView();
	
protected:
	FloatXIn1OutView(FloatXIn1Out * comp, eViewType viewType);

};


//###############################################################
//###############################################################

/** The property widget for @ref FloatXIn1Out components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class FloatXIn1OutPropertyGeneralWidget : public Float1OutPropertyGeneralWidget
{
public:
	FloatXIn1OutPropertyGeneralWidget(FloatXIn1Out * comp, QWidget *parent=0, const char *name=0)
		:	Float1OutPropertyGeneralWidget(comp, parent, name) {};
};


};  //namespace KSimLibFloatingPoint
#endif
