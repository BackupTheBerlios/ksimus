/***************************************************************************
                          ksimpackagefilewidget.cpp  -  description
                             -------------------
    begin                : Mon Oct 22 2001
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
#include <qlayout.h>

// KDE-Includes
#include <kmessagebox.h>
#include <kdialog.h>
#include <kconfig.h>
#include <klocale.h>
#include <kapp.h>
#include <kstddirs.h>

// Project-Includes
#include "ksimpackagefilewidget.h"
#include "ksimfilelistselector.h"

// Forward declaration

KSimPackageFileWidget::KSimPackageFileWidget(QWidget *parent, const char *name )
	:	PropertyWidget(parent,name)
{
	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	
	
	m_directories = new KSimDirectorySelector(this, "Package directories");
	m_directories->setTitle(i18n("Package directories:"));
	m_directories->setStartDir(":<KSimPackageFile>");
	layout->addWidget(m_directories);
	
	m_files = new KSimFileListSelector(this, "Package files");
	m_files->setTitle(i18n("Package files:"));
	m_files->setFilter(i18n("*.so|Package files (*.so)\n*|All files (*)"));
	m_files->setStartDir(":<KSimPackageFile>");
	layout->addWidget(m_files);

	// Setup data
	
	KConfig * config = kapp->config();
	
	QString group(config->group());
	
	config->setGroup("Packages");
	
	QStringList dirList;
	
	m_dirList = config->readListEntry("Directories");
	m_fileList = config->readListEntry("Files");

	config->setGroup(group);
	
	m_directories->setFileList(m_dirList);
	m_files->setFileList(m_fileList);

}

KSimPackageFileWidget::~KSimPackageFileWidget()
{
}

void KSimPackageFileWidget::slotAccept()
{
	if ( (m_dirList != m_directories->getFileList())
		|| (m_fileList != m_files->getFileList()))
	{
		KConfig * config = kapp->config();
	
		QString group(config->group());
	
		config->setGroup("Packages");
	
		config->writeEntry("Directories", m_directories->getFileList());
		config->writeEntry("Files", m_files->getFileList());

		config->setGroup(group);
		config->sync();
		
		KMessageBox::information(0,i18n("KSimus packages are configured only during the application start."
																		"You have to restart ksimus if you want to apply the changes.\n"
																		"This will be changed in a future version. Sorry!"),
															 i18n("Restart required"),
															 "Package changed - Restart Application");
	}
}

void KSimPackageFileWidget::slotDefault()
{
	KStandardDirs dirs;
	QStringList libList;

  m_files->setFileList(libList);		// Empty list!!!

	libList = dirs.findDirs("lib","ksimus");
	
 	m_directories->setFileList(libList);
}


//###################################################################################

KSimPackageFileDialog::KSimPackageFileDialog(QWidget *parent, const char * name)
	:	KDialogBase(KDialogBase::Plain,
								i18n("Package Files"),
								KDialogBase::Default | KDialogBase::Ok | KDialogBase::Cancel,
 								KDialogBase::Ok,
 								parent, name)
{
	QWidget * wid = plainPage();
			
	KSimPackageFileWidget * child = new KSimPackageFileWidget(wid, i18n("Package Files"));
			
	QBoxLayout * horLayout = new QHBoxLayout(wid);
	horLayout->setMargin(KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());
	horLayout->addWidget(child);
			
	connect(this, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));
	
}

