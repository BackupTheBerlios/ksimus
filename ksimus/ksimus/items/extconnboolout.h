/***************************************************************************
                          extconnboolout.h  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef EXTCONNBOOLOUT_H
#define EXTCONNBOOLOUT_H

#include "externalconnector.h"

/**
  *@author Rasmus Diekenbrock
  */

extern const ComponentInfo ExtConnBoolOutInfo;

class ConnectorBoolOut;
class ConnectorBoolIn;



class ExtConnBoolOut : public ExternalConnector  {
public: 
	ExtConnBoolOut(CompContainer * container, const ComponentInfo * ci);
	~ExtConnBoolOut();
	/** Shift the result of calculation to output */
	virtual void updateOutput();

protected:
	ConnectorBoolOut * out;
	ConnectorBoolIn * in;
	
};

#endif
