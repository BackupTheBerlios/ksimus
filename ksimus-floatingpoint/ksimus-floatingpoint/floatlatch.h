/***************************************************************************
                          floatlatch.h  -  description
                             -------------------
    begin                : Tue Feb 12 2002
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

#ifndef FLOATLATCH_H
#define FLOATLATCH_H

// C-Includes

// QT-Includes
#include <qarray.h>

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentpropertygeneralwidget.h"

// Forward declaration
class ConnectorBoolInEdge;
class ConnectorPack;
class ComponentControlBlock;
class ComponentLayout;
class ConnectorLabel;
class QSpinBox;
class QLabel;
class KSimDoubleEdit;


//###############################################################
//###############################################################


namespace KSimLibFloatingPoint
{

/**A floating point latch.
  *@author Rasmus Diekenbrock
  */

class FloatLatch : public Component
{

	Q_OBJECT

public:
	FloatLatch(CompContainer * container, const ComponentInfo * ci);
//	~FloatLatch();

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
	  * This function creeates a @ref FloatLatchPropertyGeneralWidget.
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
	void setResetValue(double resetValue);
	
	/** Returns the reset state. The component gets this state each call @ref reset. */
	double getResetValue() const;
	
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
	QArray<double> m_values;
	double m_resetValue;
	ConnectorBoolInEdge * m_inputReset;
	ConnectorBoolInEdge * m_inputEnable;
	ConnectorPack * m_inputPack;
	ConnectorPack * m_outputPack;
	static int tempConnCount;
	static FloatLatch * tempConnCountOwner;
};

//###############################################################

/** View class for @ref FloatLatch views.
  *
  * @author Rasmus Diekenbrock
  */


class FloatLatchView : public CompView
{

	Q_OBJECT

public:
	FloatLatchView(FloatLatch * comp, eViewType viewType);
//	~FloatLatchView();
	virtual void draw(QPainter * p);
	
	FloatLatch* getComponent() { return (FloatLatch*) CompView::getComponent(); };
	
	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };
	ComponentLayout * getComponentLayout() const { return m_layout; };


private:
	ComponentControlBlock * m_ctrlBlock;
	ComponentLayout * m_layout; 	

/*private slots:
	void addConn(ConnectorBase * conn);*/

};

//###############################################################

/** The property widget for @ref FloatLatch components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class FloatLatchPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	FloatLatchPropertyGeneralWidget(FloatLatch * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	FloatLatch* getComponent() { return (FloatLatch*) ComponentPropertyGeneralWidget::getComponent(); };
	
	/** Returns the a pointer to the @ref QLineEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimDoubleEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	int m_min;
	
	QSpinBox * m_channels;
	QLabel * m_channelsLabel;
	
	KSimDoubleEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


//###############################################################

};  //namespace KSimLibFloatingPoint

#endif
