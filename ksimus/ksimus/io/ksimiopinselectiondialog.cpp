/***************************************************************************
                          ksimiopinselectiondialog.cpp  -  description
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
#include <qlayout.h>
#include <qpushbutton.h>

// KDE-Includes
#include <klocale.h>
#include <kdialogbase.h>
#include <kconfig.h>
#include <kapp.h>

// Project-Includes
#include "ksimiopinselectiondialog.h"
#include "ksimiopinselectionwidget.h"

// Forward declaration


KSimIoPinSelectionDialog::KSimIoPinSelectionDialog(const KSimIoPin::List & pinList,
                                                   const QString & caption,
                                                   QWidget *parent,
                                                   const char *name)
	:	 KDialogBase(KDialogBase::Plain,
		             caption,
		             KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
		             KDialogBase::Ok,
		             parent,name)
{
	QWidget * wid = plainPage();

	m_selWid = new KSimIoPinSelectionWidget(pinList, wid, "KSimIoPinSelectionWidget");
	CHECK_PTR(m_selWid);
	m_selWid->setMargin(0);

	QBoxLayout * horLayout = new QHBoxLayout(wid);
	CHECK_PTR(horLayout);
	horLayout->addWidget(m_selWid);

	connect(this, SIGNAL(okClicked()), m_selWid, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), m_selWid, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), m_selWid, SLOT(slotCancel()));


	QPushButton * okButton = actionButton(KDialogBase::Ok);
	CHECK_PTR(okButton);
	connect(m_selWid, SIGNAL(signalValidDoubleClicked()), okButton, SLOT(animateClick()));
}

KSimIoPinSelectionDialog::~KSimIoPinSelectionDialog()
{
}

const KSimIoPin * KSimIoPinSelectionDialog::getCurrentPin() const
{
	return m_selWid->getCurrentPin();
}




const KSimIoPin * KSimIoPinSelectionDialog::execute(const QString & caption,
                                                    QWidget *parent,
                                                    const char *name)
{
	KSimIoPinSelectionDialog * dialog = new KSimIoPinSelectionDialog(KSimIoPin::Pool::get(), caption, parent, name);
	CHECK_PTR(dialog);


	// Load last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("IO Devices/Pin Selection");
	QSize size=config->readSizeEntry("Geometry");
	if(!size.isEmpty())
	{
		dialog->resize(size);
	}

	dialog->getSelectionWidget()->setOpenDeviceList(config->readListEntry("Opened Device List"));
	config->setGroup(group);

	// Execute dialog
	int result = dialog->exec();

	// Save size
	config->setGroup("IO Devices/Pin Selection");
	config->writeEntry("Geometry", dialog->size());
	// Save opened devices
	config->writeEntry("Opened Device List", dialog->getSelectionWidget()->getOpenDeviceList());
	config->setGroup(group);

	const KSimIoPin * pin = dialog->getCurrentPin();
	delete dialog;

	return (result == QDialog::Accepted) ? pin : (const KSimIoPin *)0;

}

#include "ksimiopinselectiondialog.moc"
