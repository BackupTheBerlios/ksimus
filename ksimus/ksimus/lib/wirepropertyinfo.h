/***************************************************************************
                          wirepropertyinfo.h  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#ifndef WIREINFO_H
#define WIREINFO_H

#include <qstring.h>

#include "baseinfo.h"

/**Info class for wire properties
  *@author Rasmus Diekenbrock
  */
class Wire;
class WireInfo;
class WireProperty;


class WirePropertyInfo : public BaseInfo
{
public:
	WirePropertyInfo(	const QString & name,
				const QString & libName,
				const QString & dataType,
				WireProperty * (*factory)(Wire*),
				const QString & shortDescr = QString::null,
				const QString & HTMLDescr = QString::null,
				const QString & oldLibNames = QString::null );
	
	const QString & getDataType() const;
	WireProperty * create(Wire*) const;
	
private:

	const QString m_dataType;
	WireProperty * (*m_factory)(Wire*);
	
	
};

typedef const WirePropertyInfo * WirePropertyInfoPtr;
typedef WirePropertyInfoPtr WirePropertyInfoList[];




#endif
