/***************************************************************************
                          propertywidget.h  -  description
                             -------------------
    begin                : Sat Mar 10 2001
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

#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

// C-Includes
#include <qwidget.h>

// QT-Includes

// KDE-Includes

// Project-Includes

// Forward declaration


/**Base widget for all property widgets
  *@author Rasmus Diekenbrock
  */

class PropertyWidget : public QWidget
{
   Q_OBJECT
public: 
	/** Creates the widget
	*/
	PropertyWidget(QWidget *parent=0, const char *name=0);
//	~PropertyWidget();


public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void slotDefault();
	/** The function slotCancel() is called, if user wants to cancel the dialog.
	 */
	virtual void slotCancel();


protected:
	/** This should called from function slotAccept() before the data is changed.
		It manages to the undo functionality.
	*/
	void changeData();
	
private:	


signals:
	/** This signal is emitted, after function slotAccept() is called and before the connector data is changed.
		It is connected to the undo functionality.
		Call function changeData(), this prevents unnecessary signals
	 */
	void signalChangeData();
};

#endif
