/***************************************************************************
                          integer1out.h  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef INTEGER1OUT_H
#define INTEGER1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimbaseint.h"

// Forward declaration
class KSimBaseIntEdit;
class QLabel;
class ConnectorIntegerOut;


namespace KSimLibInteger
{

/**Base class for components with one integer output.
  *@author Rasmus Diekenbrock
  */

class Integer1Out : public Component
{

	Q_OBJECT

public:
//	~Integer1Out();
	
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
	ConnectorIntegerOut * getOutputConnector() const { return m_out; };


	/** Sets the current component value. */
	void setValue(int newValue);
	
	/** Returns the current component value. */
	int getValue() const;
	
	/** Sets the reset value. The component gets this state each call @ref reset.*/
	void setResetValue(const KSimBaseInt & resetValue);
	/** Returns the reset state. The component gets this state each call @ref reset. */
	const KSimBaseInt & getResetValue() const { return m_resetValue; };

	/** Sets the initial value of the reset value. */
	void setInitResetValue(const KSimBaseInt & resetValue);
	/** Returns the initial value of the reset state.*/
	const KSimBaseInt & getInitResetValue() const { return m_initResetValue; };
	
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Integer1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);


	static const KSimBaseInt DefaultResetValue;

protected:
	/** Creates a component with one integer output.
	  * The constructer does not creates a component view. You have to do this in a sub class.
	  */
	Integer1Out(CompContainer * container, const ComponentInfo * ci);
	
	KSimBaseInt m_resetValue;
	KSimBaseInt m_initResetValue;
	ConnectorIntegerOut * m_out;

signals:
	void resetValueChanged(const KSimBaseInt & newResetValue);
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


class Integer1OutView : public CompView
{
public:
	Integer1OutView(Integer1Out * comp, eViewType viewType, const QString & text, const char * name);
//	~Integer1OutView();
	
	Integer1Out* getInteger1Out() { return (Integer1Out*) getComponent(); };

private:	
};

//###############################################################

/** The property widget for @ref Integer1Out components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class Integer1OutPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	Integer1OutPropertyGeneralWidget(Integer1Out * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	Integer1Out* getInteger1Out() { return (Integer1Out*) getComponent(); };
	
	/** Returns the a pointer to the @ref KSimBaseIntEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	KSimBaseIntEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


};  //namespace KSimLibInteger

#endif
