/***************************************************************************
                          moduledata.cpp  -  description
                             -------------------
    begin                : Wed Jul 19 2000
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

// C-Includes

// QT-Includes
#include <qsize.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdatastream.h>
#include <qcstring.h>
#include <qstringlist.h>
#include <qpixmap.h>

// KDE-Includes
#include <kmessagebox.h>
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "moduledata.h"
#include "module.h"
#include "ksimusdoc.h"
#include "ksimfile.h"
#include "component.h"
#include "compcontainer.h"
#include "compview.h"
#include "connectorbase.h"
#include "componentinfo.h"
#include "moduleinfo.h"
#include "resource.h"
#include "externalconnector.h"
#include "enumdict.h"
#include "ksimdebug.h"
#include "loglist.h"

// Forward declaration

// min. generic size
#define minX	8
#define minY	7

//################################################################################
//###### Some statics
//################################################################################

static const char * sViewType 			= "ViewType";
static const char * sPixmapFile 		= "PixFile";
static const char * sModuleName 		= "Module Name";
static const char * sModuleLibNames = "Module Lib Names";
static const char * sShortDescr 		= "Short Descr";
static const char * sPixmapStore	= "Pixmap Store";
static const char * sPixmapData = "Pixmap Data";

EnumDict<ModuleViewType>::tData EnumDict<ModuleViewType>::data[]
      = { {"Generic",    MV_GENERIC},
          {"User View",  MV_USERVIEW},
          {"Pixmap",     MV_PIXMAP},
          {"None",       MV_NONE},
          {0,           (ModuleViewType)0}};

static const EnumDict<ModuleViewType> & getModuleViewDict()
{
	static EnumDict<ModuleViewType> moduleViewDict;
	return moduleViewDict;
}


EnumDict<ModulePixmapStoreType>::tData EnumDict<ModulePixmapStoreType>::data[]
      = { {"Absolute",         MPS_ABSOLTUE},
          {"Relative Module",  MPS_RELATIVE_MODULE},
          {"Internal",         MPS_INTERNAL},
          {0,                 (ModulePixmapStoreType)0}};

static const EnumDict<ModulePixmapStoreType> & getModulePixmapStoreDict()
{
	static EnumDict<ModulePixmapStoreType> modulePixmapStoreDict;
	return modulePixmapStoreDict;
}



//################################################################################
//################################################################################


ModuleData::ModuleData(CompContainer * parent)
	:	m_inList(new ComponentList),
		m_outList(new ComponentList),
		m_externalList(new ComponentList),
		m_genericSize(0,0),
		m_connPosGeneric(0),
		m_pixmapSize(0,0),
		m_connPosPixmap(0),
		m_connOrientPixmap(0),
		m_pixmap(0),
		m_pixmapFile(),
		m_pixmapStore(MPS_ABSOLTUE),
		m_connPosUserView(0),
		m_connOrientUserView(0),
		m_moduleView(MV_NONE),
		m_container(parent),
		m_moduleName(),
		m_moduleLibNames(),
		m_shortDescr()
{
	CHECK_PTR(m_inList);
	CHECK_PTR(m_outList);
	CHECK_PTR(m_externalList);
}

ModuleData::~ModuleData()
{
	delete m_inList;
	delete m_outList;
	delete m_externalList;
	
	delete m_connPosGeneric;
	delete m_connPosPixmap;
	delete m_connOrientPixmap;
	delete m_pixmap;

}
	
/** set the type of module view
	Types are: MV_NONE, MV_GENERIC, MV_USERVIEW, MV_PIXMAP */
void ModuleData::setModuleView(ModuleViewType viewType)
{
	m_moduleView = viewType;
}

const QSize & ModuleData::getGenericSize() const
{
	return m_genericSize;
};
	
PointList * ModuleData::getGenericConnPos()
{
	if (!m_connPosGeneric)
	{
		m_connPosGeneric = new PointList;
		CHECK_PTR(m_connPosGeneric);
		m_connPosGeneric->setAutoDelete(true);
	}
	return m_connPosGeneric;
};

const QSize & ModuleData::getPixmapSize() const
{
	return m_pixmapSize;
};
	
PointList * ModuleData::getPixmapConnPos()
{
	if (!m_connPosPixmap)
	{
		m_connPosPixmap = new PointList;
		CHECK_PTR(m_connPosPixmap);
		m_connPosPixmap->setAutoDelete(true);
	}
	return m_connPosPixmap;
};

OrientList * ModuleData::getPixmapConnOrient()
{
	if (!m_connOrientPixmap)
	{
		m_connOrientPixmap = new OrientList;
		CHECK_PTR(m_connOrientPixmap);
		m_connOrientPixmap->setAutoDelete(true);
	}
	return m_connOrientPixmap;
};

/** returns a pointer to the pixmap
	Creates an empty pixmap, if not exist */
QPixmap * ModuleData::getPixmap()
{
	if (!m_pixmap)
	{
		m_pixmap = new QPixmap;
		CHECK_PTR(m_pixmap);
	}
	return m_pixmap;
}
/** set the pixmap filename */
void ModuleData::setPixmapFile(const QString & filename)
{
	m_pixmapFile = filename;
}
	
/** Returns the file name of the pixmap
	Returns an empty string, if not set before */
const QString & ModuleData::getPixmapFile() const
{
	return m_pixmapFile;
}

bool ModuleData::isPixmapFileValid() const
{
	return     (m_pixmapStore == MPS_INTERNAL)
	        || (
	               !m_pixmapFile.isEmpty()
	            && (QFile::exists(m_pixmapFile))
	            && (0 != QPixmap::imageFormat(m_pixmapFile))
	           );
}

/** Load the pixmap
	Returns flase, if not successful */
bool ModuleData::loadPixmap()
{
	if (m_pixmapStore == MPS_INTERNAL)
		return false;
	
	bool res;
	QPixmap * newPixmap = new QPixmap;
	CHECK_PTR(newPixmap);
	res = newPixmap->load(m_pixmapFile);
	
	if (res)
	{
		// okay
		if (m_pixmap)
			delete m_pixmap;
		m_pixmap = newPixmap;
		// Round up to next valid size and add connector space
		m_pixmapSize = QSize( (((m_pixmap->width()  + 3*gridX - 1)/gridX)*gridX),
		                      (((m_pixmap->height() + 3*gridY - 1)/gridY)*gridY) );
	}
	else
	{
		// failed
		delete newPixmap;
	}
	
	return res;
}
		
bool ModuleData::loadPixmap(const QString & filename)
{
	setPixmapFile(filename);
	return loadPixmap();
}


QSize ModuleData::getUserViewSize() const
{
	return m_container->getUserSize() + QSize(2*gridX, 2*gridY);
}

CompViewList * ModuleData::getUserViewList() const
{
	return m_container->getUserViewList();
}


PointList * ModuleData::getUserViewConnPos()
{
	if (!m_connPosUserView)
	{
		m_connPosUserView = new PointList;
		CHECK_PTR(m_connPosUserView);
		m_connPosUserView->setAutoDelete(true);
	}
	return m_connPosUserView;
};

OrientList * ModuleData::getUserViewConnOrient()
{
	if (!m_connOrientUserView)
	{
		m_connOrientUserView = new OrientList;
		CHECK_PTR(m_connOrientUserView);
		m_connOrientUserView->setAutoDelete(true);
	}
	return m_connOrientUserView;
};


/** Search externals in compList */
void ModuleData::searchExternals(const ComponentList * compList)
{
	// First remove old externals
	m_inList->clear();
	m_outList->clear();
	m_externalList->clear();
	
	// Search external connectors
	// And sort them (In/Out and position)
	FOR_EACH_COMP(it, *compList)
	{
		unsigned int i = 0;
		QPoint a,b;
		
		if (it.current()->isExtConn())
		{
			ExternalConnector * extConn = (ExternalConnector *) it.current();
			if (extConn->isInput())
			{
				// Sort position
				a =  it.current()->getSheetView()->getPos();
				for (i = 0; i < m_inList->count(); i++)
				{
					b =  m_inList->at(i)->getSheetView()->getPos();
					if ((a.y() < b.y()) ||
						 ((a.y() == b.y()) && (a.x() < b.x())))
					{
						break;
					}
				}
				m_inList->insert(i, it.current());
			}
			else
			{
				// Output
				a =  it.current()->getSheetView()->getPos();
				for (i = 0; i < m_outList->count(); i++)
				{
					b =  m_outList->at(i)->getSheetView()->getPos();
					if ((a.y() < b.y()) ||
						 ((a.y() == b.y()) && (a.x() < b.x())))
					{
						break;
					}
				}
				m_outList->insert(i, it.current());
			}
		}
	}
	// Insert outList in externalList
	FOR_EACH_COMP(it,*m_outList)
	{
		m_externalList->append(it.current());
	}
	// Insert inList in externalList
	FOR_EACH_COMP(it,*m_inList)
	{
		m_externalList->append(it.current());
	}
}

/** Setup view size and connector place for generic view
	Call searchExternals(compList) befor this function!!! */
void ModuleData::setupGenericData()
{
    unsigned int x,y,i,space,offset;
	
	if (m_connPosGeneric)
	{
		m_connPosGeneric->clear();
	}
	else
	{
		// Create position list
		m_connPosGeneric = new PointList;
		CHECK_PTR(m_connPosGeneric);
		m_connPosGeneric->setAutoDelete(true);
	}

	// Calculate and set module size
	x = minX;
	y = minY;
	if (y < (m_inList->count() * 2) + 3)
		y = m_inList->count() * 2 + 3;
	if (y < (m_outList->count() * 2) + 3)
		y = m_outList->count() * 2 + 3;
	
	// Store size (including connector areas)
	m_genericSize = QSize(x*gridX, y*gridY);
	
	/*** Calculate connector position ***/
	m_connPosGeneric->clear();
	// Space between Out-Connectors
	if (m_outList->count()>1)
		space = (y - m_outList->count() - 4) / (m_outList->count()-1);
	else
		space = 0;
	offset = (y - m_outList->count() - ((m_outList->count()-1) * space ))/2;
	//  Positioning Out-Connectors
	for (i = 0; i < m_outList->count(); i++)
	{
		QPoint * pos = new QPoint(x-1, offset+i*(space+1));
		CHECK_PTR(pos);
		m_connPosGeneric->append(pos);
	}
	// Space between In-Connectors
	if (m_inList->count()>1)
		space = (y - m_inList->count() - 4) / (m_inList->count()-1);
	else
		space = 0;
	offset = (y - m_inList->count() - ((m_inList->count()-1) * space ))/2;
	//  Positioning In-Connectors
	for (i = 0; i < m_inList->count(); i++)
	{
		QPoint * pos = new QPoint(0, offset+i*(space+1));
		CHECK_PTR(pos);
		m_connPosGeneric->append(pos);
	}
}	

/** Setup view size and connector place for pixmap view
	Call searchExternals(compList) before this function!!! */
void ModuleData::setupPixmapData()
{
	if (m_connPosPixmap)
	{
		m_connPosPixmap->clear();
	}
	else
	{
		// Create position list
		m_connPosPixmap = new PointList;
		CHECK_PTR(m_connPosPixmap);
		m_connPosPixmap->setAutoDelete(true);
	}
	
	if (m_connOrientPixmap)
	{
		m_connOrientPixmap->clear();
	}
	else
	{
		// Create position list
		m_connOrientPixmap = new OrientList;
		CHECK_PTR(m_connOrientPixmap);
		m_connOrientPixmap->setAutoDelete(true);
	}
	
	// Load connector positions and orientations
	FOR_EACH_COMP(it,*m_externalList)
	{
		QPoint * pos = new QPoint(((ExternalConnector*)it.current())->getPixmapPos());
		CHECK_PTR(pos);
		m_connPosPixmap->append(pos);
		
		ConnOrientationType * orient = new ConnOrientationType(((ExternalConnector*)it.current())
		                                                          ->getPixmapOrientation());
		CHECK_PTR(orient);
		m_connOrientPixmap->append(orient);
	}
}

/** Setup view size and connector place for user view
	Call searchExternals(compList) before this function!!! */
void ModuleData::setupUserViewData()
{
	if (m_connPosUserView)
	{
		m_connPosUserView->clear();
	}
	else
	{
		// Create position list
		m_connPosUserView = new PointList;
		CHECK_PTR(m_connPosUserView);
		m_connPosUserView->setAutoDelete(true);
	}
	
	if (m_connOrientUserView)
	{
		m_connOrientUserView->clear();
	}
	else
	{
		// Create position list
		m_connOrientUserView = new OrientList;
		CHECK_PTR(m_connOrientUserView);
		m_connOrientUserView->setAutoDelete(true);
	}
	
	// Load connector positions and orientations
	FOR_EACH_COMP(it,*m_externalList)
	{
		QPoint * pos = new QPoint(((ExternalConnector*)it.current())->getUserViewPos());
		CHECK_PTR(pos);
		m_connPosUserView->append(pos);
		
		ConnOrientationType * orient = new ConnOrientationType(((ExternalConnector*)it.current())
		                                                          ->getUserViewOrientation());
		CHECK_PTR(orient);
		m_connOrientUserView->append(orient);
	}
}

/** save module properties */
void ModuleData::save(KSimData & file)
{
	getModuleViewDict().save(file, sViewType, m_moduleView);
		
	switch(m_pixmapStore)
	{
		case MPS_ABSOLTUE:
			if(!m_pixmapFile.isEmpty())
				file.writeEntry(sPixmapFile, m_pixmapFile);
			break;
		case MPS_RELATIVE_MODULE:
			if(!m_pixmapFile.isEmpty())
				file.writeEntry(sPixmapFile, getRelativePath());
 			break;
		
		case MPS_INTERNAL:
			{
				if(m_pixmap)
				{
					file.writeEntry(sPixmapData, *m_pixmap);
				}
				if(!m_pixmapFile.isEmpty())
					file.writeEntry(sPixmapFile, m_pixmapFile);
			}
			break;
		default:
			KSIMDEBUG_VAR("Unknown pixmap store type", m_pixmapStore);
			break;
	}

	getModulePixmapStoreDict().save(file, sPixmapStore, m_pixmapStore);
	
	if(!m_moduleName.isEmpty())
		file.writeEntry(sModuleName, m_moduleName);

	if(!m_moduleLibNames.isEmpty())
		file.writeEntry(sModuleLibNames, m_moduleLibNames);

	if(!m_shortDescr.isEmpty())
		file.writeEntry(sShortDescr, m_shortDescr);
}

/** Load module properties
	Returns true if successful */
bool ModuleData::load(KSimData & file)
{
	m_moduleView = getModuleViewDict().load(file, sViewType, MV_GENERIC);
	
	m_pixmapStore = getModulePixmapStoreDict().load(file, sPixmapStore, MPS_ABSOLTUE);
	
	switch(m_pixmapStore)
	{
		case MPS_ABSOLTUE:
			m_pixmapFile = file.readEntry(sPixmapFile);
			break;
		
		case MPS_RELATIVE_MODULE:
			setRelativePath(file.readEntry(sPixmapFile));
			break;
		
		case MPS_INTERNAL:
			{
				m_pixmapFile = file.readEntry(sPixmapFile);
				if (!m_pixmap)
				{
					m_pixmap = new QPixmap;
					CHECK_PTR(m_pixmap);
				}
				*m_pixmap = file.readPixmapEntry(sPixmapData);
				// Round up to next valid size and add connector space
				m_pixmapSize = QSize( (((m_pixmap->width()  + 3*gridX - 1)/gridX)*gridX),
				                      (((m_pixmap->height() + 3*gridY - 1)/gridY)*gridY) );
			}
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown pixmap store type", m_pixmapStore);
			break;
	}
	
	
	m_moduleName = file.readEntry(sModuleName);
	
	m_moduleLibNames = file.readEntry(sModuleLibNames);
	m_shortDescr = file.readEntry(sShortDescr);
	
	return true;
}

const ModuleInfo * ModuleData::makeModuleInfo(const QString & filename)
{
	KSimData file(filename);
	file.setGroup("/Property/Module/");
	
	ModuleViewType moduleView = getModuleViewDict().load(file, sViewType, MV_NONE);
	if(moduleView == MV_NONE)
	{
		// No module
		return 0;
	}
	
	QString moduleName = file.readEntry(sModuleName);
	QString moduleLibNames = file.readEntry(sModuleLibNames);
	QString shortDescr = file.readEntry(sShortDescr);
	QString libName;
	QString addLibNames;
	
	if(moduleName.isEmpty())
	{
		moduleName = filename;
	}
	
	ViewAttribute viewAttrib(VA_SHEETVIEW);
	switch(moduleView)
	{
		case MV_NONE:
		case MV_GENERIC:
			break;
		
		case MV_USERVIEW:
			viewAttrib = VA_SHEET_AND_USER;
			break;
	
		case MV_PIXMAP:
			viewAttrib = VA_SHEET_AND_USER;
			break;
	
		default:
			KSIMDEBUG_VAR("Unknown module view", moduleView);
			break;
	}	
	
	
	QStringList moduleLibNameList = QStringList::split(';',moduleLibNames);
	if(moduleLibNameList.count() == 0)
	{
		libName = moduleName;
	}
	else
	{
		libName = moduleLibNameList[0];
		moduleLibNameList.remove(libName);
		addLibNames = moduleLibNameList.join(QString::fromLatin1(";"));
	}
	
	return new ModuleInfo( filename,
	                       moduleName,
	                       libName,
	                       addLibNames,
	                       viewAttrib);

}


KURL ModuleData::getUrl() const
{
	if (m_container->isParentDoc())
	{
		// Module data is part of document
		const KURL & url = m_container->getDoc()->URL();
//		KSIMDEBUG_VAR("Document Path",url.path());
		return url;
	}
	else
	{
		// Module data is part of a module
		Module * module = (Module*) m_container->getParentComponent();
		KURL url(module->getModuleFile());
//		KSIMDEBUG_VAR("Module Path",url.path());
		return url;
	}
}


QString ModuleData::getRelativePath()
{
	KURL url(getUrl());
	QString path(url.path());
	QFileInfo fileInfo(path);
	bool valid(true);
	
	if(!url.isLocalFile())
	{
		KSIMDEBUG_VAR("No locale file name",url.path());
		valid = false;
	}
	
	if(valid && path.isEmpty())
	{
		KSIMDEBUG("File name is empty");
		valid = false;
	}
		
/*	if(valid && !fileInfo.exists())
	{
		KSIMDEBUG_VAR("File does not exist",url.path());
		valid = false;
	}*/
	
/*	if(valid && !fileInfo.isFile())
	{
		KSIMDEBUG_VAR("File name is no file",url.path());
		valid = false;
	}*/
	
	if(valid)
	{
		QString result(KSimFile::relativePath(m_pixmapFile,fileInfo.dirPath(true)));
//		KSIMDEBUG_VAR("Relative file name",result);
		
		return result;
	}
	else
	{
		m_pixmapStore = MPS_ABSOLTUE;
		KSIMDEBUG("Switch to absolute store type");
		return m_pixmapFile;
	}
}


void ModuleData::setRelativePath(QString relPath)
{
	KURL url(getUrl());
	QString path(url.path());
	QFileInfo fileInfo(path);
//	QDir dir(fileInfo.dirPath(true));	
	
	bool valid(true);

	if(!url.isLocalFile())
	{
		KSIMDEBUG_VAR("No locale file name",url.path());
		valid = false;
	}
	
	if(valid && path.isEmpty())
	{
		KSIMDEBUG("File name is empty");
		valid = false;
	}
		
/*	if(valid && !fileInfo.exists())
	{
		KSIMDEBUG_VAR("File does not exist",url.path());
		valid = false;
	}*/
	
	if(valid && !fileInfo.isFile())
	{
		KSIMDEBUG_VAR("File name is no file",url.path());
		valid = false;
	}
	
	if(valid)
	{
		KSIMDEBUG_VAR("File path",url.directory(false, true));
		KSIMDEBUG_VAR("Pixmap relative file",relPath);
		QFileInfo fi(url.directory(false, true) + relPath);
		m_pixmapFile = fi.absFilePath();
		
/*		int lastSlash(relPath.find('/',-1));
		if(-1 != lastSlash)
		{
			// relPath contains directories
			dir.cd(relPath.left(lastSlash));
			m_pixmapFile = dir.absPath() + relPath.right(lastSlash);
		}
		else
		{
			// relPath contains no directories
			m_pixmapFile = dir.absPath() + '/' + relPath.right(lastSlash);
		}*/
		
		KSIMDEBUG_VAR("Pixmap file",m_pixmapFile);
	}
	else
	{
		// check failed
		m_pixmapFile = QString::null;
	}
}


bool ModuleData::checkConnectorPosition(bool showMessage)
{
	bool matchPos(true);
	const PointList * connPosList = (PointList *)0;
	QSize size;

	switch(getModuleView())
	{
		case MV_NONE:
		case MV_GENERIC:
			// Nothing yet
			return matchPos;
			break;

		case MV_PIXMAP:
			connPosList = getPixmapConnPos();
			size = getPixmapSize();
			break;

		case MV_USERVIEW:
			connPosList = getUserViewConnPos();
			size = getUserViewSize();
			break;

		default:
			KSIMDEBUG(QString::fromLatin1("Unknown module view = %1").arg((int)getModuleView()));
			return matchPos;
			break;
	}

	size.rwidth()  /= gridX;
	size.rheight() /= gridY;
	QRect topArea   (             1,               0, size.width()-2,               1);
	QRect bottomArea(             1, size.height()-1, size.width()-2,               1);
	QRect leftArea  (             0,               1,              1, size.height()-2);
	QRect rightArea (size.width()-1,               1,              1, size.height()-2);

	for (QListIterator<QPoint> it(*connPosList);it.current();++it)
	{
		// not hidden?
		if (it.current()->x() != -1)
		{
			// Connector inside one area
			if ( topArea.contains(*it.current())
			  || bottomArea.contains(*it.current())
			  || leftArea.contains(*it.current())
			  || rightArea.contains(*it.current()) )
			{
				// Okay
			}
			else
			{
				KSIMDEBUG(QString::fromLatin1("Failed (%1/%2)").arg(it.current()->x()).arg(it.current()->y()));
				KSIMDEBUG(QString::fromLatin1("Size (%1/%2)").arg(size.width()).arg(size.height()));
				KSIMDEBUG(QString::fromLatin1("topArea (%1/%2 - %3/%4)").arg(topArea.left()).arg(topArea.top()).arg(topArea.right()).arg(topArea.bottom()));
				KSIMDEBUG(QString::fromLatin1("bottomArea (%1/%2 - %3/%4)").arg(bottomArea.left()).arg(bottomArea.top()).arg(bottomArea.right()).arg(bottomArea.bottom()));
				KSIMDEBUG(QString::fromLatin1("leftArea (%1/%2 - %3/%4)").arg(leftArea.left()).arg(leftArea.top()).arg(leftArea.right()).arg(leftArea.bottom()));
				KSIMDEBUG(QString::fromLatin1("rightArea (%1/%2 - %3/%4)").arg(rightArea.left()).arg(rightArea.top()).arg(rightArea.right()).arg(rightArea.bottom()));
				matchPos = false;
//				break;
			}
		}
	}

	if (!matchPos && showMessage)
	{
		QString s(i18n("The placement of the external connectors don't match the module shape! Switch to generic view.\n"
		               "Hint: The common reason for this error is a resized module without rearranged external connectors."));
		if (m_container->isParentComponent())
		{
			m_container->getParentComponent()->logError(s);
		}
		else
		{
			m_container->getLogList()->error(s);
		}
	}
	return matchPos;
}


