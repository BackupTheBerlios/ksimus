/***************************************************************************
                          ksimtimeserver.h  -  description
                             -------------------
    begin                : Sun Nov 5 2000
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

#ifndef KSIMTIMESERVER_H
#define KSIMTIMESERVER_H

#include "ksimtimebase.h"

/**The time base of the simulation
  *@author Rasmus Diekenbrock
  */

class KSimTimeServer : public KSimTimeBase
{
public:
	KSimTimeServer();
	~KSimTimeServer();

	void setTick(const KSimTimeBase & time);
	const KSimTimeBase & getTick() const;
	
	void addTick();

protected:
	KSimTimeBase m_timePerTick;
};

#endif
