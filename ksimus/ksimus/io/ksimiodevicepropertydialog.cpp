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


class KSimIoDevicePropertyDialog::Private
{
public:
	Private()
	{
		tmpFile = (KTempFile *)0;
	}

	~Private()
	{
		deleteTempFile();
	}

	void deleteTempFile();
	QString getTempFileName();
	
	KTempFile * tmpFile;
};

void KSimIoDevicePropertyDialog::Private::deleteTempFile()
{
	if (tmpFile)
	{
		tmpFile->unlink();
		delete tmpFile;
		tmpFile = (KTempFile *)0;
	}
}

QString KSimIoDevicePropertyDialog::Private::getTempFileName()
{
	if (!tmpFile)
	{
		tmpFile = new KTempFile;
		Q_CHECK_PTR(tmpFile);
	}

	return tmpFile->name();
}

//#########################################################################
//#########################################################################

KSimIoDevicePropertyDialog::KSimIoDevicePropertyDialog(KSimIoDevice *device, const QString & caption, QWidget *parent, const char *name )
	:	KSimDialog(caption, parent, name),
		m_device(device)
{
	m_p = new Private;
	Q_CHECK_PTR(m_p);

	device->initPropertyDialog(this);
}

KSimIoDevicePropertyDialog::~KSimIoDevicePropertyDialog()
{
	delete m_p;
}


void KSimIoDevicePropertyDialog::slotOk()
{
	unsigned int errors = 0;

	emit okClicked();

	if (isChanged())
	{
		// Only if something changed
		errors = getDevice()->executePropertyCheck();

		if(errors)
		{
			setChanged(false);
			KSimpleConfig file(m_p->getTempFileName(), true);
			file.setGroup("/save/");
			getDevice()->load(file);
			m_p->deleteTempFile();
		}
		else
		{
			// No errors
			QDialog::accept();
		}
	}
	else
	{
		// Nothing changed
		QDialog::accept();
	}
}


void KSimIoDevicePropertyDialog::slotDataChanged()
{
	if (!isChanged())
	{
		setChanged(true);
		KSimpleConfig file(m_p->getTempFileName());
		file.setGroup("/save/");
		getDevice()->save(file);
	}
}




int KSimIoDevicePropertyDialog::execute(KSimIoDevice *device, const QString & caption, QWidget *parent, const char *name)
{
	int result;

	KSimIoDevicePropertyDialog * dia;
	dia = new KSimIoDevicePropertyDialog(device, caption, parent, name);
	Q_CHECK_PTR(dia);

	dia->readSize("IO Devices/Property Dialog");

	result = dia->exec();

	dia->writeSize("IO Devices/Property Dialog");

	delete dia;

	return result;
}

#include "ksimiodevicepropertydialog.moc"
