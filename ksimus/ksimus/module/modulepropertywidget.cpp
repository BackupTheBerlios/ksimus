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
#include <qwhatsthis.h>
#include <qtooltip.h>
#include <qhbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qvbuttongroup.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>
#include <kapp.h>
#include <kfiledialog.h>
#include <kiconloader.h>

// Project-Includes
#include "moduledialog.h"
#include "modulepropertywidget.h"
#include "ksimdebug.h"
#include "ksimlineedit.h"


#define data (m_dialog->getData())

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
	tooltip = i18n("Enter the module name here.");
	label = new QLabel(i18n("Module Name:"), this);
	CHECK_PTR(label);
	m_leModuleName = 	new KSimLineEdit(this);
	CHECK_PTR(m_leModuleName);
	label->setBuddy(m_leModuleName);
	QToolTip::add(label,tooltip);
	QWhatsThis::add(label,tooltip);
	QToolTip::add(m_leModuleName,tooltip);
	QWhatsThis::add(m_leModuleName,tooltip);
	connect(m_leModuleName, SIGNAL(changed()), SLOT(slotModuleNameChanged()));
	
	// Library names
	tooltip = i18n("Describes the entry of the module at the component supplier.\nSeparate subdirecotries by a slash\"/\".\nSeparate library names by a semicolon \";\".");
	label = new QLabel(i18n("Library Names:"), this);
	CHECK_PTR(label);
	m_leModuleLibNames = 	new KSimLineEdit(this);
	CHECK_PTR(m_leModuleLibNames);
	label->setBuddy(m_leModuleLibNames);
	QToolTip::add(label,tooltip);
	QWhatsThis::add(label,tooltip);
	QToolTip::add(m_leModuleLibNames,tooltip);
	QWhatsThis::add(m_leModuleLibNames,tooltip);
	connect(m_leModuleLibNames, SIGNAL(changed()), SLOT(slotModuleLibNamesChanged()));

	// Short Description
	tooltip = i18n("Enter a short description of the module.");
	label = new QLabel(i18n("Short Description:"), this);
	CHECK_PTR(label);
	m_leShortDescr = 	new KSimLineEdit(this);
	CHECK_PTR(m_leShortDescr);
	label->setBuddy(m_leShortDescr);
	QToolTip::add(label,tooltip);
	QWhatsThis::add(label,tooltip);
	QToolTip::add(m_leShortDescr,tooltip);
	QWhatsThis::add(m_leShortDescr,tooltip);
	connect(m_leShortDescr, SIGNAL(changed()), SLOT(slotShortDescrChanged()));


	// Pixmap file
	tooltip = i18n("Enter the path to image file (Image view only).");
	label = new QLabel(i18n("Image &File:"), this);
	CHECK_PTR(label);
	widget = createPixmapFileWidget();
	label->setBuddy(widget);
	QToolTip::add(label,tooltip);
	QWhatsThis::add(label,tooltip);
	QToolTip::add(widget,tooltip);
	QWhatsThis::add(widget,tooltip);
	connect(m_lePixmapFile, SIGNAL(changed()), SLOT(slotPixmapFileChanged()));

	
	// User View Attribute
	tooltip = i18n("Check the box, if your module should also have a view in user interface (User view only).");
	m_userViewAttribute = new QCheckBox(i18n("User Interface"),this);
	CHECK_PTR(m_userViewAttribute);
	m_userViewAttribute->setFocusPolicy(ClickFocus);
	QToolTip::add(m_userViewAttribute,tooltip);
	QWhatsThis::add(m_userViewAttribute,tooltip);
	connect(m_userViewAttribute, SIGNAL(clicked()), SLOT(slotUserViewAttributeChanged()));

	// Store selection
	m_storeBox = new QVButtonGroup(i18n("Pixmap store location"), this);

	tooltip = i18n("Stores the absolute path of the pixmap (Image view only).");
	m_storeAbsolute = new QRadioButton(i18n("Absolute path"), m_storeBox);
	CHECK_PTR(m_storeAbsolute);
	m_storeAbsolute->setFocusPolicy(ClickFocus);
	QToolTip::add(m_storeAbsolute,tooltip);
	QWhatsThis::add(m_storeAbsolute,tooltip);
	connect(m_storeAbsolute, SIGNAL(clicked()), SLOT(slotStoreAbsoluteChanged()));

	tooltip = i18n("Stores the path of the pixmap relative to module file (Image view only).");
	m_storeRelative = new QRadioButton(i18n("Relative path"), m_storeBox);
	CHECK_PTR(m_storeRelative);
	m_storeRelative->setFocusPolicy(ClickFocus);
	QToolTip::add(m_storeRelative,tooltip);
	QWhatsThis::add(m_storeRelative,tooltip);
	connect(m_storeRelative, SIGNAL(clicked()), SLOT(slotStoreRelativeChanged()));

	tooltip = i18n("Stores the pixmap into the module file (Image view only).");
	m_storeInternal = new QRadioButton(i18n("Internal"), m_storeBox);
	CHECK_PTR(m_storeInternal);
	m_storeInternal->setFocusPolicy(ClickFocus);
	QToolTip::add(m_storeInternal,tooltip);
	QWhatsThis::add(m_storeInternal,tooltip);
	connect(m_storeInternal, SIGNAL(clicked()), SLOT(slotStoreInternalChanged()));
}

ModulePropertyWidget::~ModulePropertyWidget()
{
}
	
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
	setModuleName(data->getModuleName());	
	setModuleLibNames(data->getModuleLibNames());
	setShortDescr(data->getShortDescr());
	setPixmapFile(data->getPixmapFile());
	data->loadPixmap(getPixmalFile());

	if ((data->getUserViewAttrib() == VA_SHEET_AND_USER) && (data->getModuleView() == MV_USERVIEW))
	{
		m_userViewAttribute->setChecked(true);
	}
	else
	{
		m_userViewAttribute->setChecked(false);
	}
		
	m_leModuleName->setEnabled(data->getModuleView() != MV_NONE);
	m_leModuleLibNames->setEnabled(data->getModuleView() != MV_NONE);
	m_leShortDescr->setEnabled(data->getModuleView() != MV_NONE);
	m_fileBox->setEnabled(data->getModuleView() == MV_PIXMAP);
	m_userViewAttribute->setEnabled(data->getModuleView() == MV_USERVIEW);
	
	switch(data->getPixmapStore())
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
			KSIMDEBUG_VAR("Unknown store type",data->getPixmapStore());
	}
	
	m_storeBox->setEnabled(data->getModuleView() == MV_PIXMAP);
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
	data->setModuleName(getModuleName());
	emit changed();
}

void ModulePropertyWidget::slotModuleLibNamesChanged()
{
	m_dialog->modifyProperties(i18n("Change Module Library Names"));
	data->setModuleLibNames(getModuleLibNames());
	emit changed();
}

void ModulePropertyWidget::slotShortDescrChanged()
{
	m_dialog->modifyProperties(i18n("Change Short Description"));
	data->setShortDescr(getShortDescr());
	emit changed();
}

void ModulePropertyWidget::slotPixmapFileChanged()
{
	m_dialog->modifyProperties(i18n("Change Image File"));
//	data->loadPixmap(getPixmalFile());
	data->setPixmapFile(getPixmalFile());
	emit changed();
}

QWidget * ModulePropertyWidget::createPixmapFileWidget()
{
	m_fileBox = new QHBox(this);
	CHECK_PTR(m_fileBox);
//	m_fileBox->setMargin(KDialog::marginHint());
//	m_fileBox->setSpacing(KDialog::spacingHint());

	m_lePixmapFile = new KSimLineEdit(m_fileBox);
	CHECK_PTR(m_lePixmapFile);

	QPushButton * openFileDia = new QPushButton(m_fileBox);
	CHECK_PTR(openFileDia);
	QPixmap pix;
	pix = KGlobal::iconLoader()->loadIcon(QString::fromLatin1("fileopen.png"),KIcon::Toolbar);
	openFileDia->setPixmap(pix);
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

void ModulePropertyWidget::slotUserViewAttributeChanged()
{
	m_dialog->modifyProperties(i18n("Change User View Attribute"));
	if(m_userViewAttribute->isChecked())
	{
		data->setUserViewAttrib(VA_SHEET_AND_USER);
	}
	else
	{
		data->setUserViewAttrib(VA_SHEETVIEW);
	}
	
	emit changed();
	
}

void ModulePropertyWidget::slotStoreAbsoluteChanged()
{
	if (data->getPixmapStore() != MPS_ABSOLTUE)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Absolute"));
		data->setPixmapStore(MPS_ABSOLTUE);
		emit changed();
	}
}

void ModulePropertyWidget::slotStoreRelativeChanged()
{
	if (data->getPixmapStore() != MPS_RELATIVE_MODULE)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Relative"));
		data->setPixmapStore(MPS_RELATIVE_MODULE);
		emit changed();
	}
}

void ModulePropertyWidget::slotStoreInternalChanged()
{
	if (data->getPixmapStore() != MPS_INTERNAL)
	{
		m_dialog->modifyProperties(i18n("Change Store Location to Internal"));
		data->setPixmapStore(MPS_INTERNAL);
		emit changed();
	}
}

