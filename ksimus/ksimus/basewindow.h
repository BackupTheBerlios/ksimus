/***************************************************************************
                          basewindow.h  -  description
                             -------------------
    begin                : Sat Dec 23 2000
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

#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <qwidget.h>


class QSplitter;
class LogList;
class ComponentListView;
class KSimusApp;

/**This is the base window for KSimus.
The widget is splitted in 3 child widgets:
- tree aera
- working area
- log window

  *@author Rasmus Diekenbrock
  */

class BaseWindow : public QWidget
{
   Q_OBJECT

public:
	/** Creates the base window */
	BaseWindow(KSimusApp *parent=0, const char *name=0);
	~BaseWindow();

	QWidget * getWorkingWidget() { return m_workingWidget; };
	LogList * getLogWidget() { return m_logWidget; };
	ComponentListView * getTreeWidget() { return m_treeWidget; };


	bool isWorkingWidgetHidden() const;
	bool isLogWidgetHidden() const;
	bool isTreeWidgetHidden() const;


public slots:
	void hideWorkingWidget();
	void hideLogWidget();
	void hideTreeWidget();
	void showWorkingWidget();
	void showLogWidget();
	void showTreeWidget();
	void toggleWorkingWidget();
	void toggleLogWidget();
	void toggleTreeWidget();


private:
	ComponentListView * m_treeWidget;
	LogList * m_logWidget;
	QWidget * m_workingWidget;
	QSplitter * m_vertSplitter;
	QSplitter * m_horiSplitter;
	
};

#endif
