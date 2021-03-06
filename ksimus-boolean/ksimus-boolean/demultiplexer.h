/***************************************************************************
                          demultiplexer.h  -  description
                             -------------------
    begin                : Tue Jun 18 2002
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

#ifndef DEMULTIPLEXER_H
#define DEMULTIPLEXER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentpropertygeneralwidget.h"

// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolInEdge;
class ConnectorBoolInOut;
class ConnectorPack;
class ComponentLayoutVerticalCtrl;
class ConnectorLabel;
class KSimSpinBox;
class QLabel;


//###############################################################
//###############################################################


namespace KSimLibBoolean
{

/**A boolean multiplexer.
  *@author Rasmus Diekenbrock
  */

class Demultiplexer : public Component
{

	Q_OBJECT

public:
	Demultiplexer(CompContainer * container, const ComponentInfo * ci);
//	~Demultiplexer();

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
	  * This function creeates a @ref DemultiplexerPropertyGeneralWidget.
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
	  */
	virtual void menuExecuted();
	
	void setChannelCount(unsigned int count);
	void setChannelCountDelayed(unsigned int count);
	unsigned int getChannelCount() const;
	unsigned int getMinChannelCount() const;
	
	ConnectorPack * getOutputPack() const { return m_outputPack; };
	ConnectorPack * getAddressPack() const { return m_addressPack; };
	ConnectorBoolInEdge * getLatchOutput() const { return m_latchOutput; };
	ConnectorBoolInEdge * getLatchAddress() const { return m_latchAddress; };
	ConnectorBoolIn * getInputConn() const { return m_inputConn; };


	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
	static unsigned int tempConnCount;
	static Demultiplexer * tempConnCountOwner;

private slots:
	void slotAddChannel();
	void slotDeleteChannel();

private:
	unsigned int m_addressLatch;
	ConnectorBoolInEdge * m_latchOutput;
	ConnectorBoolIn * m_inputConn;
	ConnectorBoolInEdge * m_latchAddress;
	ConnectorPack * m_outputPack;
	ConnectorPack * m_addressPack;

};

//###############################################################

/** View class for @ref Demultiplexer views.
  *
  * @author Rasmus Diekenbrock
  */


class DemultiplexerView : public CompView
{

	Q_OBJECT

public:
	DemultiplexerView(Demultiplexer * comp, eViewType viewType, const char * name);
//	~DemultiplexerView();
	
	Demultiplexer* getComponent() { return (Demultiplexer*) CompView::getComponent(); };
	
private slots:
	void addOutConn(ConnectorBase * conn);
	void addAdrConn(ConnectorBase * conn);

};

//###############################################################

/** The property widget for @ref Demultiplexer components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class DemultiplexerPropertyGeneralWidget : public ComponentPropertyGeneralWidget
{
	Q_OBJECT

public:
	DemultiplexerPropertyGeneralWidget(Demultiplexer * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	Demultiplexer* getComponent() { return (Demultiplexer*) ComponentPropertyGeneralWidget::getComponent(); };

protected:
	KSimSpinBox * m_channels;
	QLabel * m_channelsLabel;
};


//###############################################################


//###############################################################

};  //namespace KSimLibBoolean

#endif
