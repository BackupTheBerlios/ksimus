/***************************************************************************
                          loglistitem.cpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
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

#include <qstring.h>
#include <qstringlist.h>
#include <qfontmetrics.h>
#include <qpainter.h>

#include "loglist.h"
#include "loglistitem.h"

#include "ksimdebug.h"

#define TAIL_SPACE	(box->fontMetrics().lineSpacing()/4)


LogListItem::LogListItem(const char * text, unsigned int priority)
{
	QString base(text);
	
	if ((priority == LOG_DEBUG)		||
		(priority == LOG_INFO)		||
		(priority == LOG_WARNING)	||
		(priority == LOG_ERROR))
	{
		m_prio = priority;
	}
	else
	{
		KSIMDEBUG_VAR("unknown priority", priority);
		m_prio = LOG_ERROR;
	}

	switch(m_prio)
	{
		case LOG_DEBUG:
			base = "DEBUG: " + base;
			break;
		
		case LOG_INFO:
			base = "INFO: " + base;
			break;
		
		case LOG_WARNING:
			base = "WARNING: " + base;
			break;
		
		case LOG_ERROR:
			base = "ERROR: " + base;
			break;
			
		default:
			break;
	}
		
//	m_textList = new QStringList(true);
//	CHECK_PTR(m_textList);
	
	// Split (multi line) text to single line strings
//	split(m_textList,base,'\n');
	m_textList = new QStringList(QStringList::split('\n', base));
	
}

LogListItem::~LogListItem()
{
	delete m_textList;
}
	
void LogListItem::paint(QPainter * p)
{
	QFontMetrics fm = p->fontMetrics();
	int ls = fm.lineSpacing();
	int y = fm.ascent() + fm.leading()/2;
	
	for (QStringList::Iterator it = m_textList->begin(); it != m_textList->end(); ++it)
	{
		p->drawText(5, y, *it);
		y += ls;
	}
}

int LogListItem::height( const QListBox * box) const
{
	return (box->fontMetrics().lineSpacing() * m_textList->count()) + TAIL_SPACE;
}

int LogListItem::width( const QListBox * box) const
{
	int max = 0;
	
	for (QStringList::Iterator it = m_textList->begin(); it != m_textList->end(); ++it)
	{
		max = QMAX(max, box->fontMetrics().width(*it)+5);
	}
	return max;
}
	
void LogListItem::selected()
{
}

void LogListItem::highlighted()
{
	// Default: same action as selected()
	selected();
}

