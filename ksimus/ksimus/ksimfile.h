/***************************************************************************
                          ksimfile.h  -  description
                             -------------------
    begin                : Thu May 18 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

#ifndef KSIMFILE_H
#define KSIMFILE_H

#include <qdir.h>
#include <qfile.h>

/**Manages files and directories 
  *@author Rasmus Diekenbrock
  */

class KSimFile {
public: 
	KSimFile();
	~KSimFile();
	
	bool setupDir();
	
	const QDir & getTmpDir() const { return tmpDir; };
	const QDir & getSessionDir()const { return sessionDir; };
	/** Returns the name of copy past file. */
	QString getCopyPastFilename() const;
	/** Returns true if copy past file exists. */
	bool existCopyPastFile();
	
	const QString & getErrorText() const;

	/** Returns a relative file name (including path) form relPath to filename.
			filename contains the file name and the absolute path to the target.
			relPath contains the absolute path to the source location. */	
	static QString relativePath (const QString & filename, const QString & relPath);

		
protected: // Protected attributes
	bool setTmpDir();
	bool setSessionDir();
	void removeDir(QDir & dir);

	QDir tmpDir;
	QDir sessionDir;
	QString errorText;
	bool setupOk;

};

#endif
