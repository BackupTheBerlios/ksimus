/***************************************************************************
                          integerdataselector.h  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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

#ifndef INTEGERDATASELECTOR_H
#define INTEGERDATASELECTOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"
#include "integer1out.h"

// Forward declaration
class ConnectorBoolInEdge;
class ConnectorIntegerOut;
class ConnectorPack;
class ComponentLayout;
class ConnectorLabel;
class KSimSpinBox;
class QLabel;
class KSimBaseIntEdit;


//###############################################################
//###############################################################


namespace KSimLibInteger
{

/**An integer latch.
  *@author Rasmus Diekenbrock
  */

class IntegerDataSelector : public Integer1Out
{

	Q_OBJECT

public:
	IntegerDataSelector(CompContainer * container, const ComponentInfo * ci);
//	~IntegerDataSelector();

	/** Executes the simulation of this component */
	virtual void calculate();
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
	  * This function creeates a @ref IntegerDataSelectorPropertyGeneralWidget.
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
	
	void setChannelCount(unsigned int count);
	void setChannelCountDelayed(unsigned int count);
	unsigned int getChannelCount() const;
	unsigned int getMinChannelCount() const;
	
	ConnectorPack * getInputPack() const { return m_inputPack; };
	ConnectorPack * getAddressPack() const { return m_addressPack; };
	ConnectorBoolInEdge * getLatchOutput() const { return m_latchOutput; };
	ConnectorBoolInEdge * getLatchAddress() const { return m_latchAddress; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private slots:
	void slotAddChannel();
	void slotDeleteChannel();

private:
	unsigned int m_addressLatch;
	ConnectorBoolInEdge * m_latchOutput;
	ConnectorBoolInEdge * m_latchAddress;
	ConnectorPack * m_inputPack;
	ConnectorPack * m_addressPack;
	static unsigned int tempConnCount;
	static IntegerDataSelector * tempConnCountOwner;


};

//###############################################################

/** View class for @ref IntegerDataSelector views.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerDataSelectorView : public CompView
{

	Q_OBJECT

public:
	IntegerDataSelectorView(IntegerDataSelector * comp, eViewType viewType);
//	~IntegerDataSelectorView();
	
	IntegerDataSelector* getComponent() { return (IntegerDataSelector*) CompView::getComponent(); };

private slots:
	void addInConn(ConnectorBase * conn);
	void addAdrConn(ConnectorBase * conn);

};

//###############################################################

/** The property widget for @ref IntegerDataSelector components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class IntegerDataSelectorPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
public:
	IntegerDataSelectorPropertyGeneralWidget(IntegerDataSelector * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	IntegerDataSelector* getComponent() { return (IntegerDataSelector*) ComponentPropertyGeneralWidget::getComponent(); };
	
	/** Returns the a pointer to the @ref QLineEdit. Use this to set a appropriate @ref QToolTip
	  * and @ref QWhatsThis. */
	KSimBaseIntEdit * getResetValueEdit() const { return m_resetValue; };
	/** Returns the a pointer to the label of the @ref getResetValueEdit. Use this to set a appropriate
	  * @ref QToolTip and @ref QWhatsThis. */
	QLabel * getResetValueLabel() const { return m_resetValueLabel; };
	
protected:
	KSimSpinBox * m_channels;
	QLabel * m_channelsLabel;
	
	KSimBaseIntEdit * m_resetValue;
	QLabel * m_resetValueLabel;
};


//###############################################################


//###############################################################

};  //namespace KSimLibInteger

#endif
