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
#include "componentstyle.h"
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

	Component * comp = getComponent();
	
	if (comp->inherits("ComponentStyle"))
	{
		ComponentStyle * compStyle = (ComponentStyle *)getComponent();
		
		// Connect special properties
		connect(compStyle, SIGNAL(signalForegroundColor(const QColor &)), this, SLOT(setForegroundColor(const QColor &)));
		connect(compStyle, SIGNAL(signalBackgroundColor(const QColor &)), this, SLOT(setBackgroundColor(const QColor &)));
		connect(compStyle, SIGNAL(signalFont(const QFont &)), this, SLOT(setSpecialFont(const QFont &)));
		
		// Set special values
		setForegroundColor(compStyle->getForegroundColor());
		setBackgroundColor(compStyle->getBackgroundColor());
		setSpecialFont(compStyle->getFont());
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

void CompViewWidget::setForegroundColor(const QColor & color)
{
	QPalette pal = palette();
	QColor newColor(color);
	
	if (!newColor.isValid() && (parent()->inherits("QWidget")))
	{
		QPalette parentPal = ((QWidget*)parent())->palette();
		newColor = parentPal.color(QPalette::Active, QColorGroup::Foreground);
	}		
	
	pal.setColor(QPalette::Active, QColorGroup::Foreground, newColor);
	pal.setColor(QPalette::Inactive, QColorGroup::Foreground, newColor);
	
	setPalette(pal);
	update();
}

void CompViewWidget::setBackgroundColor(const QColor & color)
{
	QPalette pal = palette();
	QColor newColor(color);
	
	
	if (!newColor.isValid() && (parent()->inherits("QWidget")))
	{
		QPalette parentPal = ((QWidget*)parent())->palette();
		newColor = parentPal.color(QPalette::Active, QColorGroup::Background);
	}		
	
	pal.setColor(QPalette::Active, QColorGroup::Background, newColor);
	pal.setColor(QPalette::Inactive, QColorGroup::Background, newColor);
	
	setPalette(pal);
	update();
}

void CompViewWidget::setSpecialFont(const QFont & font)
{
	setFont(font);	
	update();
}
