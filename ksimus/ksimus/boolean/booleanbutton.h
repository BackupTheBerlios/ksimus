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
#include "component.h"
#include "compview.h"
#include "compviewwidget.h"
#include "componentinfo.h"
#include "componentpropertybasewidget.h"

// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolIn;
class QPushButton;
class QCheckBox;

/**A Button
  *@author Rasmus Diekenbrock
  */

extern const ComponentInfoList BooleanButtonList;

//###############################################################
//###############################################################


class BooleanButton : public Component
{

friend class BooleanButtonView;
friend class BooleanButtonPropertyWidget;	
	Q_OBJECT

public:
	BooleanButton(CompContainer * container, const ComponentInfo * ci);
	~BooleanButton();

	/** Shift the result of calculation to output */
	virtual void updateOutput();
	/** Reset all simulation variables */
	virtual void reset();
	/** Init the property dialog */
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);

	bool isToggleButton() const;


public slots: // Public slots
	void toggled(bool pressed);
	void setToggleButton(bool toggleButton);
	void slotPressed();
	void slotReleased();

protected: // Protected attributes
	bool m_buttonState;
	bool m_toggleButton;
	ConnectorBoolOut * out;

signals: // Signals
	void buttonChanged(bool pressed);
	/** Enables/Disables toggle functionality */
	void signalSetToggleButton(bool toggle);

};

//###############################################################
//###############################################################


class BooleanButtonView : public CompViewSize
{

public:	
	BooleanButtonView(Component * comp, eViewType viewType);
	virtual ~BooleanButtonView();
	
	/** Draws the Button to a printer */
//	virtual void print(QPainter * paint);
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
	BooleanButtonWidgetView(CompView * cv, QWidget *parent=0, const char *name=0);
	~BooleanButtonWidgetView();

public slots:
	/** This slot rename the button */
	void slotSetText(const QString & newName);
	/** Enables/Disables toggle functionality */
	void slotSetToggleButton(bool toggle);
	
protected:
	QPushButton * m_button;

};

//###############################################################
//###############################################################

class BooleanButtonPropertyWidget : public ComponentPropertyBaseWidget
{
	Q_OBJECT

public:
	BooleanButtonPropertyWidget(Component * comp, QWidget *parent=0, const char *name=0);
	~BooleanButtonPropertyWidget();


public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void slotDefault();
	/** The function slotReset() is called, if user wants to cancel the dialog.
	 */

protected:
		QCheckBox * m_toggle;

};


#endif
