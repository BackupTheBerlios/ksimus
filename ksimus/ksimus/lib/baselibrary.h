/***************************************************************************
                          baselibrary.h  -  description
                             -------------------
    begin                : Sat Sep 30 2000
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

#ifndef BASELIBRARY_H
#define BASELIBRARY_H

#include <qlist.h>
#include <qdict.h>
#include <qobject.h>

/**Base class for all libraries for info classes
  *@author Rasmus Diekenbrock
  */

class BaseInfo;
class BaseLibraryItem;
class PackageInfo;

class BaseLibrary : public QObject
{
   Q_OBJECT

public:
	bool insert(const BaseInfo * bi, const PackageInfo * packageInfo);
	bool remove(const QString & libName);
	bool remove(const BaseInfo * bi);
	
	int getIndex(const QString & libName) const;
	
	const BaseInfo * findLibName(const QString & libName) const;
	const BaseInfo * findOldLibName(const QString & oldLibName) const;
	const BaseInfo * findIndex(int idx) const;

	
	const QString & getName(const QString & libName) const;
	const QString & getShortDescr(const QString & libName) const;
	const QString & getHTMLDescr(const QString & libName) const;
	
	BaseLibraryItem * getItem(const QString & libName) const;
	const PackageInfo * getPackageInfo(const QString & libName) const;
	
	
	/** Resize QDict. Call after latest insert */
	void resize();
	
protected:
	
	BaseLibrary();
	~BaseLibrary();
	
	bool _insert_(const BaseLibraryItem * bli);
	
	QList<BaseLibraryItem> * m_library;
	QDict<BaseLibraryItem> * m_libNames;
	QDict<BaseLibraryItem> * m_oldLibNames;
	

signals:
	/** This signal is emitted, after a item is inserted */
	void signalInsert(const BaseLibraryItem *);
	/** This signal is emitted, before a item is removed */
	void signalRemove(const BaseLibraryItem *);
	/** This signal is emitted, if signal signalInsert or signalRemove is emitted */
	void signalChanged();
	
};

#endif
