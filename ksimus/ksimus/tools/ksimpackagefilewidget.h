/***************************************************************************
                          ksimpackagefilewidget.h  -  description
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

#ifndef KSIMPACKAGEFILEWIDGET_H
#define KSIMPACKAGEFILEWIDGET_H

// C-Includes

// QT-Includes
#include <qstringlist.h>

// KDE-Includes
#include <kdialogbase.h>

// Project-Includes
#include "propertywidget.h"

// Forward declaration
class KSimFileListSelector;
class KSimDirectorySelector;

/**Selector for ksimus packages.
  *@author Rasmus Diekenbrock
  */

class KSimPackageFileWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	KSimPackageFileWidget(QWidget *parent=0, const char *name=0);
	~KSimPackageFileWidget();
	
	KSimFileListSelector * getFiles();
	KSimDirectorySelector * getDirectories();
	
public slots:
	/** The function slotAccept() is called, if changes are accepted.
		You have to reimplement this slot, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void slotAccept();
	/** The function slotDefault() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void slotDefault();

private:
	KSimFileListSelector * m_files;
	KSimDirectorySelector * m_directories;
	QStringList m_fileList;
	QStringList m_dirList;
	
};

//###########################################################
//###########################################################

class KSimPackageFileDialog : public KDialogBase
{
   Q_OBJECT
public:
	KSimPackageFileDialog(QWidget *parent=0, const char * name=0);
//	virtual ~KSimPackageFileDialog();

};


#endif
