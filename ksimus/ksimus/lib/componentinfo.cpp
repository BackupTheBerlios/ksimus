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

static EnumDict<ViewAttribute> viewAttributeDict;

EnumDict<ViewAttribute>::tData EnumDict<ViewAttribute>::data[]
			= { {"VA_SHEETVIEW", 			VA_SHEETVIEW},
          {"VA_USERVIEW",  			VA_USERVIEW},
					{"VA_SHEET_AND_USER", VA_SHEET_AND_USER},
					{"VA_SHEET_XOR_USER", VA_SHEET_XOR_USER},
					{"VA_WINDOWVIEW",			VA_WINDOWVIEW},
          {0,(ViewAttribute)0}};


const char * ComponentInfo::convertViewAttribute(ViewAttribute viewAttribute)
{
	return viewAttributeDict.find(viewAttribute);
}
ViewAttribute ComponentInfo::convertViewAttribute(const char * viewAttribute, ViewAttribute defaultViewAttrib)
{
	return viewAttributeDict.find(viewAttribute, defaultViewAttrib);
}



//##########################################################
//##########################################################
// ComponentInfo
	
ComponentInfo::ComponentInfo(
				const QString & name,
				const QString & libName,
				const QString & additionalLibNames,
				ViewAttribute viewAttr,
				Component * (*factory)(CompContainer *, const ComponentInfo *),
				const QString & shortDescr,
				const QString & HTMLDescr,
				const QString & oldLibNames)
	:	BaseInfo(INFO_COMPONENT, name, libName, shortDescr, HTMLDescr, oldLibNames ),
		m_additionalLibNames(additionalLibNames),
		m_viewAttr(viewAttr),
		m_factory(factory)
{
}
	
const QString & ComponentInfo::getAdditionalLibNames() const
{
	return m_additionalLibNames;
}

ViewAttribute ComponentInfo::getViewAttr() const
{
	return m_viewAttr;
}

Component * ComponentInfo::createComponent(CompContainer * container) const
{
	CHECK_PTR(m_factory);
	return m_factory(container, this);
}

