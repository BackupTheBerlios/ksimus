/***************************************************************************
                          extconnbooltrisate.h  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#ifndef EXTCONNBOOLTRISTATE_H
#define EXTCONNBOOLTRISTATE_H

#include "ksimus/externalconnector.h"

namespace KSimLibBoolean
{

/**A external boolean input connector
  *@author Rasmus Diekenbrock
  */

const ComponentInfo * getExtConnBoolTriStateInInfo();

class ExtConnBoolTriStateIn : public ExternalConnector
{
public: 
	ExtConnBoolTriStateIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateIn();
};

const ComponentInfo * getExtConnBoolTriStateOutInfo();

class ExtConnBoolTriStateOut : public ExternalConnector
{
public:
	ExtConnBoolTriStateOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateOut();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
