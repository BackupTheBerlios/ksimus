/***************************************************************************
                          compcontainer.h  -  description
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

#ifndef COMPCONTAINER_H
#define COMPCONTAINER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// C-Includes

// QT-Includes
#include <qobject.h>

// KDE-Includes

// Project-Includes
#include "types.h"

// Forward declaration
class ConnectorBase;
class Wire;
class ComponentMap;
class KSimusApp;
class KSimusDoc;
class KSimUndo;
class KSimData;
class ModuleData;
class LogList;
class Component;
class ComponentList;
class CompView;
class CompViewList;
class KSimTimeServer;

/**
  *@author Rasmus Diekenbrock
  */

class CompContainer : public QObject
{
	Q_OBJECT

public:
	CompContainer(KSimusDoc * parent);
	CompContainer(Component * parent);
	~CompContainer();
	
	/** Insert a component to component list */
	void addComponent(Component * newComp);
	/** Delete a component from component list */
	void delComponent(Component * delComp);
	void delComponent(ComponentList * compList);
	void delComponent(CompViewList * compViewList);
	/** Cuts a component from component list */
	void cutComponent(ComponentList * compList);
	void cutComponent(CompViewList * compViewList);
	/** Move a component */
	void moveComponent(Component * comp, const QPoint & relMove);
	void moveComponent(ComponentList * compList, const QPoint & relMove);
	void moveComponent(CompViewList * compviewList, const QPoint & relMove);
	/** Copies components */
	void copyComponent(ComponentList * compList);
	void copyComponent(CompViewList * compViewList);
	/** Pasts components */
	void pastComponent(ComponentList * compList, const QPoint & relMove);
	
	/** Add a connection */
	void addConnection(ConnectorBase * start, ConnectorBase * end);
	void delConnection(ConnectorBase * delConn);
	
	
	/** Search a copmponent by the given serial number
		Returns a null pointer if no component found */
	Component * searchComponentBySerialNumber(unsigned int number);
	
	
	/** returns the size of the sheet view */
	QSize getSheetSize() const;
	/** Set the Dimension of the sheet view */
	void setSheetSize(QSize size);
	/** returns the size of the user view */
	QSize getUserSize() const;
	/** Set the Dimension of the user view */
	void setUserSize(QSize size);
	
	ComponentList * getComponentList() const { return components; };
	ComponentList * getCalculateComponentList() const { return calculateComponents; };
	ComponentList * getUpdateOutputComponentList() const { return updateOutputComponents; };
	CompViewList * getUpdateSheetViewComponentList() const { return updateSheetViewComponents; };
	CompViewList * getUpdateUserViewComponentList() const { return updateUserViewComponents; };
	CompViewList * getSheetViewList() const { return sheetViews; };
	CompViewList * getUserViewList() const { return userViews; };
	ComponentMap * getSheetMap() const { return sheetMap; };
	ComponentMap * getUserMap() const { return userMap; };
	/** Returns the first connector that's found by isCompViewHit.
		Returns a null if no connector is hit.
	*/
	ConnectorBase * getFirstConnector() const { return hitConn; };
	/** Returns the first component view that's found by isCompViewHit.
		Returns a null if no component view is hit.*/
	CompView * getFirstCompView() const { return hitCompView; };
	/**  */
	eHitType isCompViewHit(const QPoint * pos, const CompViewList * viewList);
	/** Returns a pointer to the document */
	KSimusDoc * getDoc() const;
	/** Returns the undo object */
	KSimUndo * getUndo() const;
	/** Returns a pointer to the module component
	* Returns NULL, if this is the top level conatiner
	*/
	Component * getParentComponent() const;
	/** Returns a pointer to the application */
	KSimusApp * getApp() const;
	/** Returns a pointer to the log list */
	LogList * getLogList() const;
	/** Returns the simulation timer */
	const KSimTimeServer & getTimeServer() const;
	/* Draw the components */
	void drawSheetView(QPainter * p) const;
	void drawUserView(QPainter * p) const;
	/** Load components from file
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	bool loadComponents(KSimData & file, bool copyLoad = false);
	/** Saves the component */
	bool saveComponents(KSimData & file, ComponentList * compList) const;
	/** Loads the complete sheet */
	bool load(KSimData & filename);
	/** Saves the properties */
	bool saveProperty(KSimData & filename) const;
	/** Loads the properties */
	bool loadProperty(KSimData & filename);
	/** Saves the complete sheet */
	bool save(KSimData & filename) const;
	/** Deletes all components */
	void deleteAll();
	/** Seareches for new routes */
	bool routeComponents();
	/** Returns a new unique number for a component */
	unsigned int newSerialNumber();
	/** True, if the container components are visible.
		Switches on/off routing and something else that's only requiered
		if components are displayed
	*/
	void setVisible(bool enable);
	/** returns true, if the container component is display on a view */
	bool isVisible() const;
	/**  */
	bool isCostMapVisible() const;

	bool isRunning() const;
	/**  */
	void setCostMapVisible(bool enable);

	void enableRouting(bool enable) { routingEnabled = enable; };
	bool isRoutingEnabled() const { return routingEnabled; };
	
	void setRefreshEnabled(bool enable) { m_refreshEnabled = enable; };
	bool isRefreshEnabled() const { return m_refreshEnabled; };
	
	void refresh();
	
	/** Checks the circuit and returns the result. Returns the number of errors. */
	int checkCircuit();
	/** Resets all components */
	void resetComponents();
	/** Truncate the wire at the given position  */
	void truncateWire(Wire * wire, const QPoint & pos);
	/** Truncate the wire at the given position  */
	void truncateWire(Wire * wire, int x, int y);
	/** the returned rect contains all components excluding wires
	    if cvList contains no valid components, a rect with dimension 0,0 is returned */
	static QRect getContainRect(const CompViewList * cvList);
	
	/** Returns true, if parent is a KSimusDoc */
	bool isParentDoc() const { return docParent; };
	/** Returns true, if parent is a Component */
	bool isParentComponent() const { return !docParent; };
	/** Returns a pointer to module data
		Creates moduile data if not exist */
	ModuleData * getModuleData() const { return moduleData; };

	/** Shows a status message in the application status line */
	void statusMsg(const QString &text) const;
	/** Shows a help message in the application status line */
	void statusHelpMsg(const QString &text) const;
	/** sets the modified flag for the document after a modifying action on the view connected to the document.*/
	void setModified(bool modified =true);
  /** Setup the component lists for calculation, updateOutput, updateView.
  	* Call during simulation start. */
  void setupSimulationList();
	
	/** Returns the number of components which are containing in this container and childs.
	*/
	unsigned int getComponentNumber() const;


protected:
	/** Used for creating unique Component Numbers */
	unsigned int m_lastSerialNumber;
	ComponentMap * sheetMap;
	ComponentMap * userMap;
	/** Includes all components */
	ComponentList * components;
	
	ComponentList * failedComponents;
	ComponentList * calculateComponents;
	ComponentList * updateOutputComponents;
	CompViewList * updateSheetViewComponents;
	CompViewList * updateUserViewComponents;
	
	CompViewList * sheetViews;
	CompViewList * userViews;
	/** Size of the sheet view */
	QSize sheetSize;
	/** Size of the user view */
	QSize userSize;
	/** Used for module view */
	ModuleData * moduleData;
	
	/** Inserts a component to the component list
		Sort components:
		1. Groups
		2. Normal components / Moduls
		3. Wires
	*/
	void insert(Component * comp);
    /** Insert a view to the given viewList
		Sort component views:
		1. Groups
		2. Normal components / Moduls
		3. Wires
	*/
	static void insertView(CompViewList* cvList, CompView * cv);


	union
	{
		KSimusDoc * doc;
		Component * comp;
	} myParent;

	/** True, if parent is a KSimusDoc */
	bool docParent;

	ConnectorBase * hitConn;	
	CompView * hitCompView;	
	/** true, if components are displayed */
	bool visible;
	bool costMapVisible;
	/** true, if routing is allowed */
	bool routingEnabled;
	/** true, if refresh (routing/redraw) is allowed */
	bool m_refreshEnabled;
	/** calls KSimUndo->remove(comp), if parent a document */
	void undoRemove(Component * comp);
	/** calls KSimUndo->reload(comp), if parent a document */
	void undoReload(Component * comp);
	

signals:
	void signalDelete(Component * comp);
	void signalAdd(Component * comp);
	void signalMove(Component * comp);

};

#endif
