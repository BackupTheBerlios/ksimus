/***************************************************************************
                          dataselector.h  -  description
                             -------------------
    begin                : Fri Feb 15 2002
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

#ifndef DATASELECTOR_H
#define DATASELECTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "float1out.h"

// Forward declaration
class ConnectorBoolInEdge;
class ConnectorFloatOut;
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

extern const ComponentInfo DataSelectorInfo;

/**A floating point latch.
  *@author Rasmus Diekenbrock
  */

class DataSelector : public KSimLibFloatingPoint::Float1Out
{

	Q_OBJECT

public:
	DataSelector(CompContainer * container, const ComponentInfo * ci);
//	~DataSelector();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Shifts the current component value (@ref setValue) to the output connector. */
//	virtual void updateOutput();
	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
//	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
//	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref FloatLatchPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(Component * comp, QWidget *parent);
	
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
	
	void setChannelCount(int count);
	void setChannelCountDelayed(int count);
	unsigned int getChannelCount() const;
	unsigned int getMinChannelCount() const;
	
	ConnectorPack * getInputPack() const { return m_inputPack; };
	ConnectorPack * getAddressPack() const { return m_addressPack; };
	ConnectorBoolInEdge * getLatchOutput() const { return m_latchOutput; };
	ConnectorBoolInEdge * getLatchAddress() const { return m_latchAddress; };



private slots:
	void slotAddChannel();
	void slotDeleteChannel();

private:
	unsigned int m_addressLatch;
	ConnectorBoolInEdge * m_latchOutput;
	ConnectorBoolInEdge * m_latchAddress;
	ConnectorPack * m_inputPack;
	ConnectorPack * m_addressPack;

};

//###############################################################

/** View class for @ref DataSelector views.
  *
  * @author Rasmus Diekenbrock
  */


class DataSelectorView : public CompView
{

	Q_OBJECT

public:
	DataSelectorView(DataSelector * comp, eViewType viewType);
//	~DataSelectorView();
	virtual void draw(QPainter * p);
	
	DataSelector* getComponent() { return (DataSelector*) CompView::getComponent(); };
	
	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };
	ComponentLayout * getComponentLayout() const { return m_layout; };


private:
	ComponentControlBlock * m_ctrlBlock;
	ComponentLayout * m_layout; 	

private slots:
	void addInConn(ConnectorBase * conn);
	void addAdrConn(ConnectorBase * conn);

};

//###############################################################

/** The property widget for @ref DataSelector components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class DataSelectorPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	DataSelectorPropertyGeneralWidget(DataSelector * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	DataSelector* getComponent() { return (DataSelector*) ComponentPropertyGeneralWidget::getComponent(); };
	
	/** Returns the a pointer to the @ref QLineEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimDoubleEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	QSpinBox * m_channels;
	QLabel * m_channelsLabel;
	
	KSimDoubleEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


//###############################################################

};  //namespace KSimLibFloatingPoint

#endif
