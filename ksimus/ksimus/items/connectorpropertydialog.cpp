/***************************************************************************
                          connectorpropertydialog.cpp  -  description
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

// C-Includes

// QT-Includes
#include <qvbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "component.h"
#include "connectorpropertydialog.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "ksimundo.h"
#include "propertywidget.h"

// Forward declaration

ConnectorPropertyDialog::ConnectorPropertyDialog(ConnectorList * connList, ConnectorBase * activeConn,
                                                 const QString & caption, QWidget *parent, const char *name )
	:	ComponentPropertyDialog((Component *)0, caption, parent, name)
{
	if (connList && connList->count())
	{
		PropertyWidget * wid;
		QVBox * page;
		setComponent(connList->first()->getComponent());
			
		FOR_EACH_CONNECTOR(it,*connList)
		{
			if (it.current()->getAction().isInitPropertyDialogEnabled())
			{
				page = addVBoxPage(it.current()->getName());
				wid = it.current()->propertyWidget(page);
				connectSlots(wid);
				
				if (it.current() == activeConn)
				{
					showPage(pageIndex(page));
				}
			}
		}
	}
}

ConnectorPropertyDialog::~ConnectorPropertyDialog()
{
}

