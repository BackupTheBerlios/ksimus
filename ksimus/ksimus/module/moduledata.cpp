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
static const char * sViewAttribPixmap	= "View Attribute Pixmap";
static const char * sViewAttribUserView	= "View Attribute User View";
static const char * sPixmapStore	= "Pixmap Store";
static const char * sPixmapData = "Pixmap Data";

static EnumDict<ModuleViewType> moduleViewDict;

EnumDict<ModuleViewType>::tData EnumDict<ModuleViewType>::data[]
			= { {"Generic", MV_GENERIC},
          {"User View",  MV_USERVIEW},
					{"Pixmap", MV_PIXMAP},
					{"None", MV_NONE},
          {0,(ModuleViewType)0}};

static EnumDict<ModulePixmapStoreType> modulePixmapStoreDict;

EnumDict<ModulePixmapStoreType>::tData EnumDict<ModulePixmapStoreType>::data[]
			= { {"Absolute", MPS_ABSOLTUE},
          {"Relative Module",  MPS_RELATIVE_MODULE},
					{"Internal", MPS_INTERNAL},
          {0,(ModulePixmapStoreType)0}};



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
		m_viewAttribPixmap(VA_SHEETVIEW),
		m_pixmapStore(MPS_ABSOLTUE),
		m_connPosUserView(0),
		m_connOrientUserView(0),
		m_viewAttribUserView(VA_SHEETVIEW),
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
	
	if (m_connPosGeneric)
		delete m_connPosGeneric;
	if (m_connPosPixmap)
		delete m_connPosPixmap;
	if (m_connOrientPixmap)
		delete m_connOrientPixmap;
		
	if (m_pixmap)
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
	return m_container->getUserSize();
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


ViewAttribute ModuleData::getPixmapAttrib() const
{
	return m_viewAttribPixmap;
}

void ModuleData::setPixmapAttrib(ViewAttribute viewAttrib)
{
	m_viewAttribPixmap = viewAttrib;
}

ViewAttribute ModuleData::getUserViewAttrib() const
{
	return m_viewAttribUserView;
}
	
void ModuleData::setUserViewAttrib(ViewAttribute viewAttrib)
{
	m_viewAttribUserView = viewAttrib;
}


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
	moduleViewDict.save(file, sViewType, m_moduleView);
		
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
					QByteArray array;
					QDataStream stream(array, IO_WriteOnly);
					QStringList strList;
					unsigned int i;
					
					stream << *m_pixmap;
					
					strList.append(QString::number(array.size(),16));
					
					for (i = 0; i < array.size(); i++)
					{
						strList.append(QString::number((unsigned char)array[i],16));
					}
					file.writeEntry(sPixmapData, strList);
				}
				if(!m_pixmapFile.isEmpty())
					file.writeEntry(sPixmapFile, m_pixmapFile);
			}
			break;
		default:
			KSIMDEBUG_VAR("Unknown pixmap store type", m_pixmapStore);
			break;
	}

	modulePixmapStoreDict.save(file, sPixmapStore, m_pixmapStore);
	
	if(!m_moduleName.isEmpty())
		file.writeEntry(sModuleName, m_moduleName);

	if(!m_moduleLibNames.isEmpty())
		file.writeEntry(sModuleLibNames, m_moduleLibNames);

	if(!m_shortDescr.isEmpty())
		file.writeEntry(sShortDescr, m_shortDescr);

	if(m_viewAttribPixmap != VA_SHEETVIEW)
		file.writeEntry(sViewAttribPixmap, ComponentInfo::convertViewAttribute(m_viewAttribPixmap));

	if(m_viewAttribUserView != VA_SHEETVIEW)
		file.writeEntry(sViewAttribUserView, ComponentInfo::convertViewAttribute(m_viewAttribUserView));
}

/** Load module properties
	Returns true if successful */
bool ModuleData::load(KSimData & file)
{
	m_moduleView = moduleViewDict.load(file, sViewType, MV_GENERIC);
	
	m_pixmapStore = modulePixmapStoreDict.load(file, sPixmapStore, MPS_ABSOLTUE);
	
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
				QStringList strList(file.readListEntry(sPixmapData));
			
				unsigned int size,i;
				size = strList[0].toUInt(0,16);
				QByteArray array(size);
			
				for (i = 0; i < size; i++)
				{
					array[i] = (char) strList[i+1].toUInt(0,16);
				}
			
				QDataStream stream(array, IO_ReadOnly);
				if (!m_pixmap)
					m_pixmap = new QPixmap;
				stream >> *m_pixmap;
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
	
	m_viewAttribPixmap = ComponentInfo::convertViewAttribute(file.readEntry(sViewAttribPixmap), VA_SHEETVIEW);
	m_viewAttribUserView = ComponentInfo::convertViewAttribute(file.readEntry(sViewAttribUserView), VA_SHEETVIEW);

	return true;
}
							
							
const ModuleInfo * ModuleData::makeModuleInfo(const QString & filename)
{
	KSimData file(filename);
	file.setGroup("/Property/Module/");
	
	ModuleViewType moduleView = moduleViewDict.load(file, sViewType, MV_NONE);
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
			viewAttrib = ComponentInfo::convertViewAttribute(file.readEntry(sViewAttribUserView), VA_SHEETVIEW);
			break;
	
		case MV_PIXMAP:
			viewAttrib = ComponentInfo::convertViewAttribute(file.readEntry(sViewAttribPixmap), VA_SHEETVIEW);
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
		addLibNames = moduleLibNameList.join(";");
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
		KSIMDEBUG_VAR("Document Path",url.path());
		return url;
	}
	else
	{
		// Module data is part of a module
		Module * module = (Module*) m_container->getParentComponent();
		KURL url(module->getModuleFile());
		KSIMDEBUG_VAR("Module Path",url.path());
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
		KSIMDEBUG_VAR("Relative file name",result);
		
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
	QDir dir(fileInfo.dirPath(true));	
	
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
		KSIMDEBUG_VAR("File path",dir.path());
		KSIMDEBUG_VAR("Pixmap relative file",relPath);
		int lastSlash(relPath.find('/',-1));
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
		}
		
		KSIMDEBUG_VAR("Pixmap file",m_pixmapFile);
	}
	else
	{
		// check failed
		m_pixmapFile = QString::null;
	}
}




