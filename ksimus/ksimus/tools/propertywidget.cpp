/***************************************************************************
                          propertywidget.cpp  -  description
                             -------------------
    begin                : Sat Mar 10 2001
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
#include <qtooltip.h>
#include <qwhatsthis.h>

// KDE-Includes
#include <kdialog.h>

// Project-Includes
#include "propertywidget.h"

// Forward declaration

PropertyWidget::PropertyWidget(int cols, QWidget *parent, const char *name, WFlags f)
	:	RowLayoutWidget(cols, parent, name, f)
{
	setMargin(KDialog::marginHint());
	setSpacing(KDialog::spacingHint());
}

PropertyWidget::~PropertyWidget()
{
}


void PropertyWidget::acceptPressed()
{
}

void PropertyWidget::defaultPressed()
{
}

void PropertyWidget::cancelPressed()
{
}


void PropertyWidget::slotAccept()
{
	// generate a focus out event for the child windows
	if (!hasFocus())
		setFocus();
		
	acceptPressed();
	emit signalAccept();
}

void PropertyWidget::slotDefault()
{
	// generate a focus out event for the child windows
	if (!hasFocus())
		setFocus();
		
	defaultPressed();
	emit signalDefault();
}
	
void PropertyWidget::slotCancel()
{
	// generate a focus out event for the child windows
	if (!hasFocus())
		setFocus();
		
	cancelPressed();
	emit signalCancel();
}

void PropertyWidget::changeData()
{
	emit signalChangeData();
}

void PropertyWidget::addToolTip(const QString & text, QWidget * widget)
{
	QToolTip::add(widget, text);
}

void PropertyWidget::addToolTip(const QString & text, QWidget * widget1, QWidget * widget2)
{
	QToolTip::add(widget1, text);
	QToolTip::add(widget2, text);
}

void PropertyWidget::addWhatsThis(const QString & text, QWidget * widget)
{
	QWhatsThis::add(widget, text);
}

void PropertyWidget::addWhatsThis(const QString & text, QWidget * widget1, QWidget * widget2)
{
	QWhatsThis::add(widget1, text);
	QWhatsThis::add(widget2, text);
}

#include "propertywidget.moc"
