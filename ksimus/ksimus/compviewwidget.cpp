/***************************************************************************
                          compviewwidget.cpp  -  description
                             -------------------
    begin                : Tue Jan 30 2001
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "compviewwidget.h"
#include "compview.h"
#include "ksimwidget.h"

// Forward declaration

/*class CompViewWidget::CompViewWidgetPrivate
{
public:	
	
	CompViewWidgetPrivate(CompView * cv)
		:	m_cv(cv)
	{};
	
//	~CompViewWidgetPrivate() {};
	
	CompView * m_cv;
};*/




CompViewWidget::CompViewWidget(CompView * cv, QWidget *parent, const char *name )
	:	QWidget(parent,name),
		m_p(0),
		m_compView(cv)
{
//	m_p = new CompViewWidgetPrivate(cv, parent);
//	CHECK_PTR(m_p);
	
	cv->getWidgetList()->addWidget(this);
	
	setMouseTracking(true);
	
	/* General signals */
	// Delete signal (CompView->Widget)
	connect(cv, SIGNAL(destroyed()), this, SLOT(slotDelete()));
	// Move signal (CompView->Widget)
	connect(cv, SIGNAL(signalMoveWidget(const QPoint &)), this, SLOT(move(const QPoint &)));
	// Resize signal (CompView->Widget)
	connect(cv, SIGNAL(signalResizeWidget(const QSize &)), this, SLOT(resize(const QSize &)));
	// Hide signal (CompView->Widget)
	connect(cv, SIGNAL(signalHide()), this, SLOT(hide()));
	// Show signal (CompView->Widget)
	connect(cv, SIGNAL(signalShow()), this, SLOT(show()));
	// Delete signal (Editor->Widget)
	if (parent)
	{
		connect(parent, SIGNAL(destroyed()), this, SLOT(slotDelete()));
	}

}

CompViewWidget::~CompViewWidget()
{
	emit destroyed(this);
/*	if (m_p)
		delete m_p;*/
}

/** This slot delete this widget */
void CompViewWidget::slotDelete(void)
{
	delete this;
}

