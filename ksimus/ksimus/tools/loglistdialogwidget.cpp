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
#include <qcheckbox.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>
#include <kcolorbutton.h>

// Project-Includes
#include "loglistdialogwidget.h"
#include "ksimdebug.h"

// Forward declaration



LogListDialogWidget::LogListDialogWidget(LogList * logList, QWidget *parent, const char *name )
	:	PropertyWidget(3, parent,name),
		m_logList(logList)
{
	initView();
	initData();
}

LogListDialogWidget::~LogListDialogWidget()
{
}

void LogListDialogWidget::initView()
{
	new QLabel(i18n("Show:"),this);
	new QLabel(i18n("Popup at:"),this);
	new QLabel(i18n("Color:"),this);
		
	
	m_debugEnableCheck = new QCheckBox(i18n("Debug"), this);
	Q_CHECK_PTR(m_debugEnableCheck);
	m_debugEnableCheck->setMinimumSize(m_debugEnableCheck->sizeHint());
	connect(m_debugEnableCheck, SIGNAL(toggled(bool)), SLOT(slotDebugEnableToogled(bool)));
	
	m_debugPopupCheck = new QCheckBox(i18n("Debug"), this);
	Q_CHECK_PTR(m_debugPopupCheck);
	m_debugPopupCheck->setMinimumSize(m_debugPopupCheck->sizeHint());
	connect(m_debugPopupCheck, SIGNAL(toggled(bool)), SLOT(slotDebugPopupToogled(bool)));

	m_debugColor = new KColorButton(this);
	Q_CHECK_PTR(m_debugColor);
	connect(m_debugColor, SIGNAL(changed(const QColor &)), SLOT(slotDebugColorChanged(const QColor &)));

	m_infoEnableCheck = new QCheckBox(i18n("Infos"), this);
	Q_CHECK_PTR(m_infoEnableCheck);
	m_infoEnableCheck->setMinimumSize(m_infoEnableCheck->sizeHint());
	connect(m_infoEnableCheck, SIGNAL(toggled(bool)), SLOT(slotInfoEnableToogled(bool)));
	
	m_infoPopupCheck = new QCheckBox(i18n("Infos"), this);
	Q_CHECK_PTR(m_infoPopupCheck);
	m_infoPopupCheck->setMinimumSize(m_infoPopupCheck->sizeHint());
	connect(m_infoPopupCheck, SIGNAL(toggled(bool)), SLOT(slotInfoPopupToogled(bool)));
	
	m_infoColor = new KColorButton(this);
	Q_CHECK_PTR(m_infoColor);
	connect(m_infoColor, SIGNAL(changed(const QColor &)), SLOT(slotInfoColorChanged(const QColor &)));

	m_warningEnableCheck = new QCheckBox(i18n("Warnings"), this);
	Q_CHECK_PTR(m_warningEnableCheck);
	m_warningEnableCheck->setMinimumSize(m_warningEnableCheck->sizeHint());
	connect(m_warningEnableCheck, SIGNAL(toggled(bool)), SLOT(slotWarningEnableToogled(bool)));
	
	m_warningPopupCheck = new QCheckBox(i18n("Warnings"), this);
	Q_CHECK_PTR(m_warningPopupCheck);
	m_warningPopupCheck->setMinimumSize(m_warningPopupCheck->sizeHint());
	connect(m_warningPopupCheck, SIGNAL(toggled(bool)), SLOT(slotWarningPopupToogled(bool)));
	
	m_warningColor = new KColorButton(this);
	Q_CHECK_PTR(m_warningColor);
	connect(m_warningColor, SIGNAL(changed(const QColor &)), SLOT(slotWarningColorChanged(const QColor &)));

	m_errorEnableCheck = new QCheckBox(i18n("Errors"), this);
	Q_CHECK_PTR(m_errorEnableCheck);
	m_errorEnableCheck->setMinimumSize(m_errorEnableCheck->sizeHint());
	connect(m_errorEnableCheck, SIGNAL(toggled(bool)), SLOT(slotErrorEnableToogled(bool)));
	
	m_errorPopupCheck = new QCheckBox(i18n("Errors"), this);
	Q_CHECK_PTR(m_errorPopupCheck);
	m_errorPopupCheck->setMinimumSize(m_errorPopupCheck->sizeHint());
	connect(m_errorPopupCheck, SIGNAL(toggled(bool)), SLOT(slotErrorPopupToogled(bool)));

	m_errorColor = new KColorButton(this);
	Q_CHECK_PTR(m_errorColor);
	connect(m_errorColor, SIGNAL(changed(const QColor &)), SLOT(slotErrorColorChanged(const QColor &)));
	
	//TODO add addToolTip
}

void LogListDialogWidget::initData()
{
	m_debugEnableCheck->setChecked(m_logList->isDebugEnabled());
	m_infoEnableCheck->setChecked(m_logList->isInfoEnabled());
	m_warningEnableCheck->setChecked(m_logList->isWarningEnabled());
	m_errorEnableCheck->setChecked(m_logList->isErrorEnabled());

	m_debugPopupCheck->setChecked(m_logList->isDebugPopup());
	m_infoPopupCheck->setChecked(m_logList->isInfoPopup());
	m_warningPopupCheck->setChecked(m_logList->isWarningPopup());
	m_errorPopupCheck->setChecked(m_logList->isErrorPopup());

	m_debugPopupCheck->setEnabled(m_logList->isDebugEnabled());
	m_infoPopupCheck->setEnabled(m_logList->isInfoEnabled());
	m_warningPopupCheck->setEnabled(m_logList->isWarningEnabled());
	m_errorPopupCheck->setEnabled(m_logList->isErrorEnabled());

	m_debugColor->setColor(m_logList->getDebugColor());
	m_infoColor->setColor(m_logList->getInfoColor());
	m_warningColor->setColor(m_logList->getWarningColor());
	m_errorColor->setColor(m_logList->getErrorColor());
}

/** Called if accept button is pressed */
void LogListDialogWidget::acceptPressed()
{
	m_logList->setDebugEnable(m_debugEnableCheck->isChecked());
	m_logList->setInfoEnable(m_infoEnableCheck->isChecked());
	m_logList->setWarningEnable(m_warningEnableCheck->isChecked());
	m_logList->setErrorEnable(m_errorEnableCheck->isChecked());

	m_logList->setDebugPopup(m_debugPopupCheck->isChecked());
	m_logList->setInfoPopup(m_infoPopupCheck->isChecked());
	m_logList->setWarningPopup(m_warningPopupCheck->isChecked());
	m_logList->setErrorPopup(m_errorPopupCheck->isChecked());

	m_logList->setDebugColor(m_debugColor->color());
	m_logList->setInfoColor(m_infoColor->color());
	m_logList->setWarningColor(m_warningColor->color());
	m_logList->setErrorColor(m_errorColor->color());

	m_logList->triggerUpdate(true);
}

void LogListDialogWidget::defaultPressed()
{
	m_debugEnableCheck->setChecked(LogListProperty::getDefaultPriorityMask() & LOG_DEBUG);
	m_infoEnableCheck->setChecked(LogListProperty::getDefaultPriorityMask() & LOG_INFO);
	m_warningEnableCheck->setChecked(LogListProperty::getDefaultPriorityMask() & LOG_WARNING);
	m_errorEnableCheck->setChecked(LogListProperty::getDefaultPriorityMask() & LOG_ERROR);

	m_debugPopupCheck->setChecked(LogListProperty::getDefaultPopupMask() & LOG_DEBUG);
	m_infoPopupCheck->setChecked(LogListProperty::getDefaultPopupMask() & LOG_INFO);
	m_warningPopupCheck->setChecked(LogListProperty::getDefaultPopupMask() & LOG_WARNING);
	m_errorPopupCheck->setChecked(LogListProperty::getDefaultPopupMask() & LOG_ERROR);

	m_debugPopupCheck->setEnabled(LogListProperty::getDefaultPriorityMask() & LOG_DEBUG);
	m_infoPopupCheck->setEnabled(LogListProperty::getDefaultPriorityMask() & LOG_INFO);
	m_warningPopupCheck->setEnabled(LogListProperty::getDefaultPriorityMask() & LOG_WARNING);
	m_errorPopupCheck->setEnabled(LogListProperty::getDefaultPriorityMask() & LOG_ERROR);

	m_debugColor->setColor(m_logList->getDefaultDebugColor());
	m_infoColor->setColor(m_logList->getDefaultInfoColor());
	m_warningColor->setColor(m_logList->getDefaultWarningColor());
	m_errorColor->setColor(m_logList->getDefaultErrorColor());
}

// ####### slots ######

void LogListDialogWidget::slotDebugEnableToogled(bool on)
{
	m_logList->setDebugEnable(on);
	m_debugPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotInfoEnableToogled(bool on)
{
	m_logList->setInfoEnable(on);
	m_infoPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotWarningEnableToogled(bool on)
{
	m_logList->setWarningEnable(on);
	m_warningPopupCheck->setEnabled(on);
}

void LogListDialogWidget::slotErrorEnableToogled(bool on)
{
	m_logList->setErrorEnable(on);
	m_errorPopupCheck->setEnabled(on);
}

	
void LogListDialogWidget::slotDebugPopupToogled(bool on)
{
	m_logList->setDebugPopup(on);
}

void LogListDialogWidget::slotInfoPopupToogled(bool on)
{
	m_logList->setInfoPopup(on);
}

void LogListDialogWidget::slotWarningPopupToogled(bool on)
{
	m_logList->setWarningPopup(on);
}

void LogListDialogWidget::slotErrorPopupToogled(bool on)
{
	m_logList->setErrorPopup(on);
}

void LogListDialogWidget::slotDebugColorChanged(const QColor & color)
{
	m_logList->setDebugColor(color);
}

void LogListDialogWidget::slotInfoColorChanged(const QColor & color)
{
	m_logList->setInfoColor(color);
}

void LogListDialogWidget::slotWarningColorChanged(const QColor & color)
{
	m_logList->setWarningColor(color);
}

void LogListDialogWidget::slotErrorColorChanged(const QColor & color)
{
	m_logList->setErrorColor(color);
}

#include "loglistdialogwidget.moc"

