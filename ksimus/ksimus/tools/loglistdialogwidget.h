/***************************************************************************
                          loglistdialogwidget.h  -  description
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

#ifndef LOGLISTDIALOGWIDGET_H
#define LOGLISTDIALOGWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"
#include "loglist.h"

// Forward declaration
class QCheckBox;


/**LogList property widget
  *@author Rasmus Diekenbrock
  */


class LogListDialogWidget : public PropertyWidget
{
   Q_OBJECT

public:
	LogListDialogWidget(LogList * logList, QWidget *parent=0, const char *name=0);
	~LogListDialogWidget();

protected:
	void initView();
	void initData();
	
	
	LogList * m_logList;
	LogListProperty m_property;
	
	QCheckBox * m_debugEnableCheck;
	QCheckBox * m_infoEnableCheck;
	QCheckBox * m_warningEnableCheck;
	QCheckBox * m_errorEnableCheck;

	QCheckBox * m_debugPopupCheck;
	QCheckBox * m_infoPopupCheck;
	QCheckBox * m_warningPopupCheck;
	QCheckBox * m_errorPopupCheck;
	
public slots:
	/** Called if accept button is pressed */
	virtual void slotAccept();

protected slots:
	void slotDebugEnableToogled(bool on);
	void slotInfoEnableToogled(bool on);
	void slotWarningEnableToogled(bool on);
	void slotErrorEnableToogled(bool on);
	
	void slotDebugPopupToogled(bool on);
	void slotInfoPopupToogled(bool on);
	void slotWarningPopupToogled(bool on);
	void slotErrorPopupToogled(bool on);
};

#endif
