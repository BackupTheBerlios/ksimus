/***************************************************************************
                          float1out.h  -  description
                             -------------------
    begin                : Tue Jan 2 2002
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

#ifndef FLOAT1OUT_H
#define FLOAT1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"

// Forward declaration
class ComponentLayout;
class KSimDoubleEdit;
class QLabel;
class ConnectorFloatOut;


namespace KSimLibFloatingPoint
{

/**Base class for components with one floating point output.
  *@author Rasmus Diekenbrock
  */

class Float1Out : public Component
{

	Q_OBJECT

public:
//	~Float1Out();
	
	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Returns the output connector.
	  */
	ConnectorFloatOut * getOutputConnector() const { return m_out; };


	/** Sets the current component value. */
	void setValue(double newValue);
	
	/** Returns the current component value. */
	double getValue() const;
	
	/** Sets the reset value. The component gets this state each call @ref reset.*/
	void setResetValue(double resetValue);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	double getResetValue() const;
	
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Float1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

protected:
	/** Creates a component with one floating point output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	Float1Out(CompContainer * container, const ComponentInfo * ci);
	
	double m_resetValue;
	ConnectorFloatOut * m_out;

};

//###############################################################

/** Base class for component views with one boolean output.
  * The component layout (@ref ComponentLayout) is used to place the output connector in the middle of
  * the right side. The @ref draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */


class Float1OutView : public CompView
{

	Q_OBJECT

public:
//	~Float1OutView();
	
	virtual void draw(QPainter * p);
	
	ComponentLayout * getComponentLayout() const { return m_layout; };
	
	Float1Out* getFloat1Out() { return (Float1Out*) getComponent(); };

	

protected:
	Float1OutView(Float1Out * comp, eViewType viewType);
	
private:	
	ComponentLayout * m_layout;

};

//###############################################################

/** The property widget for @ref Float1Out components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class Float1OutPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	Float1OutPropertyGeneralWidget(Float1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	Float1Out* getFloat1Out() { return (Float1Out*) getComponent(); };
	
	/** Returns the a pointer to the @ref QLineEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimDoubleEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	KSimDoubleEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


};  //namespace KSimLibFloatingPoint

#endif
