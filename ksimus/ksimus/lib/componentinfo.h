/***************************************************************************
                          componentinfo.h  -  description
                             -------------------
    begin                : Fri Sep 22 2000
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

#ifndef COMPONENTINFO_H
#define COMPONENTINFO_H

// C-Includes

// QT-Includes
#include <qstring.h>
#include <qlist.h>

// KDE-Includes

// Project-Includes
#include "baseinfo.h"

// Forward declaration
class Component;
class CompContainer;

/**This class stores informations about component types
  *@author Rasmus Diekenbrock
  */



enum ViewAttribute  {
	                   VA_SHEETVIEW = 0x0001,       // Component has ONLY a sheet view
	                   VA_USERVIEW = 0x0002,        // Component has ONLY a user view
	                   VA_SHEET_AND_USER = 0x0003,  // Component has a user and a sheet view
	                   VA_SHEET_XOR_USER = 0x0007,  // Component has exclusive a user or a sheet view
	                   VA_WINDOWVIEW = 0x0008       // Component has a window view (user view is used)
	                   };

class ComponentInfo : public BaseInfo
{
public:

	ComponentInfo(const QString & name,
	              const QString & i18nLibName,
	              const QString & libName,
	              const QString & additionalI18nLibNames,
	              ViewAttribute viewAttr,
	              Component * (*factory)(CompContainer *, const ComponentInfo *),
	              const QString & shortDescr = QString::null,
	              const QString & HTMLDescr = QString::null,
	              const QString & oldLibNames = QString::null );
	
	const QString & getI18nLibName() const;
	const QString & getAdditionalI18nLibNames() const;
	ViewAttribute getViewAttr() const;
	Component * createComponent(CompContainer *) const;

	static const char * convertViewAttribute(ViewAttribute viewAttribute);
	static ViewAttribute convertViewAttribute(const char * viewAttribute, ViewAttribute defaultViewAttrib = VA_SHEETVIEW);


private:
	const QString m_i18nLibName;
	const QString m_additionalI18nLibNames;
	ViewAttribute m_viewAttr;
	Component * (*m_factory)(CompContainer *, const ComponentInfo * ci);
};

class ComponentInfoList : public QList<const ComponentInfo>
{
};

#define FOR_EACH_COMPONENT_INFO(_it_,_compInfoList_)	\
		for(QListIterator<const ComponentInfo> _it_(_compInfoList_);_it_.current();++_it_)

#endif
