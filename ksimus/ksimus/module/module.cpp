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
#include <qobjectlist.h>

#include <kfiledialog.h>
#include <klocale.h>

#include "ksimusdoc.h"
#include "ksimusview.h"
#include "ksimeditor.h"
#include "ksimdata.h"
#include "ksimdebug.h"
#include "module.h"
#include "moduledata.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "connectorinfo.h"
#include "compcontainer.h"
#include "resource.h"
#include "displaywidget.h"
#include "ksimwidget.h"
#include "externalconnector.h"
#include "optionalconnector.h"

static const char * const sModFile = "ModuleFile";

#define minX	8
#define minY	7

//###############################################################


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Module(container, ci);
}

const ComponentInfo * getModuleBaseInfo()
{
	static const ComponentInfo Info(QString::fromLatin1("Module"),
	                                QString::fromLatin1("Module"),
	                                i18n("Component", "Module"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create);
	return &Info;
}

//###############################################################

ModuleSV::ModuleSV(Component * comp, eViewType viewType)
	: CompView(comp, viewType, "ModuleSV")
{
	widgetList = new KSimWidgetList();
	Q_CHECK_PTR(widgetList);

	if(viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, minX * gridX, minY * gridY));
		enableConnectorSpacingRight(true);
		enableConnectorSpacingTop(true);
		enableConnectorSpacingBottom(true);
		enableConnectorSpacingLeft(true);
	}
	else
	{
		setPlace(QRect(0, 0, (minX-2) * gridX, (minY-2) * gridY));
		enableConnectorSpacingRight(false);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
		enableConnectorSpacingLeft(false);
	}
}
ModuleSV::~ModuleSV()
{
	delete widgetList;
}


bool ModuleSV::load(KSimData & file)
{
	bool res = CompView::load(file);

	switch(((Module *) getComponent())->getModuleContainer()->getModuleData()->getModuleView())
	{
		case MV_GENERIC:
		case MV_PIXMAP:
			if ((getViewType() == USER_VIEW) && !isHidden())
			{
				// always hidden !!!
				setHide(true);
			}
			break;

		case MV_USERVIEW:
			// Nothing to do
			break;

		default:
			KSIMDEBUG_VAR("Unknown module view",(int)((Module *) getComponent())->getModuleContainer()->getModuleData()->getModuleView());
			break;
	}

	return res;
}

void ModuleSV::draw(QPainter * p)
{
	CompView::draw(p);
	
	Module * mod = (Module *) getComponent();
	QSize size(getPlace().size());
		
	switch(mod->getModuleContainer()->getModuleData()->getModuleView())
	{
		case MV_GENERIC:
			// Don't draw if sub module
//			if  (mod->getContainer()->isParentDoc())
			if (getViewType() == SHEET_VIEW)
			{
				drawGeneric(p, size);
			}
			break;
			
		case MV_PIXMAP:
			if (getViewType() == SHEET_VIEW)
			{
				p->drawPixmap(gridX, gridY, *mod->getModuleContainer()->getModuleData()->getPixmap());
			}
			break;
			
		case MV_USERVIEW:
			mod->getModuleContainer()->drawUserView(p);
			break;
		
		default:
			KSIMDEBUG_VAR("Unknown module view",(int)mod->getModuleContainer()->getModuleData()->getModuleView());
			break;
	}
}

void ModuleSV::drawGeneric(QPainter * p, QRect & place)
{
	p->save();
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(place.x() + gridX+1, place.y() + gridY+1, place.width()-gridX*2-1, place.height()-gridY*2-1);
	p->drawText(place.x() + gridX+1, place.y() + gridY+1, place.width()-gridX*2-1, place.height()-gridY*2-1,
	            AlignCenter, QString::fromLatin1("Module"));
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
	
//	KSIMDEBUG_VAR("ModuleSV::createCompViewWidget", getComponent()->getName());
	CompViewList * viewList = module->getModuleContainer()->getUserViewList();
	ModuleWidget * display = new ModuleWidget(module, viewList, parent, module->getName());
	Q_CHECK_PTR(display);
	widgetList->addWidget(display);
	
	/* General signals */
	// Delete signal (CompView->Widget)
	QObject::connect(this, SIGNAL(destroyed()), display, SLOT(slotDelete()));
	// Delete signal (Editor->Widget)
	QObject::connect(parent, SIGNAL(destroyed()), display, SLOT(slotDelete()));
	// Move signal (CompView->Widget)
	QObject::connect(this, SIGNAL(signalMoveWidget(const QPoint &)), display, SLOT(move(const QPoint &)));
	// Resize signal (CompView->Widget)
	QObject::connect(this, SIGNAL(signalResizeWidget(const QSize &)), display, SLOT(resize(const QSize &)));

	// Hide signal (CompView->ModuleSV)
	connect(this, SIGNAL(signalHide()), this, SLOT(slotWidgetHide()));
	// Hide signal (ModuleSV->Widget)
	connect(this, SIGNAL(signalWidgetHide()), display, SLOT(hide()));

	// Show signal (CompView->ModuleSV)
	connect(this, SIGNAL(signalShow()), this, SLOT(slotWidgetShow()));
	// Show signal (ModuleSV->Widget)
	connect(this, SIGNAL(signalWidgetShow()), display, SLOT(show()));

	display->reload();
	
	return display;
}

void ModuleSV::drawBound(QPainter * p)
{
	// Draw boundary
	if (((Module*)getComponent())->getModuleContainer()->getModuleData()->getModuleView() == MV_USERVIEW)
	{
		p->save();
		p->setRasterOp (NotROP);
		p->setPen (DashLine);
		p->setBrush (NoBrush);
		p->drawRect (getPlace().left()-1,
	               getPlace().top()-1,
	               getPlace().width()+2,
	               getPlace().height()+2);
		p->restore();
	}
	else
	{
		CompView::drawBound(p);
	}
}

void ModuleSV::reload()
{
	QPtrList<QWidget> * list = widgetList->getWidgetList();
//	KSIMDEBUG_VAR("ModuleSV::reload()", list->count());

	for(QPtrListIterator<QWidget> it(*list);it.current();++it)
	{
		((ModuleWidget*)it.current())->reload();
	}
}


void ModuleSV::slotWidgetShow()
{
	switch(((Module *) getComponent())->getModuleContainer()->getModuleData()->getModuleView())
	{
		case MV_GENERIC:
		case MV_PIXMAP:
			// always hidden !!!
			emit signalWidgetHide();
			break;

		case MV_USERVIEW:
			emit signalWidgetShow();
			// Nothing to do
			break;

		default:
			KSIMDEBUG_VAR("Unknown module view",(int)((Module *) getComponent())->getModuleContainer()->getModuleData()->getModuleView());
			break;
	}
	
}

void ModuleSV::slotWidgetHide()
{
	emit signalWidgetHide();
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
				QMouseEvent mouseEv = QMouseEvent(ev->type(),
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
/*	KSIMDEBUG_VAR("ModuleWidget::reload", m_module->getName());

	KSIMDEBUG_VAR("ModuleWidget::reload()", displayList->count());
	KSIMDEBUG_VAR("ModuleWidget::reload()", m_module->getContainer()->getUserViewList()->count());
	KSIMDEBUG_VAR("ModuleWidget::reload()", m_module->getModuleContainer()->getUserViewList()->count());*/

	// Delete all child objects
	emit QObject::destroyed();

	// Recreate child widgets if user view
	if (m_module->getModuleContainer()->getModuleData()->getModuleView() == MV_USERVIEW)
	{
		FOR_EACH_COMPVIEW(it, *displayList)
		{
			it.current()->makeWidget(this);
		}
	}

/*	if (children())
	{
		QObjectListIt it(*children());
		for (; it.current(); ++it)
		{
			if (it.current()->isWidgetType())
			{
				KSIMDEBUG(QString::fromLatin1("ModuleWidget::reload Module %1 Widget %2").arg(m_module->getName()).arg(QString::fromLatin1(it.current()->name())));
			}
		}
	}*/

/*	// Show display list
	if (displayList)
	{
		FOR_EACH_COMPVIEW(it, *displayList)
		{
			KSIMDEBUG(QString::fromLatin1("ModuleWidget::reload displayList Module %1 Widget %2").arg(m_module->getName()).arg(QString::fromLatin1(it.current()->name())));
		}
	}*/

}

//###############################################################



Module::Module(CompContainer * _container, const ComponentInfo * ci)
	: Component(_container, ci),
		moduleFile()
{
	setComponentType(eModule);
	
	m_moduleContainer = new CompContainer(this);
	Q_CHECK_PTR(m_moduleContainer);
	
	new ModuleSV(this, SHEET_VIEW);
	new ModuleSV(this, USER_VIEW);
	
	extList = new ComponentList;
	Q_CHECK_PTR(extList);
}
Module::~Module()
{
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
	
	Component::save(file);
}

/** load module properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Module::load(KSimData & file, bool copyLoad)
{
	moduleFile = file.readEntry(sModFile);
	
	reloadModule();
	
	return Component::load(file, copyLoad);
}
	
/** reloads the module and recreates the view */
void Module::reloadModule()
{
	PointList * posList = (PointList *)0;
	unsigned int i;
	ModuleData * mdata;

	// remove optional connector if exist
	if (getAddOnList())
	{
		QPtrListIterator<ComponentAddOn> it(*getAddOnList());
		while(it.current())
		{
			if(it.current()->inherits("OptionalConnector"))
			{
				delete it.current();
			}
			else
			{
				++it;
			}
		}
	}

	ConnectorList * oldConnList = new ConnectorList(*getConnList());
	oldConnList->setAutoDelete(false);

	// Remove all information from container
	m_moduleContainer->deleteAll();
	mdata = m_moduleContainer->getModuleData();
	
	// First remove old connectors
	getConnList()->setAutoDelete(false);  //Do not delete connectors
	getConnList()->clear();               //Clear list
	getConnList()->setAutoDelete(true);
	extList->clear();
	
	QFileInfo fileInfo(getModuleFile());
	
	if (!fileInfo.exists())
	{
		logError(i18n("Module file %1 does not exist.").arg(getModuleFile()));
	}
	else if (!fileInfo.isReadable())
	{
		logError(i18n("Module file %1 is not readable.").arg(getModuleFile()));
	}
	else if (fileInfo.isDir())
	{
		logError(i18n("Module file %1 seems to be a directory.").arg(getModuleFile()));
	}
	else if (checkRecursion())
	{
		logError(i18n("Recursion detected in module file %1.\nRecursive modules are not allowed.").arg(getModuleFile()));
	}
	else
	{
		// open file read only
		KSimData file(getModuleFile(), KSimData::versionAsIs, true);

		// Load all components from file
		file.setGroup("/");
		getModuleContainer()->loadProperty(file);
		getModuleContainer()->loadComponents(file);

		setName(mdata->getModuleName());
	}
	
	// Search external connectors
	mdata->searchExternals(m_moduleContainer->getComponentList());

	// Some view type depending checks and setup 
	switch(mdata->getModuleView())
	{
		case MV_GENERIC:
			// Nothing yet
			break;

		case MV_PIXMAP:
			if (mdata->isPixmapFileValid())
			{
				// Calculate and set module size
				mdata->setupPixmapData();
				mdata->loadPixmap();
				if (!mdata->checkConnectorPosition(true))
				{
					mdata->setModuleView(MV_GENERIC);
				}
			}
			else
			{
				logError(i18n("Pixmap file '%1' is not valid! Switch to generic view.").arg(mdata->getPixmapFile()));
				mdata->setModuleView(MV_GENERIC);
			}
			break;

		case MV_USERVIEW:
			// Calculate and set module size
			mdata->setupUserViewData();
			if (!mdata->checkConnectorPosition(true))
			{
				mdata->setModuleView(MV_GENERIC);
			}
			break;
		
		case MV_NONE:
			logError(i18n("File %1 is not a module! Switch to generic view.").arg(getModuleFile()));
			mdata->setModuleView(MV_GENERIC);
			break;

		default:
			KSIMDEBUG(QString::fromLatin1("Unknown module view = %1 Filename = %2").arg((int)mdata->getModuleView()).arg(getModuleFile()));
			mdata->setModuleView(MV_GENERIC);
			break;
	}

	if(getSheetView())
		((ModuleSV*)getSheetView())->reload();
	if(getUserView())
		((ModuleSV*)getUserView())->reload();
	
	m_moduleView = mdata->getModuleView();
	switch(m_moduleView)
	{
		case MV_GENERIC:
		{
			// Calculate and set module size
			mdata->setupGenericData();
			
			// set view size
			if (getSheetView())
			{
				if (getSheetView()->isHidden())
					getSheetView()->setHide(false);
				emit getSheetView()->signalHide();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getGenericSize()));
			}
			
			if (getUserView())
			{
				if (!getUserView()->isHidden())
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
			// set view size
			if (getSheetView())
			{
				if (getSheetView()->isHidden())
					getSheetView()->setHide(false);
				emit getSheetView()->signalHide();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getPixmapSize()));
			}
			else
			{
				KSIMDEBUG_VAR("Module::reload MV_PIXMAP without SheetView", getModuleFile());
			}
			if (getUserView())
			{
				if (!getUserView()->isHidden())
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
			// set view size
			if (getSheetView())
			{
				if (getSheetView()->isHidden())
					getSheetView()->setHide(false);
				emit getSheetView()->signalShow();
				getSheetView()->setPlace(QRect(getSheetView()->getPos(), mdata->getUserViewSize()));
			}
			else
			{
				KSIMDEBUG_VAR("Module::reload MV_USERVIEW without SheetView", getModuleFile());
			}
			if (getUserView())
			{
				getUserView()->setPlace(QRect(getUserView()->getPos(), mdata->getUserViewSize()-QSize(2*gridX,2*gridY)));
			}
			else
			{
				KSIMDEBUG_VAR("Module::reload MV_USERVIEW without UserView", getModuleFile());
			}
			// Create Connectors
			posList = mdata->getUserViewConnPos();
		}
		break;
		
		default:
			KSIMDEBUG(QString::fromLatin1("Unknown module view = %1 Filename = %2").arg((int)mdata->getModuleView()).arg(getModuleFile()));
			break;
	}
	
	Q_CHECK_PTR(posList);
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
				addConnector(conn);
			}
			else
			{
				// The connector doesn't exist. Create it
				conn = extConn->getExternalConn()->getConnInfo()
				        ->create(this,
				                 QString::fromLatin1("(extConn) %1").arg(extConn->getSerialNumber()),
				                 extConn->getName(),
				                 QPoint(0,0));
			}

			// Remove wire from external connector
			if(extConn->getExternalConn()->isConnected())
			{
				extConn->getContainer()->delConnection(extConn->getExternalConn());
			}

			// Create optional conn if required
			if(extConn->isOptionalConn())
			{
				OptionalConnector * oc;
				oc = new OptionalConnector(conn, QString::fromLatin1("OptionalExtCon ")+conn->getName(),
				                           conn->getName());
				Q_CHECK_PTR(oc);
				oc->setEnabled(extConn->isOptionalConnEnabled() || conn->isConnected());
				
			}
			extConn->setModuleConn(conn);
			conn->setNegate(extConn->getExternalConn()->isNegated(),true);
			
			extList->append(extConn);
			
			conn->setGridPos(*(posList->at(i)));
				
			switch(mdata->getModuleView())
			{
				case MV_GENERIC:
					conn->setOrientation(extConn->getExternalConn()->getOrientation());
					break;
				case MV_PIXMAP:
					conn->setOrientation(*mdata->getPixmapConnOrient()->at(i));
					break;
				case MV_USERVIEW:
					conn->setOrientation(*mdata->getUserViewConnOrient()->at(i));
					break;
				default:
					break;
			}
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

bool Module::checkRecursion() const
{
	CompContainer * parentContainer = getContainer();
	
	while(parentContainer && parentContainer->isParentComponent())
	{
		Component * comp = parentContainer->getParentComponent();
		if (!comp)
		{
			KSIMDEBUG("parentContainer->getParentComponent() returns a null pointer!")
			break;
		}
		
		if (!comp->inherits("Module"))
		{
			KSIMDEBUG_VAR("Component should be a module but is not", comp->getName());
			break;
		}
		
		if (((Module *)comp)->getModuleFile() == getModuleFile())
		{
			// Same module file !!!!
			return true;
		}
		parentContainer = comp->getContainer();
	}
	return false;
}

/** search the connector
	returns 0,if no connector is found */
ConnectorBase * Module::searchConn(const ExternalConnector * extConn, const ConnectorList * connList)
{
	const ConnectorInfo * extConnInfo = extConn->getExternalConn()->getConnInfo();
	QString connWireName = QString::fromLatin1("(extConn) %1").arg(extConn->getSerialNumber());
	FOR_EACH_CONNECTOR(it, *connList)
	{
		// Compare connector type and name
		if ((it.current()->getConnInfo() == extConnInfo)
			  &&(connWireName == it.current()->getWireName()))
		{
			// Both have same type and connector name have the correct wire name
			return it.current();
		}
	}
	return (ConnectorBase*)0;
}


/** Searches the external connector which is represented by the given connector.
  * Returns 0 if no external connector will be found. */
ExternalConnector * Module::searchExtConn(const ConnectorBase * conn) const
{
	for(unsigned int i = 0; i < getConnList()->count(); i++)
	{
		if (getConnList()->at(i) == conn)
		{
			return (ExternalConnector*) extList->at(i);
		}
	}
	
	return  (ExternalConnector*) 0;
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

/** Setup the Component for a new circuit execution.
  * Calls the default implementation and the setup function of all containing components.
  */
void Module::setupCircuit()
{
	Component::setupCircuit();
	
	m_moduleContainer->setupCircuit();
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
	popup->insertItem(i18n("&Reload module"), this, SLOT(slotReload()));
	return true;
}

/** Executes the reload function */
void Module::slotReload()
{
	reloadModule();
	refresh();
}

/** Setup the component lists for calculation, updateView.
 	* Call during simulation start. */
void Module::setupSimulationList()
{
	getModuleContainer()->setupSimulationList();
}

#include "module.moc"
