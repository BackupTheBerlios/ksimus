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

// C-Includes

// QT-Includes
#include <qsplitter.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmultilineedit.h>
#include <qtabwidget.h>

// KDE-Includes
#include <klocale.h>
#include <kconfig.h>
#include <kapp.h>

// Project-Includes
#include "ksimus.h"
#include "basewindow.h"
#include "loglist.h"
#include "componentlistview.h"
#include "watchwidget.h"
#include "ksimdebug.h"

// Forward declaration



BaseWindow::BaseWindow(KSimusApp *parent, const char *name )
	:	QWidget(parent,name)
{
	m_horiSplitter = new QSplitter(Horizontal, this, "HorizontalSplitter");
	CHECK_PTR(m_horiSplitter);
	
	m_listWidget = new QTabWidget(m_horiSplitter);
	
	m_componentListWidget = new ComponentListView(m_listWidget, "TreeWidget");
	m_listWidget->insertTab(m_componentListWidget, i18n("KSimus", "Components"));
	
	m_watchWidget = new WatchWidget(parent, m_listWidget, "WatchWidget");
	m_listWidget->insertTab(m_watchWidget, i18n("KSimus", "Watches"));
	connect(m_watchWidget, SIGNAL(signalShowMe()), SLOT(showWatchWidget()));
	
	CHECK_PTR(m_componentListWidget);
	connect(m_componentListWidget, SIGNAL(signalSelection(const ComponentInfo *)),
	        parent, SLOT(slotSelectCI(const ComponentInfo *)));
	connect(parent, SIGNAL(signalViewChanged(eAppViewType)),
	        m_componentListWidget, SLOT(slotSetCurrentView(eAppViewType)));
	
	m_vertSplitter = new QSplitter(Vertical, m_horiSplitter, "VerticalSplitter");
	CHECK_PTR(m_vertSplitter);

	m_workingWidget = new QWidget(m_vertSplitter, "WorkingWidget");
	CHECK_PTR(m_workingWidget);

	m_logWidget = new LogList(parent, m_vertSplitter, "LogWidget");
	CHECK_PTR(m_logWidget);
	connect(m_logWidget,SIGNAL(signalShow()),SLOT(showLogWidget()));



	// Layout
	QBoxLayout * lay = new QHBoxLayout(this);
	CHECK_PTR(lay);
	lay->addWidget(m_horiSplitter);
}

BaseWindow::~BaseWindow()
{
	/*QValueList<int> sizeList;
	sizeList = m_horiSplitter->sizes();
	KSIMDEBUG(QString("m_horiSplitter %1 %2 %3").arg(sizeList.count()).arg(sizeList[0]).arg(sizeList[1]));
	sizeList = m_vertSplitter->sizes();
	KSIMDEBUG(QString("m_vertSplitter %1 %2 %3").arg(sizeList.count()).arg(sizeList[0]).arg(sizeList[1]));
*/

	// Store last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("BaseWindow");

	config->writeEntry("Horizontal Splitter", m_horiSplitter->sizes());
	config->writeEntry("Vertical Splitter", m_vertSplitter->sizes());
	config->writeEntry("Log Widget Hidden", isLogWidgetHidden());
	config->writeEntry("List Widget Hidden", isListWidgetHidden());

	config->setGroup(group);
}


void BaseWindow::polish()
{
	QWidget::polish();

	// Load last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("BaseWindow");
	QValueList<int> sizeList;

	sizeList = config->readIntListEntry("Horizontal Splitter");
	if (sizeList.count() != 2)
	{
		sizeList.clear();
		sizeList.append(20);
		sizeList.append(80);
	}
	m_horiSplitter->setSizes(sizeList);

	sizeList = config->readIntListEntry("Vertical Splitter");
	if (sizeList.count() != 2)
	{
		sizeList.clear();
		sizeList.append(80);
		sizeList.append(20);
	}
	m_vertSplitter->setSizes(sizeList);

	if (config->readBoolEntry("Log Widget Hidden", false))
	{
		((KSimusApp *)parentWidget())->slotViewHideLog();
	}

	if (config->readBoolEntry("List Widget Hidden", false))
	{
		((KSimusApp *)parentWidget())->slotViewHideList();
	}

	config->setGroup(group);


/*	sizeList = m_horiSplitter->sizes();
	KSIMDEBUG(QString("m_horiSplitter %1 %2 %3").arg(sizeList.count()).arg(sizeList[0]).arg(sizeList[1]));
	sizeList = m_vertSplitter->sizes();
	KSIMDEBUG(QString("m_vertSplitter %1 %2 %3").arg(sizeList.count()).arg(sizeList[0]).arg(sizeList[1]));
*/
}


void BaseWindow::hideWorkingWidget()
{
	m_workingWidget->hide();
}
void BaseWindow::hideLogWidget()
{
	m_logWidget->hide();
}
void BaseWindow::hideListWidget()
{
	m_listWidget->hide();
}


void BaseWindow::showWorkingWidget()
{
	m_workingWidget->show();
}
void BaseWindow::showLogWidget()
{
	m_logWidget->show();
}
void BaseWindow::showListWidget()
{
	m_listWidget->show();
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
void BaseWindow::toggleListWidget()
{
	if (isListWidgetHidden())
	{
		showListWidget();
	}
	else
	{
		hideListWidget();
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
bool BaseWindow::isListWidgetHidden() const
{
	return m_listWidget->isHidden();
}

void BaseWindow::showWatchWidget()
{
	if (isListWidgetHidden())
	{
		showListWidget();
	}
	if (m_listWidget->currentPage() != getWatchWidget())
	{
		m_listWidget->showPage(getWatchWidget());
	}
}
void BaseWindow::showComponentListWidget()
{
	if (isListWidgetHidden())
	{
		showListWidget();
	}
	if (m_listWidget->currentPage() != getComponentListWidget())
	{
		m_listWidget->showPage(getComponentListWidget());
	}
}

