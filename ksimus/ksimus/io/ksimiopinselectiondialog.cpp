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
	:	 KSimDialog(caption,parent,name)
{
}

KSimIoPinSelectionDialog::~KSimIoPinSelectionDialog()
{
}

const KSimIoPin * KSimIoPinSelectionDialog::execute(const KSimIoPin::List & pinList,
                                                    const QString & caption,
                                                    QWidget *parent,
                                                    const char *name)
{
	KDialogBase * dialog = new KDialogBase(KDialogBase::Plain,
	                                       caption /*i18n("IO Devices")*/,
	                                       KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
	                                       KDialogBase::Ok,
	                                       parent, name);


	QWidget * wid = dialog->plainPage();

	KSimIoPinSelectionWidget * child = new KSimIoPinSelectionWidget(pinList, wid, "KSimIoPinSelectionWidget");
	CHECK_PTR(child);

	QBoxLayout * horLayout = new QHBoxLayout(wid);
	horLayout->setMargin(KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());
	horLayout->addWidget(child);

	connect(dialog, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(dialog, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(dialog, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));


	// Load last size
	KConfig * config=kapp->config();
	QString group(config->group());
	config->setGroup("IO Devices/Pin Selection");
	QSize size=config->readSizeEntry("Geometry");
	config->setGroup(group);
	if(!size.isEmpty())
	{
		dialog->resize(size);
	}

	// Execute dialog
	int result = dialog->exec();

	// Save size
	config->setGroup("IO Devices/Pin Selection");
	config->writeEntry("Geometry", dialog->size());
	config->setGroup(group);

	const KSimIoPin * pin = child->getCurrentPin();
	delete dialog;

	return (result == QDialog::Accepted) ? pin : (const KSimIoPin *)0;

}

