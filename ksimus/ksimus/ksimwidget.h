/***************************************************************************
                          ksimwidget.h  -  description
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

#ifndef KSIMWIDGET_H
#define KSIMWIDGET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qobject.h>
#include <qlist.h>

#include <qpushbutton.h>

/**
  *@author Rasmus Diekenbrock
  */
class QWidget;


class KSimWidgetList : public QObject
{
	Q_OBJECT
public:
	KSimWidgetList();
	~KSimWidgetList();
	void addWidget(const QWidget * widget);
	void removeWidget(const QWidget * widget);
	QWidget * getWidget(int index);
	QWidget * getFirstWidget() const;
	QList<QWidget> * getWidgetList() { return &widgetList; };
	
	QWidget * getActiveWidget();

protected:
	QList<QWidget> widgetList;
	
public slots:
	void slotRemoveWidget(const QWidget * widget);
	
};

#endif
