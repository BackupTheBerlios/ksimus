/***************************************************************************
                          booleanxor.h  -  description
                             -------------------
    begin                : Mon May 14 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BOOLEANXOR_H
#define BOOLEANXOR_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/booleanxin1out.h"

// Forward declaration

/**A boolean Xor
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{


class BooleanXor : public BooleanXIn1Out
{
public:
	BooleanXor(CompContainer * container, const ComponentInfo * ci);
//	~BooleanXor();
	
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticXorInfo();
	static const ComponentInfo * getStaticXnorInfo();
};

//###############################################################

class BooleanXorView : public BooleanXIn1OutView
{
public:
	BooleanXorView(BooleanXor * comp, eViewType viewType);
//	~BooleanOrView();
	virtual void draw(QPainter * p);
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
