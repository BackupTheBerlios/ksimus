/***************************************************************************
                          ksimiodevice.h  -  description
                             -------------------
    begin                : Sun Mar 30 2003
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

#ifndef KSIMIODEVICE_H
#define KSIMIODEVICE_H

// C/C++ includes

// QT includes
#include <qobject.h>
#include <qstring.h>

// KDE includes

// Project includes
#include "ksimiopin.h"

// Forward declaration
class KSimusDoc;
class KConfigBase;
class KSimIoDeviceInfo;
class KSimIoDevicePropertyDialog;
class KSimIoDevicePropertyBaseWidget;

/**Base class for IO devices
  *@author Rasmus Diekenbrock
  */

class KSimIoDevice : public QObject
{
class Private;
class JoinItem;
class JoinItemList;

   Q_OBJECT
public:
	virtual ~KSimIoDevice();


	/** Sets the name of the device. */
	void setName(const QString & newName);
	/** Returns the name of the device. */
	QString getName() const { return m_deviceName; };

	/** Sets the description of the device. */
	void setDescription(const QString & newDescription);
	/** Returns the description of the device. */
	QString getDescription() const { return m_deviceDescription; };

	/** Returns the type of the device. */
	const KSimIoDeviceInfo * getInfo() const { return m_info; };


	virtual void save(KConfigBase & config) const;
	virtual bool load(KConfigBase & config);

	virtual void checkCircuit(const KSimusDoc * doc, QStringList & errorMsgList);

	virtual bool openDevice(QString & errorMsg);
	virtual void closeDevice();

	unsigned int isDeviceOpened() const;

	/** Sets an io pin.
	  * @param ioPinID The ID of the pin to set.
	  * @param pValue  A pointer to the value to set. The type depends on the pin type.
	  */
	virtual void setIO(int ioPinID, const void * pValue) = 0;
	/** Reads an io pin.
	  * @param ioPinID The ID of the pin to set.
	  * @param pValue  A pointer to the value will be writen. The type depends on the pin type.
	  */
	virtual void getIO(int ioPinID, void * pValue) const = 0;

	void setSerial(unsigned int serial);
	unsigned int getSerial() const { return m_mySerial; };
	
	/** The IO device may be used only once. 
	  * The default is not exclusive. */
	bool isExclusive() const;
	void setExclusive(bool exclusive);

	//#####################################
	// Pins
	const KSimIoPin::List & getPinList() const { return m_pinList; };
	KSimIoPin::List & getPinList() { return m_pinList; };
	const KSimIoPin * findPinByID(int ioPinID) const;
	
	void addPins2Pool();

	//#####################################
	// Joins

	/** Register a join. The join use this device. */
	void registerJoin(KSimIoJoin * join);

	/** Unregister a join. The join had used this device.
	  * Returns false, if join was not registered. */
	bool unregisterJoin(KSimIoJoin * join);

	//#####################################
	// Dialog
	/** Init the property dialog */
	virtual void initPropertyDialog(KSimIoDevicePropertyDialog * dialog);
	void addGeneralProperty(KSimIoDevicePropertyDialog * dialog);
	/** Creates the general property page for the property dialog.
	  * Overload this function if you want to use a modified General Propery Page. Use as base class
	  * @ref KSimIoDevicePropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual KSimIoDevicePropertyBaseWidget * createGeneralProperty(QWidget *parent);

	/** Checks the io device property. The functions is called after the
	*   property dialog.
	*   Eg. all connectors have unique names.
	*   The default implementation checks the names.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);
	
	/** Executes the property check.
	*
	* Calls @ref checkProperty and evaluates the result. The function does nothing without detected errors.
	* With one or more error the error messages will be display in the log list and in a message box.
	*
	* The function returns the number of errors.
	*/
  unsigned int executePropertyCheck(QWidget * parent=0);

protected:
	KSimIoDevice(const KSimIoDeviceInfo * info, QObject *parent=0, const char *name=0);

private: // Private attributes
	QString m_deviceName;
	QString m_deviceDescription;
	const KSimIoDeviceInfo * m_info;
	KSimIoPin::List m_pinList;
	unsigned int m_mySerial;
	Private * m_p;

};

//################################################################################
//################################################################################

//#include "ksimiodeviceinfo.h"
class KSimIoDeviceInfo;

/** Test device */

class KSimIoDeviceTest : public KSimIoDevice
{
   Q_OBJECT
public:
	KSimIoDeviceTest(const KSimIoDeviceInfo * info, QObject *parent=0, const char *name=0);


	static const KSimIoDeviceInfo * getStaticInfo();
	static KSimIoDevice * create(const KSimIoDeviceInfo * info);

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

private:
	bool m_inBool1;
	bool m_outBool1;
	double m_inDouble1;
	double m_outDouble1;
};


#endif
