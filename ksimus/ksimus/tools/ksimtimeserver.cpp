/***************************************************************************
                          ksimtimeserver.cpp  -  description
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

#include "ksimtimeserver.h"

KSimTimeServer::KSimTimeServer()
	:	m_timePerTick(10 * KSIMTIME_MSEC)
{

}

KSimTimeServer::~KSimTimeServer()
{
}
	
void KSimTimeServer::setTick(const KSimTimeBase & time)
{
	m_timePerTick = time;
}

const KSimTimeBase & KSimTimeServer::getTick() const
{
	return m_timePerTick;
}

void KSimTimeServer::addTick()
{
	m_time += m_timePerTick.raw();
}

const KSimTimeBase & KSimTimeServer::getTime() const
{
	return (const KSimTimeBase &)*this;
}
