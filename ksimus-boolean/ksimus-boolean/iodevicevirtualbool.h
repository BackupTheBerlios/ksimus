/***************************************************************************
                          iodevicevirtualbool.h  -  description
                             -------------------
    begin                : Sun May 18 2003
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

#ifndef IODEVICEVIRTUALBOOL_H
#define IODEVICEVIRTUALBOOL_H

// C/C++ includes

// QT includes
#include <qobject.h>
#include <qstring.h>
#include <qbitarray.h>

// KDE includes

// KSimus includes
#include "ksimus/ksimiodevice.h"
#include "ksimus/ksimiodevicepropertygeneralwidget.h"

// Project includes

// Forward declaration
class KSimIoDeviceInfo;
class KSimSpinBox;
class QLabel;

//###############################################################

namespace KSimLibBoolean
{

/**A IO Device with virtual boolean pins.
	*
  * @author Rasmus Diekenbrock
  */


class IoDeviceVirtualBool : public KSimIoDevice
{
   Q_OBJECT
public:
	IoDeviceVirtualBool(const KSimIoDeviceInfo * info);
	~IoDeviceVirtualBool();


	/** Sets an io pin.
	  * @param ioPinID The ID of the pin to set.
	  * @param pValue  A pointer to the value to set. The type depends on the pin type.
	  */
	virtual void setIO(int ioPinID, const void * pValue);
	/** Reads an io pin.
	  * @param ioPinID The ID of the pin to set.
	  * @param pValue  A pointer to the value will be writen. The type depends on the pin type.
	  */
	virtual void getIO(int ioPinID, void * pValue) const;


	void setChannelCount(unsigned int count);
	void setChannelCountDelayed(unsigned int count);
	unsigned int getChannelCount() const;
	unsigned int getMinChannelCount() const;
	
	virtual void save(KConfigBase & config) const;
	virtual bool load(KConfigBase & config);

	/** Creates the general property page for the property dialog.
	  * Overload this function if you want to use a modified General Propery Page. Use as base class
	  * @ref KSimIoDevicePropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual KSimIoDevicePropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Is called after execution of the PropertyDialog
	  * (@ref initPropertyDialog). Use this function to adjust things which are difficult
	  * to handle inside a slot or the function @ref PropertyWidget::acceptPressed.
	  *
	  * For example it is problematic to change the connector count inside the PropertyDialog
	  * because each connector has also a property widget. If you reduce the connector count
	  * some of these propert widgets have no valid connector. This function delays the connector
	  * count modification until all property widgets are removed.
	  */
	virtual void menuExecuted();

	static const KSimIoDeviceInfo * getStaticInfo();
	static KSimIoDevice * create(const KSimIoDeviceInfo * info);
	static unsigned int tempConnCount;
	static IoDeviceVirtualBool * tempConnCountOwner;

private:
	QBitArray m_bitArray;
	int m_maxPinID;

};


//###############################################################
//###############################################################

class IoDeviceVirtualBoolPropertyGeneralWidget : public KSimIoDevicePropertyGeneralWidget
{
	Q_OBJECT

public:
	IoDeviceVirtualBoolPropertyGeneralWidget(IoDeviceVirtualBool * device, QWidget *parent, const char *name);
//	~IoDeviceVirtualBoolPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();

	IoDeviceVirtualBool * getIoDeviceVirtualBool() const { return (IoDeviceVirtualBool*) getDevice(); };


protected:
	KSimSpinBox * m_channels;
	QLabel * m_channelsLabel;
};


//###############################################################

}; //namespace KSimLibBoolean

#endif
