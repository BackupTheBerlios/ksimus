/***************************************************************************
                          ksimiopinselectionwidget.cpp  -  description
                             -------------------
    begin                : Sun Apr 6 2003
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
#include <qlabel.h>
#include <qlistview.h>
#include <qdict.h>
#include <qstringlist.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimiopinselectionwidget.h"
#include "ksimiodevice.h"
#include "ksimiodeviceinfo.h"
#include "ksimiopin.h"
#include "ksimiojoin.h"

// Forward declaration

class KSimIoPinSelectionWidget::ListItem : public QListViewItem
{
public:
	ListItem(QListView * list, bool isDevice)
	:	QListViewItem(list),
		m_isDevice(isDevice)
	{};

	ListItem(QListViewItem * listItem, bool isDevice)
	:	QListViewItem(listItem),
		m_isDevice(isDevice)
	{};

//	~ListItem();

	bool isDevice() const { return m_isDevice; };


private:
	bool m_isDevice;
};


//################################################################################
//################################################################################

class KSimIoPinSelectionWidget::DeviceListItem : public KSimIoPinSelectionWidget::ListItem
{
public:
	DeviceListItem(QListView * list, const KSimIoDevice * dev)
	:	ListItem(list, true),
		m_dev(dev)
	{
		init();
	};

//	~KSimIoDeviceListItem();

	const KSimIoDevice * getDevice() const { return m_dev; };


private:
	void init();
	const KSimIoDevice * m_dev;
};


void KSimIoPinSelectionWidget::DeviceListItem::init()
{
	setText(0, getDevice()->getName());
	setText(1, getDevice()->getInfo()->getI18nLibName());
}


//################################################################################
//################################################################################

class KSimIoPinSelectionWidget::PinListItem : public KSimIoPinSelectionWidget::ListItem
{
public:
	PinListItem(QListView * list, const KSimIoPin * pin)
	:	ListItem(list, false),
		m_pin(pin)
	{
		init();
	};

	PinListItem(QListViewItem * item, const KSimIoPin * pin)
	:	ListItem(item, false),
		m_pin(pin)
	{
		init();
	};

//	~KSimIoDeviceListItem();

	const KSimIoPin * getPin() const { return m_pin; };


private:
	void init();
	const KSimIoPin * m_pin;
};


void KSimIoPinSelectionWidget::PinListItem::init()
{
	setText(0,getPin()->getName());
	setText(1, getPin()->getSelectedJoinInfo()->getI18nLibName());
}

//################################################################################
//################################################################################

class KSimIoPinSelectionWidget::Private
{
public:
	Private()
	{};

//	~Private();

	QListView * list;
	QDict<DeviceListItem> * deviceDict;

};

//################################################################################
//################################################################################

KSimIoPinSelectionWidget::KSimIoPinSelectionWidget(const KSimIoPin::List & pinList, QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name)
{
	m_p = new Private();
	CHECK_PTR(m_p);

	QLabel * label = new QLabel(i18n("IO Pins:"), this);

	m_p->list = new QListView(this, "m_list");
	CHECK_PTR(m_p->list);
	m_p->list->addColumn(i18n("Name"));
	m_p->list->addColumn(i18n("Type"));
	m_p->list->setRootIsDecorated(true);
	label->setBuddy(m_p->list);
	
	//TODO add addToolTip


	connect(m_p->list, SIGNAL(doubleClicked(QListViewItem *)), this, SLOT(slotDoubleClicked(QListViewItem *)));

	m_p->deviceDict = new QDict<DeviceListItem>();
	CHECK_PTR(m_p->deviceDict);

	FOR_EACH_IO_PIN(it, pinList)
	{
		QString devName = it.current()->getDevice()->getName();

		DeviceListItem * devItem = m_p->deviceDict->find(devName);
		if (!devItem)
		{
			devItem = new DeviceListItem(m_p->list, it.current()->getDevice());
			CHECK_PTR(devItem);
			m_p->deviceDict->insert(devName, devItem);
			devItem->setSelectable(false);
		}
		new PinListItem(devItem, it.current());
	}
}

KSimIoPinSelectionWidget::~KSimIoPinSelectionWidget()
{
	delete m_p->deviceDict;
}

const KSimIoPin * KSimIoPinSelectionWidget::getCurrentPin() const
{
	ListItem* item = (ListItem *)m_p->list->currentItem();
	if (item && !item->isDevice())
	{
		return ((KSimIoPinSelectionWidget::PinListItem *)item)->getPin();
	}
	else
	{
		return (const KSimIoPin *)0;
	}
}

void KSimIoPinSelectionWidget::setOpenDeviceList(const QStringList & openList)
{
	QDictIterator<DeviceListItem> it(*m_p->deviceDict);

	while (it.current())
	{
		it.current()->setOpen(openList.contains(it.currentKey()));
		++it;
	}
}

QStringList KSimIoPinSelectionWidget::getOpenDeviceList() const
{
	QStringList openList;

	QDictIterator<DeviceListItem> it(*m_p->deviceDict);

	while (it.current())
	{
		if (it.current()->isOpen())
		{
			openList.append(it.currentKey());
		}
		++it;
	}

	return openList;
}

void KSimIoPinSelectionWidget::slotDoubleClicked(QListViewItem * lvi)
{
	if (lvi)
	{
		ListItem* item = (ListItem *)lvi;
		if (!item->isDevice())
		{
			emit signalValidDoubleClicked();
		}
	}
}

#include "ksimiopinselectionwidget.moc"

