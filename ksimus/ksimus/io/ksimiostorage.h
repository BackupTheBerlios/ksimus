/***************************************************************************
                          ksimiostorage.h  -  description
                             -------------------
    begin                : Mon Apr 7 2003
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

#ifndef KSIMIOSTORAGE_H
#define KSIMIOSTORAGE_H

// C/C++ includes

// QT includes
#include <qvaluelist.h>

// KDE includes

// Project includes

// Forward declaration
class KSimpleConfig;
class KSimIoDevice;
class KSimIoDeviceList;


/**Storage for io things.
  *@author Rasmus Diekenbrock
  */

class KSimIoStorage
{
public: 
	KSimIoStorage();
	~KSimIoStorage();

	bool open(bool readOnly = false);
	void close();
	void loadSerialList();
	void saveSerialList() const;

	unsigned int getNextSerial();
	void save(KSimIoDevice * device);

	bool load(unsigned int serial, KSimIoDevice * * device);
	bool loadAll(KSimIoDeviceList * list);
	void remove(KSimIoDevice * device);

	bool isReadOnly() const { return m_readOnly; };

protected:
	bool existDeviceSerial(unsigned int lastSerial) const;
	
	KSimpleConfig * m_conf;
	QValueList<unsigned int> m_deviceSerialList;


	// file name
	static const char * const s_filename;
	static const char * const s_type;
	// entries
	static const char * const s_storage;
	static const char * const s_deviceList;
	// devices
	static const char * const s_deviceDir;
	static const char * const s_lastSerial;
	static const char * const s_deviceType;

private:
	bool m_readOnly;
};

#endif
