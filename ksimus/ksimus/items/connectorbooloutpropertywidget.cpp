/***************************************************************************
                          connectorbooloutpropertywidget.cpp  -  description
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


// C-Includes

// QT-Includes


// KDE-Includes

// Project-Includes
#include "connectorbooloutpropertywidget.h"
#include "connectorboolout.h"

// Forward declaration


ConnectorBoolOutPropertyWidget::ConnectorBoolOutPropertyWidget(ConnectorBoolOut * connector, QWidget *parent, const char *name)
	:	ConnectorPropertyWidget(connector, parent, name)
{
}



ConnectorBoolOutPropertyWidget::~ConnectorBoolOutPropertyWidget()
{

}


