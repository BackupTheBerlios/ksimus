/***************************************************************************
                        booltristate1out.h  -  description
                             -------------------
    begin                : Fri Nov 15 2001
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

#ifndef BOOLTRISTATE1OUT_H
#define BOOLTRISTATE1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbooltristate.h"




// Forward declaration
class QLabel;
class ConnectorBoolTristate;
class ConnectorBoolIn;
class ComponentLayoutVerticalCtrl;
class ComponentControlBlock;
class ConnectorPack;
class KSimBoolTristateBox;


namespace KSimLibBoolean
{

/**Base class for components with one boolean tristate output.
  *@author Rasmus Diekenbrock
  */

class BoolTristate1Out : public Component
{

	Q_OBJECT

public:
//	~BoolTristate1Out();
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
	ConnectorBoolTristate * getOutputConnector() const { return m_out; };


	/** Sets the current component state. */
	void setState(KSimBoolTristate newState);
	
	/** Returns the current component state. */
	KSimBoolTristate getState() const;
	
	/** Sets the reset state. The component gets this state each call @ref reset.
	 *  If init is true, the reset State is used as init value. The resetState is
	 *  only saved, if the current resetState is not equal to resetState set as init value.*/
	void setResetState(KSimBoolTristate resetState, bool init = false);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTristate getResetState() const;
	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTristate getResetStateInit() const;
	
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTristate1Out(CompContainer * container, const ComponentInfo * ci);
	
private:
	ConnectorBoolTristate * m_out;
	KSimBoolTristate m_resetState;
	KSimBoolTristate m_resetStateInit;
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


class BoolTristate1OutView : public CompView
{
public:
	BoolTristate1OutView(BoolTristate1Out * comp, eViewType viewType, const QString & text, const char * name);
//	~BoolTristate1OutView();
	
	BoolTristate1Out * getBoolTristate1Out() const { return (BoolTristate1Out*) getComponent(); };
};

//###############################################################

/** The property widget for @ref BoolTristate1Out components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTristate1OutPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	BoolTristate1OutPropertyGeneralWidget(BoolTristate1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	BoolTristate1Out * getBoolTristate1Out() const { return (BoolTristate1Out*) getComponent(); };

	/** Returns the a pointer to the @ref KSimBoolTristateBox. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBoolTristateBox * getResetStateBox() const { return m_resetState; };
	/** Returns the a pointer to the label of the @ref KSimBoolTristateBox. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetStateBoxLabel() const { return m_resetStateLabel; };
	
protected:
	KSimBoolTristateBox * m_resetState;
	QLabel * m_resetStateLabel;
};


//###############################################################

/**Base class for components with one boolean tristate output and one outout enable input.
  *@author Rasmus Diekenbrock
  */

class BoolTristate1Out1Ena : public BoolTristate1Out
{

	Q_OBJECT

public:
//	~BoolTristate1Out1Ena();

	/** Returns the output connector.
	  */
	ConnectorBoolIn * getEnableOutputConnector() const { return m_enableOutput; };


	/** Sets the current component state. */
	void setState(bool newState);

	/** Returns the current component state without the enable property. */
	bool getState() const;

	/** Returns the current component state. */
	KSimBoolTristate getTristate() const { return BoolTristate1Out::getState(); };

protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTristate1Out1Ena(CompContainer * container, const ComponentInfo * ci);

private:
	ConnectorBoolIn * m_enableOutput;
};

//###############################################################

/** Base class for component views with one boolean output and one output enable input.
  * The component layout (@ref ComponentLayout) is used to place the output connector in the middle of
  * the right side. The @ref draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTristate1Out1EnaView : public CompView
{

	Q_OBJECT

public:
	BoolTristate1Out1EnaView(BoolTristate1Out1Ena * comp, eViewType viewType, const QString & text, const char * name);
//	~BoolTristate1Out1EnaView();

	BoolTristate1Out1Ena * getBoolTristate1Out1Ena() const { return (BoolTristate1Out1Ena*) getComponent(); };
};

//###############################################################

/** The property widget for @ref BoolTristate1Out1Ena components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTristate1Out1EnaPropertyGeneralWidget : public BoolTristate1OutPropertyGeneralWidget
{

	Q_OBJECT

public:
	BoolTristate1Out1EnaPropertyGeneralWidget(BoolTristate1Out1Ena * comp, QWidget *parent=0, const char *name=0)
		:	BoolTristate1OutPropertyGeneralWidget(comp, parent, name) {};
};

//###############################################################


/**Base class for components with some boolean inputs, one boolean tristate output and one outout enable input.
  *@author Rasmus Diekenbrock
  */

class BoolTristateXIn1Out1Ena : public BoolTristate1Out1Ena
{

	Q_OBJECT

public:
//	~BoolTristateXIn1Out1Ena();

	/** Returns the output connector.
	  */
	ConnectorPack * getInputConnector() const { return m_inPack; };


protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTristateXIn1Out1Ena(CompContainer * container, const ComponentInfo * ci);

private:
	ConnectorPack * m_inPack;
};

//###############################################################

/** Base class for component views with some boolean inputs, one boolean output and one output enable input.
  * The component layout (@ref ComponentLayout) is used to place the output connector in the middle of
  * the right side. The @ref draw function draws a rectangle.
  *
  * The component layout is only created and connectors are only placed, if the viewType is SHEET_VIEW.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTristateXIn1Out1EnaView : public CompView
{

	Q_OBJECT

public:
	BoolTristateXIn1Out1EnaView(BoolTristateXIn1Out1Ena * comp, eViewType viewType, const QString & text, const char * name);
//	~BoolTristateXIn1Out1EnaView();

	BoolTristateXIn1Out1Ena * getBoolTristateXIn1Out1Ena() const { return (BoolTristateXIn1Out1Ena*) getComponent(); };
};

//###############################################################

/** The property widget for @ref BoolTristateXIn1Out1Ena components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTristateXIn1Out1EnaPropertyGeneralWidget : public BoolTristate1Out1EnaPropertyGeneralWidget
{

	Q_OBJECT

public:
	BoolTristateXIn1Out1EnaPropertyGeneralWidget(BoolTristateXIn1Out1Ena * comp, QWidget *parent=0, const char *name=0)
		:	BoolTristate1Out1EnaPropertyGeneralWidget(comp, parent, name) {};
};


//###############################################################

}; //namespace KSimLibBoolean

#endif
