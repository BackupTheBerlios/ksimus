/***************************************************************************
                          extconnfloatin.h  -  description
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

#ifndef EXTCONNFLOATIN_H
#define EXTCONNFLOATIN_H

#include "ksimus/externalconnector.h"


class ConnectorFloatOut;
class ConnectorFloatIn;

namespace KSimLibFloatingPoint
{

extern const ComponentInfo ExtConnFloatInInfo;

/**A external floating point input connector
  *@author Rasmus Diekenbrock
  */

class ExtConnFloatIn : public ExternalConnector
{
public: 
	ExtConnFloatIn(CompContainer * container, const ComponentInfo * ci);
//	~ExtConnFloatIn();
	/** Shift the result of calculation to output */
	virtual void updateOutput();

protected:
	ConnectorFloatOut * out;
	ConnectorFloatIn * in;
	

};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
