/***************************************************************************
                          ksimmodulefilewidget.h  -  description
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

#ifndef KSIMMODULEFILEWIDGET_H
#define KSIMMODULEFILEWIDGET_H

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

/**Selector for ksimus modules.
  *@author Rasmus Diekenbrock
  */

class KSimModuleFileWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	KSimModuleFileWidget(QWidget *parent=0, const char *name=0);
	~KSimModuleFileWidget();
	
	KSimFileListSelector * getFiles();
	KSimDirectorySelector * getDirectories();
	
	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();


private:
	KSimFileListSelector * m_files;
	KSimDirectorySelector * m_directories;
	QStringList m_fileList;
	QStringList m_dirList;
	
};

//###########################################################
//###########################################################

class KSimModuleFileDialog : public KDialogBase
{
   Q_OBJECT
public:
	KSimModuleFileDialog(QWidget *parent=0, const char * name=0);
//	virtual ~KSimModuleFileDialog();

};


#endif
