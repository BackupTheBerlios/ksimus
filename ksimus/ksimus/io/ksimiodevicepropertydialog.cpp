/***************************************************************************
                          ksimiodevicepropertydialog.cpp  -  description
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
//#include <klocale.h>
#include <ktempfile.h>
#include <ksimpleconfig.h>

// Project-Includes
#include "ksimiodevicepropertydialog.h"
#include "ksimiodevice.h"
#include "ksimdebug.h"

// Forward declaration



//#########################################################################
//#########################################################################

KSimIoDevicePropertyDialog::KSimIoDevicePropertyDialog(KSimIoDevice *device, const QString & caption, QWidget *parent, const char *name )
	:	KSimDialog(caption, parent, name),
		m_device(device)
{
}

KSimIoDevicePropertyDialog::~KSimIoDevicePropertyDialog()
{
}

int KSimIoDevicePropertyDialog::execute(KSimIoDevice *device, const QString & caption, QWidget *parent, const char *name)
{
	int result;
	bool ready;
	KTempFile tmpFile;

	KSimIoDevicePropertyDialog * dia;
	dia = new KSimIoDevicePropertyDialog(device, caption, parent, name);
	CHECK_PTR(dia);

	{
		KSimpleConfig file(tmpFile.name());
		file.setGroup("/save/");
		device->save(file);
		// Close "file"
	}
	
	device->initPropertyDialog(dia);

	dia->readSize("IO Devices/Property Dialog");

	do
	{
		ready = true;
		result = dia->exec();
		if ((result == QDialog::Accepted)
		 && (device->executePropertyCheck(parent) != 0))
		{
			ready = false;
		}
	}
	while(!ready);

	if(result == QDialog::Rejected)
	{
		KSimpleConfig file(tmpFile.name(), true);
		file.setGroup("/save/");
		device->load(file);
		// Close "file"
	}

	tmpFile.unlink();

	dia->writeSize("IO Devices/Property Dialog");

	delete dia;

	return result;
}

