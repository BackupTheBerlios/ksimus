/***************************************************************************
                          modulepropertywidget.h  -  description
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

#ifndef MODULEPROPERTYWIDGET_H
#define MODULEPROPERTYWIDGET_H

// C-Includes

// QT-Includes
#include <qgroupbox.h>

// KDE-Includes

// Project-Includes

// Forward declaration
class QString;
class QHBox;
class QCheckBox;
class KSimLineEdit;
class ModuleDialog;
class QVButtonGroup;
class QRadioButton;

/**Property widget for modules
  *@author Rasmus Diekenbrock
  */






class ModulePropertyWidget : public QGroupBox
{
	Q_OBJECT
public: 
	ModulePropertyWidget(ModuleDialog * dialog, QWidget * parent, const char * name = 0);
	~ModulePropertyWidget();
	
	QString getModuleName() const;
	QString getModuleLibNames() const;
	QString getShortDescr() const;
	QString getPixmalFile() const;
	
	
public slots:
	void setup();
	void setModuleName(const QString & name);
	void setModuleLibNames(const QString & name);
	void setShortDescr(const QString & name);
	void setPixmapFile(const QString & file);


private:
	QWidget * createPixmapFileWidget();
	
	ModuleDialog * m_dialog;
	KSimLineEdit * m_leModuleName;
	KSimLineEdit * m_leModuleLibNames;
	KSimLineEdit * m_leShortDescr;
	KSimLineEdit * m_lePixmapFile;
	QHBox * m_fileBox;
	QCheckBox * m_userViewAttribute;
	QVButtonGroup * m_storeBox;
	QRadioButton * m_storeAbsolute;
	QRadioButton * m_storeRelative;
	QRadioButton * m_storeInternal;

private slots:
	void slotModuleNameChanged();
	void slotModuleLibNamesChanged();
	void slotShortDescrChanged();
	void slotPixmapFileChanged();
	void slotPixmapOpenFileDialog();
	void slotUserViewAttributeChanged();
	void slotStoreAbsoluteChanged();
	void slotStoreRelativeChanged();
	void slotStoreInternalChanged();

signals:
	void changed();
};

#endif
