/***************************************************************************
                          ksimspinbox.h  -  description
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

#ifndef KSIMSPINBOX_H
#define KSIMSPINBOX_H

// C-Includes

// QT-Includes
#include <qspinbox.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes

// Forward declaration

/**An enhanced QSpinBox.
  *@author Rasmus Diekenbrock
  */

class KSimSpinBox : public QSpinBox
{
   Q_OBJECT
public: 

	KSimSpinBox(QWidget* parent = 0, const char *name = 0);
	KSimSpinBox(int minValue, int maxValue, int step = 1, QWidget* parent = 0, const char* name = 0);

	/** Sets the alignment of the editor. */
	void setAlignment(int flag);
	void setAlignRight();
	void setAlignLeft();

	/** Returns the alignment of the editor. */
	int alignment() const;
};

#endif
