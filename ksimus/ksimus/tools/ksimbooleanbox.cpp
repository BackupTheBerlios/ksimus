/***************************************************************************
                          ksimbooltristatebox.cpp  -  description
                             -------------------
    begin                : Thu Nov 14 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include "ksimdebug.h"
#include "ksimbooltristatebox.h"

// Forward declaration

KSimBoolTristateBox::KSimBoolTristateBox(QWidget *parent, const char *name )
	: QComboBox(parent,name)
{
	init();
}

KSimBoolTristateBox::KSimBoolTristateBox(bool value, bool active, QWidget *parent, const char *name )
	: QComboBox(parent,name)
{
	init();
	setValue(value, active);
}

KSimBoolTristateBox::KSimBoolTristateBox(KSimBoolTriState state, QWidget *parent, const char *name)
	: QComboBox(parent,name)
{
	init();
	setValue(state);
}

void KSimBoolTristateBox::init()
{
	insertItem(i18n("Text for false in a combo box", "False"), KSIMBOOLTRISTATE_FALSE);
	insertItem(i18n("Text for true in a combo box", "True"), KSIMBOOLTRISTATE_TRUE);
	insertItem(i18n("Text for inactive in a combo box", "Inactive"), KSIMBOOLTRISTATE_INACTIVE);

	
	connect(this,SIGNAL(activated(int)),this,SLOT(slotActivated(int)));
	connect(this,SIGNAL(highlighted(int)),this,SLOT(slotHighlighted(int)));
}

KSimBoolTristateBox::~KSimBoolTristateBox()
{
}


void KSimBoolTristateBox::setFalseText(const QString & name)
{
	changeItem(name, KSIMBOOLTRISTATE_FALSE);
}

QString KSimBoolTristateBox::getFalseText() const
{
	return text(KSIMBOOLTRISTATE_FALSE);
}

		
void KSimBoolTristateBox::setTrueText(const QString & name)
{
	changeItem(name, KSIMBOOLTRISTATE_TRUE);
}

QString KSimBoolTristateBox::getTrueText() const
{
	return text(KSIMBOOLTRISTATE_TRUE);
}

void KSimBoolTristateBox::setActiveText(const QString & name)
{
	changeItem(name, KSIMBOOLTRISTATE_INACTIVE);
}

QString KSimBoolTristateBox::getActiveText() const
{
	return text(KSIMBOOLTRISTATE_INACTIVE);
}


void KSimBoolTristateBox::slotActivated(int i)
{
	switch((eKSimBoolTriState)i)
	{
		case KSIMBOOLTRISTATE_FALSE:
		case KSIMBOOLTRISTATE_TRUE:
		case KSIMBOOLTRISTATE_INACTIVE:
			emit activated(KSimBoolTriState((eKSimBoolTriState)i));
			break;

		default:
		// Unknown
		KSIMDEBUG(QString::fromLatin1("Unkown input %1").arg(i));
	}
	
}

void KSimBoolTristateBox::slotHighlighted(int i)
{
	switch((eKSimBoolTriState)i)
	{
		case KSIMBOOLTRISTATE_FALSE:
		case KSIMBOOLTRISTATE_TRUE:
		case KSIMBOOLTRISTATE_INACTIVE:
			emit highlighted(KSimBoolTriState((eKSimBoolTriState)i));
			break;

		default:
		// Unknown
		KSIMDEBUG(QString::fromLatin1("Unkown input %1").arg(i));
	}
	
}

void KSimBoolTristateBox::setValue(KSimBoolTriState state)
{
	setCurrentItem((int)state);
}

void KSimBoolTristateBox::setValue(bool state, bool active)
{
	if (!active)
	{
		setCurrentItem((int)KSIMBOOLTRISTATE_INACTIVE);
	}
	else if (state)
	{
		setCurrentItem((int)KSIMBOOLTRISTATE_TRUE);
	}
	else
	{
		setCurrentItem((int)KSIMBOOLTRISTATE_FALSE);
	}
}

KSimBoolTriState KSimBoolTristateBox::getValue() const
{
	return KSimBoolTriState((eKSimBoolTriState)currentItem());
}
