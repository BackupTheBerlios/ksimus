/***************************************************************************
                          library.h  -  description
                             -------------------
    begin                : Tue Dec 26 2000
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

#ifndef LIBRARY_H
#define LIBRARY_H

class ComponentLibrary;
class ConnectorLibrary;
class WirePropertyLibrary;
class ImplicitConverterLibrary;
class QStringList;
class PackageInfo;

/**Contains all Component Info, Wire Info, ...
  *@author Rasmus Diekenbrock
  */

class Library
{
class Private;

public: 
	Library();
	~Library();
	
	const ComponentLibrary * getComponentLib() const;
	const ConnectorLibrary * getConnectorLib() const;
	const WirePropertyLibrary * getWirePropertyLib() const;
	const ImplicitConverterLibrary * getImplicitConverterLib() const;
	const QStringList & getInfoMessages() const;
	const QStringList & getErrorMessages() const;
	void clearMessageLists();
	
protected:
	void insertPackage(const PackageInfo * packageInfo);
	void loadPackageFiles();
	void addPackageDirs();
	void addPackageFiles();
	void makeHandle(const QString & filename);
	void makeHandle(const QStringList & filenames);
	void scanPackageDir(const QString & dirname);

	void addModuleDirs();
	void scanModuleDir(const QString & dirname);
	void addModuleFiles();
	void loadModule(const QString & filename);


private:
	Private * m_p;
	ComponentLibrary * m_componentLibrary;
	ConnectorLibrary * m_connectorLibrary;
	WirePropertyLibrary * m_wirePropertyLibrary;
	ImplicitConverterLibrary * m_implicitConverterLibrary;
};

extern Library * g_library;


#endif
