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


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();
	/** The function cancelPressed() is called, if user wants to cancel the dialog.
	 */
	virtual void cancelPressed();


public slots:
	/** The slot slotAccept() is called if user press the accept button.
	  * The slot calls the virtual function @ref acceptPressed().
		* Do not change this function!
	 */
	void slotAccept();
	/** The slot slotDefault() is called if user press the default button.
	  * The slot calls the virtual function @ref defaultPressed().
		* Do not change this function!
	 */
	void slotDefault();
	/** The slot slotCancel() is called if user press the cancel button.
	  * The slot calls the virtual function @ref cancelPressed().
		* Do not change this function!
	 */
	void slotCancel();
	


protected:
	/** This should called from function acceptPressed() before the data is changed.
		It manages to the undo functionality.
	*/
	void changeData();
	
private:	

private slots:

signals:
	/** This signal is emitted, after function slotAccept() is called and before the connector data is changed.
		It is connected to the undo functionality.
		Call function changeData(), this prevents unnecessary signals
	 */
	void signalChangeData();
};

#endif
