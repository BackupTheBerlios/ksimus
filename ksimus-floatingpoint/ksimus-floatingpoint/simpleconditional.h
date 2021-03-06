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
class QPixmap;

namespace KSimLibFloatingPoint
{

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
	
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Returns true, if the parameter type is a proper reload type. This is useful if a component
	  * supports more than one types and the type decides the different functionality (e.g.
	  * @ref BooleanButton). The implementation returns true, if type is on of the conditionals.*/
	virtual bool isProperReloadType(const QString & type) const;
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref SimpleConditionalPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	ConnectorFloatIn * getInputA() { return m_inputA;	};
	ConnectorFloatIn * getInputB() { return m_inputB;	};
	
	eConditionalType getConditionalType() const { return m_conditionalType; };
	void setConditionalType(eConditionalType newCond);
	void setConditionalType(const QString & type);

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticLesserInfo();
	static const ComponentInfo * getStaticLesserEqualInfo();
	static const ComponentInfo * getStaticEqualInfo();
	static const ComponentInfo * getStaticLargerEqualInfo();
	static const ComponentInfo * getStaticLargerInfo();

protected:
	ConnectorFloatIn * m_inputA;	
	ConnectorFloatIn * m_inputB;
	
	eConditionalType m_conditionalType;

	class View;
};

//###############################################################

/** View class for @ref SimpleConditional views.
  *
  * @author Rasmus Diekenbrock
  */


class SimpleConditional::View : public CompView
{
public:
	View(SimpleConditional * comp, eViewType viewType, const char * name);
//	~View();
	
	virtual void draw(QPainter * p);
	
	SimpleConditional* getSimpleConditional() { return (SimpleConditional*) getComponent(); };
};

//###############################################################

/** The property widget for @ref SimpleConditional components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class SimpleConditionalPropertyGeneralWidget : public Boolean1OutPropertyGeneralWidget
{
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

	SimpleConditional* getSimpleConditional() { return (SimpleConditional*) Boolean1OutPropertyGeneralWidget::getComponent(); };
	
	
protected:
	QLabel * m_conditionalLabel;
	QComboBox * m_conditional;
};


//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
