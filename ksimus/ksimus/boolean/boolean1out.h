/***************************************************************************
                          boolean1out.h  -  description
                             -------------------
    begin                : Thu Dec 6 2001
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

#ifndef BOOLEAN1OUT_H
#define BOOLEAN1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "component.h"
#include "compview.h"
#include "componentinfo.h"
#include "componentpropertygeneralwidget.h"



// Forward declaration
class ConnectorBoolOut;
class ComponentLayout;
class KSimBooleanBox;
class QLabel;



/**Base class for components with one boolean output.
  *@author Rasmus Diekenbrock
  */

class Boolean1Out : public Component
{
//friend class Boolean1OutPropertyGeneralWidget;

	Q_OBJECT

public:
//	~Boolean1Out();
	/** Shifts the current component state (@ref setState) to output connector. */
	virtual void updateOutput();
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
	ConnectorBoolOut * getOutputConnector() const { return m_out; };


	/** Sets the current component state. */
	void setState(bool newState) { m_state = newState; };
	
	/** Returns the current component state. */
	bool getState() const { return m_state; };
	
	/** Sets the reset state. The component gets this state each call @ref reset.
	 *  If init is true, the reset State is used as init value. The resetState is
	 *  only saved, if the current resetState is not equal to resetState set as init value.*/
	void setResetState(bool resetState, bool init = false);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	bool getResetState() const;
	/** Returns the reset state. The component gets this state each call @ref reset. */
	bool getResetStateInit() const;
	
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);

protected:
	/** Creates a component with one boolean output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	Boolean1Out(CompContainer * container, const ComponentInfo * ci);
	
	bool m_state;
	Q_UINT32 m_flags;
	ConnectorBoolOut * m_out;

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


class Boolean1OutView : public CompView
{

	Q_OBJECT

public:
//	~Boolean1OutView();
	
	virtual void draw(QPainter * p);
	
	ComponentLayout * getComponentLayout() const { return m_layout; };
	

protected:
	Boolean1OutView(Boolean1Out * comp, eViewType viewType);
	
private:	
	ComponentLayout * m_layout;

};

//###############################################################

/** The property widget for @ref Boolean1Out components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class Boolean1OutPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	Boolean1OutPropertyGeneralWidget(Boolean1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	/** Returns the a pointer to the @ref KSimBooleanBox. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBooleanBox * getResetStateBox() const { return m_resetState; };
	/** Returns the a pointer to the label of the @ref KSimBooleanBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetStateBoxLabel() const { return m_resetStateLabel; };
	
protected:
	KSimBooleanBox * m_resetState;
	QLabel * m_resetStateLabel;
};


//###############################################################


#endif
