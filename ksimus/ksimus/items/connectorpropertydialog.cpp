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
#include <qlabel.h>
#include <qlayout.h>
#include <qhgroupbox.h>
#include <qpushbutton.h>
#include <qscrollview.h>

// KDE-Includes
#include <klocale.h>
#include <kmessagebox.h>

// Project-Includes
#include "component.h"
#include "connectorpropertydialog.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "ksimundo.h"

// Forward declaration



ConnectorPropertyDialog::ConnectorPropertyDialog(ConnectorList * connList, ConnectorBase * activeConn,
                                                 QString * caption, QWidget *parent, const char *name )
	:	KDialogBase(TreeList,
					caption ? *caption : i18n("Connector Properties"),
					Default | Ok | Cancel,
 					Ok,
 					parent,
 					name),
 		ComponentItem((Component *)0),
		m_dataChanged(false)		
{
	if (connList && connList->count())
	{
		QHGroupBox * box;
		QWidget * wid;
		QVBox * page;

		setComponent(connList->first()->getComponent());
			
		FOR_EACH_CONNECTOR(it,*connList)
		{
			if (it.current()->getAction().isInitPropertyDialogEnabled())
			{
				page = addVBoxPage(it.current()->getName());
				box = new QHGroupBox (page);
				wid = it.current()->propertyWidget(box);
				connect(this, SIGNAL(defaultClicked()), wid, SLOT(slotDefault()));
				connect(this, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
				connect(wid, SIGNAL(signalChangeData()), this, SLOT(slotDataChanged()));
				
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

void ConnectorPropertyDialog::slotOk()
{
//	unsigned int errors = 0;
	QStringList errMsg;
	emit okClicked();
	
/*	if (m_dataChanged)
	{
		// Only if something changed	
		errors = getComponent()->executePropertyCheck();
	
		if(errors)
		{	
	    m_dataChanged = false;
		}
		else
		{
			// No errors
			QDialog::accept();
		}
	}
	else
	{
		// Nothing changed
		QDialog::accept();
	}*/
	getComponent()->checkProperty(errMsg);
	
	if(errMsg.count())
	{	
		// Error detected
		QString errText(i18n("Property Errors"));
		errText += "\n" + errMsg.join("\n");
		
		logError(errText);
		
		KMessageBox::error(0,errText,i18n("Property Errors"));

		// Restore data
		if(m_dataChanged && (getComponent()->getUndo()))
		{
			getComponent()->getUndo()->hiddenUndo();
	    m_dataChanged = false;
		}
		
	}
	else
	{
		// No errors
		QDialog::accept();
	}
}

void ConnectorPropertyDialog::slotDataChanged()
{
	if (!m_dataChanged)
	{
		getComponent()->undoChangeProperty(i18n("Change Connector Properties"));
		m_dataChanged = true;
		getComponent()->setModified();
	}
}
