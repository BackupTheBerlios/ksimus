/***************************************************************************
                          booleanor.h  -  description
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

#ifndef BOOLEANOR_H
#define BOOLEANOR_H


/**A boolean or and nor gate
  *@author Rasmus Diekenbrock
  */

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/booleanxin1out.h"

// Forward declaration

/**A boolean and gate
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{

const ComponentInfo * getBooleanOrInfo();
const ComponentInfo * getBooleanNorInfo();

class BooleanOr : public BooleanXIn1Out
{
public:
	BooleanOr(CompContainer * container, const ComponentInfo * ci);
	
	/** Executes the simulation of this component */
	virtual void calculate();
};

//###############################################################

class BooleanOrView : public BooleanXIn1OutView
{
public:
	BooleanOrView(BooleanOr * comp, eViewType viewType);
	
	virtual void draw(QPainter * p);
};

}; //namespace KSimLibBoolean

#endif
