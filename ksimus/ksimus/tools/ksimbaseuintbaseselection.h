/***************************************************************************
                          ksimbaseuintbaseselection.h  -  description
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

#ifndef KSIMBASEUINTBASESELECTION_H
#define KSIMBASEUINTBASESELECTION_H

// C-Includes

// QT-Includes
#include <qcombobox.h>

// KDE-Includes

// Project-Includes
#include "ksimbaseuint.h"

// Forward declaration


/** A combo box for base selection for @ref KSimBaseUInt objects.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseUIntBaseSelection : public QComboBox
{

	Q_OBJECT

public:
  /** Constructs a combo box for selecting the base of a @ref KSimBaseUInt object. The default is decimal. */
	KSimBaseUIntBaseSelection(QWidget * parent = 0, const char * name = 0);
  /** Constructs a combo box for selecting the base of a @ref KSimBaseUInt object. The default is given by parameter base. */
	KSimBaseUIntBaseSelection(KSimBaseUInt::eBaseType base, QWidget * parent = 0, const char * name = 0);

	/** Sets the current base. */
	void setBase(KSimBaseUInt::eBaseType base);

	/** Returns the current base. */
	KSimBaseUInt::eBaseType base() const;
	KSimBaseUInt::eBaseType getBase() const { return this->base(); };


private:
	void init();
};

#endif
