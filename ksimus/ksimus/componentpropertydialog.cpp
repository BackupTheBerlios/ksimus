/***************************************************************************
                          componentpropertydialog.cpp  -  description
                             -------------------
    begin                : Sun Feb 11 2001
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
#include <qstringlist.h>

// KDE-Includes
#include <klocale.h>
#include <kmessagebox.h>

// Project-Includes
#include "componentpropertydialog.h"
#include "component.h"
#include "ksimundo.h"
#include "loglist.h"
#include "loglistitem.h"

// Forward declaration



ComponentPropertyDialog::ComponentPropertyDialog(Component * comp, QString * caption, QWidget *parent, const char *name )
	:	KDialogBase(TreeList,
					caption ? *caption : i18n("Component Properties"),
					Default | Ok | Cancel,
 					Ok,
 					parent,
 					name),
		m_component(comp),
		m_dataChanged(false)		
{
}

ComponentPropertyDialog::~ComponentPropertyDialog()
{
	if (m_dataChanged)
	{
		getComponent()->refresh();
	}
}

void ComponentPropertyDialog::connectSlots(QWidget * wid)
{
	connect(this, SIGNAL(okClicked()), wid, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), wid, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), wid, SLOT(slotCancel()));
	connect(wid, SIGNAL(signalChangeData()), this, SLOT(slotDataChanged()));
}


void ComponentPropertyDialog::slotOk()
{
	unsigned int errors = 0;
	
	emit okClicked();
	
	if (m_dataChanged)
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
	}
}


void ComponentPropertyDialog::slotDataChanged()
{
	if (!m_dataChanged)
	{
		getComponent()->undoChangeProperty(i18n("Change Component Properties"));
		m_dataChanged = true;
		getComponent()->setModified();
	}
}
