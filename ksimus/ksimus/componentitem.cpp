/***************************************************************************
                          componentitem.cpp  -  description
                             -------------------
    begin                : Wed Mar 29 2000
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

#include "componentitem.h"
#include "component.h"

ComponentItem::ComponentItem(Component * comp)
	:	m_component(comp)
{
}
ComponentItem::~ComponentItem()
{
}

/** Appends a info to the log list. */
void ComponentItem::logInfo(const QString & msg) const
{
	getComponent()->logInfo(msg);
}
/** Appends a warning to the log list. */
void ComponentItem::logWarning(const QString & msg) const
{
	getComponent()->logWarning(msg);
}
/** Appends a error to the log list. */
void ComponentItem::logError(const QString & msg) const
{
	getComponent()->logError(msg);
}

/** Returns a pointer to the watch widget */	
WatchWidget * ComponentItem::getWatchWidget() const
{
	return getComponent()->getWatchWidget();
}

