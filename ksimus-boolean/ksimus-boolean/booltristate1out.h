/***************************************************************************
                        booltristate1out.h  -  description
                             -------------------
    begin                : Fre Nov 15 2001
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
#include "ksimus/componentinfo.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbooltristate.h"




// Forward declaration
class QLabel;
class ConnectorBoolTriState;
class ConnectorBoolIn;
class ComponentLayout;
class ComponentControlBlock;
class ConnectorPack;
class KSimBoolTristateBox;


namespace KSimLibBoolean
{

/**Base class for components with one boolean tristate output.
  *@author Rasmus Diekenbrock
  */

class BoolTriState1Out : public Component
{

	Q_OBJECT

public:
//	~BoolTriState1Out();
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
	ConnectorBoolTriState * getOutputConnector() const { return m_out; };


	/** Sets the current component state. */
	void setState(KSimBoolTriState newState);
	
	/** Returns the current component state. */
	KSimBoolTriState getState() const;
	
	/** Sets the reset state. The component gets this state each call @ref reset.
	 *  If init is true, the reset State is used as init value. The resetState is
	 *  only saved, if the current resetState is not equal to resetState set as init value.*/
	void setResetState(KSimBoolTriState resetState, bool init = false);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTriState getResetState() const;
	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBoolTriState getResetStateInit() const;
	
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTriState1Out(CompContainer * container, const ComponentInfo * ci);
	
private:
	ConnectorBoolTriState * m_out;
	KSimBoolTriState m_resetState;
	KSimBoolTriState m_resetStateInit;
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


class BoolTriState1OutView : public CompView
{

	Q_OBJECT

public:
//	~BoolTriState1OutView();
	
	virtual void draw(QPainter * p);
	
	ComponentLayout * getComponentLayout() const { return m_layout; };
	
	BoolTriState1Out * getBoolTriState1Out() const { return (BoolTriState1Out*) getComponent(); };

protected:
	BoolTriState1OutView(BoolTriState1Out * comp, eViewType viewType);
	
private:	
	ComponentLayout * m_layout;

};

//###############################################################

/** The property widget for @ref BoolTriState1Out components.
  * This class adds to the general property dialog a item for selecting the reset state of the component.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTriState1OutPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	BoolTriState1OutPropertyGeneralWidget(BoolTriState1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	BoolTriState1Out * getBoolTriState1Out() const { return (BoolTriState1Out*) getComponent(); };

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

class BoolTriState1Out1Ena : public BoolTriState1Out
{

	Q_OBJECT

public:
//	~BoolTriState1Out1Ena();

	/** Returns the output connector.
	  */
	ConnectorBoolIn * getEnableOutputConnector() const { return m_enableOutput; };


	/** Sets the current component state. */
	void setState(bool newState);

	/** Returns the current component state without the enable property. */
	bool getState() const;

	/** Returns the current component state. */
	KSimBoolTriState getTriState() const { return BoolTriState1Out::getState(); };

protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTriState1Out1Ena(CompContainer * container, const ComponentInfo * ci);

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


class BoolTriState1Out1EnaView : public BoolTriState1OutView
{

	Q_OBJECT

public:
//	~BoolTriState1Out1EnaView();

	virtual void draw(QPainter * p);

	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };

	BoolTriState1Out1Ena * getBoolTriState1Out1Ena() const { return (BoolTriState1Out1Ena*) getComponent(); };

protected:
	BoolTriState1Out1EnaView(BoolTriState1Out1Ena * comp, eViewType viewType);
	BoolTriState1Out1EnaView(const QString & label, BoolTriState1Out1Ena * comp, eViewType viewType);

private:
	void init(eViewType viewType);
	ComponentControlBlock * m_ctrlBlock;
	QString m_label;
};

//###############################################################

/** The property widget for @ref BoolTriState1Out1Ena components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTriState1Out1EnaPropertyGeneralWidget : public BoolTriState1OutPropertyGeneralWidget
{

	Q_OBJECT

public:
	BoolTriState1Out1EnaPropertyGeneralWidget(BoolTriState1Out1Ena * comp, QWidget *parent=0, const char *name=0)
		:	BoolTriState1OutPropertyGeneralWidget(comp, parent, name) {};
};

//###############################################################


/**Base class for components with some boolean inputs, one boolean tristate output and one outout enable input.
  *@author Rasmus Diekenbrock
  */

class BoolTriStateXIn1Out1Ena : public BoolTriState1Out1Ena
{

	Q_OBJECT

public:
//	~BoolTriStateXIn1Out1Ena();

	/** Returns the output connector.
	  */
	ConnectorPack * getInputConnector() const { return m_inPack; };


protected:
	/** Creates a component with one boolean tristate output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	BoolTriStateXIn1Out1Ena(CompContainer * container, const ComponentInfo * ci);

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


class BoolTriStateXIn1Out1EnaView : public CompView
{

	Q_OBJECT

public:
//	~BoolTriStateXIn1Out1EnaView();

	BoolTriStateXIn1Out1Ena * getBoolTriStateXIn1Out1Ena() const { return (BoolTriStateXIn1Out1Ena*) getComponent(); };

	virtual void draw(QPainter * p);

	ComponentLayout * getComponentLayout() const { return m_layout; };
	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };

protected:
	BoolTriStateXIn1Out1EnaView(BoolTriStateXIn1Out1Ena * comp, eViewType viewType);
	BoolTriStateXIn1Out1EnaView(const QString & label, BoolTriStateXIn1Out1Ena * comp, eViewType viewType);

private:
	void init(eViewType viewType);
	ComponentLayout * m_layout;
	ComponentControlBlock * m_ctrlBlock;
	QString m_label;
};

//###############################################################

/** The property widget for @ref BoolTriStateXIn1Out1Ena components.
  *
  * Currently the class does nothing. It is added to supply a uniform API.
  *
  * @author Rasmus Diekenbrock
  */


class BoolTriStateXIn1Out1EnaPropertyGeneralWidget : public BoolTriState1Out1EnaPropertyGeneralWidget
{

	Q_OBJECT

public:
	BoolTriStateXIn1Out1EnaPropertyGeneralWidget(BoolTriStateXIn1Out1Ena * comp, QWidget *parent=0, const char *name=0)
		:	BoolTriState1Out1EnaPropertyGeneralWidget(comp, parent, name) {};
};


//###############################################################

}; //namespace KSimLibBoolean

#endif
