/***************************************************************************
                          ksimwidget.cpp  -  description
                             -------------------
    begin                : Wed May 10 2000
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

#include <qpainter.h>

#include "ksimwidget.h"



//###############################################################
//###############################################################


KSimWidgetList::KSimWidgetList()
	: QObject()
{
}

KSimWidgetList::~KSimWidgetList()
{
}


void KSimWidgetList::addWidget(const QWidget * widget)
{
	widgetList.append(widget);
	connect(widget, SIGNAL(destroyed(const QWidget*)),SLOT(slotRemoveWidget(const QWidget*)));
}

void KSimWidgetList::removeWidget(const QWidget * widget)
{
	widgetList.removeRef(widget);
}


QWidget * KSimWidgetList::getWidget(int index)
{
	return widgetList.at(index);
}
	
QWidget * KSimWidgetList::getFirstWidget() const
{
	return widgetList.getFirst();
}

void KSimWidgetList::slotRemoveWidget(const QWidget * widget)
{
	removeWidget(widget);
}
	
