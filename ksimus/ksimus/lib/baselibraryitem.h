/***************************************************************************
                          baselibraryitem.h  -  description
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

#ifndef BASELIBRARYITEM_H
#define BASELIBRARYITEM_H


class BaseInfo;
class QStringList;
class QString;
class PackageInfo;

/**Item class for the base library class
  *@author Rasmus Diekenbrock
  */

class BaseLibraryItem
{

public:
	BaseLibraryItem(const BaseInfo * bi, const PackageInfo * packageInfo);
	virtual ~BaseLibraryItem();

	const BaseInfo * getBaseInfo() const;
	const PackageInfo * getPackageInfo() const;
	const QString & getName() const;
	const QString & getLibName() const;
	const QString & getShortDescr() const;
	const QString & getHTMLDescr() const;

		
	const QStringList * getOldLibNames() const;
	/** Returns true, if name is a old library name */
	bool isOldLibName(const QString & name) const;

protected:
	const BaseInfo * const m_baseInfo;
	const PackageInfo * const m_packageInfo;
	QStringList * m_oldLibNames;

};

#endif
