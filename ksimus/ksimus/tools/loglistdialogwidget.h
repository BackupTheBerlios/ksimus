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
class KColorButton;


/**LogList property widget
  *@author Rasmus Diekenbrock
  */


class LogListDialogWidget : public PropertyWidget
{
   Q_OBJECT

public:
	LogListDialogWidget(LogList * logList, QWidget *parent=0, const char *name=0);
	~LogListDialogWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();

protected:
	void initView();
	void initData();
	
	
	LogList * m_logList;
	
	QCheckBox * m_debugEnableCheck;
	QCheckBox * m_infoEnableCheck;
	QCheckBox * m_warningEnableCheck;
	QCheckBox * m_errorEnableCheck;

	QCheckBox * m_debugPopupCheck;
	QCheckBox * m_infoPopupCheck;
	QCheckBox * m_warningPopupCheck;
	QCheckBox * m_errorPopupCheck;
	
	KColorButton * m_debugColor;
	KColorButton * m_infoColor;
	KColorButton * m_warningColor;
	KColorButton * m_errorColor;


protected slots:
	void slotDebugEnableToogled(bool on);
	void slotInfoEnableToogled(bool on);
	void slotWarningEnableToogled(bool on);
	void slotErrorEnableToogled(bool on);
	
	void slotDebugPopupToogled(bool on);
	void slotInfoPopupToogled(bool on);
	void slotWarningPopupToogled(bool on);
	void slotErrorPopupToogled(bool on);

	void slotDebugColorChanged(const QColor & color);
	void slotInfoColorChanged(const QColor & color);
	void slotWarningColorChanged(const QColor & color);
	void slotErrorColorChanged(const QColor & color);
};

#endif
