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

class BaseInfo
{


public:
	enum eInfoType { INFO_TYPE_UNKNOWN,      // Unknown Info Type
	                 INFO_COMPONENT,         // Component Info
	                 INFO_CONNECTOR,         // Connector Info
	                 INFO_WIREPROPERTY,      // Wire Property Info
	                 INFO_IMPLICITCONVERTER, // Implicit Converter Info
	                 INFO_IO_DEVICE,         // IO Device Info
	                 INFO_IO_JOIN            // IO Join Info
	               };

//	~BaseInfo();

	eInfoType getType() const              { return m_type;        };
	const QString & getName() const        { return m_name;        };
	const QString & getLibName() const     { return m_libName;     };
	const QString & getOldLibNames() const { return m_oldLibNames; };
	const QString & getShortDescr() const  { return m_shortDescr;  };
	const QString & getHTMLDescr() const   { return m_HTMLDescr;   };

protected:
	BaseInfo(eInfoType type,
	         const QString & name,
	         const QString & libName,
	         const QString & shortDescr,
	         const QString & HTMLDescr,
	         const QString & oldLibNames );

private:
	eInfoType m_type;
	const QString m_name;
	const QString m_libName;
	const QString m_shortDescr;
	const QString m_HTMLDescr;
	const QString m_oldLibNames;
};

#endif
