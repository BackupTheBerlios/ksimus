/***************************************************************************
                          compviewvbox.h  -  description
                             -------------------
    begin                : Sat Dec 29 2001
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

#ifndef COMPVIEWVBOX_H
#define COMPVIEWVBOX_H

#include "compviewhbox.h"

/** @ref QHBox class for compViews.
  * Use this class like @ref CompViewWidget or @ref CompViewHBox but with more features.
  *@author Rasmus Diekenbrock
  */

class CompViewVBox : public CompViewHBox
{

   Q_OBJECT

public:
	CompViewVBox(CompView * cv, QWidget *parent=0, const char *name=0)
		: CompViewHBox(false, cv, parent, name)
	{
	};
	
	~CompViewVBox()
	{
	};
};

#endif
