/***************************************************************************
                          ksimiopinselectiondialog.h  -  description
                             -------------------
    begin                : Sun Apr 6 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef KSIMIOPINSELECTIONDIALOG_H
#define KSIMIOPINSELECTIONDIALOG_H

// C-Includes

// QT-Includes

// KDE-Includes
#include <kdialogbase.h>

// Project-Includes
#include "ksimiopin.h"

// Forward declaration
class KSimIoDevice;
class KSimIoPinSelectionWidget;


/**Base dialog for io device property
  *@author Rasmus Diekenbrock
  */

class KSimIoPinSelectionDialog : public KDialogBase
{
// Some internal classes
class Private;

   Q_OBJECT
public:
	KSimIoPinSelectionDialog(const KSimIoPin::List & pinList,
	                         const QString & caption,
	                         QWidget *parent=(QWidget *)0,
	                         const char *name=(const char *)0);
	~KSimIoPinSelectionDialog();

	/** Returns the currently selected pin. */
	const KSimIoPin * getCurrentPin() const;

	KSimIoPinSelectionWidget * getSelectionWidget() { return m_selWid; };
	const KSimIoPinSelectionWidget * getSelectionWidget() const { return m_selWid; };

	/** Executes a KSimIoPinSelectionDialog with KSimIoPin::Pool::get().
	    Do some additional stuff like size storing.
	    Returns the selected Pin or a null pointer if canceled. */
	static const KSimIoPin * execute(const QString & caption,
	                         QWidget *parent= (QWidget *)0,
	                         const char *name=(const char *)0);

protected slots:

private:
	Private * m_p;
	KSimIoPinSelectionWidget * m_selWid;
};

#endif
