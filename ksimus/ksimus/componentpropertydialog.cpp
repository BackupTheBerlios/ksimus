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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "componentpropertydialog.h"
#include "component.h"
#include "ksimundo.h"
#include "ksimdebug.h"

// Forward declaration



//#########################################################################
//#########################################################################

ComponentPropertyDialog::ComponentPropertyDialog(Component * comp, const QString & caption, QWidget *parent, const char *name )
	:	KSimDialog(caption, parent, name),
		ComponentItem(comp)
{
}

ComponentPropertyDialog::~ComponentPropertyDialog()
{
	if (isChanged())
	{
		getComponent()->refresh();
	}
}

void ComponentPropertyDialog::slotOk()
{
	unsigned int errors = 0;
	
	emit okClicked();
	
	if (isChanged())
	{
		// Only if something changed	
		errors = getComponent()->executePropertyCheck();
	
		if(errors)
		{
			setChanged(false);
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
	if (!isChanged())
	{
		getComponent()->undoChangeProperty(i18n("Change Component Properties"));
		setChanged(true);
		getComponent()->setModified();
	}
}


int ComponentPropertyDialog::execute(Component *comp, const QString & caption, QWidget *parent, const char *name)
{
	int result;

	ComponentPropertyDialog * dia;
	dia = new ComponentPropertyDialog(comp, caption, parent, name);
	comp->initPropertyDialog(dia);

	// Load last size
	dia->readSize("Component/Property Dialog");

	result = dia->exec();

	// Save size
	dia->writeSize("Component/Property Dialog");

	delete dia;

	return result;
}

#include "componentpropertydialog.moc"
