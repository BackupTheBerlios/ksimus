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

/**Property class for bool input connectors
  *@author Rasmus Diekenbrock
  */

class ConnectorBoolInPropertyWidget : public ConnectorPropertyWidget  {
   Q_OBJECT
public: 
	ConnectorBoolInPropertyWidget(ConnectorBase * connector, QWidget *parent=0, const char *name=0);
	~ConnectorBoolInPropertyWidget();

};

#endif
