/***************************************************************************
                          connectorinfo.h  -  description
                             -------------------
    begin                : Sun Oct 1 2000
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

#ifndef CONNECTORINFO_H
#define CONNECTORINFO_H

#include <qstring.h>
#include <qlist.h>

#include "baseinfo.h"

/**Information class for connectors
  *@author Rasmus Diekenbrock
  */

class QPoint;
class Component;
class ConnectorBase;

class ConnectorInfo : public BaseInfo
{
public: 
	ConnectorInfo(const QString & name,
	              const QString & libName,
	              const QString & dataType,
	              ConnectorBase * (*factory)(Component* , const QString & , const QString & , const QPoint&),
	              const QString & shortDescr = QString::null,
	              const QString & HTMLDescr = QString::null,
	              const QString & oldLibNames = QString::null );

	const QString & getDataType() const;
	ConnectorBase * create(Component* , const QString & , const QString & , const QPoint&) const;
	
private:

	const QString m_dataType;
	ConnectorBase * (*m_factory)(Component* , const QString & , const QString & , const QPoint&);
};

class ConnectorInfoList : public QList<const ConnectorInfo>
{
};

#define FOR_EACH_CONNECTOR_INFO(_it_,_connectorInfoList_)	\
		for(QListIterator<const ConnectorInfo> _it_(_connectorInfoList_);_it_.current();++_it_)

#endif

