/***************************************************************************
                          ksimiodevicepropertydialog.h  -  description
                             -------------------
    begin                : Sat Apr 5 2003
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

#ifndef KSIMIODEVICEPROPERTYDIALOG_H
#define KSIMIODEVICEPROPERTYDIALOG_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"
#include "ksimdialog.h"

// Forward declaration
class KSimIoDevice;


/**Base dialog for io device property
  *@author Rasmus Diekenbrock
  */

class KSimIoDevicePropertyDialog : public KSimDialog
{
// Some internal classes
class Private;

   Q_OBJECT
public:
	KSimIoDevicePropertyDialog(KSimIoDevice *device, const QString & caption, QWidget *parent=0, const char *name=0);
	~KSimIoDevicePropertyDialog();

	KSimIoDevice * getDevice() const { return m_device; };
	
	/** Executes a KSimIoDevicePropertyDialog.
	    Do some additional stuff like size storing. */
	static int execute(KSimIoDevice *device, const QString & caption, QWidget *parent= (QWidget *)0, const char *name=(const char *)0);

protected slots:
	virtual void slotDataChanged();
	virtual void slotOk();


private:
	KSimIoDevice * m_device;
	Private * m_p;
};

#endif
