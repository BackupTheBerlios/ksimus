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
#include <kapplication.h>
#include <kstandarddirs.h>

// Project-Includes
#include "ksimpackagefilewidget.h"
#include "ksimfilelistselector.h"

// Forward declaration

KSimPackageFileWidget::KSimPackageFileWidget(QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name)
{
	setMargin(0);
	
	m_directories = new KSimDirectorySelector(this, "Package directories");
	m_directories->setTitle(i18n("Package directories:"));
	m_directories->setStartDir(QString::fromLatin1(":<KSimPackageFile>"));
	
	m_files = new KSimFileListSelector(this, "Package files");
	m_files->setTitle(i18n("Package files:"));
	m_files->setFilter(i18n("*.so|Package files (*.so)\n*|All files (*)"));
	m_files->setStartDir(QString::fromLatin1(":<KSimPackageFile>"));

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

void KSimPackageFileWidget::acceptPressed()
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
		
		KMessageBox::information(parentWidget() ,
		                         i18n("KSimus packages are configured only during the application start."
		                              "You have to restart ksimus if you want to apply the changes.\n"
		                              "This will be changed in a future version. Sorry!"),
		                         i18n("Restart required"),
		                         QString::fromLatin1("Package changed - Restart Application"));
	}
}

void KSimPackageFileWidget::defaultPressed()
{
	KStandardDirs dirs;
	QStringList libList;

	m_files->setFileList(libList);   // Empty list!!!

	libList = dirs.findDirs(QString::fromLatin1("lib"),QString::fromLatin1("ksimus"));
	
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
/*	horLayout->setMargin(KDialog::marginHint());
	horLayout->setSpacing(KDialog::spacingHint());*/
	horLayout->addWidget(child);
			
	connect(this, SIGNAL(okClicked()), child, SLOT(slotAccept()));
	connect(this, SIGNAL(defaultClicked()), child, SLOT(slotDefault()));
	connect(this, SIGNAL(cancelClicked()), child, SLOT(slotCancel()));
	
}

#include "ksimpackagefilewidget.moc"
