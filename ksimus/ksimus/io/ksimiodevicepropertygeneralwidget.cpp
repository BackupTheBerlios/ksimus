/***************************************************************************
                          ksimiodevicepropertygeneralwidget.cpp  -  description
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
#include <qlabel.h>
#include <qlineedit.h>
#include <qmultilineedit.h>
#include <qvbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimiodevicepropertygeneralwidget.h"
#include "ksimiodevice.h"
#include "ksimiodeviceinfo.h"
#include "ksimdebug.h"

// Forward declaration


KSimIoDevicePropertyGeneralWidget::KSimIoDevicePropertyGeneralWidget(KSimIoDevice * device, QWidget *parent, const char *name)
	:	KSimIoDevicePropertyBaseWidget(device, 2, parent, name)
{
	setCenterRowStretch(1);
	setBottomRowStretch(0);
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QString str;
	QLabel * label;
	
	// Name edit	
	str = i18n("Change the name of the IO device here.");
	label = new QLabel(i18n("Name:"), this,"LineEditLabel");
	CHECK_PTR(label);
	QToolTip::add(label, str);
	QWhatsThis::add(label, str);
	
	m_nameEdit = new QLineEdit(this,"m_nameEdit");
	CHECK_PTR(m_nameEdit);
	m_nameEdit->setText(getDevice()->getName());
	QToolTip::add(m_nameEdit, str);
	QWhatsThis::add(label, str);

	// Device type
	str = i18n("Shows the IO device type.");
	
	label = new QLabel(i18n("Type:"), this);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	QWhatsThis::add(label, str);
	
	label = new QLabel(getDevice()->getInfo()->getName(), this);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	QWhatsThis::add(label, str);
	
	// Device description  
	QVBox * descBox = newRowVBox("descBox");
	str = i18n("Enter a description for the IO device here.");

	label = new QLabel(i18n("Description:"), descBox);
	CHECK_PTR(label);
	QToolTip::add(label, str);
	QWhatsThis::add(label, str);

	m_descriptionEdit = new QMultiLineEdit(descBox, "m_descriptionEdit");
	CHECK_PTR(label);
	m_descriptionEdit->setText(getDevice()->getDescription());
	m_descriptionEdit->setMaxLines(-1);
	QToolTip::add(m_descriptionEdit, str);
	QWhatsThis::add(m_descriptionEdit, str);
	
}

KSimIoDevicePropertyGeneralWidget::~KSimIoDevicePropertyGeneralWidget()
{
}

void KSimIoDevicePropertyGeneralWidget::acceptPressed()
{
	KSimIoDevicePropertyBaseWidget::acceptPressed();

	QString name(m_nameEdit->text().simplifyWhiteSpace());
	m_nameEdit->setText(name);  // Rewrite simplified text

	if (getDevice()->getName() != name)
	{
		changeData();
		getDevice()->setName(name);
	}

	if (getDevice()->getDescription() != m_descriptionEdit->text())
	{
		changeData();
		getDevice()->setDescription(m_descriptionEdit->text());
	}

}

void KSimIoDevicePropertyGeneralWidget::defaultPressed()
{
	KSimIoDevicePropertyBaseWidget::defaultPressed();

	m_nameEdit->setText(getDevice()->getInfo()->getI18nLibName());
//	m_descriptionEdit->setText(QString::null);
}
