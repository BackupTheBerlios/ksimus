/***************************************************************************
                          modulepropertywidget.cpp  -  description
                             -------------------
    begin                : Sun Apr 22 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qhbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qvbuttongroup.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>
#include <kapplication.h>
#include <kfiledialog.h>

// Project-Includes
#include "moduledialog.h"
#include "modulepropertywidget.h"
#include "ksimdebug.h"
#include "ksimlineedit.h"
#include "ksimicon.h"
#include "propertywidget.h"


//#define data (m_dialog->getData())

// Forward declaration



ModulePropertyWidget::ModulePropertyWidget(ModuleDialog * dialog, QWidget * parent, const char * name)
	:	QGroupBox(2,Horizontal,i18n("Properties"), parent, name),
		m_dialog(dialog)
{
//	setMargin(KDialog::marginHint());
//	setSpacing(KDialog::spacingHint());
	QWidget * widget;
	QLabel * label;
	QString tooltip;

	// Module name
	label = new QLabel(i18n("Module Name:"), this);
	Q_CHECK_PTR(label);
	m_leModuleName = 	new KSimLineEdit(this);
	Q_CHECK_PTR(m_leModuleName);
	label->setBuddy(m_leModuleName);
	tooltip = i18n("Enter the module name here.");
	PropertyWidget::addToolTip(tooltip, label, m_leModuleName);
	PropertyWidget::addWhatsThis(tooltip, label, m_leModuleName);
	connect(m_leModuleName, SIGNAL(changed()), SLOT(slotModuleNameChanged()));
	
	// Library names
	label = new QLabel(i18n("Library Names:"), this);
	Q_CHECK_PTR(label);
	m_leModuleLibNames = 	new KSimLineEdit(this);
	Q_CHECK_PTR(m_leModuleLibNames);
	label->setBuddy(m_leModuleLibNames);
	tooltip = i18n("Describes the entry of the module at the component supplier.\nSeparate subdirecotries by a slash\"/\".\nSeparate library names by a semicolon \";\".");
	PropertyWidget::addToolTip(tooltip, label, m_leModuleLibNames);
	PropertyWidget::addWhatsThis(tooltip, label, m_leModuleLibNames);
	connect(m_leModuleLibNames, SIGNAL(changed()), SLOT(slotModuleLibNamesChanged()));

	// Short Description
	label = new QLabel(i18n("Short Description:"), this);
	Q_CHECK_PTR(label);
	m_leShortDescr = 	new KSimLineEdit(this);
	Q_CHECK_PTR(m_leShortDescr);
	label->setBuddy(m_leShortDescr);
	tooltip = i18n("Enter a short description of the module.");
	PropertyWidget::addToolTip(tooltip, label, m_leShortDescr);
	PropertyWidget::addWhatsThis(tooltip, label, m_leShortDescr);
	connect(m_leShortDescr, SIGNAL(changed()), SLOT(slotShortDescrChanged()));


	// Pixmap file
	label = new QLabel(i18n("Image &File:"), this);
	Q_CHECK_PTR(label);
	widget = createPixmapFileWidget();
	label->setBuddy(widget);
	tooltip = i18n("Enter the path to image file (Image view only).");
	PropertyWidget::addToolTip(tooltip, label, widget);
	PropertyWidget::addWhatsThis(tooltip, label, widget);
	connect(m_lePixmapFile, SIGNAL(changed()), SLOT(slotPixmapFileChanged()));


	new QLabel(this);  // empty 1st col

	// Store selection
	m_storeBox = new QVButtonGroup(i18n("Pixmap store location"), this);

	m_storeAbsolute = new QRadioButton(i18n("Absolute path"), m_storeBox);
	Q_CHECK_PTR(m_storeAbsolute);
	m_storeAbsolute->setFocusPolicy(ClickFocus);
	tooltip = i18n("Stores the absolute path of the pixmap (Image view only).");
	PropertyWidget::addToolTip(tooltip, m_storeAbsolute);
	PropertyWidget::addWhatsThis(tooltip, m_storeAbsolute);
	connect(m_storeAbsolute, SIGNAL(clicked()), SLOT(slotStoreAbsoluteChanged()));

	m_storeRelative = new QRadioButton(i18n("Relative path"), m_storeBox);
	Q_CHECK_PTR(m_storeRelative);
	m_storeRelative->setFocusPolicy(ClickFocus);
	tooltip = i18n("Stores the path of the pixmap relative to module file (Image view only).");
	PropertyWidget::addToolTip(tooltip, m_storeRelative);
	PropertyWidget::addWhatsThis(tooltip, m_storeRelative);
	connect(m_storeRelative, SIGNAL(clicked()), SLOT(slotStoreRelativeChanged()));

	m_storeInternal = new QRadioButton(i18n("Internal"), m_storeBox);
	Q_CHECK_PTR(m_storeInternal);
	m_storeInternal->setFocusPolicy(ClickFocus);
	tooltip = i18n("Stores the pixmap into the module file (Image view only).");
	PropertyWidget::addToolTip(tooltip, m_storeInternal);
	PropertyWidget::addWhatsThis(tooltip, m_storeInternal);
	connect(m_storeInternal, SIGNAL(clicked()), SLOT(slotStoreInternalChanged()));
}

ModulePropertyWidget::~ModulePropertyWidget()
{
}
	
ModuleData * ModulePropertyWidget::getData() const { return m_dialog->getData(); };


QString ModulePropertyWidget::getModuleName() const
{
	return m_leModuleName->text();
}
QString ModulePropertyWidget::getModuleLibNames() const
{
	return m_leModuleLibNames->text();
}
QString ModulePropertyWidget::getShortDescr() const
{
	return m_leShortDescr->text();
}
QString ModulePropertyWidget::getPixmalFile() const
{
	return m_lePixmapFile->text();
}
	
	
void ModulePropertyWidget::setup()
{
	setModuleName(getData()->getModuleName());	
	setModuleLibNames(getData()->getModuleLibNames());
	setShortDescr(getData()->getShortDescr());
	setPixmapFile(getData()->getPixmapFile());
	getData()->loadPixmap(getPixmalFile());

	m_leModuleName->setEnabled(getData()->getModuleView() != MV_NONE);
	m_leModuleLibNames->setEnabled(getData()->getModuleView() != MV_NONE);
	m_leShortDescr->setEnabled(getData()->getModuleView() != MV_NONE);
	m_fileBox->setEnabled(getData()->getModuleView() == MV_PIXMAP);
	
	switch(getData()->getPixmapStore())
	{
		case MPS_ABSOLTUE:
			m_storeAbsolute->setChecked(true);
			break;
			
		case MPS_RELATIVE_MODULE:
			m_storeRelative->setChecked(true);
			break;
			
		case MPS_INTERNAL:
			m_storeInternal->setChecked(true);
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown store type", getData()->getPixmapStore());
	}
	
	m_storeBox->setEnabled(getData()->getModuleView() == MV_PIXMAP);
}

void ModulePropertyWidget::setModuleName(const QString & name)
{
	if (m_leModuleName->text() != name)
		m_leModuleName->setText(name);	
}

void ModulePropertyWidget::setModuleLibNames(const QString & name)
{
	if (m_leModuleLibNames->text() != name)
		m_leModuleLibNames->setText(name);	
}

void ModulePropertyWidget::setShortDescr(const QString & name)
{
	if (m_leShortDescr->text() != name)
		m_leShortDescr->setText(name);	
}

void ModulePropertyWidget::setPixmapFile(const QString & name)
{
	if (m_lePixmapFile->text() != name)
		m_lePixmapFile->setText(name);	
}

void ModulePropertyWidget::slotModuleNameChanged()
{
	m_dialog->modifyProperties(i18n("Change Module Name"));
	getData()->setModuleName(getModuleName());
	emit changed();
}

void ModulePropertyWidget::slotModuleLibNamesChanged()
{
	m_dialog->modifyProperties(i18n("Change Module Library Names"));
	getData()->setModuleLibNames(getModuleLibNames());
	emit changed();
}

void ModulePropertyWidget::slotShortDescrChanged()
{
	m_dialog->modifyProperties(i18n("Change Short Description"));
	getData()->setShortDescr(getShortDescr());
	emit changed();
}

void ModulePropertyWidget::slotPixmapFileChanged()
{
	m_dialog->modifyProperties(i18n("Change Image File"));
//	getData()->loadPixmap(getPixmalFile());
	getData()->setPixmapFile(getPixmalFile());
	emit changed();
}

QWidget * ModulePropertyWidget::createPixmapFileWidget()
{
	m_fileBox = new QHBox(this);
	Q_CHECK_PTR(m_fileBox);
//	m_fileBox->setMargin(KDialog::marginHint());
//	m_fileBox->setSpacing(KDialog::spacingHint());

	m_lePixmapFile = new KSimLineEdit(m_fileBox);
	Q_CHECK_PTR(m_lePixmapFile);

	QPushButton * openFileDia = new QPushButton(m_fileBox);
	Q_CHECK_PTR(openFileDia);
	openFileDia->setPixmap(KSimIcon::load("fileopen", KIcon::Small));
	connect(openFileDia, SIGNAL(released()), SLOT(slotPixmapOpenFileDialog()));

	return m_fileBox;
}

void ModulePropertyWidget::slotPixmapOpenFileDialog()
{
	QString file;
	
	file=KFileDialog::getOpenFileName(QDir::currentDirPath(),
	                                    i18n("*|All files"),
	                                    this, i18n("Load Image File..."));
	
	if(!file.isEmpty())
	{
		setPixmapFile(file);
		slotPixmapFileChanged();
	}
}

void ModulePropertyWidget::slotStoreAbsoluteChanged()
{
	if (getData()->getPixmapStore() != MPS_ABSOLTUE)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Absolute"));
		getData()->setPixmapStore(MPS_ABSOLTUE);
		emit changed();
	}
}

void ModulePropertyWidget::slotStoreRelativeChanged()
{
	if (getData()->getPixmapStore() != MPS_RELATIVE_MODULE)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Relative"));
		getData()->setPixmapStore(MPS_RELATIVE_MODULE);
		emit changed();
	}
}

void ModulePropertyWidget::slotStoreInternalChanged()
{
	if (getData()->getPixmapStore() != MPS_INTERNAL)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Internal"));
		getData()->setPixmapStore(MPS_INTERNAL);
		emit changed();
	}
}

#include "modulepropertywidget.moc"
