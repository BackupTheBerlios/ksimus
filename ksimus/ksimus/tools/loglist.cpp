/***************************************************************************
                          loglist.cpp  -  description
                             -------------------
    begin                : Mon Sep 11 2000
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


// include C-Headers
#include <stdarg.h>/* va_list, va_arg, va_end*/
#include <stdio.h>

// include QT-Headers
#include <qpopupmenu.h>
#include <qlayout.h>
#include <qcursor.h>

// include KDE-Headers
#include <klocale.h>
#include <kconfigbase.h>
#include <kdialogbase.h>

// include Project-Headers
#include "loglist.h"
#include "loglistitem.h"
#include "ksimus.h"
#include "loglistdialogwidget.h"


#define MAX_TEXT_LENGHT		1000

#define MAX_DEBUG_LENGHT	MAX_TEXT_LENGHT
#define MAX_INFO_LENGHT		MAX_TEXT_LENGHT
#define MAX_WARNING_LENGHT	MAX_TEXT_LENGHT
#define MAX_ERROR_LENGHT	MAX_TEXT_LENGHT


LogListProperty::LogListProperty()
	:
#ifdef DEBUG
	m_priorityMask(LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR),
#else
	m_priorityMask(LOG_INFO|LOG_WARNING|LOG_ERROR),
#endif
	m_popupMask(m_priorityMask)
{
}

LogListProperty::LogListProperty(const LogListProperty & llp)
	:	m_priorityMask(llp.m_priorityMask),
		m_popupMask(llp.m_popupMask)
{
}

LogListProperty::~LogListProperty()
{
}

const LogListProperty & LogListProperty::operator=(const LogListProperty & llp)
{
	if (this != &llp)
	{
		m_priorityMask = llp.m_priorityMask;
		m_popupMask = llp.m_popupMask;
	}
	return llp;
}


void LogListProperty::setDebugEnable(bool enable)
{
	if (enable)
		m_priorityMask |= LOG_DEBUG;
	else
		m_priorityMask &= ~LOG_DEBUG;
}
	
void LogListProperty::setInfoEnable(bool enable)
{
	if (enable)
		m_priorityMask |= LOG_INFO;
	else
		m_priorityMask &= ~LOG_INFO;
}

void LogListProperty::setWarningEnable(bool enable)
{
	if (enable)
		m_priorityMask |= LOG_WARNING;
	else
		m_priorityMask &= ~LOG_WARNING;
}

void LogListProperty::setErrorEnable(bool enable)
{
	if (enable)
		m_priorityMask |= LOG_ERROR;
	else
		m_priorityMask &= ~LOG_ERROR;
}
	
void LogListProperty::setDebugPopup(bool popup)
{
	if (popup)
		m_popupMask |= LOG_DEBUG;
	else
		m_popupMask &= ~LOG_DEBUG;
}
	
void LogListProperty::setInfoPopup(bool popup)
{
	if (popup)
		m_popupMask |= LOG_INFO;
	else
		m_popupMask &= ~LOG_INFO;
}

void LogListProperty::setWarningPopup(bool popup)
{
	if (popup)
		m_popupMask |= LOG_WARNING;
	else
		m_popupMask &= ~LOG_WARNING;
}

void LogListProperty::setErrorPopup(bool popup)
{
	if (popup)
		m_popupMask |= LOG_ERROR;
	else
		m_popupMask &= ~LOG_ERROR;
}
	
/** Save LogList property */
void LogListProperty::save(KConfigBase & file) const
{
	file.writeEntry("Debug Enabled",isDebugEnabled());
	file.writeEntry("Info Enabled",isInfoEnabled());
	file.writeEntry("Warning Enabled",isWarningEnabled());
	file.writeEntry("Error Enabled",isErrorEnabled());

	file.writeEntry("Debug Popup",isDebugPopup());
	file.writeEntry("Info Popup",isInfoPopup());
	file.writeEntry("Warning Popup",isWarningPopup());
	file.writeEntry("Error Popup",isErrorPopup());
}

/** Load LogList property */
void LogListProperty::load(KConfigBase & file)
{
	setDebugEnable(file.readBoolEntry("Debug Enabled",true));
	setInfoEnable(file.readBoolEntry("Info Enabled",true));
	setWarningEnable(file.readBoolEntry("Warning Enabled",true));
	setErrorEnable(file.readBoolEntry("Error Enabled",true));

	setDebugPopup(file.readBoolEntry("Debug Popup",true));
	setInfoPopup(file.readBoolEntry("Info Popup",true));
	setWarningPopup(file.readBoolEntry("Warning Popup",true));
	setErrorPopup(file.readBoolEntry("Error Popup",true));
}

//##################################################################################
//##################################################################################



LogList::LogList(KSimusApp * app, QWidget *parent, const char *name )
	:	QListBox(parent,name),
		m_app(app)
{

	connect(this, SIGNAL(selected(int)),SLOT(slotSelected(int)));
	connect(this, SIGNAL(highlighted(int)),SLOT(slotHighlighted(int)));
}

LogList::~LogList()
{
}

void LogList::mousePressEvent(QMouseEvent * ev)
{
	if (ev->button() == RightButton)
	{
		QPopupMenu * menu = new QPopupMenu();
		int idx, propertyidx;
		
		idx = menu->insertItem(i18n("&Remove Messages"), this, SLOT(slotClear()));
		menu->setItemEnabled(idx,count());
		menu->insertItem(i18n("&Hide Log Window"), m_app, SLOT(slotViewHideLog()));
		menu->insertSeparator();
		propertyidx = menu->insertItem(i18n("&Properties..."));
		
		idx = menu->exec(QCursor::pos());
		
		
		if (idx == propertyidx)
		{
			KDialogBase * dialog = new KDialogBase(KDialogBase::Plain,
																i18n("Log Window Properties"),
																KDialogBase::Ok | KDialogBase::Cancel,
											 					KDialogBase::Ok,
											 					this);
			QWidget * wid = dialog->plainPage();
			
			LogListDialogWidget * child = new LogListDialogWidget(this, wid, i18n("Log Window Properties"));
			
			QBoxLayout * horLayout = new QHBoxLayout(wid);
			horLayout->setMargin(KDialog::marginHint());
			horLayout->setSpacing(KDialog::spacingHint());
			horLayout->addWidget(child);
			
			connect(dialog,SIGNAL(okClicked()),child,SLOT(slotAccept()));
			dialog->exec();
		}
	}
	else
	{
		QListBox::mousePressEvent(ev);
	}
}

void LogList::append(LogListItem * item)
{
	if (item->getPriority() & m_priorityMask)
	{
		insertItem(item);
		setBottomItem(count()-1);

		if (item->getPriority() & m_popupMask)
		{
			emit signalShow();
		}
	}
	else
		delete item;
}
void LogList::debug(const char * file, int line, const char * format, ...)
{
	char text[MAX_DEBUG_LENGHT];
	va_list ap;
	QString s;
	
	va_start(ap,format);
	vsnprintf(text, MAX_DEBUG_LENGHT, format, ap);
	va_end(ap);
	
	s.sprintf("%s(%d) %s",file, line, text);
	LogListItem * lli = new LogListItem(s, LOG_DEBUG);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::info(const char * format, ...)
{
	char text[MAX_INFO_LENGHT];
	va_list ap;
	va_start(ap,format);
	vsnprintf(text, MAX_INFO_LENGHT, format, ap);
	va_end(ap);
	
	LogListItem * lli = new LogListItem(text,LOG_INFO);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::warning(const char * format, ...)
{
	char text[MAX_WARNING_LENGHT];
	va_list ap;
	va_start(ap,format);
	vsnprintf(text, MAX_WARNING_LENGHT, format, ap);
	va_end(ap);
	
	LogListItem * lli = new LogListItem(text,LOG_WARNING);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::error(const char * format, ...)
{
	char text[MAX_ERROR_LENGHT];
	va_list ap;
	va_start(ap,format);
	vsnprintf(text, MAX_ERROR_LENGHT, format, ap);
	va_end(ap);
	
	LogListItem * lli = new LogListItem(text,LOG_ERROR);
	CHECK_PTR(lli);
	append(lli);
}


void LogList::slotSelected(int index)
{
	LogListItem * lli = (LogListItem*)item(index);
	
	lli->selected();
}

void LogList::slotHighlighted(int index)
{
	LogListItem * lli = (LogListItem*)item(index);
	
	lli->highlighted();
}

void LogList::slotToggleDebug()
{
	setDebugEnable(!isDebugEnabled());
}

void LogList::slotToggleInfo()
{
	setInfoEnable(!isInfoEnabled());
}

void LogList::slotToggleWarning()
{
	setWarningEnable(!isWarningEnabled());
}

void LogList::slotToggleError()
{
	setErrorEnable(!isErrorEnabled());
}

void LogList::slotClear()
{
	clear();
}
