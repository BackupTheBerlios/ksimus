/***************************************************************************
                          ksimbaseintbaseselection.h  -  description
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

#ifndef KSIMBASEINTBASESELECTION_H
#define KSIMBASEINTBASESELECTION_H

// C-Includes

// QT-Includes
#include <qcombobox.h>

// KDE-Includes

// Project-Includes
#include "ksimbaseint.h"

/** A combo box for base selection for @ref KSimBaseInt objects.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseIntBaseSelection : public QComboBox
{

	Q_OBJECT

public:
  /** Constructs a combo box for selecting the base of a @ref KSimBaseInt object. The default is decimal. */
	KSimBaseIntBaseSelection(QWidget * parent = 0, const char * name = 0);
  /** Constructs a combo box for selecting the base of a @ref KSimBaseInt object. The default is given by parameter base. */
	KSimBaseIntBaseSelection(KSimBaseInt::eBaseType base, QWidget * parent = 0, const char * name = 0);

	/** Sets the current base. */
	void setBase(KSimBaseInt::eBaseType base);

	/** Returns the current base. */
	KSimBaseInt::eBaseType base() const;
	KSimBaseInt::eBaseType getBase() const { return this->base(); };


private:
	void init();
};

#endif
