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

/**A external boolean tristate input connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTriStateIn : public ExternalConnector
{
public: 
	ExtConnBoolTriStateIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateIn();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

/**A external boolean tristate output connector
  *@author Rasmus Diekenbrock
  */

class ExtConnBoolTriStateOut : public ExternalConnector
{
public:
	ExtConnBoolTriStateOut(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnBoolTriStateOut();

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
