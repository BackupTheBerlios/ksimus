/***************************************************************************
                          ksimbaseuintbaseselection.cpp  -  description
                             -------------------
    begin                : Sun Dec 23 2001
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
#include "ksimbaseuintbaseselection.h"
#include "ksimdebug.h"

// Forward declaration



KSimBaseUIntBaseSelection::KSimBaseUIntBaseSelection(QWidget * parent, const char * name)
	:	QComboBox(parent, name)
{
	init();
}

KSimBaseUIntBaseSelection::KSimBaseUIntBaseSelection(KSimBaseUInt::eBaseType base, QWidget * parent, const char * name)
	:	QComboBox(parent, name)
{
	init();
	setBase(base);
}

void KSimBaseUIntBaseSelection::init()
{
	insertItem(i18n("integer base", "Binary"),      0);
	insertItem(i18n("integer base", "Octal"),       1);
	insertItem(i18n("integer base", "Decimal"),     2);
	insertItem(i18n("integer base", "Hexadecimal"), 3);
}

void KSimBaseUIntBaseSelection::setBase(KSimBaseUInt::eBaseType base)
{
	switch(base)
	{
		case KSimBaseUInt::Binary:       setCurrentItem(0); break;
		case KSimBaseUInt::Octal:        setCurrentItem(1); break;
		case KSimBaseUInt::Decimal:      setCurrentItem(2); break;
		case KSimBaseUInt::Hexadecimal:  setCurrentItem(3); break;
	}
}

KSimBaseUInt::eBaseType KSimBaseUIntBaseSelection::base() const
{
	KSimBaseUInt::eBaseType base;

	switch(currentItem())
	{
		case 0:  base = KSimBaseUInt::Binary;      break;
		case 1:  base = KSimBaseUInt::Octal;       break;
		case 2:  base = KSimBaseUInt::Decimal;     break;
		case 3:  base = KSimBaseUInt::Hexadecimal; break;
		default: base = KSimBaseUInt::Decimal;     break;
	}

	return base;
}
