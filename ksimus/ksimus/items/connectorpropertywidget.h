/***************************************************************************
                          connectorpropertywidget.h  -  description
                             -------------------
    begin                : Thu Jan 11 2001
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

#ifndef CONNECTORPROPERTYWIDGET_H
#define CONNECTORPROPERTYWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "componentpropertybasewidget.h"

// Forward declaration
class ConnectorBase;


/**Property widget for connectors
  *@author Rasmus Diekenbrock
  */


class ConnectorPropertyWidget : public ComponentPropertyBaseWidget
{
   Q_OBJECT


class Private;


public:
	ConnectorPropertyWidget(ConnectorBase * connector, QWidget *parent=0, const char *name=0);
	~ConnectorPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

public slots:
	
	virtual void polish();
	
protected:
	/** Use this widget as parent widget for the full row setting widget.
	*/
	QWidget * rowWidget();
	/** Use this widget as parent widget for the box setting widget.
	*/
	QWidget * boxWidget();
	/** Use this widget as parent widget for simple check boxes.
	*/
	QWidget * settingWidget();
	
	ConnectorBase * m_conn;


private:
	Private * m_p;

};

#endif
