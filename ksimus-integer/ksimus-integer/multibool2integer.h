/***************************************************************************
                          multibool2integer.h  -  description
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

#ifndef MULTIBOOL2INTEGER_H
#define MULTIBOOL2INTEGER_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "integer1out.h"

// Forward declaration
class KSimSpinBox;
class QLabel;
class ConnectorPack;


//###############################################################
//###############################################################


namespace KSimLibInteger
{


/** The gate converts several boolean inputs into an integer ooutput.
	*
  * @author Rasmus Diekenbrock
  */

class MultiBool2Integer : public Integer1Out
{

	Q_OBJECT

public:
	MultiBool2Integer(CompContainer * container, const ComponentInfo * ci);
// ~MultiBool2Integer();

	/** Executes the simulation of this component */
	virtual void calculate();

	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref MultiBool2Integer::PropertyGeneralWidget.
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
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };
	void setInputCount(unsigned int count);
	void setInputCountDelayed(unsigned int count);
	int getInputCount() const;

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:


private:
	ConnectorPack * m_inPack;

	static int tempConnCount;
	static MultiBool2Integer * tempConnCountOwner;
	class PropertyGeneralWidget;
	class View;

private slots:
	void addConn(ConnectorBase * conn);
};



//###############################################################
//###############################################################

/** View class for @ref MultiBool2Integer views.
  *
  * @author Rasmus Diekenbrock
  */

class MultiBool2Integer::View : public CompView
{
public:
	View(MultiBool2Integer * comp, eViewType viewType, const char *name);
//	~View();
};


//###############################################################
//###############################################################

/** The property widget for @ref IntegerLatch components.
  * This class adds to the general property dialog a item for selecting the reset value of the component.
  *
  * @author Rasmus Diekenbrock
  */


class MultiBool2Integer::PropertyGeneralWidget : public Integer1OutPropertyGeneralWidget
{
public:
	PropertyGeneralWidget(MultiBool2Integer * comp, QWidget *parent=0, const char *name=0);

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

	MultiBool2Integer* getComponent() { return (MultiBool2Integer*) Integer1OutPropertyGeneralWidget::getComponent(); };

protected:
	int m_min;

	KSimSpinBox * m_inputs;
	QLabel * m_inputsLabel;
};


//###############################################################
//###############################################################


};  //namespace KSimLibInteger
#endif
