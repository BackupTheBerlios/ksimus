/***************************************************************************
                          ksimiodevicepropertybasewidget.h  -  description
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

#ifndef KSIMIODEVICEPROPERTYBASEWIDGET_H
#define KSIMIODEVICEPROPERTYBASEWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"

// Forward declaration
class KSimIoDevice;


/**Base widget for io device properties
  *@author Rasmus Diekenbrock
  */

class KSimIoDevicePropertyBaseWidget : public PropertyWidget
{
	Q_OBJECT

public:
	KSimIoDevicePropertyBaseWidget(KSimIoDevice * device, int n, QWidget *parent=0, const char *name=0, WFlags f=0);
	~KSimIoDevicePropertyBaseWidget();

	KSimIoDevice * getDevice() const { return m_device; };

private:
	KSimIoDevice * m_device;
	
};

#endif
