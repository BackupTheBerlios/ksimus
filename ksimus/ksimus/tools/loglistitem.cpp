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

#include <klocale.h>

#include "loglist.h"
#include "loglistitem.h"

#include "ksimdebug.h"

#define TAIL_SPACE	(box->fontMetrics().lineSpacing()/4)


LogListItem::LogListItem(const char * text, unsigned int priority)
{
	setPriority(priority);
	m_textList = new QStringList(QStringList::split('\n', getPrioText() + QString::fromLatin1(text)));
	CHECK_PTR(m_textList);
}

LogListItem::LogListItem(const QString & text, unsigned int priority)
{
	setPriority(priority);
	m_textList = new QStringList(QStringList::split('\n', getPrioText() + text));
	CHECK_PTR(m_textList);
}

LogListItem::~LogListItem()
{
	delete m_textList;
}

void LogListItem::setPriority(unsigned int priority)
{
	if ((priority == LOG_DEBUG)    ||
	    (priority == LOG_INFO)     ||
	    (priority == LOG_WARNING)  ||
	    (priority == LOG_ERROR))
	{
		m_prio = priority;
	}
	else
	{
		KSIMDEBUG_VAR("unknown priority", priority);
		m_prio = LOG_ERROR;
	}
}

QString LogListItem::getPrioText() const
{
	switch(m_prio)
	{
		case LOG_DEBUG:
			return i18n("DEBUG: ");
			break;

		case LOG_INFO:
			return i18n("INFO: ");
			break;

		case LOG_WARNING:
			return i18n("WARNING: ");
			break;

		case LOG_ERROR:
			return i18n("ERROR: ");
			break;
	}

	return QString::null;   // Because compiler warning
}


void LogListItem::paint(QPainter * p)
{
	p->save();
	bool firstLine = true;
	QFontMetrics fm = p->fontMetrics();
	int ls = fm.lineSpacing();
	int y = fm.ascent() + fm.leading()/2;
	int leadingSpace = (m_textList->count() > 1) ? fm.width(getPrioText())+5 : 0;

	// Use default color schema if selected
	if (QListBoxItem::selected() == false)
	{
		switch(m_prio)
		{
			case LOG_DEBUG:   p->setPen(((LogList*)listBox())->getDebugColor());   break;
			case LOG_INFO:    p->setPen(((LogList*)listBox())->getInfoColor());    break;
			case LOG_WARNING: p->setPen(((LogList*)listBox())->getWarningColor()); break;
			case LOG_ERROR:   p->setPen(((LogList*)listBox())->getErrorColor());   break;
		}
	}

	
	for (QStringList::ConstIterator it = m_textList->begin(); it != m_textList->end(); ++it)
	{
		p->drawText(firstLine ? 5 : leadingSpace, y, *it);
		y += ls;
		firstLine = false;
	}
	p->restore();
}

int LogListItem::height( const QListBox * box) const
{
	return (box->fontMetrics().lineSpacing() * m_textList->count()) + TAIL_SPACE;
}

int LogListItem::width( const QListBox * box) const
{
	int max = 0;
	
	for (QStringList::ConstIterator it = m_textList->begin(); it != m_textList->end(); ++it)
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

