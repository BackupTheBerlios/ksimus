/***************************************************************************
                          simpleconditional.h  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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

#ifndef SIMPLECONDITIONAL_H
#define SIMPLECONDITIONAL_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/boolean1out.h"



// Forward declaration
class ConnectorFloatIn;
class QLabel;
class QComboBox;

namespace KSimLibFloatingPoint
{

const ComponentInfo * getConditionalLesserInfo();
const ComponentInfo * getConditionalLesserEqualInfo();
const ComponentInfo * getConditionalEqualInfo();
const ComponentInfo * getConditionalLargerEqualInfo();
const ComponentInfo * getConditionalLargerInfo();

enum eConditionalType { eLesser, eLesserEqual, eEqual, eLargerEqual, eLarger };


//###############################################################

/**Class for compare floating point values.
  *@author Rasmus Diekenbrock
  */

class SimpleConditional : public Boolean1Out
{

	Q_OBJECT

public:
	/** Creates the component.
	  */
	SimpleConditional(CompContainer * container, const ComponentInfo * ci);
//	~SimpleConditional();
	
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref SimpleConditionalPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);

	ConnectorFloatIn * getInputA() { return m_inputA;	};
	ConnectorFloatIn * getInputB() { return m_inputB;	};
	
	eConditionalType getConditionalType() const { return m_conditionalType; };
	void setConditionalType(eConditionalType newCond);
	


protected:

	ConnectorFloatIn * m_inputA;	
	ConnectorFloatIn * m_inputB;
	
	eConditionalType m_conditionalType;	
};

//###############################################################

/** View class for @ref SimpleConditional views.
  *
  * @author Rasmus Diekenbrock
  */


class SimpleConditionalView : public Boolean1OutView
{

	Q_OBJECT

public:
	SimpleConditionalView(SimpleConditional * comp, eViewType viewType);
//	~SimpleConditionalView();
	
	virtual void draw(QPainter * p);
	
	SimpleConditional* getComponent() { return (SimpleConditional*) Boolean1OutView::getComponent(); };

protected:
	
private:	
};

//###############################################################

/** The property widget for @ref SimpleConditional components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class SimpleConditionalPropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	SimpleConditionalPropertyGeneralWidget(SimpleConditional * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	SimpleConditional* getComponent() { return (SimpleConditional*) Boolean1OutPropertyGeneralWidget::getComponent(); };
	
	
protected:
	QLabel * m_conditionalLabel;
	QComboBox * m_conditional;
};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
