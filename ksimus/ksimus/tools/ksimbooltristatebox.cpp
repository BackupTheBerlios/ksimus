/***************************************************************************
                          ksimbooleanbox.cpp  -  description
                             -------------------
    begin                : Tue Dec 4 2001
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
#include "ksimdebug.h"
#include "ksimbooleanbox.h"

// Forward declaration



#define ID_FALSE    0
#define ID_TRUE     1



KSimBooleanBox::KSimBooleanBox(QWidget *parent, const char *name )
	: QComboBox(parent,name)
{
	init();
}

KSimBooleanBox::KSimBooleanBox(bool value, QWidget *parent, const char *name )
	: QComboBox(parent,name)
{
	init();
	setValue(value);
}



void KSimBooleanBox::init()
{
	insertItem(i18n("Text for false in a combo box", "False"), ID_FALSE);
	insertItem(i18n("Text for true in a combo box", "True"), ID_TRUE);

	
	connect(this,SIGNAL(activated(int)),this,SLOT(slotActivated(int)));
	connect(this,SIGNAL(highlighted(int)),this,SLOT(slotHighlighted(int)));
}

KSimBooleanBox::~KSimBooleanBox()
{
}


void KSimBooleanBox::setFalseText(const QString & name)
{
	changeItem(name, ID_FALSE);
}

QString KSimBooleanBox::getFalseText() const
{
	return text(ID_FALSE);
}

		
void KSimBooleanBox::setTrueText(const QString & name)
{
	changeItem(name, ID_TRUE);
}

QString KSimBooleanBox::getTrueText() const
{
	return text(ID_TRUE);
}


void KSimBooleanBox::slotActivated(int i)
{
	if (i == ID_FALSE)
	{
		emit activated(false);
		emit deactivated(true);
	}
	else if (i == ID_TRUE)
	{
		emit activated(true);
		emit deactivated(false);
	}
	else
	{
		// Unknown
		KSIMDEBUG(QString::fromLatin1("Unkown input %1").arg(i));
	}
	
}

void KSimBooleanBox::slotHighlighted(int i)
{
	if (i == ID_FALSE)
	{
		emit highlighted(false);
	}
	else if (i == ID_TRUE)
	{
		emit highlighted(true);
	}
	else
	{
		// Unknown
		KSIMDEBUG(QString::fromLatin1("Unkown input %1").arg(i));
	}
	
}

void KSimBooleanBox::setValue(bool b)
{
	setCurrentItem(b ? ID_TRUE : ID_FALSE);
}

bool KSimBooleanBox::getValue() const
{
	return currentItem() == ID_TRUE;
}
