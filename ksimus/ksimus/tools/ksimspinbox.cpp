/***************************************************************************
                          ksimspinbox.cpp  -  description
                             -------------------
    begin                : Sun Sep 7 2003
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
#include <qlineedit.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes
#include "ksimspinbox.h"

// Forward declaration

KSimSpinBox::KSimSpinBox(QWidget* parent, const char *name)
	:	QSpinBox(parent, name)
{
	setAlignRight();
};

KSimSpinBox::KSimSpinBox(int minValue, int maxValue, int step, QWidget* parent, const char* name)
	:	QSpinBox(minValue, maxValue, step, parent, name)
{
	setAlignRight();
};

void KSimSpinBox::setAlignment(int flag)
{
	editor()->setAlignment(flag);
}

void KSimSpinBox::setAlignRight()
{
	setAlignment(Qt::AlignRight);
}

void KSimSpinBox::setAlignLeft()
{
	setAlignment(Qt::AlignLeft);
}


int KSimSpinBox::alignment() const
{
	return editor()->alignment();
}


