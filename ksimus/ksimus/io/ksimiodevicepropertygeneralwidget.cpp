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
#include <qstring.h>

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
	Q_CHECK_PTR(label);
	
	m_nameEdit = new QLineEdit(this,"m_nameEdit");
	Q_CHECK_PTR(m_nameEdit);
	m_nameEdit->setText(getDevice()->getName());

	label->setBuddy(m_nameEdit);
	addToolTip(str, m_nameEdit, label);
	addWhatsThis(str, m_nameEdit, label);

	// Device type
	str = i18n("Shows the IO device type.");
	
	label = new QLabel(i18n("Type:"), this);
	Q_CHECK_PTR(label);
	
	QLabel * label1 = new QLabel(getDevice()->getInfo()->getName(), this);
	Q_CHECK_PTR(label1);
	
	label->setBuddy(label1);
	addToolTip(str, label, label1);
	addWhatsThis(str, label, label1);

	// Device description  
	QVBox * descBox = newRowVBox("descBox");
	str = i18n("Enter a description for the IO device here.");

	label = new QLabel(i18n("Description:"), descBox);
	Q_CHECK_PTR(label);

	m_descriptionEdit = new QMultiLineEdit(descBox, "m_descriptionEdit");
	Q_CHECK_PTR(label);
	m_descriptionEdit->setText(getDevice()->getDescription());
	m_descriptionEdit->setMaxLines(-1);

	label->setBuddy(m_descriptionEdit);
	addToolTip(str, m_descriptionEdit, label);
	addWhatsThis(str, m_descriptionEdit, label);
	
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

#include "ksimiodevicepropertygeneralwidget.moc"
