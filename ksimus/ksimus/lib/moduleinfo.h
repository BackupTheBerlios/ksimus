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

#include "componentinfo.h"

class QString;

/**Info class for modules
  *@author Rasmus Diekenbrock
  */

class ModuleInfo : public ComponentInfo
{
public: 
	ModuleInfo(			const QString & moduleFile,
									const QString & name,
									const QString & libName,
									const QString & additionalLibNames,
									ViewAttribute viewAttr,
									const QString & shortDescr = 0,
									const QString & HTMLDescr = 0,
									const QString & oldLibNames = 0 );
	~ModuleInfo();
  const QString & getModuleFile() const;

private:

	const QString m_moduleFile;
};

#endif
