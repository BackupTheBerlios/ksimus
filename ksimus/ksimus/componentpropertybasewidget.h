/***************************************************************************
                          componentpropertybasewidget.h  -  description
                             -------------------
    begin                : Sat Feb 10 2001
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

#ifndef COMPONENTPROPERTYBASEWIDGET_H
#define COMPONENTPROPERTYBASEWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"

// Forward declaration
class Component;

/**Base widget for component properties
  *@author Rasmus Diekenbrock
  */

class ComponentPropertyBaseWidget : public PropertyWidget
{
	Q_OBJECT
public: 
	ComponentPropertyBaseWidget(Component * comp, QWidget *parent=0, const char *name=0);
//	~ComponentPropertyBaseWidget();
	
	/** Returns the component */
	Component * getComponent() const;


public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
//	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
//	virtual void slotDefault();
	/** The function slotCancel() is called, if user wants to cancel the dialog.
	 */
//	virtual void slotCancel();


protected:
	/** This should called from function slotAccept() before the data is changed.
		It manages to the undo functionality.
	*/
//	void changeData();
	
private:	
	/** The properties of this component are displayed
	*/
	Component * m_component;
	
//	bool m_dataChanged;


signals:
	/** This signal is emitted, after function slotAccept() is called and before the connector data is changed.
		It is connected to the undo functionality.
		Call function changeData(), this prevents unnecessary signals
	 */
//	void signalChangeData();
};

#endif
