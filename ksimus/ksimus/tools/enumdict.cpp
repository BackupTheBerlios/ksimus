/***************************************************************************
                          enumdict.cpp  -  description
                             -------------------
    begin                : Fri Mar 23 2001
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

#include <qdict.h>


#include "enumdict.h"
#include "globals.h"
#include "ksimdebug.h"
#include "ksimdata.h"

EnumBaseDict::EnumBaseDict(const tData * pData)
	: m_data(pData)
{
	m_dict = new QDict<int>;
	int i = 0;
	const tData * pCount = pData;
	while (pCount->name != 0)
	{
		pCount ++;
		i ++;
	};
	
	m_dict->resize(getGreaterPrim(i));
	
	while (pData->name != 0)
	{
		m_dict->insert(pData->name, &pData->value);
		pData ++;
	};
}
	
EnumBaseDict::~EnumBaseDict()
{
	delete m_dict;
}

const int * EnumBaseDict::find(const char * name) const
{
	return m_dict->find(name);
}

int EnumBaseDict::find(const char * name, int defaultValue) const
{
	// Null pointer not allowed
	if(name == 0)
		return defaultValue;
	
	const int * pI;
	
	pI = m_dict->find(name);
	if (pI)
		return *pI;
	return defaultValue;
}

const char * EnumBaseDict::find(int value) const
{
	const tData * pData = m_data;
	while (pData->name != 0)
	{
		if (pData->value == value)
			return pData->name;
		pData ++;
	};
	
	// Not found.
	return (const char *)0;
}


void EnumBaseDict::save(KSimData & conf, const char * key, int value) const
{
	conf.writeEntry(key, find(value));
}

int EnumBaseDict::load(const KSimData & conf, const char * key, int defaultValue) const
{
	QString str;
	int res;
	str = conf.readEntry(key);
	if(str.isNull())
		res = defaultValue;
	else
		res = find(str, defaultValue);
	return res;
}
	
const int * EnumBaseDict::load(const KSimData & conf, const char * key) const
{
	QString str;
	const int * res;
	str = conf.readEntry(key);
	if(str.isNull())
		res = 0;
	else
		res = find(str);
	return res;
}


