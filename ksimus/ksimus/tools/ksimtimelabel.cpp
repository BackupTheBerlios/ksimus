/***************************************************************************
                          ksimtimelabel.cpp  -  description
                             -------------------
    begin                : Sun Nov 26 2000
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

#include "ksimtimelabel.h"
#include "ksimtimebase.h"

KSimTimeLabel::KSimTimeLabel(const KSimTimeBase * time, int precision, QWidget *parent, const char *name )
	:	QLabel(parent,name),
		m_time(time),
		m_precision(precision)
{
	slotTimeChanged();
}

KSimTimeLabel::KSimTimeLabel(const KSimTimeBase * time, QWidget *parent, const char *name)
	:	QLabel(parent,name),
		m_time(time),
		m_precision(2)
{
	slotTimeChanged();
}


KSimTimeLabel::~KSimTimeLabel()
{
}

void KSimTimeLabel::slotTimeChanged()
{
	setText(m_time->getValueString(m_precision));	
}

#include "ksimtimelabel.moc"
