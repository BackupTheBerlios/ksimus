/***************************************************************************
                          ksimdialog.h  -  description
                             -------------------
    begin                : Mon Jun 10 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

#ifndef KSIMDIALOG_H
#define KSIMDIALOG_H

// C-Includes

// QT-Includes

// KDE-Includes
#include <kdialogbase.h>

// Project-Includes

// Forward declaration
class PropertyWidget;

/**The dialog class for ksimus dialogs.
  *@author Rasmus Diekenbrock
  */

class KSimDialog : public KDialogBase
{

// Some internal classes
class Private;
class List;
class ListItem;

	Q_OBJECT

public:
	KSimDialog(const QString & caption, QWidget *parent=0, const char *name=0);
	~KSimDialog();
	
	QVBox * addVBoxPage(const QString & item);
	QVBox * addVBoxPage(const QStringList & item);
	
	PropertyWidget * getPage(const QStringList & item);

	/** Connects dialog signals okClicked(), defaultClicked() and cancelClicked() to the widget slots
		slotAccept(), slotDefault() and slotCancel(). Connects the widget signal signalDataChanged() to
		the slot signalChangeData().
	*/
	void connectSlots(PropertyWidget * wid);
	
	void setChanged(bool changed);
	/** Returns true, if one or more PropertyWidget contains changed data. */
	bool isChanged() const;

protected slots:
	virtual void slotDataChanged();
	virtual void slotOk();
	virtual void slotDefault();

private slots:
	void pageDeleted();

private:
	QStringList makeUniqueName(QStringList i18nName);
	Private * m_p;

};

#endif
