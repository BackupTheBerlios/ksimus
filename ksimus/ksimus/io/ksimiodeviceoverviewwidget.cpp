/***************************************************************************
                          ksimiodeviceoverviewwidget.cpp  -  description
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

// C/C++ includes

// QT includes
#include <qlistview.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>

// KDE includes
#include <klocale.h>
#include <kdialogbase.h>
#include <kconfig.h>
#include <kapp.h>

// Project includes
#include "library.h"
#include "ksimiodeviceoverviewwidget.h"
#include "ksimiodevicelist.h"
#include "ksimiodeviceinfo.h"
#include "ksimiodevicelibrary.h"
#include "ksimiodevicelibraryitem.h"
#include "ksimiodevicepropertydialog.h"
#include "ksimiostorage.h"

// Forward declaration


class KSimIoDeviceListItem : public QListViewItem
{
public:
	KSimIoDeviceListItem(QListView * list, KSimIoDevice * device)
	:	QListViewItem(list),
		m_device(device)
	{
		
	};
	
//	~KSimIoDeviceListItem();

	virtual QString text(int column) const;

	KSimIoDevice * getDevice() const { return m_device; };

	
private:
	KSimIoDevice * m_device;
};


QString KSimIoDeviceListItem::text(int column) const
{
	switch(column)
	{
		case 0:
			return getDevice()->getName();
			break;

		case 1:
			return getDevice()->getInfo()->getI18nLibName();
			break;

		default:
			return QString::null;
	}
}


//################################################################################
//################################################################################

class KSimIoDeviceInfoListItem : public QListViewItem
{
public:
	KSimIoDeviceInfoListItem(QListView * list, const KSimIoDeviceInfo * deviceInfo)
	:	QListViewItem(list),
		m_deviceInfo(deviceInfo)
	{

	};

//	~KSimIoDeviceInfoListItem();

	virtual QString text(int column) const;

	const KSimIoDeviceInfo * getDeviceInfo() const { return m_deviceInfo; };


private:
	const KSimIoDeviceInfo * m_deviceInfo;
};


QString KSimIoDeviceInfoListItem::text(int /*column*/) const
{
	return getDeviceInfo()->getI18nLibName();
}


//################################################################################
//################################################################################





KSimIoDeviceOverviewWidget::KSimIoDeviceOverviewWidget(QWidget *parent, const char *name )
	:	PropertyWidget(3, parent,name)
{
	setColStretch(0, 10);
	setColStretch(1, 0);
	setColStretch(2, 0);
	addColSpacing(1, KDialog::spacingHint());

	QVBox * leftBox = new QVBox(this, "leftBox");
	CHECK_PTR(leftBox);
	leftBox->setSpacing(KDialog::spacingHint());

	new QLabel(i18n("IO Devices known by KSimus:"), leftBox, "label");

	m_list = new QListView(leftBox, "m_list");
	CHECK_PTR(m_list);
	m_list->addColumn(i18n("Name"));
	m_list->addColumn(i18n("Type"));
	connect(m_list, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()));

	addEmptyCell();

	QVBox * buttonBox = newVBox(0, "buttonBox");
	CHECK_PTR(buttonBox);


	m_buttonConfDev = new QPushButton(i18n("Configure"), buttonBox, "m_buttonConfDev");
	CHECK_PTR(m_buttonConfDev);
	connect(m_buttonConfDev, SIGNAL(clicked()), this, SLOT(slotConfDevice()));
	
	m_buttonNewDev = new QPushButton(i18n("New"), buttonBox, "m_buttonNewDev");
	CHECK_PTR(m_buttonNewDev);
	connect(m_buttonNewDev, SIGNAL(clicked()), this, SLOT(slotNewDevice()));

	m_buttonDelDev = new QPushButton(i18n("Delete"), buttonBox, "m_buttonDelDev");
	CHECK_PTR(m_buttonDelDev);
	connect(m_buttonDelDev, SIGNAL(clicked()), this, SLOT(slotDelDevice()));

	connect(m_list, SIGNAL(doubleClicked(QListViewItem*)), m_buttonConfDev, SLOT(animateClick()));

	setup();
	slotSelectionChanged();
}

KSimIoDeviceOverviewWidget::~KSimIoDeviceOverviewWidget()
{
}                   

void KSimIoDeviceOverviewWidget::setup()
{
	FOR_EACH_IO_DEVICE(it,*KSimIoDeviceList::getList())
	{
		new KSimIoDeviceListItem(m_list, it.current());
	}
}


// Slots

void KSimIoDeviceOverviewWidget::slotConfDevice()
{
	if (m_list->currentItem())
	{
		KSimIoDevice * dev = ((KSimIoDeviceListItem *)m_list->currentItem())->getDevice();
		int res = KSimIoDevicePropertyDialog::execute(dev, i18n("Configure IO Device"), this);

		if (res == QDialog::Accepted)
		{
			// Save to file
			KSimIoStorage storage;
			storage.open();
			storage.save(dev);
			storage.close();
		}
	}
}

void KSimIoDeviceOverviewWidget::slotNewDevice()
{
	KDialogBase * dialog = new KDialogBase(KDialogBase::Plain,
	                                       i18n("Select New IO Device"),
	                                       /*KDialogBase::Default |*/ KDialogBase::Ok | KDialogBase::Cancel,
	                                       KDialogBase::Ok,
	                                       parentWidget(), "Select New IO Device");

	CHECK_PTR(dialog);
	QWidget * wid = dialog->plainPage();

	QBoxLayout * horLayout = new QVBoxLayout(wid);
	horLayout->setMargin(0);//KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());

	QLabel * l = new QLabel(i18n("Select a IO Device Type:"), wid, "label");
	CHECK_PTR(l);
	horLayout->addWidget(l);


	QListView * list = new QListView(wid, "new device list");
	CHECK_PTR(list);
	list->addColumn(i18n("Type"));
	QPushButton * okButton = dialog->actionButton(KDialogBase::Ok);
	CHECK_PTR(okButton);
	connect(list, SIGNAL(doubleClicked(QListViewItem*)), okButton, SLOT(animateClick()));

	horLayout->addWidget(list);

	
	// Create device list items
	QListIterator<BaseLibraryItem> it(*g_library->getIoDeviceLib()->getList());
	for(;it.current();++it)
	{
		KSimIoDeviceLibraryItem * item = (KSimIoDeviceLibraryItem *)it.current();
		new KSimIoDeviceInfoListItem(list, item->getKSimIoDeviceInfo());
	}

	// Load last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("IO Devices/New Dialog");
	QSize size=config->readSizeEntry("Geometry");
	config->setGroup(group);
	if(!size.isEmpty())
	{
		dialog->resize(size);
	}

	// Execute dialog
	int res = dialog->exec();

	// Get current device info
	const KSimIoDeviceInfo * devInfo = (const KSimIoDeviceInfo *)0;
	if ((res == QDialog::Accepted) && list->currentItem())
	{
		devInfo = ((KSimIoDeviceInfoListItem *)list->currentItem())->getDeviceInfo();
	}

	delete dialog;

	// Save size
	config->setGroup("IO Devices/New Dialog");
	config->writeEntry("Geometry", dialog->size());
	config->setGroup(group);

	if (devInfo != (const KSimIoDeviceInfo *)0)
	{
		KSimIoDevice * dev = devInfo->create(devInfo);

		res = KSimIoDevicePropertyDialog::execute(dev, i18n("Configure IO Device"), parentWidget());

		if (res == QDialog::Accepted)
		{
			// Append to globale list
			KSimIoDeviceList::getList()->append(dev);
			// Add pins to pool
			dev->addPins2Pool();
			// Add to list view
			KSimIoDeviceListItem * item = new KSimIoDeviceListItem(m_list, dev);
			CHECK_PTR(item);

			m_list->setCurrentItem(item);
			
			// Save to file
			KSimIoStorage storage;
			storage.open();
			storage.save(dev);
			storage.close();
		}
		else
		{
			// Canceled
			delete dev;
		}
	}
}

void KSimIoDeviceOverviewWidget::slotDelDevice()
{
	if (m_list->currentItem())
	{
		KSimIoDevice * dev = ((KSimIoDeviceListItem *)m_list->currentItem())->getDevice();
		// Remove from list view
		delete m_list->currentItem();
		
		// Remove from file
		KSimIoStorage storage;
		storage.open();
		storage.remove(dev);
		storage.close();

		// Remove from globale list
		KSimIoDeviceList::getList()->remove(dev);
		slotSelectionChanged();
	}
}

void KSimIoDeviceOverviewWidget::slotSelectionChanged()
{
	if (m_list->currentItem())
	{
		// A device is selected
		m_buttonConfDev->setEnabled(true);
		m_buttonDelDev->setEnabled(true);
	}
	else
	{
		// No device is selected
		m_buttonConfDev->setEnabled(false);
		m_buttonDelDev->setEnabled(false);
	}
}


//################################################################################
//################################################################################

// Static executor
int KSimIoDeviceOverviewWidget::executeDialog(QWidget *parent, const char *name)
{
	KDialogBase * dialog = new KDialogBase(KDialogBase::Plain,
	                                       i18n("IO Devices"),
	                                       KDialogBase::Close,
	                                       KDialogBase::Close,
	                                       parent, name);
	                                       

	CHECK_PTR(dialog);
	QWidget * wid = dialog->plainPage();

	KSimIoDeviceOverviewWidget * child = new KSimIoDeviceOverviewWidget(wid, "KSimIoDeviceOverviewWidget");
	CHECK_PTR(child);
	child->setMargin(0);//KDialog::marginHint());
//	child->setSpacing(0);//KDialog::spacingHint());

	QBoxLayout * horLayout = new QVBoxLayout(wid);
//	horLayout->setMargin(0);//KDialog::marginHint());
//	horLayout->setSpacing(0);//KDialog::spacingHint());
	horLayout->addWidget(child);

	connect(dialog, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(dialog, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(dialog, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));

	// Load last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("IO Devices/OverView Dialog");
	QSize size=config->readSizeEntry("Geometry");
	config->setGroup(group);
	if(!size.isEmpty())
	{
		dialog->resize(size);
	}

	// Execute dialog
	int result = dialog->exec();

	// Save size
	config->setGroup("IO Devices/OverView Dialog");
	config->writeEntry("Geometry", dialog->size());
	config->setGroup(group);

	delete dialog;

	return result;

}



//################################################################################
//################################################################################

