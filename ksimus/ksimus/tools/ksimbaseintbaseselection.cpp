/***************************************************************************
                          ksimbaseintbaseselection.cpp  -  description
                             -------------------
    begin                : Mon Jul 28 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include "ksimbaseintbaseselection.h"
#include "ksimdebug.h"

// Forward declaration



KSimBaseIntBaseSelection::KSimBaseIntBaseSelection(QWidget * parent, const char * name)
	:	QComboBox(parent, name)
{
	init();
}

KSimBaseIntBaseSelection::KSimBaseIntBaseSelection(KSimBaseInt::eBaseType base, QWidget * parent, const char * name)
	:	QComboBox(parent, name)
{
	init();
	setBase(base);
}

void KSimBaseIntBaseSelection::init()
{
	insertItem(i18n("integer base", "Binary"),      0);
	insertItem(i18n("integer base", "Octal"),       1);
	insertItem(i18n("integer base", "Decimal"),     2);
	insertItem(i18n("integer base", "Hexadecimal"), 3);
}

void KSimBaseIntBaseSelection::setBase(KSimBaseInt::eBaseType base)
{
	switch(base)
	{
		case KSimBaseInt::Binary:       setCurrentItem(0); break;
		case KSimBaseInt::Octal:        setCurrentItem(1); break;
		case KSimBaseInt::Decimal:      setCurrentItem(2); break;
		case KSimBaseInt::Hexadecimal:  setCurrentItem(3); break;
	}
}

KSimBaseInt::eBaseType KSimBaseIntBaseSelection::base() const
{
	KSimBaseInt::eBaseType base;

	switch(currentItem())
	{
		case 0:  base = KSimBaseInt::Binary;      break;
		case 1:  base = KSimBaseInt::Octal;       break;
		case 2:  base = KSimBaseInt::Decimal;     break;
		case 3:  base = KSimBaseInt::Hexadecimal; break;
		default: base = KSimBaseInt::Decimal;     break;
	}
	return base;
}
