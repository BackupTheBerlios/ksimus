/***************************************************************************
                          ksimiodevicepropertypinwidget.cpp  -  description
                             -------------------
    begin                : Fri Jun 6 2003
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
#include <qcombobox.h>
#include <qstring.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimiodevicepropertypinwidget.h"
#include "ksimiojoininfo.h"

// Forward declaration



KSimIoDevicePropertyPinWidget::KSimIoDevicePropertyPinWidget(KSimIoPin * pin, QWidget *parent, const char *name)
	:	KSimIoDevicePropertyBaseWidget(pin->getDevice(), 2, parent, name),
		m_pin(pin)
{
//	setCenterRowStretch(1);
//	setBottomRowStretch(0);
//	setColStretch(0,0);
	setColStretch(1,1);

	QString str;
	QLabel * label;

	// Name edit
	str = i18n("Change the name of the IO pin here.");
	label = new QLabel(i18n("Name:"), this,"LineEditLabel");
	CHECK_PTR(label);

	m_nameEdit = new QLineEdit(this,"m_nameEdit");
	CHECK_PTR(m_nameEdit);
	m_nameEdit->setText(getPin()->getName());

	addToolTip(str, m_nameEdit, label);
	addWhatsThis(str, m_nameEdit, label);

	// Device type
	str = i18n("Shows the IO pin type.");

	label = new QLabel(i18n("Type:"), this);
	CHECK_PTR(label);

/*	QLabel * label1 = new QLabel(getPin()->getJoinInfoList().getFirst()->getI18nLibName(), this);
	CHECK_PTR(label1);

	addToolTip(str, label, label1);
	addWhatsThis(str, label, label1);*/


	m_typeSelector = new QComboBox(false, this, "m_typeSelector");
	CHECK_PTR(m_typeSelector);
	int currentIndex = 0;
	FOR_EACH_IO_JOIN_INFO(it, getPin()->getJoinInfoList())
	{
		m_typeSelector->insertItem(it.current()->getI18nLibName());
		if (it.current() == getPin()->getSelectedJoinInfo())
		{
			m_typeSelector->setCurrentItem(currentIndex);
		}
		currentIndex++;
	}
	addToolTip(str, label, m_typeSelector);
	addWhatsThis(str, label, m_typeSelector);
}


KSimIoDevicePropertyPinWidget::~KSimIoDevicePropertyPinWidget()
{
}

void KSimIoDevicePropertyPinWidget::acceptPressed()
{
	KSimIoDevicePropertyBaseWidget::acceptPressed();

	QString name(m_nameEdit->text().simplifyWhiteSpace());
	m_nameEdit->setText(name);  // Rewrite simplified text

	if (getPin()->getName() != name)
	{
		changeData();
		getPin()->setName(name);
	}

	if (m_typeSelector->currentText() != getPin()->getSelectedJoinInfo()->getI18nLibName())
	{
		changeData();
		FOR_EACH_IO_JOIN_INFO(it, getPin()->getJoinInfoList())
		{
			if (it.current()->getI18nLibName() == m_typeSelector->currentText())
			{
				getPin()->setSelectedJoinInfo(it.current());
				break;
			}
		}
	}
}

void KSimIoDevicePropertyPinWidget::defaultPressed()
{
	KSimIoDevicePropertyBaseWidget::defaultPressed();

	m_nameEdit->setText(getPin()->getDefaultI18nName());
//	m_descriptionEdit->setText(QString::null);
	m_typeSelector->setCurrentItem(0);
}

#include "ksimiodevicepropertypinwidget.moc"
