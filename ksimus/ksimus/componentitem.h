/***************************************************************************
                          componentitem.h  -  description
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

#ifndef COMPONENTITEM_H
#define COMPONENTITEM_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


/**Base class for all classes you can add to a component
  *@author Rasmus Diekenbrock
  */

class QString;
class Component;


class ComponentItem
{

public:
	ComponentItem(Component * parent = 0);
	~ComponentItem();
	
	void setComponent(Component * comp) { m_component = comp; };
	Component * getComponent() const { return m_component; };

	/** Appends a info to the log list. */
	void logInfo(const QString & msg) const;	
	/** Appends a warning to the log list. */
	void logWarning(const QString & msg) const;	
	/** Appends a error to the log list. */
	void logError(const QString & msg) const;	

protected: // Protected attributes
	Component * m_component;

};

#endif
