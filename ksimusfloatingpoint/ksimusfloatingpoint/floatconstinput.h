/***************************************************************************
                          floatconstinput.h  -  description
                             -------------------
    begin                : Tue Feb 5 2002
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

#ifndef FLOATCONSTINPUT_H
#define FLOATCONSTINPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/compviewhbox.h"

#include "float1out.h"

// Forward declaration
class ComponentLayout;
class QLabel;



namespace KSimLibFloatingPoint
{


extern const ComponentInfo FloatConstInputInfo;


//#######################################################################
//#######################################################################

/**A constant component with floating point output connector.
  *@author Rasmus Diekenbrock
  */

class FloatConstInput : public KSimLibFloatingPoint::Float1Out
{
	Q_OBJECT
	
public:

	FloatConstInput(CompContainer * container, const ComponentInfo * ci);
//	~FloatConstInput();

	/** Reset all simulation variables */
	void reset();

	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Boolean1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);

private slots:
	void editValue();


signals:
	void signalSetNumber(double newNumber);
};

//#######################################################################
//#######################################################################

class FloatConstInputView : public CompViewSize
{

	Q_OBJECT

public:	
	FloatConstInputView(FloatConstInput * comp, eViewType viewType);
//	virtual ~FloatConstInputView();
	
	virtual QWidget * createCompViewWidget(QWidget * parent);

	FloatConstInput * getComponent() const { return (FloatConstInput *)CompViewSize::getComponent(); };


protected:
	virtual void resize();

	
private:


signals:
};

//#######################################################################
//#######################################################################

class FloatConstInputWidgetView : public CompViewHBox
{
	Q_OBJECT

friend class FloatConstInputView;
	
public:
	FloatConstInputWidgetView(FloatConstInputView * cv, QWidget *parent=0, const char *name=0);
//	~FloatLineOutputWidgetView();

	FloatConstInput * getComponent() const { return (FloatConstInput *)getCompView()->getComponent(); };

protected:
	QLabel * m_label;
};

//#######################################################################
//#######################################################################

/** The property widget for @ref FloatConstInput components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class FloatConstInputPropertyGeneralWidget : public KSimLibFloatingPoint::Float1OutPropertyGeneralWidget
{
	Q_OBJECT

public:
	FloatConstInputPropertyGeneralWidget(FloatConstInput * comp, QWidget *parent=0, const char *name=0);

	FloatConstInput* getComponent() { return (FloatConstInput*) Float1OutPropertyGeneralWidget::getComponent(); };
	
};


//#######################################################################
//#######################################################################


};  //namespace KSimLibFloatingPoint


#endif
