/***************************************************************************
                          ksimmodulefilewidget.cpp  -  description
                             -------------------
    begin                : Fri Dec 27 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include <kmessagebox.h>
#include <kdialog.h>
#include <kconfig.h>
#include <klocale.h>
#include <kapp.h>
#include <kstddirs.h>

// Project-Includes
#include "ksimmodulefilewidget.h"
#include "ksimfilelistselector.h"

// Forward declaration

KSimModuleFileWidget::KSimModuleFileWidget(QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name)
{
	setMargin(0);
	
	m_directories = new KSimDirectorySelector(this, "Module directories");
	m_directories->setTitle(i18n("Module directories:"));
	m_directories->setStartDir(QString::fromLatin1(":<KSimModuleFile>"));
	
	m_files = new KSimFileListSelector(this, "Module files");
	m_files->setTitle(i18n("Module files:"));
	m_files->setFilter(i18n("*.sim|Module files (*.sim)\n*|All files (*)"));
	m_files->setStartDir(QString::fromLatin1(":<KSimModuleFile>"));

	// Setup data
	
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Modules");
	
	QStringList dirList;
	
	m_dirList = config->readListEntry("Directories");
	m_fileList = config->readListEntry("Files");

	config->setGroup(group);
	
	m_directories->setFileList(m_dirList);
	m_files->setFileList(m_fileList);

}

KSimModuleFileWidget::~KSimModuleFileWidget()
{
}

void KSimModuleFileWidget::acceptPressed()
{
	if ( (m_dirList != m_directories->getFileList())
		|| (m_fileList != m_files->getFileList()))
	{
		KConfig * config = kapp->config();
	
		QString group(config->group());
	
		config->setGroup("Modules");
	
		config->writeEntry("Directories", m_directories->getFileList());
		config->writeEntry("Files", m_files->getFileList());

		config->setGroup(group);
		config->sync();
		
		KMessageBox::information(parentWidget(),
		                         i18n("KSimus modules are loaded only during the application start."
		                              "You have to restart ksimus if you want to apply the changes.\n"
		                              "This will be changed in a future version. Sorry!"),
		                         i18n("Restart required"),
		                         QString::fromLatin1("Modules changed - Restart Application"));
	}
}

void KSimModuleFileWidget::defaultPressed()
{
	KStandardDirs dirs;
	QStringList libList;

	m_files->setFileList(libList);   // Empty list!!!

//	libList = dirs.findDirs(QString::fromLatin1("lib"),QString::fromLatin1("ksimus"));
	
	m_directories->setFileList(libList);
}


//###################################################################################

KSimModuleFileDialog::KSimModuleFileDialog(QWidget *parent, const char * name)
	:	KDialogBase(KDialogBase::Plain,
		            i18n("Module Files"),
		            KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
		            KDialogBase::Ok,
		            parent, name)
{
	QWidget * wid = plainPage();
			
	KSimModuleFileWidget * child = new KSimModuleFileWidget(wid, i18n("Module Files"));
			
	QBoxLayout * horLayout = new QHBoxLayout(wid);
/*	horLayout->setMargin(KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());*/
	horLayout->addWidget(child);
			
	connect(this, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));
	
}

#include "ksimmodulefilewidget.moc"
