/***************************************************************************
                          baseinfo.h  -  description
                             -------------------
    begin                : Sat Sep 30 2000
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

#ifndef BASEINFO_H
#define BASEINFO_H


#include <qstring.h>
/**The base class for all information classes
  *@author Rasmus Diekenbrock
  */

enum eInfoType { INFO_COMPONENT, INFO_CONNECTOR, INFO_WIREPROPERTY };

class QString;

class BaseInfo
{

protected:	
	BaseInfo(	eInfoType type,
						const QString & name,
						const QString & libName,
						const QString & shortDescr,
						const QString & HTMLDescr,
						const QString & oldLibNames );
//	~BaseInfo();

public:
	eInfoType getType() const;
	const QString & getName() const;
	const QString & getLibName() const;
	const QString & getOldLibNames() const;
	const QString & getShortDescr() const;
	const QString & getHTMLDescr() const;

private:
	eInfoType m_type;
	const QString m_name;
	const QString m_libName;
	const QString m_shortDescr;
	const QString m_HTMLDescr;
	const QString m_oldLibNames;
};

#endif
