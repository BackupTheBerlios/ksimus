/***************************************************************************
                          llicompviewsel.cpp  -  description
                             -------------------
    begin                : Tue Sep 12 2000
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

#include "llicompsel.h"
#include "compview.h"
#include "component.h"
#include "globals.h"
#include "ksimusdoc.h"
#include "ksimeditor.h"

LLICompViewSel::LLICompViewSel(	const char * text, unsigned int priority,
								KSimusDoc * doc, CompViewList * cvList)
	:	LogListItem(text,priority),
		m_doc(doc)
{
	m_cvList = new CompViewList(*cvList);
	CHECK_PTR(m_cvList);
}

LLICompViewSel::LLICompViewSel(	const char * text, unsigned int priority,
								KSimusDoc * doc, CompView * cv)
	:	LogListItem(text,priority),
		m_doc(doc)
{
	m_cvList = new CompViewList();
	CHECK_PTR(m_cvList);
	m_cvList->append(cv);
}

LLICompViewSel::~LLICompViewSel()
{
	delete m_cvList;
}

void LLICompViewSel::selected()
{
	if (g_docList->containsRef(m_doc))
	{
		KSimEditor * editor = m_doc->getActiveEditor();
	
		editor->unselectAll();
		FOR_EACH_COMPVIEW(it,*m_cvList)
		{
			editor->select(it.current(),true);
		}
	}
}

//##############################################################################
//##############################################################################
//##############################################################################


LLICompSel::LLICompSel(	const char * text, unsigned int priority,
						KSimusDoc * doc, ComponentList * compList)
	:	LogListItem(text,priority),
		m_doc(doc)
{
	m_compList = new ComponentList(*compList);
	CHECK_PTR(m_compList);
}

LLICompSel::LLICompSel(	const char * text, unsigned int priority,
						KSimusDoc * doc, Component * comp)
	:	LogListItem(text,priority),
		m_doc(doc)
{
	m_compList = new ComponentList();
	CHECK_PTR(m_compList);
	m_compList->append(comp);
}

LLICompSel::~LLICompSel()
{
	delete m_compList;
}

void LLICompSel::selected()
{
	if (g_docList->containsRef(m_doc))
	{
		KSimEditor * editor = m_doc->getActiveEditor();
	
		editor->unselectAll();
		FOR_EACH_COMP(it,*m_compList)
		{
			editor->select(it.current(),true);
		}
	}
}
