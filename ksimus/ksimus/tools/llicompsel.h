/***************************************************************************
                          llicompviewsel.h  -  description
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

#ifndef LLICOMPVIEWSEL_H
#define LLICOMPVIEWSEL_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "loglistitem.h"

// Forward declaration
class KSimusDoc;
class Component;
class ComponentList;
class CompView;
class CompViewList;

/**
  *@author Rasmus Diekenbrock
  */

class LLICompViewSel : public LogListItem
{
public:
	LLICompViewSel(const char * text, unsigned int priority,
					KSimusDoc * doc, CompViewList * cvList);
	LLICompViewSel(const char * text, unsigned int priority,
					KSimusDoc * doc, CompView * cv);
	
protected:	
	~LLICompViewSel();
	virtual void selected();
	
	KSimusDoc * m_doc;
	CompViewList * m_cvList;
	
	
	
};


class LLICompSel : public LogListItem
{
public:
	LLICompSel(const char * text, unsigned int priority,
				KSimusDoc * doc, ComponentList * compList);
	LLICompSel(const char * text, unsigned int priority,
				KSimusDoc * doc, Component * comp);
	
protected:	
	~LLICompSel();
	virtual void selected();
	
	KSimusDoc * m_doc;
	ComponentList * m_compList;
	
	
	
};




#endif
