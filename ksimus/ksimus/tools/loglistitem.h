/***************************************************************************
                          loglistitem.h  -  description
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

#ifndef LOGLISTITEM_H
#define LOGLISTITEM_H

#include <qlistbox.h>

class QStringList;

/**
  *@author Rasmus Diekenbrock
  */

class LogListItem : protected QListBoxItem
{
friend class LogList;

public:
	LogListItem(const char * text, unsigned int priority);
	LogListItem(const QString & text, unsigned int priority);

protected:
	~LogListItem();
	unsigned int getPriority() const { return m_prio; };
	void setPriority(unsigned int priority);
	virtual void selected();
	virtual void highlighted();
	virtual void paint( QPainter * );
	virtual int height( const QListBox * ) const;
	virtual int width( const QListBox * ) const;
	QString getPrioText() const;
	
	unsigned int m_prio;
	QStringList * m_textList;
	
};

#endif
