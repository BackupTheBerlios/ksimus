/***************************************************************************
                          integerlatch.h  -  description
                             -------------------
    begin                : Sun Aug 31 2003
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

#ifndef INTEGERLATCH_H
#define INTEGERLATCH_H

// C-Includes

// QT-Includes
#include <qarray.h>

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "ksimus/ksimbaseint.h"

// Forward declaration
class ConnectorBoolInEdge;
class ConnectorPack;
class ComponentControlBlock;
class ComponentLayout;
class ConnectorLabel;
class KSimSpinBox;
class QLabel;
class KSimBaseIntEdit;


//###############################################################
//###############################################################


namespace KSimLibInteger
{

/**A integer latch.
  *@author Rasmus Diekenbrock
  */

class IntegerLatch : public Component
{
	
	Q_OBJECT

public:
	IntegerLatch(CompContainer * container, const ComponentInfo * ci);
//	~IntegerLatch();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Shifts the current component value (@ref setValue) to the output connector. */
	void setOutput();
	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref IntegerLatchPropertyGeneralWidget.
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
	
	/** Sets the reset value. The component gets this state each call @ref reset.*/
	void setResetValue(KSimBaseInt resetValue);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	KSimBaseInt getResetValue() const;
	
	void setChannelCount(unsigned int count);
	void setChannelCountDelayed(unsigned int count);
	int getChannelCount() const;
	
	ConnectorPack * getInputPack() const { return m_inputPack; };
	ConnectorPack * getOutputPack() const { return m_outputPack; };
	ConnectorBoolInEdge * getInputReset() const { return m_inputReset; };
	ConnectorBoolInEdge * getInputEnable() const { return m_inputEnable; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private slots:
	void slotAddChannel();
	void slotDeleteChannel();

private:
	QArray<int> m_values;
	KSimBaseInt m_resetValue;
	ConnectorBoolInEdge * m_inputReset;
	ConnectorBoolInEdge * m_inputEnable;
	ConnectorPack * m_inputPack;
	ConnectorPack * m_outputPack;
	static int tempConnCount;
	static IntegerLatch * tempConnCountOwner;
	class PropertyGeneralWidget;
	class View;
};

//###############################################################

/** View class for @ref IntegerLatch views.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerLatch::View : public CompView
{

public:
	View(IntegerLatch * comp, eViewType viewType);
//	~View();
	
	IntegerLatch* getComponent() { return (IntegerLatch*) CompView::getComponent(); };
};

//###############################################################

/** The property widget for @ref IntegerLatch components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerLatch::PropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(IntegerLatch * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerLatch* getComponent() { return (IntegerLatch*) ComponentPropertyGeneralWidget::getComponent(); };
	
	/** Returns the a pointer to the @ref QLineEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	int m_min;
	
	KSimSpinBox * m_channels;
	QLabel * m_channelsLabel;
	
	KSimBaseIntEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


//###############################################################

};  //namespace KSimLibInteger

#endif
