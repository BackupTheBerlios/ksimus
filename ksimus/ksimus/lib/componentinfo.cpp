/***************************************************************************
                          componentinfo.cpp  -  description
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

// C-Includes

// QT-Includes
#include <qapplication.h>

// KDE-Includes

// Project-Includes
#include "componentinfo.h"
#include "enumdict.h"

// Forward declaration

//##########################################################
//##########################################################


EnumDict<ViewAttribute>::tData EnumDict<ViewAttribute>::data[]
      = { {"VA_SHEETVIEW",      VA_SHEETVIEW},
          {"VA_USERVIEW",       VA_USERVIEW},
          {"VA_SHEET_AND_USER", VA_SHEET_AND_USER},
          {"VA_SHEET_XOR_USER", VA_SHEET_XOR_USER},
          {"VA_WINDOWVIEW",     VA_WINDOWVIEW},
          {0,                   (ViewAttribute)0}};

static const EnumDict<ViewAttribute> & getViewAttributeDict()
{
	static EnumDict<ViewAttribute> viewAttributeDict(5);
	return viewAttributeDict;
}



const char * ComponentInfo::convertViewAttribute(ViewAttribute viewAttribute)
{
	return getViewAttributeDict().find(viewAttribute);
}
ViewAttribute ComponentInfo::convertViewAttribute(const char * viewAttribute, ViewAttribute defaultViewAttrib)
{
	return getViewAttributeDict().find(viewAttribute, defaultViewAttrib);
}



//##########################################################
//##########################################################
// ComponentInfo
	
ComponentInfo::ComponentInfo(
                             const QString & name,
                             const QString & libName,
                             const QString & i18nLibName,
                             const QString & additionalI18nLibNames,
                             ViewAttribute viewAttr,
                             Component * (*factory)(CompContainer *, const ComponentInfo *),
                             const QString & shortDescr,
                             const QString & HTMLDescr,
                             const QString & oldLibNames)
	:	BaseInfo(INFO_COMPONENT, name, libName, shortDescr, HTMLDescr, oldLibNames ),
		m_i18nLibName(i18nLibName),
		m_additionalI18nLibNames(additionalI18nLibNames),
		m_viewAttr(viewAttr),
		m_factory(factory)
{
}
	

ComponentInfo::ComponentInfo(const ComponentInfo & ci)
	:	BaseInfo(ci)
{
	// Copies are not allowed because there are some pointer comparisons !!!
	Q_ASSERT(0);
}

const QString & ComponentInfo::getI18nLibName() const
{
	return m_i18nLibName;
}

const QString & ComponentInfo::getAdditionalI18nLibNames() const
{
	return m_additionalI18nLibNames;
}

ViewAttribute ComponentInfo::getViewAttr() const
{
	return m_viewAttr;
}

Component * ComponentInfo::createComponent(CompContainer * container) const
{
	Q_CHECK_PTR(m_factory);
	return m_factory(container, this);
}

