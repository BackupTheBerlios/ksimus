/***************************************************************************
                          ksimiodevicepropertypinwidget.h  -  description
                             -------------------
    begin                : Fri Jun 6 2003
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

#ifndef KSIMIODEVICEPROPERTYPINWIDGET_H
#define KSIMIODEVICEPROPERTYPINWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimiodevicepropertybasewidget.h"
#include "ksimiopin.h"

// Forward declaration
class QLineEdit;
class QComboBox;

/**Property widget for io pins.
  *@author Rasmus Diekenbrock
  */

class KSimIoDevicePropertyPinWidget : public KSimIoDevicePropertyBaseWidget
{
	Q_OBJECT
	
public: 
	KSimIoDevicePropertyPinWidget(KSimIoPin * pin, QWidget *parent=0, const char *name=0);
	~KSimIoDevicePropertyPinWidget();

	KSimIoPin * getPin() { return m_pin; };
	const KSimIoPin * getPin() const { return m_pin; };

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
	QComboBox * m_typeSelector;

private:
	KSimIoPin * m_pin;
};

#endif
