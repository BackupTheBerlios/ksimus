/***************************************************************************
                          loglistdialogwidget.cpp  -  description
                             -------------------
    begin                : Fri Nov 3 2000
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
#include <qvgroupbox.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "loglistdialogwidget.h"

// Forward declaration



LogListDialogWidget::LogListDialogWidget(LogList * logList, QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name),
		m_logList(logList),
		m_property(*(LogListProperty*)logList)
{
	initView();
	initData();
}

LogListDialogWidget::~LogListDialogWidget()
{
}

void LogListDialogWidget::initView()
{
	QButtonGroup * box = new QButtonGroup(2,Qt::Horizontal,QString::fromLatin1(name()),this);
	
	new QLabel(i18n("Show:"),box);
	new QLabel(i18n("Popup at:"),box);
		
	
	m_debugEnableCheck = new QCheckBox(i18n("Debug"), box);
	CHECK_PTR(m_debugEnableCheck);
	m_debugEnableCheck->setMinimumSize(m_debugEnableCheck->sizeHint());
	connect(m_debugEnableCheck, SIGNAL(toggled(bool)), SLOT(slotDebugEnableToogled(bool)));
	
	m_debugPopupCheck = new QCheckBox(i18n("Debug"), box);
	CHECK_PTR(m_debugPopupCheck);
	m_debugPopupCheck->setMinimumSize(m_debugPopupCheck->sizeHint());
	connect(m_debugPopupCheck, SIGNAL(toggled(bool)), SLOT(slotDebugPopupToogled(bool)));
	
	m_infoEnableCheck = new QCheckBox(i18n("Infos"), box);
	CHECK_PTR(m_infoEnableCheck);
	m_infoEnableCheck->setMinimumSize(m_infoEnableCheck->sizeHint());
	connect(m_infoEnableCheck, SIGNAL(toggled(bool)), SLOT(slotInfoEnableToogled(bool)));
	
	m_infoPopupCheck = new QCheckBox(i18n("Infos"), box);
	CHECK_PTR(m_infoPopupCheck);
	m_infoPopupCheck->setMinimumSize(m_infoPopupCheck->sizeHint());
	connect(m_infoPopupCheck, SIGNAL(toggled(bool)), SLOT(slotInfoPopupToogled(bool)));
	
	m_warningEnableCheck = new QCheckBox(i18n("Warnings"), box);
	CHECK_PTR(m_warningEnableCheck);
	m_warningEnableCheck->setMinimumSize(m_warningEnableCheck->sizeHint());
	connect(m_warningEnableCheck, SIGNAL(toggled(bool)), SLOT(slotWarningEnableToogled(bool)));
	
	m_warningPopupCheck = new QCheckBox(i18n("Warnings"), box);
	CHECK_PTR(m_warningPopupCheck);
	m_warningPopupCheck->setMinimumSize(m_warningPopupCheck->sizeHint());
	connect(m_warningPopupCheck, SIGNAL(toggled(bool)), SLOT(slotWarningPopupToogled(bool)));
	
	m_errorEnableCheck = new QCheckBox(i18n("Errors"), box);
	CHECK_PTR(m_errorEnableCheck);
	m_errorEnableCheck->setMinimumSize(m_errorEnableCheck->sizeHint());
	connect(m_errorEnableCheck, SIGNAL(toggled(bool)), SLOT(slotErrorEnableToogled(bool)));
	
	m_errorPopupCheck = new QCheckBox(i18n("Errors"), box);
	CHECK_PTR(m_errorPopupCheck);
	m_errorPopupCheck->setMinimumSize(m_errorPopupCheck->sizeHint());
	connect(m_errorPopupCheck, SIGNAL(toggled(bool)), SLOT(slotErrorPopupToogled(bool)));
}

void LogListDialogWidget::initData()
{
	m_debugEnableCheck->setChecked(m_property.isDebugEnabled());
	m_infoEnableCheck->setChecked(m_property.isInfoEnabled());
	m_warningEnableCheck->setChecked(m_property.isWarningEnabled());
	m_errorEnableCheck->setChecked(m_property.isErrorEnabled());

	m_debugPopupCheck->setChecked(m_property.isDebugPopup());
	m_infoPopupCheck->setChecked(m_property.isInfoPopup());
	m_warningPopupCheck->setChecked(m_property.isWarningPopup());
	m_errorPopupCheck->setChecked(m_property.isErrorPopup());

	m_debugPopupCheck->setEnabled(m_property.isDebugEnabled());
	m_infoPopupCheck->setEnabled(m_property.isInfoEnabled());
	m_warningPopupCheck->setEnabled(m_property.isWarningEnabled());
	m_errorPopupCheck->setEnabled(m_property.isErrorEnabled());
}

/** Called if accept button is pressed */
void LogListDialogWidget::acceptPressed()
{
	*(LogListProperty*)m_logList = m_property;
}

// ####### Checkbox slots ######

void LogListDialogWidget::slotDebugEnableToogled(bool on)
{
	m_property.setDebugEnable(on);
	m_debugPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotInfoEnableToogled(bool on)
{
	m_property.setInfoEnable(on);
	m_infoPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotWarningEnableToogled(bool on)
{
	m_property.setWarningEnable(on);
	m_warningPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotErrorEnableToogled(bool on)
{
	m_property.setErrorEnable(on);
	m_errorPopupCheck->setEnabled(on);
}

	
void LogListDialogWidget::slotDebugPopupToogled(bool on)
{
	m_property.setDebugPopup(on);
}

void LogListDialogWidget::slotInfoPopupToogled(bool on)
{
	m_property.setInfoPopup(on);
}

void LogListDialogWidget::slotWarningPopupToogled(bool on)
{
	m_property.setWarningPopup(on);
}

void LogListDialogWidget::slotErrorPopupToogled(bool on)
{
	m_property.setErrorPopup(on);
}

