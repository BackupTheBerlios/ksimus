/***************************************************************************
                          ksimiodeviceoverviewwidget.h  -  description
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

#ifndef KSIMIODEVICEOVERVIEWWIDGET_H
#define KSIMIODEVICEOVERVIEWWIDGET_H

// C/C++ includes

// QT includes

// KDE includes

// Project includes
#include "propertywidget.h"

// Forward declaration
class QListView;
class QPushButton;

/**Overview about all io devices.
  *@author Rasmus Diekenbrock
  */

class KSimIoDeviceOverviewWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	KSimIoDeviceOverviewWidget(QWidget *parent=0, const char *name=0);
	~KSimIoDeviceOverviewWidget();

	void setup();

	static int executeDialog(QWidget *parent=0, const char *name=0);


private:
	QListView * m_list;
	QPushButton * m_buttonConfDev;
	QPushButton * m_buttonNewDev;
	QPushButton * m_buttonDelDev;


private slots:
	void slotConfDevice();
	void slotNewDevice();
	void slotDelDevice();
	void slotSelectionChanged();
};




#endif
