/***************************************************************************
                          connectorpropertydialog.h  -  description
                             -------------------
    begin                : Fri Jan 12 2001
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

#ifndef CONNECTORPROPERTYDIALOG_H
#define CONNECTORPROPERTYDIALOG_H

// C-Includes

// QT-Includes

// KDE-Includes
#include <kdialogbase.h>

// Project-Includes
#include "componentitem.h"

// Forward declaration
class QString;
class Component;
class ConnectorList;
class ConnectorBase;

/**A property dialog for one or more connectors
  *@author Rasmus Diekenbrock
  */

class ConnectorPropertyDialog : public KDialogBase, public ComponentItem
{
   Q_OBJECT
public: 
	ConnectorPropertyDialog(ConnectorList * connList, ConnectorBase * activeConn = 0,
	                        QString * caption = 0, QWidget *parent=0, const char *name=0);
	~ConnectorPropertyDialog();

protected slots:
	void slotDataChanged();
	virtual void slotOk();

private:
	bool m_dataChanged;

};

#endif
