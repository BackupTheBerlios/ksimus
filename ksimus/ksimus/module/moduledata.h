/***************************************************************************
                          moduledata.h  -  description
                             -------------------
    begin                : Mon Jul 17 2000
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

#ifndef MODULEDATA_H
#define MODULEDATA_H

// C-Includes

// QT-Includes
#include <qlist.h>
#include <qrect.h>

// KDE-Includes
#include <kurl.h>
// Project-Includes
#include "types.h"
#include "componentinfo.h"

// Forward declaration
class QString;
class QSize;
class QPixmap;
class KSimData;
class CompContainer;
class ComponentList;
class CompViewList;
class ModuleInfo;


/**Contains module data
  *@author Rasmus Diekenbrock
  */

enum ModuleViewType {
                       MV_GENERIC = 0,
                       MV_USERVIEW,
                       MV_PIXMAP,
                       MV_NONE
                    };
enum ModulePixmapStoreType {
                              MPS_ABSOLTUE,
                              MPS_RELATIVE_MODULE,
                              MPS_INTERNAL
                           };


typedef QList<QPoint> PointList;
typedef QList<ConnOrientationType> OrientList;

class ModuleData
{
class Private;

public:
	
	ModuleData(CompContainer * parent);
	~ModuleData();
	/** set the type of module view.
		Types are: MV_NONE, MV_GENERIC, MV_USERVIEW, MV_PIXMAP. */
	void setModuleView(ModuleViewType viewType);
	/** returns the module view type.
		Types are: MV_NONE, MV_GENERIC, MV_USERVIEW, MV_PIXMAP. */
	ModuleViewType getModuleView() const { return m_moduleView; };	
	ComponentList * getInList() const { return m_inList; };
	ComponentList * getOutList() const { return m_outList; };
	ComponentList * getExternalList() const { return m_externalList; };
	const QSize & getGenericSize() const;
	const QSize & getPixmapSize() const;
	QSize getUserViewSize() const;
	
	PointList * getGenericConnPos();
	PointList * getPixmapConnPos();
	PointList * getUserViewConnPos();
	OrientList * getPixmapConnOrient();
	OrientList * getUserViewConnOrient();
	CompViewList * getUserViewList() const;
	
	/** set the pixmap filename. */
	void setPixmapFile(const QString & filename);
	/** Returns the file name of the pixmap.
		Returns an empty string, if not set before. */
	const QString & getPixmapFile() const;
	/** Returns true, if pixmap file is valid. */
	bool isPixmapFileValid() const;	
	/** Returns a pointer to the pixmap.
		Creates a empty pixmap, if not exist. */
	QPixmap * getPixmap();
	/** Load the pixmap
		Returns flase, if not successful. */
	bool loadPixmap();
	bool loadPixmap(const QString & filename);
	
	/** Search externals in compList */
	void searchExternals(const ComponentList * compList);
	/** Setup view size and connector place for generic view.
		Call searchExternals(compList) befor this function!!! */
	void setupGenericData();
	/** Setup view size and connector place for pixmap view.
		Call searchExternals(compList) before this function!!! */
	void setupPixmapData();
	/** Setup view size and connector place for user view.
		Call searchExternals(compList) before this function!!! */
	void setupUserViewData();

	/** save module properties. */
	void save(KSimData & file);
	/** load module properties.
		Returns true if successful. */
	bool load(KSimData & file);

	/** Creates a Module Info and return the pointer. The function returns a null pointer if
			file is not a valid module.
		*/
	static const ModuleInfo * makeModuleInfo(const QString & filename);
	
	void setModuleName(const QString & name) { m_moduleName = name; };
	const QString & getModuleName() const { return m_moduleName; };
	
	void setModuleLibNames(const QString & libNames) { m_moduleLibNames = libNames; };
	const QString & getModuleLibNames() const { return m_moduleLibNames; };
	
	void setShortDescr(const QString & descr) { m_shortDescr = descr; };
	const QString & getShortDescr() const { return m_shortDescr; };
	
	void setPixmapStore(ModulePixmapStoreType newStore) { m_pixmapStore = newStore; };
	ModulePixmapStoreType getPixmapStore() const { return m_pixmapStore; };

	/** Checks the consistence of the module connectors.
	  * If showMessage is true a messages is shown if the connector position does not
	  * match the connector area of the module. */
	bool checkConnectorPosition(bool showMessage);

protected:

	/** Returns the current file name.
			If this module data is part of a ducoment, the document file name is returned.
			If this module data is part of a module, th module file name is returned.
		*/
	KURL getUrl() const;
	/** Returns a relative path to the pixmap file.
			The base path is given by function getUrl().
			If the url is not valid, the function switches to absolute store.
		*/
	QString getRelativePath();
	/** Set the pixmap file. Connects the base path and the relative path.
			Set pixmap file invalid, if something goes wrong.
		*/
	void setRelativePath(QString relPath);

	
	ComponentList * m_inList;
	ComponentList * m_outList;
	ComponentList * m_externalList;

	QSize m_genericSize;
	PointList * m_connPosGeneric;
	
	QSize m_pixmapSize;
	PointList * m_connPosPixmap;
	OrientList * m_connOrientPixmap;
	QPixmap * m_pixmap;	
	QString m_pixmapFile;
	ModulePixmapStoreType m_pixmapStore;
	
	PointList * m_connPosUserView;
	OrientList * m_connOrientUserView;
	
	ModuleViewType  m_moduleView;
	
	CompContainer * m_container;
	
	QString m_moduleName;
	QString m_moduleLibNames;
	QString m_shortDescr;

private:
	Private * m_private;
};

#endif
