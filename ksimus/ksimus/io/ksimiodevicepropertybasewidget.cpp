/***************************************************************************
                          ksimiodevicepropertybasewidget.cpp  -  description
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimiodevicepropertybasewidget.h"

// Forward declaration

KSimIoDevicePropertyBaseWidget::KSimIoDevicePropertyBaseWidget(KSimIoDevice * device, int n, QWidget *parent, const char *name, WFlags f)
	:	PropertyWidget(n,parent,name,f),
		m_device(device)
{
	setCenterRowStretch(0);
	setBottomRowStretch(1);
}

KSimIoDevicePropertyBaseWidget::~KSimIoDevicePropertyBaseWidget()
{
}

#include "ksimiodevicepropertybasewidget.moc"
