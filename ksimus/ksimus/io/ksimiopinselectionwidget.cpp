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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimiopinselectionwidget.h"
#include "ksimiodevice.h"
#include "ksimiopin.h"

// Forward declaration

class KSimIoPinSelectionWidget::ListItem : public QListViewItem
{
public:
	ListItem(QListView * list, const KSimIoPin * pin)
	:	QListViewItem(list),
		m_pin(pin)
	{

	};

//	~KSimIoDeviceListItem();

	virtual QString text(int column) const;

	const KSimIoPin * getPin() const { return m_pin; };


private:
	const KSimIoPin * m_pin;
};


QString KSimIoPinSelectionWidget::ListItem::text(int column) const
{
	switch(column)
	{
		case 0:
			return getPin()->getDevice()->getName();
			break;

		case 1:
			return getPin()->getName();
			break;

		default:
			return QString::null;
			break;
	}
}


//################################################################################
//################################################################################

KSimIoPinSelectionWidget::KSimIoPinSelectionWidget(const KSimIoPin::List & pinList, QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name)
{
	new QLabel(i18n("IO Pins:"), this);

	m_list = new QListView(this, "m_list");
	CHECK_PTR(m_list);
	m_list->addColumn(i18n("Device"));
	m_list->addColumn(i18n("IO Pin"));


	FOR_EACH_IO_PIN(it, pinList)
	{
		new ListItem(m_list, it.current());
	}
}

KSimIoPinSelectionWidget::~KSimIoPinSelectionWidget()
{
}

const KSimIoPin * KSimIoPinSelectionWidget::getCurrentPin() const
{
	QListViewItem* item = m_list->currentItem();
	if (item)
	{
		return ((KSimIoPinSelectionWidget::ListItem *)item)->getPin();
	}
	else
	{
		return (const KSimIoPin *)0;
	}
}




