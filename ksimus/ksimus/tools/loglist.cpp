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

// include QT-Headers
#include <qpopupmenu.h>
#include <qlayout.h>
#include <qcursor.h>
#include <qpalette.h>

// include KDE-Headers
#include <klocale.h>
#include <kconfigbase.h>
#include <kdialogbase.h>

// include Project-Headers
#include "loglist.h"
#include "loglistitem.h"
#include "ksimus.h"
#include "ksimdebug.h"
#include "loglistdialogwidget.h"


LogListProperty::LogListProperty()
	:	m_priorityMask(getDefaultPriorityMask()),
		m_popupMask(getDefaultPopupMask()),
		m_debugColor(getDefaultDebugColor()),
		m_infoColor(getDefaultInfoColor()),
		m_warningColor(getDefaultWarningColor()),
		m_errorColor(getDefaultErrorColor())
{
}

LogListProperty::~LogListProperty()
{
}

unsigned int LogListProperty::getDefaultPriorityMask()
{
#ifdef DEBUG
	return (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR);
#else
	return (LOG_INFO|LOG_WARNING|LOG_ERROR);
#endif
}

unsigned int LogListProperty::getDefaultPopupMask()
{
#ifdef DEBUG
	return (LOG_DEBUG|LOG_INFO|LOG_WARNING|LOG_ERROR);
#else
	return (LOG_INFO|LOG_WARNING|LOG_ERROR);
#endif
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
	
void LogListProperty::setDebugColor(const QColor & color)
{
	m_debugColor = color;
}

void LogListProperty::setInfoColor(const QColor & color)
{
	m_infoColor = color;
}

void LogListProperty::setWarningColor(const QColor & color)
{
	m_warningColor = color;
}

void LogListProperty::setErrorColor(const QColor & color)
{
	m_errorColor = color;
}

const QColor & LogListProperty::getDefaultDebugColor()
{
	return Qt::magenta;
}

const QColor & LogListProperty::getDefaultInfoColor()
{
	return Qt::black;
}

const QColor & LogListProperty::getDefaultWarningColor()
{
	return Qt::darkGreen;
}

const QColor & LogListProperty::getDefaultErrorColor()
{
	return Qt::red;
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

	file.writeEntry("Debug Color",getDebugColor());
	file.writeEntry("Info Color",getInfoColor());
	file.writeEntry("Warning Color",getWarningColor());
	file.writeEntry("Error Color",getErrorColor());
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

	setDebugColor(file.readColorEntry("Debug Color", &getDefaultDebugColor()));
	setInfoColor(file.readColorEntry("Info Color", &getDefaultInfoColor()));
	setWarningColor(file.readColorEntry("Warning Color", &getDefaultWarningColor()));
	setErrorColor(file.readColorEntry("Error Color", &getDefaultErrorColor()));
}

//##################################################################################
//##################################################################################

class LogList::Private
{
public:
	Private(KSimusApp * ksimApp)
		:	app(ksimApp)
	{};

	KSimusApp * app;
};


//##################################################################################
//##################################################################################



LogList::LogList(KSimusApp * app, QWidget *parent, const char *name )
	:	QListBox(parent,name)
{
	m_p = new Private(app);
	CHECK_PTR(app);
	
	connect(this, SIGNAL(selected(int)),SLOT(slotSelected(int)));
	connect(this, SIGNAL(highlighted(int)),SLOT(slotHighlighted(int)));
}

LogList::~LogList()
{
	delete m_p;
}

void LogList::mousePressEvent(QMouseEvent * ev)
{
	if (ev->button() == RightButton)
	{
		QPopupMenu * menu = new QPopupMenu();
		int idx, propertyidx;
		
		idx = menu->insertItem(i18n("&Remove Messages"), this, SLOT(slotClear()));
		menu->setItemEnabled(idx,count());
		menu->insertItem(i18n("&Hide Log Window"), m_p->app, SLOT(slotViewHideLog()));
		menu->insertSeparator();
		propertyidx = menu->insertItem(i18n("&Properties..."));
		
		idx = menu->exec(QCursor::pos());
		
		
		if (idx == propertyidx)
		{
			KDialogBase * dialog = new KDialogBase(KDialogBase::Plain,
			                                       i18n("Log Window Properties"),
			                                       KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
			                                       KDialogBase::Ok,
			                                       this);
			CHECK_PTR(dialog);
			QWidget * wid = dialog->plainPage();
			
			LogListDialogWidget * child = new LogListDialogWidget(this, wid, i18n("Log Window Properties"));
			
			QBoxLayout * horLayout = new QHBoxLayout(wid);
			horLayout->setMargin(KDialog::marginHint());
			horLayout->setSpacing(KDialog::spacingHint());
			horLayout->addWidget(child);
			
			connect(dialog, SIGNAL(okClicked()), child, SLOT(slotAccept()));
			connect(dialog, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
			connect(dialog, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));
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

void LogList::debug(const char * file, int line, const char * text)
{
	LogListItem * lli = new LogListItem(QString::fromLatin1("%1(%2) %3")
	                                    .arg(QString::fromLatin1(file))
	                                    .arg(line)
	                                    .arg(QString::fromLatin1(text)),
	                                    LOG_DEBUG);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::debug(const char * file, int line, const QString & text)
{
	LogListItem * lli = new LogListItem(QString::fromLatin1("%1(%2) %3")
	                                    .arg(QString::fromLatin1(file))
	                                    .arg(line)
	                                    .arg(text),
	                                    LOG_DEBUG);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::info(const QString & s)
{
	LogListItem * lli = new LogListItem(s,LOG_INFO);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::warning(const QString & s)
{
	LogListItem * lli = new LogListItem(s,LOG_WARNING);
	CHECK_PTR(lli);
	append(lli);
}

void LogList::error(const QString & s)
{
	LogListItem * lli = new LogListItem(s,LOG_ERROR);
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




