/***************************************************************************
                          integer2multibool.h  -  description
                             -------------------
    begin                : Sat Sep 6 2003
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

#ifndef INTEGER2MULTIBOOL_H
#define INTEGER2MULTIBOOL_H


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
class KSimSpinBox;
class ConnectorIntegerIn;
class ConnectorPack;


namespace KSimLibInteger
{

/** A Gate which converts a integer input into several boolean outputs.
  *@author Rasmus Diekenbrock
  */

class Integer2MultiBool : public Component
{

	Q_OBJECT

public:
	Integer2MultiBool(CompContainer * container, const ComponentInfo * ci);
//	~Integer2MultiBool();

	/** Resets the component state. */
	virtual void reset();

	/** Executes the simulation of this component */
	virtual void calculate();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);

	/** Returns the output connector.
	  */
	ConnectorIntegerIn * getInputConnector() const { return m_input; };


	/** Sets the reset value. The component gets this state each call @ref reset.*/
	void setResetValue(const KSimBaseInt & resetValue);
	/** Returns the reset state. The component gets this state each call @ref reset. */
	const KSimBaseInt & getResetValue() const { return m_resetValue; };

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref Integer1OutPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Is called after execution of the PopupMenu (@ref initPopupMenu) or PropertyDialog
	  * (@ref initPropertyDialog). Use this function to adjust things which are difficult
	  * to handle inside a slot or the function @ref PropertyWidget::acceptPressed.
	  *
	  * For example it is problematic to change the connector count inside the PropertyDialog
	  * because each connector has also a property widget. If you reduce the connector count
	  * some of these propert widgets have no valid connector. This function delays the connector
	  * count modification until all property widgets are removed.
	  *
	  * The default implementation calls the @ref ComponentAddOn::menuExecuted functions.
	  */
	virtual void menuExecuted();

	/** Returns the input connector pack.
	  */
	ConnectorPack * getOutputConnectorPack() const { return m_outPack; };

	/** Sets the current component value. */
	void setValue(int newValue);

	void setOutputCount(unsigned int count);
	void setOutputCountDelayed(unsigned int count);
	int getOutputCount() const;

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


private:

	ConnectorPack * m_outPack;
	KSimBaseInt m_resetValue;
	ConnectorIntegerIn * m_input;

	static int tempConnCount;
	static Integer2MultiBool * tempConnCountOwner;
	static const KSimBaseInt DefaultResetValue;

	class View;
	class PropertyGeneralWidget;

private slots:
	void addConn(ConnectorBase * conn);
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


class Integer2MultiBool::View : public CompView
{
public:
	View(Integer2MultiBool * comp, eViewType viewType);
//	~View();

	Integer2MultiBool* getInteger2MultiBool() { return (Integer2MultiBool*) getComponent(); };
};

//###############################################################

/** The property widget for @ref Integer1Out components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class Integer2MultiBool::PropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(Integer2MultiBool * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	Integer2MultiBool* getInteger2MultiBool() { return (Integer2MultiBool*) getComponent(); };
	
	/** Returns the a pointer to the @ref KSimBaseIntEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	KSimBaseIntEdit * m_resetValue;
	QLabel * m_resetValueLabel;
	int m_min;
	KSimSpinBox * m_outputs;
	QLabel * m_outputsLabel;
};


//###############################################################


};  //namespace KSimLibInteger

#endif
