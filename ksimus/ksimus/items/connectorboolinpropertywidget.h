/***************************************************************************
                          connectorboolinpropertywidget.h  -  description
                             -------------------
    begin                : Sat Jan 27 2001
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

#ifndef CONNECTORBOOLINPROPERTYWIDGET_H
#define CONNECTORBOOLINPROPERTYWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorpropertywidget.h"

// Forward declaration
class QCheckBox;



/**Property class for bool input connectors
  *@author Rasmus Diekenbrock
  */

class ConnectorBoolInPropertyWidget : public ConnectorPropertyWidget  {
   Q_OBJECT
public: 
	ConnectorBoolInPropertyWidget(ConnectorBase * connector, QWidget *parent=0, const char *name=0);
	~ConnectorBoolInPropertyWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

protected:
	QCheckBox * m_negate;

};

#endif
