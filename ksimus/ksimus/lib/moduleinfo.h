/***************************************************************************
                          moduleinfo.h  -  description
                             -------------------
    begin                : Sun Apr 15 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <qstring.h>

#include "componentinfo.h"


/**Info class for modules
  *@author Rasmus Diekenbrock
  */

class ModuleInfo : public ComponentInfo
{
public: 
	ModuleInfo( const QString & moduleFile,
	            const QString & name,
	            const QString & libName,
	            const QString & additionalLibNames,
	            ViewAttribute viewAttr,
	            const QString & shortDescr = QString::null,
	            const QString & HTMLDescr = QString::null,
	            const QString & oldLibNames = QString::null );
	~ModuleInfo();
	const QString & getModuleFile() const;

private:

	const QString m_moduleFile;
};

#endif
