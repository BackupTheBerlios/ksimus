/***************************************************************************
                          basewindow.cpp  -  description
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

#include <qsplitter.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmultilineedit.h>


#include "ksimus.h"
#include "basewindow.h"
#include "loglist.h"
#include "componentlistview.h"

BaseWindow::BaseWindow(KSimusApp *parent, const char *name )
	:	QWidget(parent,name)
{
	m_vertSplitter = new QSplitter(Vertical, this, "VerticalSplitter");
	CHECK_PTR(m_vertSplitter);
	m_horiSplitter = new QSplitter(Horizontal, m_vertSplitter, "HorizontalSplitter");
	CHECK_PTR(m_horiSplitter);

	m_logWidget = new LogList(parent, m_vertSplitter, "LogWidget");
	CHECK_PTR(m_logWidget);
	connect(m_logWidget,SIGNAL(signalShow()),SLOT(showLogWidget()));
	
	m_treeWidget = new ComponentListView(m_horiSplitter, "TreeWidget");
	CHECK_PTR(m_treeWidget);
	connect(m_treeWidget, SIGNAL(signalSelection(const ComponentInfo *)),
			parent, SLOT(slotSelectCI(const ComponentInfo *)));
	connect(parent, SIGNAL(signalViewChanged(eAppViewType)),
			m_treeWidget, SLOT(slotSetCurrentView(eAppViewType)));


	m_workingWidget = new QWidget(m_horiSplitter, "WorkingWidget");
	CHECK_PTR(m_workingWidget);

    // Layout
	QBoxLayout * lay = new QHBoxLayout(this);
	CHECK_PTR(lay);
	lay->addWidget(m_vertSplitter);

	// Widget sizes
	QValueList<int> sizeList;

	sizeList.append(20);
	sizeList.append(80);
	m_horiSplitter->setSizes(sizeList);

	sizeList.clear();
	sizeList.append(80);
	sizeList.append(20);
	m_vertSplitter->setSizes(sizeList);
}

BaseWindow::~BaseWindow()
{
}

void BaseWindow::hideWorkingWidget()
{
	m_workingWidget->hide();
}
void BaseWindow::hideLogWidget()
{
	m_logWidget->hide();
}
void BaseWindow::hideTreeWidget()
{
	m_treeWidget->hide();
}


void BaseWindow::showWorkingWidget()
{
	m_workingWidget->show();
}
void BaseWindow::showLogWidget()
{
	m_logWidget->show();
}
void BaseWindow::showTreeWidget()
{
	m_treeWidget->show();
}

void BaseWindow::toggleWorkingWidget()
{
	if (isWorkingWidgetHidden())
	{
		showWorkingWidget();
	}
	else
	{
		hideWorkingWidget();
	}		
}
void BaseWindow::toggleLogWidget()
{
	if (isLogWidgetHidden())
	{
		showLogWidget();
	}
	else
	{
		hideLogWidget();
	}		
}
void BaseWindow::toggleTreeWidget()
{
	if (isTreeWidgetHidden())
	{
		showTreeWidget();
	}
	else
	{
		hideTreeWidget();
	}		
}


bool BaseWindow::isWorkingWidgetHidden() const
{
	return m_workingWidget->isHidden();
}
bool BaseWindow::isLogWidgetHidden() const
{
	return m_logWidget->isHidden();
}
bool BaseWindow::isTreeWidgetHidden() const
{
	return m_treeWidget->isHidden();
}
