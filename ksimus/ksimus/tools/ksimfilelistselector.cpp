/***************************************************************************
                          ksimfilelistselector.cpp  -  description
                             -------------------
    begin                : Sat Oct 20 2001
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

#include <qstring.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qgroupbox.h>

#include <kmimetype.h>
#include <kfiledialog.h>
#include <klocale.h>

#include "ksimfilelistselector.h"

KSimFileListSelector::KSimFileListSelector(QWidget *parent, const char *name )
	: KSimFileListSelectorUI(parent,name),
		m_filter(),
		m_mode(KFile::File),
		m_fileDialogCaption(i18n("Select File")),
		m_startDir(QString::fromLatin1(":<File>"))
{
	fileList->clear();
	
	connect(addButton, SIGNAL(clicked()), this, SLOT(slotAdd()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(slotRemove()));
	connect(fileOpenButton, SIGNAL(clicked()), this, SLOT(slotOpenFile()));
}


KSimFileListSelector::~KSimFileListSelector()
{
}

void KSimFileListSelector::slotAdd()
{
	if (!fileName->text().isEmpty())
	{
		bool found = false;
		unsigned int i;
		for (i=0; i < fileList->count(); i++)
		{
			if (fileList->text(i) == fileName->text())
			{
				found = true;
				break;
			}
		}
		
		if (!found)
		{
			fileList->insertItem(fileName->text());
			fileName->setText(QString::fromLatin1(""));	
		}
	}

}

void KSimFileListSelector::slotRemove()
{
	if (fileList->currentItem() != -1)
	{
		fileList->removeItem(fileList->currentItem());
	}	
}

void KSimFileListSelector::slotOpenFile()
{
	KFileDialog * fileDialog = new KFileDialog(getStartDir(), getFilter(), this, 0, true);
	
  fileDialog->setCaption(i18n("Select library"));
	fileDialog->setMode(getMode());
	fileDialog->exec();
	
	QString file = fileDialog->selectedFile();
	
	if (!file.isEmpty())
	{
		fileName->setText(file);
	}	
	
	delete fileDialog;
}

void KSimFileListSelector::setFilter(const QString & filter)
{
	m_filter = filter;
}

QString KSimFileListSelector::getFilter() const
{
	return m_filter;
}
	
void KSimFileListSelector::setMode(KFile::Mode mode)
{
	m_mode = mode;
}

KFile::Mode KSimFileListSelector::getMode() const
{
	return m_mode;
}

void KSimFileListSelector::setFileList(const QStringList & list)
{
	fileList->clear();
	
	fileList->insertStringList(list);
}

QStringList KSimFileListSelector::getFileList() const
{
	QStringList list;
	unsigned int u;
	
	for (u = 0; u < fileList->count(); u++)
	{
		list.append(fileList->text(u));
	}
	
	return list;
}

void KSimFileListSelector::setTitle(const QString & title)
{
	frame->setTitle(title);
}
	
void KSimFileListSelector::setFileDialogCaption(const QString & fileDialogCaption)
{
	m_fileDialogCaption = fileDialogCaption;
}


void KSimFileListSelector::setStartDir(const QString & startDir)
{
	m_startDir = startDir;
}

QString KSimFileListSelector::getStartDir() const
{
	return m_startDir;
}

//#####################################################################################
//#####################################################################################

KSimDirectorySelector::KSimDirectorySelector(QWidget *parent, const char *name)
	:	KSimFileListSelector(parent, name)	
{
	setMode(KFile::Directory);
	setFileDialogCaption(i18n("Select Directory"));
}

//#####################################################################################
//#####################################################################################

KSimPackageSelector::KSimPackageSelector(QWidget *parent, const char *name)
	:	QVBox(parent,name)
{
	m_directories = new KSimDirectorySelector(this, "Package directories");
	m_directories->setTitle(i18n("Package directoires:"));
	m_directories->setStartDir(QString::fromLatin1(":<KSimPackageFile>"));
	
	m_files = new KSimFileListSelector(this, "Package files");
	m_files->setTitle(i18n("Package files:"));
	m_files->setFilter(i18n("*.so|Package files (*.so)\n*|All files (*)"));
	m_files->setStartDir(QString::fromLatin1(":<KSimPackageFile>"));
}

KSimPackageSelector::~KSimPackageSelector()
{
}	

KSimFileListSelector * KSimPackageSelector::getFiles()
{
	return m_files;
}

KSimDirectorySelector * KSimPackageSelector::getDirectories()
{
	return m_directories;
}

	