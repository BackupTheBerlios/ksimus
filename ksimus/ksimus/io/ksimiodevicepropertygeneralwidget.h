/***************************************************************************
                          ksimiodevicepropertygeneralwidget.h  -  description
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

#ifndef KSIMIODEVICEPROPERTYGENERALWIDGET_H
#define KSIMIODEVICEPROPERTYGENERALWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimiodevicepropertybasewidget.h"

// Forward declaration
class QLineEdit;
class QMultiLineEdit;

/**General properties
  *@author Rasmus Diekenbrock
  */


class KSimIoDevicePropertyGeneralWidget : public KSimIoDevicePropertyBaseWidget
{
	Q_OBJECT

public:
	KSimIoDevicePropertyGeneralWidget(KSimIoDevice * device, QWidget *parent=0, const char *name=0);
	~KSimIoDevicePropertyGeneralWidget();


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
	QLineEdit * m_nameEdit;
	QMultiLineEdit * m_descriptionEdit;

};

#endif
