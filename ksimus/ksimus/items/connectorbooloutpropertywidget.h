/***************************************************************************
                          connectorbooloutpropertywidget.h  -  description
                             -------------------
    begin                : Sat Jan 13 2001
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

#ifndef CONNECTORBOOLOUTPROPERTYWIDGET_H
#define CONNECTORBOOLOUTPROPERTYWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "connectorpropertywidget.h"

// Forward declaration
class ConnectorBoolOut;

/**Property widget for boolean outputs
  *@author Rasmus Diekenbrock
  */

class ConnectorBoolOutPropertyWidget : public ConnectorPropertyWidget
{
	Q_OBJECT

public:
	ConnectorBoolOutPropertyWidget(ConnectorBoolOut * connector, QWidget *parent=0, const char *name=0);
	~ConnectorBoolOutPropertyWidget();

};

#endif
