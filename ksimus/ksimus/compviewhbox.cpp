/***************************************************************************
                          compviewhbox.cpp  -  description
                             -------------------
    begin                : Sat Dec 29 2001
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
#include "compviewhbox.h"
#include "ksimwidget.h"
#include "componentstyle.h"
#include "ksimdebug.h"

// Forward declaration

/* For feature use

class CompViewHBox::CompViewHBoxPrivate
{
public:	
	
	CompViewHBoxPrivate()
	{};
	
//	~CompViewHBoxPrivate() {};
	
};*/




CompViewHBox::CompViewHBox(CompView * cv, QWidget *parent, const char *name )
	:	QHBox(parent,name),
		m_p(0),
		m_compView(cv)
{
	init();
}

CompViewHBox::CompViewHBox(bool horizontal, CompView * cv, QWidget *parent, const char *name )
	:	QHBox(horizontal,parent,name),
		m_p(0),
		m_compView(cv)
{
	init();
}


void CompViewHBox::init()
{
//	m_p = new CompViewHBoxPrivate();
//	CHECK_PTR(m_p);
	
	getCompView()->getWidgetList()->addWidget(this);
	
	setMouseTracking(true);
	
	/* General signals */
	// Delete signal (CompView->Widget)
	connect(getCompView(), SIGNAL(destroyed()), this, SLOT(slotDelete()));
	// Move signal (CompView->Widget)
	connect(getCompView(), SIGNAL(signalMoveWidget(const QPoint &)), this, SLOT(move(const QPoint &)));
	// Resize signal (CompView->Widget)
	connect(getCompView(), SIGNAL(signalResizeWidget(const QSize &)), this, SLOT(resize(const QSize &)));
	// Hide signal (CompView->Widget)
	connect(getCompView(), SIGNAL(signalHide()), this, SLOT(hide()));
	// Show signal (CompView->Widget)
	connect(getCompView(), SIGNAL(signalShow()), this, SLOT(show()));
	// Delete signal (Editor->Widget)
/*	if (parent())
	{
		connect(parent(), SIGNAL(destroyed()), this, SLOT(slotDelete()));
	}*/

	Component * comp = getComponent();
	
	if (comp->inherits("ComponentStyle"))
	{
		ComponentStyle * compStyle = (ComponentStyle *)getComponent();
		
		// Connect special properties
		connect(compStyle, SIGNAL(signalFrameEnabled(bool)), this, SLOT(setFrameEnabled(bool)));
		connect(compStyle, SIGNAL(signalForegroundColor(const QColor &)), this, SLOT(setForegroundColor(const QColor &)));
		connect(compStyle, SIGNAL(signalBackgroundColor(const QColor &)), this, SLOT(setBackgroundColor(const QColor &)));
		connect(compStyle, SIGNAL(signalFont(const QFont &)), this, SLOT(setSpecialFont(const QFont &)));
		
		// Set special values
		setForegroundColor(compStyle->getForegroundColor());
		setBackgroundColor(compStyle->getBackgroundColor());
		setFrameEnabled(compStyle->isFrameEnabled());
		setSpecialFont(compStyle->getFont());
	}
		
}

CompViewHBox::~CompViewHBox()
{
	emit destroyed(this);
/*	if (m_p)
		delete m_p;*/
}

/** This slot delete this widget */
void CompViewHBox::slotDelete(void)
{
	delete this;
}

void CompViewHBox::setForegroundColor(const QColor & color)
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

void CompViewHBox::setBackgroundColor(const QColor & color)
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

void CompViewHBox::setFrameEnabled(bool enaFrame)
{
	if (enaFrame || (getCompView()->getViewType() == SHEET_VIEW))
	{
//		setFrameStyle(QFrame::Box | QFrame::Raised);
		setFrameStyle(QFrame::WinPanel | QFrame::Raised);
	}
	else
	{
		setFrameStyle(QFrame::NoFrame);
	}
	update();
}

void CompViewHBox::setSpecialFont(const QFont & font)
{
	setFont(font);	
	update();
}
