/***************************************************************************
                          moduleinfo.cpp  -  description
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

#include <qstring.h>

#include "moduleinfo.h"
#include "module.h"
#include "ksimdebug.h"

static Component * moduleInfoFactory(CompContainer * container, const ComponentInfo * ci);

ModuleInfo::ModuleInfo(			const QString & moduleFile,
														const QString & name,
														const QString & libName,
														const QString & additionalLibNames,
														ViewAttribute viewAttr,
														const QString & shortDescr,
														const QString & HTMLDescr,
														const QString & oldLibNames)
	: ComponentInfo(name, libName, libName, additionalLibNames, viewAttr, moduleInfoFactory,
									shortDescr, HTMLDescr, oldLibNames),
		m_moduleFile(moduleFile)
{
}
	

ModuleInfo::~ModuleInfo()
{
}


const QString & ModuleInfo::getModuleFile() const
{
	return m_moduleFile;
}

static Component * moduleInfoFactory(CompContainer * container, const ComponentInfo * ci)
{
	Module * module;
	ModuleInfo * mi = (ModuleInfo*) ci;
	
	module = (Module*)getModuleBaseInfo()->createComponent(container);
	module->setModuleFile(mi->getModuleFile());
	
	return module;
}
