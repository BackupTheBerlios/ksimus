/***************************************************************************
                          compcontainer.cpp  -  description
                             -------------------
    begin                : Tue Apr 4 2000
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

#include <limits.h>

#include <qmessagebox.h>
#include <qpainter.h>

#include <ksimpleconfig.h>
#include <klocale.h>

#include "compcontainer.h"

#include "globals.h"
#include "resource.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "component.h"
#include "componentmap.h"
#include "library.h"
#include "componentlibrary.h"
#include "wire.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimdebug.h"
#include "ksimeditor.h"
#include "ksimusview.h"
#include "ksimfile.h"
#include "ksimundo.h"
#include "moduledata.h"
#include "module.h"

#include "loglist.h"
#include "llicompsel.h"

#include "ksimgrid.h"

static const char * sPropertyGrp ="Property/";
static const char * sModuleGrp   ="Module/";
static const char * sSheetSize   ="Sheetsize";
static const char * sUserSize    ="Usersize";


class WirePropertyInfo;

CompContainer::CompContainer(KSimusDoc * parent)
	:	QObject(parent),
		m_lastSerialNumber(0)
{
	docParent = true;
	myParent.doc = parent;
	
	sheetMap = new ComponentMap;
	CHECK_PTR(sheetMap);
	userMap = new ComponentMap;
	CHECK_PTR(userMap);
	components = new ComponentList;
	CHECK_PTR(components);
	components->setAutoDelete(true);
	
	failedComponents = new ComponentList;
	CHECK_PTR(failedComponents);
	failedComponents->setAutoDelete(false);

	calculateComponents = new ComponentList;
	CHECK_PTR(calculateComponents);
	calculateComponents->setAutoDelete(false);

	updateOutputComponents = new ComponentList;
	CHECK_PTR(updateOutputComponents);
	updateOutputComponents->setAutoDelete(false);

	updateSheetViewComponents = new CompViewList;
	CHECK_PTR(updateSheetViewComponents);
	updateSheetViewComponents->setAutoDelete(false);

	updateUserViewComponents = new CompViewList;
	CHECK_PTR(updateUserViewComponents);
	updateUserViewComponents->setAutoDelete(false);

	sheetViews = new CompViewList;
	CHECK_PTR(sheetViews);
	sheetViews->setAutoDelete(false);

	userViews = new CompViewList;
	CHECK_PTR(userViews);
	userViews->setAutoDelete(false);

	moduleData = new ModuleData(this);
	CHECK_PTR(moduleData);
		
	setVisible(true);
	enableRouting(true);
	setCostMapVisible(false);
	
	hitConn = 0;
	hitCompView = 0;
}

CompContainer::CompContainer(Component * parent)
	:	QObject(parent),
		m_lastSerialNumber(0)
{
	docParent = false;
	myParent.comp = parent;
	
	sheetMap = new ComponentMap;
	CHECK_PTR(sheetMap);
//	sheetMap = 0;
	userMap = new ComponentMap;
	CHECK_PTR(userMap);
	components = new ComponentList;
	CHECK_PTR(components);
	components->setAutoDelete(true);
	
//	failedComponents = new ComponentList;
//	CHECK_PTR(failedComponents);
//	failedComponents->setAutoDelete(false);
	failedComponents = 0;
	
	calculateComponents = new ComponentList;
	CHECK_PTR(calculateComponents);
	calculateComponents->setAutoDelete(false);

	updateOutputComponents = new ComponentList;
	CHECK_PTR(updateOutputComponents);
	updateOutputComponents->setAutoDelete(false);

	updateSheetViewComponents = new CompViewList;
	CHECK_PTR(updateSheetViewComponents);
	updateSheetViewComponents->setAutoDelete(false);

	updateUserViewComponents = new CompViewList;
	CHECK_PTR(updateUserViewComponents);
	updateUserViewComponents->setAutoDelete(false);

	sheetViews = new CompViewList;
	CHECK_PTR(sheetViews);
	sheetViews->setAutoDelete(false);
//	sheetViews = 0;

	userViews = new CompViewList;
	CHECK_PTR(userViews);
	userViews->setAutoDelete(false);

	moduleData = new ModuleData(this);
	CHECK_PTR(moduleData);
	
	setVisible(false);
	enableRouting(false);
	setCostMapVisible(false);
	
	hitConn = 0;
	hitCompView = 0;
}

CompContainer::~CompContainer()
{
	if (components)
		delete components;
	if (sheetMap)
		delete sheetMap;
	if (userMap)
		delete userMap;
	if (failedComponents)
		delete failedComponents;
	if (calculateComponents)
		delete calculateComponents;
	if (updateOutputComponents)
		delete updateOutputComponents;
	if (updateSheetViewComponents)
		delete updateSheetViewComponents;
	if (updateUserViewComponents)
		delete updateUserViewComponents;
	if (sheetViews)
		delete sheetViews;
	if (userViews)
		delete userViews;
	if (moduleData)
		delete moduleData;
}
	
/** Returns a pointer to the document */
KSimusDoc * CompContainer::getDoc() const
{
	if (docParent)
	{
		return myParent.doc;
	}
	else
	{
		return myParent.comp->getDoc();
	}
}

/** Returns the undo object */
KSimUndo * CompContainer::getUndo() const
{
	return getDoc()->getUndo();
}

/** Returns a pointer to the module component
* Returns NULL, if this is the top level conatiner
*/
Component * CompContainer::getParentComponent() const
{
	if (docParent)
	{
		return 0;
	}
	else
	{
		return myParent.comp;
	}
}	

/** Returns a pointer to the application */
KSimusApp * CompContainer::getApp() const
{
	return getDoc()->getApp();
}

/** Returns a pointer to the log list */
LogList * CompContainer::getLogList() const
{
	return getApp()->getLogList();
}

const KSimTimeServer & CompContainer::getTimeServer() const
{
	return getDoc()->getTimeServer();
}

/** calls KSimUndo->remove(comp), if parent a document */
void CompContainer::undoRemove(Component * comp)
{
	if (docParent)
	{
		// Add to undo
		getDoc()->getUndo()->remove(comp);
	}
}
/** calls KSimUndo->reload(comp), if parent a document */
void CompContainer::undoReload(Component * comp)
{
	if (docParent)
	{
		// Add to undo
		getDoc()->getUndo()->reload(comp);
	}
}

/** Inserts a component to the component list
	Sort components:
	1. Groups
	2. Normal components / Moduls
	3. Wires
*/
void CompContainer::insert(Component * comp)
{
	unsigned int i;
	
	// Todo Add Group
	
	if (!comp->isWire())
	{
		// Add component in front of wires
		// Todo: Optimize search function
		i = 0;
		while ((i<components->count()) && (!components->at(i)->isWire()))
		{
			i++;
		}
		components->insert(i,comp);
	}
	else
	{
		// Add wire at the end
		components->append(comp);
	}	
		
	
	// Add component views
	if (comp->getSheetView())
	{
		insertView (sheetViews, comp->getSheetView());
	}
	if (comp->getUserView())
	{
		insertView (userViews, comp->getUserView());
	}
}
	
/** Insert a view to the given viewList
	Sort component views:
	1. Groups
	2. Normal components / Moduls
	3. Wires
*/
void CompContainer::insertView(CompViewList* cvList, CompView * cv)
{
	unsigned int i;
	
	// Todo: Add Group view
	
	if (!cv->getComponent()->isWire())
	{
		// Add component view in front of the first wire view
		// Todo: Optimize search function
		i = 0;
		while ((i<cvList->count()) && (!cvList->at(i)->getComponent()->isWire()))
		{
			i++;
		}
		cvList->insert(i,cv);
	}
	else
	{
		// Insert Wire view at the end
		cvList->append(cv);
	}
}


/** Insert a component to component list without undo */
void CompContainer::addComponent(Component * newComp)
{
	// Nullpointer not allowed
	ASSERT(newComp);
	// Insert one component only one time
	ASSERT(-1 == components->find(newComp));

	// Has valid serial number ?
	if (newComp->getSerialNumber() == 0)
	{
	    // Search and set serial number
		newComp->setSerialNumber(newSerialNumber());
	}
	newComp->setContainer(this);
	insert(newComp);
	
	// Add to undo
	undoRemove(newComp);
	
	if (isVisible())
	{
		// Exist a sheet view
		if (newComp->getSheetView())
		{
			if (getDoc()->getActiveEditor()->getEditorView() != EV_SHEETVIEW)
			{
				newComp->getSheetView()->moveToBestPlace();
			}
		}
		// Exist a user view
		if (newComp->getUserView())
		{
			if (getDoc()->getActiveEditor()->getEditorView() != EV_USERVIEW)
			{
				newComp->getUserView()->moveToBestPlace();
			}
		}
	}
	emit signalAdd(newComp);
}

/** Delete a component to component list */
void CompContainer::delComponent(Component * delComp)
{
	// Nullpointer not allowed
	ASSERT(delComp);
	// Exist component in list
	ASSERT(-1 != components->find(delComp));

	ConnectorBase *conn;
	
	emit signalDelete(delComp);

	// Add to undo
	undoReload(delComp);
	if (!delComp->isWire())
	{
		FOR_EACH_CONNECTOR(it, *delComp->getConnList())
		{
			if (it.current()->getWire())
			{
				// Add to undo
				undoRemove(it.current()->getWire());
				undoReload(it.current()->getWire());
			}
		}
	}
	
	if (delComp->getSheetView())
	{
	    sheetViews->removeRef(delComp->getSheetView());
		if (isVisible())
		{
//			delComp->getSheetView()->updateSheetMap(false);
		}
	}
	if (delComp->getUserView())
	{
	    userViews->removeRef(delComp->getUserView());
		if (isVisible())
		{
//			delComp->getUserView()->updateSheetMap(false);
		}
	}

	// remove connections (do not if wire)
	if (!delComp->isWire())
	{
		for (conn = delComp->getConnList()->first(); conn;
			 conn = delComp->getConnList()->next())
		{
			if (0 != conn->getWire())
			{
				delConnection(conn);
			}
		}
	}

    // remove component
    components->removeRef(delComp);
}
void CompContainer::delComponent(ComponentList * compList)
{
	ComponentList listTemp = *compList;
	Component * comp;
	copyComponent(&listTemp);
	
	QListIterator<Component> it(*compList);
	while(it.toFirst())
	{
		comp = it.toFirst();
		delComponent(comp);
		compList->removeRef(comp);
	}
}
void CompContainer::delComponent(CompViewList * compViewList)
{
	CompViewList listTemp = *compViewList;
	CompView * cv;
	copyComponent(&listTemp);
	
	QListIterator<CompView> it(*compViewList);
	while(it.toFirst())
	{
		cv = it.toFirst();
		delComponent(cv->getComponent());
		compViewList->removeRef(cv);
	}
}
/** Move a component */
void CompContainer::moveComponent(ComponentList * compList, QPoint * relMove)
{
	FOR_EACH_COMP(it,*compList)
	{
		CompView * cv = it.current()->getSheetView();
		cv->setPos(cv->getPos() + *relMove);
	}
}
void CompContainer::moveComponent(CompViewList * compViewList, QPoint * relMove)
{
	FOR_EACH_COMPVIEW(it,*compViewList)
	{
		it.current()->setPos(it.current()->getPos() + *relMove);
	}
}
/** Move a component */
void CompContainer::moveComponent(Component * comp, QPoint * relMove)
{
	CompView * cv;
	if (getDoc()->getActiveEditor()->getEditorView() == EV_SHEETVIEW)
	{	
		cv = comp->getSheetView();
	}
	else
	{	
		cv = comp->getUserView();
	}
	
	cv->setPos(cv->getPos() + *relMove);
	emit signalMove(comp);

}

/** Copies components */
void CompContainer::copyComponent(ComponentList * compList)
{
	CHECK_PTR(getDoc());
	
	QString fileName = getDoc()->getFiles()->getCopyPastFilename();
	
	QListIterator<Component> it(*compList);
	
	// If file exists, remove it
	if (QFile::exists(fileName))
	{
		bool res = QFile::remove(fileName);
		if (!res)
		{
			QMessageBox::critical (0, i18n("File Error"),
									  i18n("Cant remove file :") + fileName +
									  i18n("\n\nAbort copy/past function."),
									QMessageBox::Abort,0,0);
			return;
		}
	}
	
	// Search and include wires
	for (it.toFirst(); it.current(); ++it)
	{
		Component * comp = it.current();
		if (!comp->isWire())
		{
			FOR_EACH_CONNECTOR(itConn, *comp->getConnList())
			{
				ConnectorBase * conn =itConn.current();
				if (0 != conn->getWire())
				{
					if (!compList->containsRef(conn->getWire()))
					{
						compList->append(conn->getWire());
					}
				}
			}
		}
	}

	KSimpleConfig file (fileName);
	saveComponents(file, compList);
}

void CompContainer::copyComponent(CompViewList * compViewList)
{
	ComponentList compList;
	FOR_EACH_COMPVIEW(it,*compViewList)
	{
		compList.append(it.current()->getComponent());
	}
	copyComponent(&compList);
}

/** Pasts components */
void CompContainer::pastComponent(ComponentList * compList, QPoint * relMove)
{
	CHECK_PTR(getDoc());
		
	QString fileName = getDoc()->getFiles()->getCopyPastFilename();
	// If file doesn't exists, error and abort past
	if (!QFile::exists(fileName))
	{
		QMessageBox::critical (0, i18n("File Error"),
								  i18n("Cant find file :") + fileName +
								  i18n("\n\nAbort copy/past function."),
								QMessageBox::Abort,0,0);
		return;
	}
	
	CompContainer * container = new CompContainer(getDoc());
	container->setVisible(false);
	container->enableRouting(false);
//	container->getComponentList()->setAutoDelete(false);
	QListIterator<Component> itNew(*container->getComponentList());
	
	{
		KSimpleConfig file (fileName);
		// Dont track delete wire
		if (getUndo()) getUndo()->pause(true);
		container->loadComponents(file, true);	//copy load
		if (getUndo()) getUndo()->pause(false);
	}

	/* Add Components to component list */
	for (itNew.toFirst(); itNew.current(); ++itNew)
	{
		Component * newComp = itNew.current();
		/* Search wire with one or no connections and delete them */
		if ((newComp->isWire()) && (newComp->getConnList()->count() < 2))
		{
			// Dont track delete wire
			if (getUndo()) getUndo()->pause(true);
			delete newComp;
//			container->delComponent(newComp);
			if (getUndo()) getUndo()->pause(false);
		}
		else
		{
			newComp->setSerialNumber(newSerialNumber());
			newComp->setContainer(this);
			if (newComp->getSheetView())
			{
				newComp->getSheetView()->updateSheetMap(true);
			}
			if (newComp->getUserView())
			{
				newComp->getUserView()->updateSheetMap(true);
			}
			addComponent(newComp);
			if (!newComp->isWire())
			{
				compList->append(newComp);
				moveComponent(newComp, relMove);
				if (isVisible())
					getDoc()->addComponentToEditor(newComp);
			}
		}
	}
	container->getComponentList()->setAutoDelete(false);
	// Dont track delete
	if (getUndo()) getUndo()->pause(true);
	delete container;
	if (getUndo()) getUndo()->pause(false);
}

/** Add a connection */
void CompContainer::addConnection(ConnectorBase * start, ConnectorBase * end)
{
	Wire * wire;
	const WirePropertyInfo * wirePropInfo;
	
	// Do nothing, if start and end the same connector
	if (start == end)
	{
//		getLogList()->logDebug("Start and end the same connector");
		return;
	}
	// or one is a 0-pointer
	if ((start == 0) || (end == 0))
	{
		getLogList()->logDebug("Start or end connector is a null pointer");
		return;
	}
	
	wirePropInfo = Wire::findWireProperty(start,end);
	if (!wirePropInfo)
	{
		getLogList()->error("Data types do not match");
		return;
	}
	
	// create new wire, if both connectors not wired
	if ((start->getWire() == 0) && (end->getWire() == 0))
	{
		if (g_library->getComponentLib()->createComponent((Component **)&wire, this, WireInfo.getLibName()))
		{
			addComponent(wire);
			wire->addConnector(start);
			wire->addConnector(end);
			// Set new wire property
			wire->setProperty(wirePropInfo);
		}
		else
		{
			getLogList()->logDebug("Wire creation failed");
		}
	}
	// Both connectors are wired
	else if ((start->getWire() != 0) && (end->getWire() != 0))
	{
		ConnectorBase * conn;
		//Same wire
		if (start->getWire() == end->getWire())
		{
			// Nothing to do!
//			KSIMDEBUG("Wired with the same wire");
		}
		// Differnet wires
		// move all connections to the new
		// remove old wires
		else if (g_library->getComponentLib()->createComponent((Component **)&wire, this, WireInfo.getLibName()))
		{
//			KSIMDEBUG("Both connectors are wired");
			Wire * startWire = start->getWire();
			Wire * endWire = end->getWire();
			addComponent(wire);
			for (conn = startWire->getConnList()->first();
				 conn;
				 conn = startWire->getConnList()->next())
			{
				wire->addConnector(conn);
			}
			for (conn = endWire->getConnList()->first();
				 conn;
				 conn = endWire->getConnList()->next())
			{
				wire->addConnector(conn);
			}
			// Set new wire property
			wire->setProperty(wirePropInfo);
			
			// Remove old wires
			delComponent(startWire);
			delComponent(endWire);
		}
		else
		{
			getLogList()->logDebug("Wire creation failed");
		}
	}
	// Only "start" connector wired
	else if ((start->getWire() != 0) && (end->getWire() == 0))
	{
//		KSIMDEBUG ("Only start connector wired");
		// Add to undo
		undoRemove(start->getWire());
		undoReload(start->getWire());
		// add end connector to start wire
		start->getWire()->addConnector(end);
		// Set new wire property
		start->getWire()->setProperty(wirePropInfo);
	}
	// Only "end" connector wired
	else if ((start->getWire() == 0) && (end->getWire() != 0))
	{
//		KSIMDEBUG ("Only end connector wired");
		// Add to undo
		undoRemove(end->getWire());
		undoReload(end->getWire());
		// add end connector to start wire
		end->getWire()->addConnector(start);
		// Set new wire property
		end->getWire()->setProperty(wirePropInfo);
	}
	else
	{
		getLogList()->logDebug("connection failed");
	}
}

/**  */
void CompContainer::delConnection(ConnectorBase * delConn)
{
	Wire * wire = delConn->getWire();
	
	undoRemove(wire);
	undoReload(wire);
	
	// Has wire more than 2 connections ?
	if (wire->getConnList()->count() > 2)
	{
		// Remove connection
		wire->delConnector(delConn);
	}
	else
	{
		// Remove wire
		delComponent(wire);
	}
}

/** Search a copmponent by the given number
	Returns a 0-pointer if no component found */
Component * CompContainer::searchComponentBySerialNumber(unsigned int number)
{
	Component * comp;
	for (comp = components->first(); comp; comp = components->next())
	{
		if (comp->getSerialNumber() == number)
			break;
	}
	
	return comp;
}

void CompContainer::drawSheetView(QPainter * p) const
{
	p->save();
	QFont newFont("helvetica",12);
	p->setFont(newFont);

	// Draw grid
	getDoc()->getSheetGrid()->draw(p, getSheetSize());
	
	if (isCostMapVisible())
	{
		sheetMap->draw(p);
	}
	
	FOR_EACH_COMPVIEW(it, *sheetViews)
	{
		it.current()->draw(p);
	}
	p->restore();
}

void CompContainer::drawUserView(QPainter * p) const
{
	p->save();
	QFont newFont("helvetica",12);
	p->setFont(newFont);
	
	// Draw grid
	getDoc()->getUserGrid()->draw(p, getUserSize());
	
	if (isCostMapVisible())
	{
		userMap->draw(p);
	}
	
	FOR_EACH_COMPVIEW(it, *userViews)
	{
		if (!it.current()->isHidden())
			it.current()->draw(p);
	}
	p->restore();
}

eHitType CompContainer::isCompViewHit(const QPoint * pos, const CompViewList * viewList)
{
	eHitType hit = NO_HIT;

    hitConn = 0;
    hitCompView = 0;

	
	FOR_EACH_COMPVIEW(it, *viewList)
	{
		hit = it.current()->isHit(pos->x(),pos->y());
		if (hit != NO_HIT)
		{
			hitCompView = it.current();
			if ((hit == CONNECTOR_HIT) || (hit == WIRE_HIT))
			{
				hitConn = it.current()->getLastHitConnector();
			}
			break;
		}
	}
	return hit;
}

/** Load components from file
	Reload component, if component (ID) exist
	copyLoad is true, if the load function is used as a copy function
	Returns true if successful */
bool CompContainer::loadComponents(KSimpleConfig & file, bool copyLoad)
{
	unsigned int numOfComp;
	unsigned int err = 0;
	QString baseGroup;
	
	// Correct group is selected by caller !!!
	baseGroup = file.group();
	
	numOfComp = file.readUnsignedNumEntry("Components", 0);

	// First load components (no wires)
	for (unsigned int i=0;i < numOfComp; i++)
	{
		Component * comp;
		QString id;
		unsigned int serialNo;
		QString grp;
		grp.sprintf("Component %u/", i);
		file.setGroup(baseGroup + grp);
		id = file.readEntry(Component::sType);
		serialNo = file.readUnsignedNumEntry(Component::sSerialNumber, 0);
		// No Wire ?
		if (id != WireInfo.getLibName())
		{
			bool compExist = false;
			FOR_EACH_COMP(it, *components)
			{
				if (serialNo == it.current()->getSerialNumber())
				{
					if (id == it.current()->getType())
					{
						comp = it.current();
						compExist = true;
					}
					else
					{
						KSIMDEBUG("ERROR compNo exists, different ID");
						KSIMDEBUG_VAR("ID", id);
						KSIMDEBUG_VAR("Typ", it.current()->getType());
						err = 1;
					}
				}
			}						
			if (!compExist)
			{
				if (g_library->getComponentLib()->createComponent(&comp, this, id))
				{
					insert(comp);
					undoRemove(comp);
				}
				else
				{
					getLogList()->error(i18n("Unknown Component %1").arg(id));
					KSIMDEBUG("Create component failed");
					err = 2;
				}
			}
			else
			{
				undoReload(comp);
			}

			if (comp)
			{
				// Load component properties
				comp->load(file, copyLoad);						
			
			    // Insert widget views in editor
		    	if (!compExist && isVisible())
			    {
					CHECK_PTR(getDoc());
					getDoc()->addComponentToEditor(comp);
				}
			}
		}
	}

	// Then load wires
	for (unsigned int i=0;i < numOfComp; i++)
	{
		Component * comp;
		QString id;
		unsigned int serialNo;
		QString grp;
		grp.sprintf("Component %u/", i);
		file.setGroup(baseGroup + grp);
		id = file.readEntry(Component::sType, 0);
		serialNo = file.readUnsignedNumEntry(Component::sSerialNumber, 0);
		// Wire ?
		if (id == WireInfo.getLibName())
		{
			bool compExist = false;
			FOR_EACH_COMP(it, *components)
			{
				if (serialNo == it.current()->getSerialNumber())
				{
					if (id == it.current()->getType())
					{
						comp = it.current();
						compExist = true;
					}
					else
					{
						KSIMDEBUG("ERROR compNo exsits, different ID");
						KSIMDEBUG_VAR("ID", id);
						KSIMDEBUG_VAR("Typ", it.current()->getType());
						err = 1;
					}
				}
			}						
			if (!compExist)
			{
				if (g_library->getComponentLib()->createComponent(&comp, this, id))
				{
					insert(comp);
					undoRemove(comp);
				}
				else
				{
					getLogList()->error(i18n("Unknown Component %1").arg(id));
					KSIMDEBUG("Create component failed");
					err = 2;
				}
			}
			else
			{
				undoReload(comp);
			}
			
			if (comp)
			{
				comp->load(file, copyLoad);
				// Remove wires with none or one connectors
				if (comp->getConnList()->count() < 2)
				{
					delComponent(comp);
				}
			}
		}
	}
	
	file.setGroup(baseGroup);
	routeComponents();
  return true;
}

/** Saves the component */
bool CompContainer::saveComponents(KSimpleConfig & file, ComponentList * compList) const
{
	QString baseGroup = file.group();
	
	// Correct group is selected by caller
	unsigned int numOfComp = file.readUnsignedNumEntry("Components", 0);
	
	file.writeEntry("Components", compList->count() + numOfComp);

	// Save components
	for (unsigned int i=0;i < compList->count(); i++)
	{
		QString grp;
		grp.sprintf("Component %u/", i + numOfComp);
		file.setGroup(baseGroup + grp);
		Component * comp = compList->at(i);
		comp->save(file);
	}
	
	file.setGroup(baseGroup);
	return true;
}
	
/** Loads the complete sheet */
bool CompContainer::load(KSimpleConfig & file)
{
	bool result = true;
	
	result &= loadProperty(file);
	// Load all componets from file
	result &= loadComponents(file);
	
	return result;
}

/** Saves the complete sheet */
bool CompContainer::save(KSimpleConfig & file) const
{
	bool result = true;

	result &= saveProperty(file);
	result &= saveComponents(file, components);
	
	return result;
}

/** Saves the properties */
bool CompContainer::saveProperty(KSimpleConfig & file) const
{
	QString group;
	QString oldGrp = file.group();
	file.setGroup(oldGrp + sPropertyGrp);
	
	file.writeEntry(sSheetSize, getSheetSize());
	file.writeEntry(sUserSize, getUserSize());

	group = file.group();
	file.setGroup(group + sModuleGrp);
	moduleData->save(file);
	file.setGroup(group);

	// TODO: Add more gerneral setings

	file.setGroup(oldGrp);
	return true;
}

/** Loads the properties */
bool CompContainer::loadProperty(KSimpleConfig & file)
{
	QString oldGrp = file.group();
	
	if (file.hasGroup(oldGrp + sPropertyGrp))
	{
		if (docParent)
		{
			// Add to undo
			getDoc()->getUndo()->reloadContainerProperty();
		}
		
		file.setGroup(oldGrp + sPropertyGrp);
		
		QString group;
		QSize defaultSize (1000,504);
		
		// Store Size
		setSheetSize(file.readSizeEntry(sSheetSize, &defaultSize));
		setUserSize(file.readSizeEntry(sUserSize, &defaultSize));
	
		
		group = file.group();
		if (file.hasGroup(group + sModuleGrp))
		{
			file.setGroup(group + sModuleGrp);
			moduleData->load(file);
			file.setGroup(group);
		}
		
		// TODO: Add more gerneral setings

		file.setGroup(oldGrp);
	}
	return true;
}

/** Deletes all components */
void CompContainer::deleteAll()
{
	
	while (components->first())
	{
		delComponent(components->first());
	}
	
	// "Reset" Module Data
	delete moduleData;
	moduleData = new ModuleData(this);
	CHECK_PTR(moduleData);
	
	// Reset component number generator
	m_lastSerialNumber = 0;
	
	hitConn = 0;
	hitCompView = 0;
}
/** Returns a new unique number for a component */
unsigned int CompContainer::newSerialNumber()
{
	QListIterator<Component> it(*components);
	bool found;
	
	m_lastSerialNumber ++;
	
	do
	{
		found = false;
	
		for (it.toFirst(); it.current(); ++it)
		{
			if (it.current()->getSerialNumber() == m_lastSerialNumber)
			{
	    		// Same number is used, try next number
	    		m_lastSerialNumber ++;
				found = true;
				break;
			}
		}
	}
	while (found);
	
	return m_lastSerialNumber;
}
/** returns true, if the container component is display on a view */
bool CompContainer::isVisible() const
{
	return visible;
}
/** True, if the container components are visible.
	Switches on/off routing and something else that's only requiered
	if components are displayed */
void CompContainer::setVisible(bool enable)
{
	visible = enable;
}
/** Searches for new routes */
bool CompContainer::routeComponents()
{
	bool res = true;
	if (isRoutingEnabled())
	{
		for (Component * comp = components->first(); comp; comp = components->next())
		{
			if (comp->isWire())
			{
				((WireSV *)comp->getSheetView())->route();
			}
		}
	}
	return res;
}

void CompContainer::setCostMapVisible(bool enable)
{
	costMapVisible = enable;
}
bool CompContainer::isCostMapVisible() const
{
	return costMapVisible;
}
/**  */
bool CompContainer::isRunning() const
{
	return getDoc()->isSimulationRunning();
}
/** Resets all components */
void CompContainer::resetComponents()
{
	FOR_EACH_COMP(it, *components)
	{
		it.current()->reset();
	}
}

/** Checks the circuit and returns the result
 */
int CompContainer::checkCircuit()
{
	int errorCounter = 0;
	
	FOR_EACH_COMP(it, *components)
	{
		errorCounter += it.current()->checkCircuit();
	}
	
	return errorCounter;
	
}

/** Truncate the wire at the given position  */
void CompContainer::truncateWire(Wire * wire, const QPoint & pos)
{
	truncateWire(wire, pos.x(), pos.y());
}
/** Truncate the wire at the given position  */
void CompContainer::truncateWire(Wire * wire, int x, int y)
{
//	QList<tPointList> routes;
//	QList<tPointList> orgRoutes;	
//	QList<tPointList> breakRoutes;
//	tPointList a,b;
	CPointListList routes;
	CPointListList orgRoutes;	
	CPointListList breakRoutes;
	CPointList a,b;
	int breakRoute=-1;
	int breakSegment=-1;
	
	// Convert to grid
	x /= gridX;
	y /= gridY;

	// Get routes
	orgRoutes = *((WireSV*)wire->getSheetView())->getRouteList();
	
	// Search the break route and break segment
	for (int j = 0; j < (int)orgRoutes.count(); j++)
	{
        if ((breakSegment=WireSV::isWireHit(orgRoutes.at(j),x,y)))
        {
			breakRoute = j;
//			KSIMDEBUG_VAR("truncateWire: break route found", j);
			break;
		}
	}

	// No route on break point found ?
	if (breakRoute == -1)
	{
		KSIMDEBUG("ERROR: no break route found");
		return;
	}
		
	// Special case route 0 - only route 0 connect 2 conntectors
	// any another route connect only 1 connector to another route
	if (breakRoute == 0)
	{
		// Route 0 is truncated
		// Add start positon to wire A
		QPoint * pos;
		pos = new QPoint( * orgRoutes.at(0)->first() );
		a.append(pos);
//		KSIMDEBUG ("default add a");
		// Add end positon to wire B
		pos = new QPoint( * orgRoutes.at(0)->last() );
		b.append(pos);
//		KSIMDEBUG ("default add b");
	}
	else
	{
		// Another route is truncated
		// Add start and end positon of route 0 to wire A
		QPoint * pos;
		pos = new QPoint( * orgRoutes.at(0)->first() );
		a.append(pos);
//		KSIMDEBUG ("default add a");
		pos = new QPoint( * orgRoutes.at(0)->last() );
		a.append(pos);
//		KSIMDEBUG ("default add a");
		// Add end position of the truncated route to wire B
		pos = new QPoint( * orgRoutes.at(breakRoute)->last() );
		b.append(pos);
//		KSIMDEBUG ("default add b");
	}
	// xb,yb = end position of break segment
	int xb = orgRoutes.at(breakRoute)->at(breakSegment)->x();
	int yb = orgRoutes.at(breakRoute)->at(breakSegment)->y();
	
	/*** Add all other routes to wire A or B
			Add the connector (the position) of route to wire A, if:
			-	route is connected to route 0 and route 0 isn't truncated
			-	parent route is connected to route 0 and route 0 isn't truncated
			-	route is connected to the truncated route and the point where the
				route connects the truncated route is in a segment in front of
				the truncated segment
			-	route is connected to the truncated route
				and the point where the	route connects the truncated route is the same segment
				and the point where the	route connects the truncated route is not between
				the truncate point and and point of the truncated segment
			Add the connector (the position) of route to wire B if not add to wire A
	
			parent route: A route where a the "child" route starts, or the parent of the parent, ...
	*/
	
	
	for (int j = 1; j < (int)orgRoutes.count(); j++)
	{
		int searchRoute = j;
		if (j != breakRoute)
		{
			bool found = false;
			bool add_a = false;
			bool add_b = false;
			int seg;
	        do
	        {
				// x1,y1: Start position of this route
				int x1 = orgRoutes.at(searchRoute)->first()->x();
				int y1 = orgRoutes.at(searchRoute)->first()->y();
				found = false;
				for (int i = searchRoute-1; i >= 0; i--)
				{
			        if ((seg = WireSV::isWireHit(orgRoutes.at(i),x1,y1)))
			        {
						found = true;
						if (i != breakRoute)
						{
							// First route (and first route isn't truncated)
							if (i == 0)
							{
			       				// Add to wire A
			       				add_a = true;
							}
							else
							{
								searchRoute = i;
							}
						}
						else
						{
							if (seg < breakSegment)
								// Connect the break route in a segment before break segment
			       				// Add to wire A
			       				add_a = true;
							else if (seg == breakSegment)
							{
								// Connect the break route at break segment
								if (x == x1)
								{
									if (  (y <= y1) && (y1 <= yb)||(y >= y1) && (y1 >= yb) )
					       				// Add to wire B
					       				add_b = true;
									else
					       				// Add to wire A
					       				add_a = true;
								}
								else
								{
									if (  (x <= x1) && (x1 <= xb)||(x >= x1) && (x1 >= xb) )
					       				// Add to wire B
					       				add_b = true;
									else
					       				// Add to wire A
					       				add_a = true;
								}
							
							}
							else
								// Connect the break route in a segment behind break segment
			       				// Add to wire B
			       				add_b = true;
						}
						break;
					}
				}
				ASSERT(found);
			}
			while (found && !(add_a||add_b));
			QPoint * pos;
			pos = new QPoint( * orgRoutes.at(j)->last() );
			if (add_a)
				a.append(pos);
			else if (add_b)
				b.append(pos);
			else
				KSIMDEBUG("add_a==add_b==false");
		}
	}
	
	// Delete the old wire
	delComponent(wire);
	
	// Create new wire A, if required
	if (a.count()>1)
	{
		if (g_library->getComponentLib()->createComponent((Component **)&wire, this, WireInfo.getLibName()))
		{
			addComponent(wire);
		    // Add connectors
		    for (unsigned int i = 0; i < a.count(); i++)
		    {
		    	QPoint pos;
		    	pos.setX( a.at(i)->x()*gridX);
		    	pos.setY( a.at(i)->y()*gridY);
		    	if (CONNECTOR_HIT == isCompViewHit(&pos, sheetViews))
		    	{
		    		wire->addConnector(getFirstConnector());
		    	}
		    	else
		    	{
					KSIMDEBUG("connector not found (A)");
				}
			}
			const WirePropertyInfo * info = Wire::findWireProperty(wire->getConnList());
			if (info)
			{
				wire->setProperty(info);
			}
			else
			{
				KSIMDEBUG("wire property creation failed (A)");
			}
		}
		else
		{
			KSIMDEBUG("wire creation failed (A)");
		}
	}
		
	// Create new wire B, if required
	if (b.count()>1)
	{
		if (g_library->getComponentLib()->createComponent((Component **)&wire, this, WireInfo.getLibName()))
		{
			addComponent(wire);
		    // Add connectors
		    for (unsigned int i = 0; i < b.count(); i++)
		    {
		    	QPoint pos;
		    	pos.setX( b.at(i)->x()*gridX);
		    	pos.setY( b.at(i)->y()*gridY);
		    	if (CONNECTOR_HIT == isCompViewHit(&pos, sheetViews))
		    	{
		    		wire->addConnector(getFirstConnector());
		    	}
		    	else
		    	{
					KSIMDEBUG("connector not found (B)");
				}
			}
			const WirePropertyInfo * info = Wire::findWireProperty(wire->getConnList());
			if (info)
			{
				wire->setProperty(info);
			}
			else
			{
				KSIMDEBUG("wire property creation failed (B)");
			}
		}
		else
		{
			KSIMDEBUG("wire creation failed (B)");
		}
	}
}
/** Set the Dimension of the sheet */
void CompContainer::setSheetSize(QSize size)
{
	if (docParent)
	{
		// Add to undo
		getDoc()->getUndo()->changeSheetSize(sheetSize);
	}
	// Fit to Grid
	size.setWidth(((size.width()+gridX-1)/gridX)*gridX);
	size.setHeight(((size.height()+gridY-1)/gridY)*gridY);
	
	sheetSize = size;
	sheetMap->setSize(QSize(size.width()/gridX, size.height()/gridY));	
	
	// Update map
	FOR_EACH_COMPVIEW(it, *sheetViews)
	{
		it.current()->updateSheetMap(true);
	}
		
	if (docParent)
	{
		for(QListIterator<KSimusView> it(*getDoc()->getViewList());it.current();++it)
		{
			KSimEditor * ed = it.current()->getEditor();
			if (ed->getEditorView() == EV_SHEETVIEW)
			{
				ed->setSize(size);
			}
		}
		routeComponents();
		getDoc()->slotUpdateAllViews(0);
	}
}
/** Set the Dimension of the user */
void CompContainer::setUserSize(QSize size)
{
	if (docParent)
	{
		// Add to undo
		getDoc()->getUndo()->changeUserSize(userSize);
	}
	// Fit to Grid
	size.setWidth(((size.width()+gridX-1)/gridX)*gridX);
	size.setHeight(((size.height()+gridY-1)/gridY)*gridY);
	
	userSize = size;
	userMap->setSize(QSize(size.width()/gridX, size.height()/gridY));	
	
	// Update map
	FOR_EACH_COMPVIEW(it, *userViews)
	{
		it.current()->updateSheetMap(true);
	}
	
	if (docParent)
	{
		for(QListIterator<KSimusView> it(*getDoc()->getViewList());it.current();++it)
		{
			KSimEditor * ed = it.current()->getEditor();
			if (ed->getEditorView() == EV_USERVIEW)
			{
				ed->setSize(size);
			}
		}
		getDoc()->slotUpdateAllViews(0);
	}
}
/** returns the size of the sheet */
QSize CompContainer::getSheetSize() const
{
	return sheetSize;
}
/** returns the size of the user */
QSize CompContainer::getUserSize() const
{
	return userSize;
}

/** the returned rect contains all components excluding wires */
QRect CompContainer::getContainRect(const CompViewList * cvList)
{
	bool empty = true;
	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = INT_MIN;
	int maxY = INT_MIN;
	// Calculate the dimension of all components
	FOR_EACH_COMPVIEW(it, *cvList)
	{
		// do not if wire
		if (!it.current()->getComponent()->isWire())
		{
			empty = false;
			if (minX > it.current()->getPlace().left())
				minX = it.current()->getPlace().left();
			if (maxX < it.current()->getPlace().right())
				maxX = it.current()->getPlace().right();
			if (minY > it.current()->getPlace().top())
				minY = it.current()->getPlace().top();
			if (maxY < it.current()->getPlace().bottom())
				maxY = it.current()->getPlace().bottom();
		}
	};
	if (empty)
		return QRect(0,0,0,0);
	else		
		return QRect(QPoint(minX,minY),QPoint(maxX,maxY));
}

/** Shows a status message in the application status line */
void CompContainer::statusMsg(const QString &text) const
{
	getApp()->slotStatusMsg(text);
}

/** Shows a help message in the application status line */
void CompContainer::statusHelpMsg(const QString &text) const
{
	getApp()->slotStatusHelpMsg(text);
}

/** sets the modified flag for the document after a modifying action on the view connected to the document.*/
void CompContainer::setModified(bool modified)
{
	getDoc()->setModified(modified);
}

/** Setup the component lists for calculation, updateOutput, updateView.
	* Call during simulation start. */
void CompContainer::setupSimulationList()
{
	calculateComponents->clear();
	updateOutputComponents->clear();
	updateSheetViewComponents->clear();
	updateUserViewComponents->clear();
	
	FOR_EACH_COMP(it, *components)
	{
		if (it.current()->getAction().isCalculateEnabled())
		{
			calculateComponents->append(it.current());
		}
		if (it.current()->getAction().isUpdateOutputEnabled())
		{
			updateOutputComponents->append(it.current());
		}
		if (it.current()->getAction().isUpdateViewEnabled())
		{
			if (it.current()->getSheetView())
			{
				updateSheetViewComponents->append(it.current()->getSheetView());
			}
			if (it.current()->getUserView())
			{
				updateUserViewComponents->append(it.current()->getUserView());
			}
		}
		if (it.current()->isModule())
		{
			((Module*)it.current())->setupSimulationList();
		}
	}
}

unsigned int CompContainer::getComponentNumber() const
{
	unsigned int no(getComponentList()->count());
	
	FOR_EACH_COMP(it, *getComponentList())
	{
		if (it.current()->isModule())
		{
			 no += ((Module*)it.current())->getContainer()->getComponentNumber();
		}
	}
	
	return no;
}


	