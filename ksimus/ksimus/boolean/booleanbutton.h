/***************************************************************************
                          booleanbutton.h  -  description
                             -------------------
    begin                : Fri Feb 2 2001
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

#ifndef BOOLEANBUTTON_H
#define BOOLEANBUTTON_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentstyle.h"
#include "compview.h"
#include "compviewwidget.h"
#include "componentinfo.h"
#include "componentpropertygeneralwidget.h"

// Forward declaration
class ConnectorBoolOut;
class QPushButton;
class KSimBooleanBox;

/**A Button
  *@author Rasmus Diekenbrock
  */

const ComponentInfo * getBooleanButtonInfo();
const ComponentInfo * getBooleanToggleButtonInfo();


//###############################################################
//###############################################################


class BooleanButton : public ComponentStyle
{

friend class BooleanButtonView;
friend class BooleanButtonPropertyWidget;	
	Q_OBJECT

public:
	BooleanButton(CompContainer * container, const ComponentInfo * ci);
//	~BooleanButton();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** Returns true, if the parameter type is a proper reload type. This is useful if a component
	  * supports more than one types and the type decides the different functionality (e.g.
	  * @ref BooleanButton). The implementation returns true, if type is a button or a toggle button.*/
	virtual bool isProperReloadType(const QString & type) const;
	/** Reset all simulation variables */
	virtual void reset();
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** Returns the output connector.
	  */
	ConnectorBoolOut * getOutputConnector() const { return m_out; };


	/** Sets the current component state. */
	void setState(bool newState);
	/** Returns the current component state. */
	bool getState() const;
	
	/** Sets the reset state. The component gets this state each call @ref reset.
	 *  If init is true, the reset State is used as init value. The resetState is
	 *  only saved, if the current resetState is not equal to resetState set as init value.*/
	void setResetState(bool resetState, bool init = false);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	bool getResetState() const;
	/** Returns the reset state. The component gets this state each call @ref reset. */
	bool getResetStateInit() const;
	
	/** Creates the general property page for the property dialog.
	  * This function creates a @ref BooleanButtonPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	bool isToggleButton() const;

	void setToggleButton(bool toggleButton);

	/** Returns the default button text.
	  * The text depends on the current language and the button type (toogle). */
	const QString & getDefaultButtonText() const;

public slots: // Public slots
	void toggled(bool pressed);
	void slotPressed();
	void slotReleased();

protected: // Protected attributes
	Q_UINT32 m_flags;
	ConnectorBoolOut * m_out;
	bool m_toggleButton;

signals: // Signals
	void buttonChanged(bool pressed);
	/** Enables/Disables toggle functionality */
	void signalSetToggleButton(bool toggle);
	void releaseButton();

};

//###############################################################
//###############################################################


class BooleanButtonView : public CompViewSize
{

public:	
	BooleanButtonView(BooleanButton * comp, eViewType viewType);
//	virtual ~BooleanButtonView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

protected:
	virtual void resize();

};

//###############################################################
//###############################################################

class BooleanButtonWidgetView : public CompViewWidget
{
	Q_OBJECT

friend class BooleanButtonView;
	
public:
	BooleanButtonWidgetView(BooleanButtonView * cv, QWidget *parent=0, const char *name=0);
//	~BooleanButtonWidgetView();


	BooleanButton * getButton();

public slots:
	/** This slot rename the button */
	void slotSetText(const QString & newName);
	/** Enables/Disables toggle functionality */
	void slotSetToggleButton(bool toggle);
	/** Release the button */
	void slotReleaseButton();	

protected:
	QPushButton * m_buttonWidget;

};

//###############################################################
//###############################################################

class BooleanButtonPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	BooleanButtonPropertyGeneralWidget(BooleanButton * comp, QWidget *parent=0, const char *name=0);
//	~BooleanButtonPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	
private slots:
	void slotActivateToggled(bool state);

private:
	KSimBooleanBox * m_resetState;
	KSimBooleanBox * m_toggle;
		


};


#endif
