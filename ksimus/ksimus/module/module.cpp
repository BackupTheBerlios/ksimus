/***************************************************************************
                          module.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
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

#include <qpainter.h>
#include <qdir.h>
#include <qrect.h>
#include <qsize.h>
#include <qpopupmenu.h>
#include <kfiledialog.h>
#include <klocale.h>

#include "ksimdata.h"
#include "ksimdebug.h"
#include "module.h"
#include "moduledata.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "componentinfo.h"
#include "connectorinfo.h"
#include "compcontainer.h"
#include "resource.h"
#include "displaywidget.h"
#include "ksimwidget.h"
#include "externalconnector.h"

static const char * sModFile = "ModuleFile";
//static const char * sConnector = "Connector";
#define minX	8
#define minY	7

//###############################################################


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Module(container, ci);
}

const ComponentInfo ModuleBaseInfo (	"Module",
									"Module",
									0,
									VA_SHEET_AND_USER,
									create );

//###############################################################

ModuleSV::ModuleSV(Component * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	widgetList = new KSimWidgetList();
	CHECK_PTR(widgetList);
	setPlace(QRect(0, 0, minX*gridX, minY*gridY));
}
ModuleSV::~ModuleSV()
{
	if (widgetList)
		delete widgetList;
}

void ModuleSV::draw(QPainter * p)
{
	Module * mod = (Module *) getComponent();
	QSize size(getPlace().size());
		
	switch(mod->getModuleContainer()->getModuleData()->getModuleView())
	{
		case MV_GENERIC:
			drawGeneric(p, size);
			break;
			
		case MV_PIXMAP:
			p->drawPixmap(gridX, gridY, *mod->getModuleContainer()->getModuleData()->getPixmap());
			break;
			
		case MV_USERVIEW:
			mod->getModuleContainer()->drawUserView(p);
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown module view",(int)mod->getModuleContainer()->getModuleData()->getModuleView());
			break;
	}
	
	CompView::draw(p);
}

void ModuleSV::drawGeneric(QPainter * p, QRect & place)
{
	p->save();
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(place.x() + gridX+1, place.y() + gridY+1, place.width()-gridX*2-1, place.height()-gridY*2-1);
	p->drawText(place.x() + gridX+1, place.y() + gridY+1, place.width()-gridX*2-1, place.height()-gridY*2-1,
	            AlignCenter, "Module");
	p->restore();
}	

void ModuleSV::drawGeneric(QPainter * p, QSize & size)
{
	QRect place (QPoint(0,0), size);
	drawGeneric(p,place);
}

void ModuleSV::print(QPainter * paint)
{
	draw(paint);

  QWidget * pWid;
	if (widgetList && (pWid = widgetList->getFirstWidget()))
	{
		// Reset the compView translation (a little bit ugly :)
		paint->restore();
		paint->drawPixmap(pWid->pos(), QPixmap::grabWidget(pWid));
		// Push one
		paint->save();
	}
}

/**  */
QWidget * ModuleSV::createCompViewWidget(QWidget * parent)
{
	
	Module * module = (Module*)getComponent();
	
	if (module->getModuleContainer()->getModuleData()->getModuleView() != MV_USERVIEW)
	{
		return (QWidget *)0;
	}
	
	CompViewList * viewList = module->getModuleContainer()->getUserViewList();
	ModuleWidget * display = new ModuleWidget(module, viewList , parent, module->getName());
	CHECK_PTR(display);
	widgetList->addWidget(display);
	display->setBackgroundMode(QWidget::PaletteDark);

	display->installEventFilter(parent);
	display->setMouseTracking(true);
	
	/* General signals */
	// Delete signal (CompView->Widget)
	QObject::connect(this, SIGNAL(destroyed()), display, SLOT(slotDelete()));
	// Delete signal (Editor->Widget)
	QObject::connect(parent, SIGNAL(destroyed()), display, SLOT(slotDelete()));
	// Move signal (CompView->Widget)
	QObject::connect(this, SIGNAL(signalMove(const QPoint &)), display, SLOT(move(const QPoint &)));
	// Resize signal (CompView->Widget)
	QObject::connect(this, SIGNAL(signalResize(const QSize &)), display, SLOT(resize(const QSize &)));

	// Hide signal (CompView->Widget)
	connect(this, SIGNAL(signalHide()), display, SLOT(hide()));
	// Show signal (CompView->Widget)
	connect(this, SIGNAL(signalShow()), display, SLOT(show()));
	
	display->reload();
	
	//Work around for widget positioning
	resize();
	display->show();
	
	return display;
}

void ModuleSV::setPos(const QPoint & pos)
{
	CompView::setPos(pos);
	
	QPoint gridPos = mapToGrid(pos);
	QPoint newPos (gridPos.x()+gridX, gridPos.y()+gridY);
	emit signalMove(newPos);
}

void ModuleSV::resize()
{
	CompView::resize();	
	
	QSize newSize;
	
	newSize.setWidth(getPlace().width() - 2*gridX );
	newSize.setHeight(getPlace().height() - 2*gridY );

	emit signalResize(newSize);
	setPos(getPlace().topLeft());
	
}

void ModuleSV::reload()
{
	QList<QWidget> * list = widgetList->getWidgetList();
	for(QListIterator<QWidget> it(*list);it.current();++it)
	{
		((ModuleWidget*)it.current())->reload();
	}
}

//###############################################################



Module::Module(CompContainer * _container, const ComponentInfo * ci)
	: Component(_container, ci),
		moduleFile()
{
	m_isModule = true;
	
	m_moduleContainer = new CompContainer(this);
	CHECK_PTR(m_moduleContainer);
	
	new ModuleSV(this, SHEET_VIEW);
	new ModuleSV(this, USER_VIEW);
	
	extList = new ComponentList;
	CHECK_PTR(extList);
}
Module::~Module()
{
//	delete container;
	delete extList;
}

void Module::setModuleFile(const QString & fileName)
{
	moduleFile = fileName;
	reloadModule();
}
	
const QString & Module::getModuleFile() const
{
	return moduleFile;
}
	
void Module::save(KSimData & file) const
{
	file.writeEntry(sModFile, moduleFile);
//	file.writeEntry(sConnector, getConnList()->count());
	
	Component::save(file);
}

/** load module properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Module::load(KSimData & file, bool copyLoad)
{
//	unsigned int connNo;//,i;
	
	moduleFile = file.readEntry(sModFile);
//	connNo = file.readUnsignedNumEntry(sConnector,0);
	
	reloadModule();
	
	return Component::load(file, copyLoad);

}
	
/** reloads the module and recreates the view */
void Module::reloadModule()
{
	PointList * posList = 0;
	unsigned int i;
	ModuleData * mdata;
	ConnectorList * oldConnList = new ConnectorList(*getConnList());
	oldConnList->setAutoDelete(false);
	
	// Remove all information from container
	m_moduleContainer->deleteAll();
	mdata = m_moduleContainer->getModuleData();
	
	// First remove old connectors
	getConnList()->setAutoDelete(false);	//Do not delete connectors
	getConnList()->clear();								//Clear list
	getConnList()->setAutoDelete(true);
	extList->clear();
	
	
	// open file
	KSimData file(moduleFile);

	// Load all components from file
	file.setGroup("/");
	getModuleContainer()->loadProperty(file);
	getModuleContainer()->loadComponents(file);

	setName(mdata->getModuleName());
	
	if ((mdata->getModuleView() == MV_PIXMAP) && (!mdata->isPixmapFileValid()))
	{
		logWarning(i18n("Pixmap file '%1' is not valid! Switch to generic view.").arg(mdata->getPixmapFile()));
		mdata->setModuleView(MV_GENERIC);
	}

	if(getSheetView())
		((ModuleSV*)getSheetView())->reload();
	if(getUserView())
		((ModuleSV*)getUserView())->reload();
	
	
	// Search external connectors
	mdata->searchExternals(m_moduleContainer->getComponentList());
	
	switch(mdata->getModuleView())
	{
		case MV_GENERIC:
		{
		    // Calculate and set module size
			mdata->setupGenericData();
	
			// set view size
			if (getSheetView())
			{
				getSheetView()->setHide(false);
				emit getSheetView()->signalHide();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getGenericSize()));
			}
			
			if (getUserView())
			{
				getUserView()->setHide(true);
				emit getUserView()->signalHide();
				getUserView()->setPlace(QRect(getUserView()->getPos(), mdata->getGenericSize()));
			}
	
			// Create Connectors
			posList = mdata->getGenericConnPos();
		}
		break;
		
		case MV_PIXMAP:
		{
		    // Calculate and set module size
			mdata->setupPixmapData();
			mdata->loadPixmap();
	
			// set view size
			if (getSheetView())
			{
				getSheetView()->setHide(false);
				emit getSheetView()->signalHide();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getPixmapSize()));
			}
			if (getUserView())
			{
				getUserView()->setHide(true);
				emit getUserView()->signalHide();
				getUserView()->setPlace(QRect(getUserView()->getPos(), mdata->getPixmapSize()));
			}
	
			// Create Connectors
			posList = mdata->getPixmapConnPos();
		}
		break;
		
		case MV_USERVIEW:
		{
		    // Calculate and set module size
			mdata->setupUserViewData();
	
			// set view size
			if (getSheetView())
			{
				getSheetView()->setHide(false);
				emit getSheetView()->signalShow();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getUserViewSize()+QSize(2*gridX,2*gridY)));
			}
			if (getUserView())
			{
				if(mdata->getUserViewAttrib() == VA_SHEET_AND_USER)
				{
					getUserView()->setHide(false);
					emit getUserView()->signalShow();
				}
				else
				{
					getUserView()->setHide(true);
					emit getUserView()->signalHide();
				}
					
				getUserView()->setPlace(QRect(getUserView()->getPos(), mdata->getUserViewSize()+QSize(2*gridX,2*gridY)));
			}
			// Create Connectors
			posList = mdata->getUserViewConnPos();
		}
		break;
		
		default:
			KSIMDEBUG_VAR("Unknown module view",(int)mdata->getModuleView());
			break;
	}

	CHECK_PTR(posList);
	for	(i=0; i < posList->count(); i++)
	{
		ExternalConnector * extConn = (ExternalConnector *)mdata->getExternalList()->at(i);
		ConnectorBase *conn = searchConn (extConn, oldConnList);
		
		if (posList->at(i)->x() != -1)
		{
			if (conn)
			{
				// The connector exist
				oldConnList->removeRef(conn);
//				getConnList()->append(conn);
				addConnector(conn);
			}
			else
			{
				// The connector doesn't exist. Create it
/*				conn = extConn->getExternalConn()->getConnInfo()->create(this, "", QPoint(0,0));
				conn->setName(extConn->getName());
				conn->setNegate(extConn->getExternalConn()->isNegated(),true);*/
				conn = extConn->getExternalConn()->getConnInfo()->create(this, extConn->getName(), QPoint(0,0));
				conn->setWireName(QString("(extConn) %1").arg(extConn->getSerialNumber()));
				conn->setNegate(extConn->getExternalConn()->isNegated(),true);
			}
	
			extList->append(extConn);
	
			conn->setGridPos(*(posList->at(i)));
	    	
			if (mdata->getModuleView() == MV_PIXMAP)
				conn->setOrientation(*mdata->getPixmapConnOrient()->at(i));
		}
		else
		{
			// Remove connection
			if (conn && conn->getWire())
			{
				getContainer()->delConnection(conn);
				oldConnList->removeRef(conn);
				delete conn;
			}
		}
	    	
	}
	
	// Remove unused connecetors
	oldConnList->setAutoDelete(true);
	while(oldConnList->count())
	{
		if (oldConnList->first()->getWire())
			getContainer()->delConnection(oldConnList->first());
		oldConnList->removeFirst();
	}
	delete oldConnList;
}

/** search the connector
	returns 0,if no connector is found */
ConnectorBase * Module::searchConn(ExternalConnector * extConn, ConnectorList * connList)
{
	const ConnectorInfo * extConnInfo = extConn->getExternalConn()->getConnInfo();
//	QString extName = extConn->getName();
	QString connWireName = QString("(extConn) %1").arg(extConn->getSerialNumber());
	FOR_EACH_CONNECTOR(it, *connList)
	{
		// Compare connector type and name
			if ((it.current()->getConnInfo() == extConnInfo)
//			  &&(extName == it.current()->getName()))
			  &&(connWireName == it.current()->getWireName()))
		{
			// Both have same type and connector name have the correct wire name
			return it.current();
		}
	}
	return (ConnectorBase*)0;
}


/** Checks the component
*   eg. all required inputs are connected
*   The implementation calls the checks functions of the connectors and container.
*	Returns the number of errors
*/
int Module::checkCircuit()
{
	int errors = Component::checkCircuit();
	
	errors += m_moduleContainer->checkCircuit();
	
	return errors;
}

/** Executes the simulation of this component */
void Module::calculate()
{
	Component::calculate();
	
	// First get all input signals
	for(unsigned int i = 0; i < extList->count(); i++)
	{
   		ExternalConnector * extConn = (ExternalConnector*) extList->at(i);
		// Input ?
		if (extConn->isInput())
		{
    		ConnectorInputBase * inExternal = (ConnectorInputBase*) getConnList()->at(i);
    		ConnectorOutputBase * outInternal = (ConnectorOutputBase*) extConn->getInternalConn();
    		
    		// Set data to wire
    		const void * pData;
    		pData = inExternal->readoutData();
    		outInternal->copyData(pData);
    	}
    }
    // Calculate components
    FOR_EACH_COMP(it, *getModuleContainer()->getCalculateComponentList())
    {
    	it.current()->calculate();
		
			if (it.current()->getSheetView())
			{
				if (it.current()->getSheetView()->isViewChanged())
				{
					getSheetView()->setViewChanged(true);
				}
			}
    }
}

/** Shift the result of calculation to output */
void Module::updateOutput()
{
	Component::updateOutput();

	// update outputs components
	FOR_EACH_COMP(it, *m_moduleContainer->getUpdateOutputComponentList())
	{
		if (!it.current()->isExtConn())
		{
			it.current()->updateOutput();
		}
	}

	// At last set all output signals
	for(unsigned int i = 0; i < extList->count(); i++)
	{
   		ExternalConnector * extConn = (ExternalConnector*) extList->at(i);
		// Output ?
		if (!extConn->isInput())
		{
    		ConnectorInputBase * inInternal = (ConnectorInputBase*)extConn->getInternalConn();
    		ConnectorOutputBase * outExternal = (ConnectorOutputBase*) getConnList()->at(i);
    		
    		// Set data to wire
    		const void * pData;
    		pData = inInternal->readoutData();
    		outExternal->copyData(pData);
    	}
    }
}
/** Reset all simulation variables */
void Module::reset()
{
	Component::reset();
	// Call reset for all components
	FOR_EACH_COMP(it,*m_moduleContainer->getComponentList())
	{
		it.current()->reset();
	}
}

	
/** Initialize the component popup menu */
bool Module::initPopupMenu(QPopupMenu * popup)
{
	bool insert = Component::initPopupMenu(popup);
	if (!insert)
		popup->insertSeparator();
	popup->insertItem((const char*)i18n("&Reload module"), this, SLOT(slotReload()));
	return true;
}

/** Executes the reload function */
void Module::slotReload()
{
	reloadModule();
	refresh();
//	getContainer()->routeComponents();
//	getDoc()->slotUpdateAllViews(0);
}

/** Setup the component lists for calculation, updateOutput, updateView.
 	* Call during simulation start. */
void Module::setupSimulationList()
{
	getModuleContainer()->setupSimulationList();
}

	
//###############################################################

ModuleWidget::ModuleWidget(Module * module, CompViewList * viewList, QWidget *parent, const char *name)
	: DisplayWidget(viewList, parent, name),
		m_module(module)
{
}

ModuleWidget::~ModuleWidget()
{
}

/** The event filter has to be installed by compViews based on widgets */
bool ModuleWidget::eventFilter( QObject * obj, QEvent * ev)
{
	if (m_module->getContainer()->isRunning())
	{
		return false;
	}
	else
	{
		switch (ev->type())
		{
			case QEvent::MouseButtonPress:
			case QEvent::MouseButtonRelease:
			case QEvent::MouseButtonDblClick:
			case QEvent::MouseMove:
			{
				QMouseEvent mouseEv = QMouseEvent(  ev->type(),
								 	 				((QWidget*)obj)->mapToParent(((QMouseEvent*)ev)->pos()),
								  					((QMouseEvent*)ev)->button(),
								  					((QMouseEvent*)ev)->state()
								  				 );
				return event(&mouseEv);
			}
			
			default:
				return false;
		}
	}
}
	
void ModuleWidget::reload()
{
	emit QObject::destroyed();
	
	if (m_module->getModuleContainer()->getModuleData()->getModuleView() == MV_USERVIEW)
	{
		FOR_EACH_COMPVIEW(it, *displayList)
		{
			it.current()->makeWidget(this);
		}
	}
}

