/***************************************************************************
                          extconnfloatout.h  -  description
                             -------------------
    begin                : Sun Feb 10 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#ifndef EXTCONNFLOATOUT_H
#define EXTCONNFLOATOUT_H

#include "ksimus/externalconnector.h"

namespace KSimLibFloatingPoint
{

const ComponentInfo * getExtConnFloatOutInfo();


/**A external floating point output connector
  *@author Rasmus Diekenbrock
  */


class ExtConnFloatOut : public ExternalConnector
{
	Q_OBJECT

public: 
	ExtConnFloatOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnFloatOut();
	/** Executes the simulation of this component */
	virtual void calculate();

protected:
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
