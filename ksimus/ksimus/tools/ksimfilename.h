/***************************************************************************
                          ksimfilename.h  -  description
                             -------------------
    begin                : Sun Mar 24 2002
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

#ifndef KSIMFILENAME_H
#define KSIMFILENAME_H

// C-Includes

// QT-Includes
#include <qstring.h>
#include <qwidget.h>


// KDE-Includes

// Project-Includes

// Forward declaration
class Component;
class KSimData;


//#########################################################################

/**This class contains a filename.
  *@author Rasmus Diekenbrock
  */

class KSimFilename
{
public: 
	enum ePathType { PATH_ABSOLUTE          = 0x0001,
	                 PATH_RELATIVE_DOCUMENT = 0x0002,
	                 PATH_RELATIVE_MODULE   = 0x0004,
	                 PATH_RELATIVE_HOME     = 0x0008 };
	
	static const char * convertPathType(ePathType pathType);
	static ePathType convertPathType(const char * pathType, ePathType defaultPathType = PATH_RELATIVE_DOCUMENT);
	
	/** Constructs a new object with a "empty" filename.*/
	KSimFilename(Component * parent, QString filename = QString::null, ePathType pathType = PATH_RELATIVE_DOCUMENT);
	
	/** Constructs a new object with the filename.*/
	KSimFilename(const KSimFilename & filename);
	
	/** The destructor. */
	~KSimFilename();
	
	bool operator==(const KSimFilename & filename) const;
	bool operator!=(const KSimFilename & filename) const;
	const KSimFilename & operator=(const KSimFilename & filename);
	
	/** Sets a new filename. */
	void setFilename(QString newFilename);

	/** Returns the filename. */
	QString getFilename() const;
	
	/** Returns the file converted to relative path if possible.*/
	QString getRelativeFilename() const;
	
	
	/** Sets the path type. */
	void setPathType(ePathType pathType);
	
	/** Returns the path type. */
	ePathType getPathType() const;
	
	/** Returns ture, if filename is absolute. */
	bool isAbsolutePath() const;
	
	/** Returns true, if the path is valid. This means that the absoulte part of
	  * the filename is set properly. E.g. the function returns false, if the document has
	  * no name (was not saved) and relative to document is selected.
	  */
	bool isPathValid() const;
	
	/** Returns the component. */
	Component * getComponent();
	/** Returns the component. */
	Component * getComponent() const;
	
	/** Saves the current value in a @ref KSimData object. pGroup is the used group.  */
	void save(KSimData & config, const char * pGroup) const;
	/** Loads the value from a @ref KSimData object. pGroup is the used group. */
	bool load(KSimData & config, const char * pGroup, const KSimFilename* pDefault = (KSimFilename*)0 );

private:
	Component * m_component;
	QString m_filename;
	ePathType m_pathType;
};


//#########################################################################

class QLabel;
class KSimLineEdit;
class QPushButton;
class QComboBox;

/**This class contains widget for filenames.
  *@author Rasmus Diekenbrock
  */

class KSimFilenameWidget : public QWidget
{
	class KSimFilenameWidgetPrivate;
	
	Q_OBJECT
	
	
public:
	KSimFilenameWidget(KSimFilename & fileInfo, int mask, QWidget * parent, const char * name = 0);
	~KSimFilenameWidget();
	
	
	/** Sets a new filename. */
	void setFilename(QString newFilename);

	/** Returns the filename. */
	QString getFilename() const;	
	
	/** Sets the path type. */
	void setPathType(KSimFilename::ePathType pathType);
	
	/** Returns the path type. */
	KSimFilename::ePathType getPathType() const;

	
	const KSimFilename & getFileInfo() const;
	KSimFilename & getFileInfo();


	KSimLineEdit * getFilenameEdit();
	QPushButton * getFilenameDialogButton();
	QComboBox * getPathTypeBox();
	static QString getPathTypeBoxToolTip();
	static QString getPathTypeBoxWhatsThis(int mask);
	

	/** Returns the file filter for the open file dialog. */
	QString getFileFilter() const;
	/** Sets the file filter for the open file dialog. */
	void setFileFilter(const QString & filter);

private slots:
	void slotOpenFileDialog();
	void slotPathType(int idx);
	void slotFilenameChanged();
	
private:
	KSimFilenameWidgetPrivate * m_p;
};






#endif
