/***************************************************************************
                          extconnintegerout.h  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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

#ifndef EXTCONNINTEGROUT_H
#define EXTCONNINTEGROUT_H

#include "ksimus/externalconnector.h"

namespace KSimLibInteger
{

/**A external integer output connector
  *@author Rasmus Diekenbrock
  */


class ExtConnIntegerOut : public ExternalConnector
{
public: 
	ExtConnIntegerOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnIntegerOut();
	/** Executes the simulation of this component */
	virtual void calculate();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

protected:
};

//###############################################################


};  //namespace KSimLibInteger
#endif
