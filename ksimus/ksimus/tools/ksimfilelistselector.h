/***************************************************************************
                          ksimfilelistselector.h  -  description
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

#ifndef KSIMFILELISTSELECTOR_H
#define KSIMFILELISTSELECTOR_H

#include <qwidget.h>
#include <qstringlist.h>
#include <qvbox.h>

#include <kfile.h>

#include <ksimfilelistselectorui.h>

/**A selector widget for files or dircetories
  *@author Rasmus Diekenbrock
  */

class KSimFileListSelector : public KSimFileListSelectorUI
{
   Q_OBJECT
public: 
	KSimFileListSelector(QWidget *parent=0, const char *name=0);
	~KSimFileListSelector();

	void setFilter(const QString & filter);
	QString getFilter() const;
	
	void setMode(KFile::Mode mode);
	KFile::Mode getMode() const;
	
	void setTitle(const QString & title);
	
	void setFileDialogCaption(const QString & fileDialogCaption);
	
	void setFileList(const QStringList & list);
	QStringList getFileList() const;

	void setStartDir(const QString & startDir);
	QString getStartDir() const;

protected:

private slots:

	void slotAdd();
	void slotRemove();
	void slotOpenFile();

private:
	QString m_filter;	
	KFile::Mode m_mode;
	QString m_fileDialogCaption;
	QString m_path;
	QString m_startDir;
	

};

//#####################################################################################
//#####################################################################################

class KSimDirectorySelector : public KSimFileListSelector
{
   Q_OBJECT
public:
	KSimDirectorySelector(QWidget *parent=0, const char *name=0);
//	~KSimDirectorySelector();

};


//#####################################################################################
//#####################################################################################

class KSimPackageSelector : public QVBox
{
   Q_OBJECT
public:
	KSimPackageSelector(QWidget *parent=0, const char *name=0);
	~KSimPackageSelector();

	KSimFileListSelector * getFiles();
	KSimDirectorySelector * getDirectories();
	
	

private:
	KSimFileListSelector * m_files;
	KSimDirectorySelector * m_directories;
	

};




#endif
