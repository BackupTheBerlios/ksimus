/***************************************************************************
                          ksimlineedit.cpp  -  description
                             -------------------
    begin                : Sun Apr 22 2001
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
#include "ksimlineedit.h"
#include "ksimdebug.h"

// Forward declaration


KSimLineEdit::KSimLineEdit( QWidget *parent, const char *name )
	:	QLineEdit(parent,name)
{
//	connect(this, SIGNAL(textChanged(const QString&)),SLOT(slotChanged()));
	connect(this, SIGNAL(returnPressed()),SLOT(slotChanged()));

}

KSimLineEdit::KSimLineEdit(const QString & contents, QWidget *parent, const char *name)
	:	QLineEdit(contents,parent,name)
{
//	connect(this, SIGNAL(textChanged(const QString&)),SLOT(slotChanged()));
	connect(this, SIGNAL(returnPressed()),SLOT(slotChanged()));
}

/*KSimLineEdit::~KSimLineEdit()
{
} */

void KSimLineEdit::focusOutEvent(QFocusEvent * ev)
{
	QLineEdit::focusOutEvent(ev);
	slotChanged();
/*	if (edited())
	{
		emit changed();
		emit changed(text ());
		setEdited(false);
	}*/
}

void KSimLineEdit::slotChanged()
{
	if (edited())
	{
		emit changed();
		emit changed(text ());
		setEdited(false);
	}
}

