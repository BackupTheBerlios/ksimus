/***************************************************************************
                          component.h  -  description
                             -------------------
    begin                : Thu Mar 16 2000
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

#ifndef COMPONENT_H
#define COMPONENT_H

// Include QT-Header
#include <qptrlist.h>
#include <qobject.h>

// Include KDE-Header

// Include Project-Header
#include "types.h"
#include "ksimaction.h"
#include "componentinfo.h"


// Forward declaration
class KSimData;
class QStringList;
class QPopupMenu;
class KSimusDoc;
class CompContainer;
class ComponentMap;
class CompView;
class LogList;
class KSimUndo;
class ConnectorList;
class ConnectorBase;
class ComponentPropertyDialog;
class ComponentPropertyBaseWidget;
class KSimTimeServer;
class KSimTime;
class ComponentAddOnList;
class KInstance;
class PackageInfo;
class WatchWidget;

/**Base class for all components
  *@author Rasmus Diekenbrock
  */

class Component : public QObject
{
	Q_OBJECT

friend class CompView;
friend class ComponentAddOn;

class Private;

// Public attributes
public:

	// Type definition.
	// Gives also the sorting order of components and views
	enum eComponentType {
	                       eGroup             = 0x1000,  // is a group
	                       eComponent         = 0x2000,  // is a simple component
	                       eExternalConnector = 0x3000,  // is a external connector
	                       eModule            = 0x4000,  // is a module
	                       eGuiComponent      = 0x5000,  // is a GUI component
	                       eWire              = 0x6000   // is a wire
	                    };
	
	
	Component(CompContainer * container, const ComponentInfo * ci);
	virtual ~Component();
	
	/** Returns the component type */
	const QString & getType() const;
	/** Returns true, if the parameter type is a proper reload type. This is useful if a component
	  * supports more than one types and the type decides the different functionality (e.g.
	  * @ref BooleanButton). The default implementation returns true, if type is equal to the current
	  * component type.*/
	virtual bool isProperReloadType(const QString & type) const;
	
	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	/** The sheet view of the component */
	CompView * getSheetView() const { return m_sheetView; };
	/** The user view of the component */
	CompView * getUserView() const { return m_userView; };
	/** Set the number of the component.
	  * This number have to be unique in the document.
	  * @see CompContainer
	  * @see CompContainer::newSerialNumber
	  * @see Component::setSerialNumber
	  */
	void setSerialNumber(unsigned int no) { m_serialNumber=no; };
	/** Get the number of the component.
	* @see Component::setSerialNumber
	*/
	unsigned int getSerialNumber() const { return m_serialNumber; };
	
	/** Add a connector to the connector list and set the serial ID of the connector (if required). */
	void addConnector(ConnectorBase * conn);
	
	ConnectorList * getConnList() const { return m_connList; };
	
	ComponentAddOnList * getAddOnList() { return m_addonList; };
	const ComponentAddOnList * getAddOnList() const { return m_addonList; };
	
	CompContainer * getContainer() const { return m_container; };
	
	/** Returns the related document */
	KSimusDoc * getDoc() const;
	
	/** Returns the module depth. This means how many modules are 'between' this
	  * component and the top level document.
	  * Returns 0 if the component is not a member of  module.*/
	 unsigned int getModuleDepth() const;
		
	/** Returns the log window */
	LogList * getLogList() const;
	/** Returns a pointer to the watch widget. */
	WatchWidget * getWatchWidget() const;
	/** Returns the undo object */
	KSimUndo * getUndo() const;
	/** Returns the simulation timer */
	const KSimTimeServer & getTimeServer() const;
	/** Executes the undo functionality (save this component) */
	void undoChangeProperty(const QString & description) const;
	/** Shows a status message in the application status line */
	void statusMsg(const QString &text) const;
	/** Shows a help message in the application status line */
	void statusHelpMsg(const QString &text) const;
	
	/** Set the document modified */
	void setModified() const;
	/** Reroute all component and update views */
	void refresh() const;
	
	void setContainer(CompContainer * parent);
	/** Returns true, if component is a wire.  */
	bool isWire() const;
	/** Returns true, if component is a module.  */
	bool isModule() const;
	/** Returns true, if component is an external connector.  */
	bool isExtConn() const;
	/** Returns true, if component is a group container.  */
	bool isGroup() const;
	/** Returns true, if component is a GUI component.  */
	bool isGuiComp() const;
	
	/** Returns true, if simulation is running. */
	bool isRunning() const;
	
	/** Checks the component
	*   Eg. all required inputs are connected.
	*   The default implementation calls the checks functions of the connectors.
	*   Returns the number of errors.
	*/
	virtual int checkCircuit();
	
	/** Setup the Component for a new circuit execution.
	  * Calls the setup functions of the connectors.
	  */
	virtual void setupCircuit();
	
	/** Checks the component property. The functions is called after the
	*   property dialog.
	*   Eg. all connectors have unique names.
	*   The default implementation checks the connector names and calls the checks functions of the connectors.
	*
	*   Append the message to errorMsg.
	*/
	virtual void checkProperty(QStringList & errorMsg);
	
	/** Executes the property check.
	*
	* Calls @ref checkProperty and evaluates the result. The function does nothing without detected errors.
	* With one or more error the error messages will be display in the log list and in a message box and a
	* (hidden) undo will be executed.
	*
	* Don't forget to serve the undo mechanism *before* calling this function.
	*
	* The function returns the number of errors.
	*/
  unsigned int executePropertyCheck();
	
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Reset all simulation variables */
	virtual void reset();

	/** Adds the component in the list for execute next cycle. */
	void executeNext();
	/** Adds the component in the timed list. timerNo is any magic number to differentiate between different timers.
	    In this way you can manage more than one timer. */
	void executeAt(unsigned int timerNo, const KSimTime & time);
	/** Adds the component in the timed list. timerNo is any magic number to differentiate between different timers.
	    In this way you can manage more than one timer. */
	void executeAfter(unsigned int timerNo, const KSimTime & diffTime);
	
	
	/** Returns the name of the component */
	QString getName() const;
	/** Sets the name of the component. The default name is used if newName is empty */
	void setName(const QString & newName);
	/** Returns the default name of the component.
	  * This is: "initName serialNumber"
	  */
	QString getDefaultName() const;
	/** Returns true, if the component uses the default name .*/
	bool hasDefaultName() const;
	/** Returns the name of the component.
	* If component is member of a module, the returned name includes the module name. */
	QString requestTopLevelName() const;
	/** Returns a pointer to the top level component.
	* A top level component is member of the top level container.
	* If the component is a member of a module, the pointer of the top level module is returned.
	*/
	Component * getTopLevelComponent();
	
	/** Init the property dialog */
	virtual void initPropertyDialog(ComponentPropertyDialog * dialog);
	/** Adds the connector property pages to the property dialog.
		This function is called by @ref initPropertyDialog()*/
	void addConnectorProperty(ComponentPropertyDialog * dialog);
	/** Add the general property page to the property dialog
		This function is called by @ref initPropertyDialog.*/
	void addGeneralProperty(ComponentPropertyDialog * dialog);
	/** Creates the general property page for the property dialog.
	  * Overload this function if you want to use a modified General Propery Page. Use as base class
	  * @ref ComponentPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);
	/** Add the info page to the property dialog.
		This function is called by @ref initPropertyDialog.*/
	void addInfoProperty(ComponentPropertyDialog * dialog);
	/** Creates the info property page for the property dialog.
	  * Overload this function if you want to use a modified Info Propery Page. Use as base class
	  * @ref ComponentPropertyGeneralWidget.
	  * This function is called by @ref addInfoProperty*/
	virtual ComponentPropertyBaseWidget * createInfoProperty(QWidget *parent);
	
	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** Is called after execution of the PopupMenu (@ref initPopupMenu) or PropertyDialog
	  * (@ref initPropertyDialog). Use this function to adjust things which are difficult
	  * to handle inside a slot or the function @ref PropertyWidget::acceptPressed.
	  *
	  * For example it is problematic to change the connector count inside the PropertyDialog
	  * because each connector has also a property widget. If you reduce the connector count
	  * some of these propert widgets have no valid connector. This function delays the connector
	  * count modification until all property widgets are removed.
	  *
	  * The default implementation calls the @ref ComponentAddOn::menuExecuted functions.
	  */
	virtual void menuExecuted();
	
	/** Returns a pointer to the sheet map from the parent conatiner */
	ComponentMap * getSheetMap() const;
	/** Returns a pointer to the sheet map from the parent conatiner */
	ComponentMap * getUserMap() const;

	/** Search a connector by the given wire name (@ref ConnectorBase:getWireName).
		Return null, if connector is not found */
	ConnectorBase * searchConnector(const QString & wireName) const;
	
	/** returns the info class for this component */
	const ComponentInfo * getInfo() const { return m_info; };
	/** Sets the info class for this component */
	void setInfo(const ComponentInfo * info) { m_info = info; };
	/** Returns the package info. Returns a null pointer if no package info is avaible.*/
	const PackageInfo * getPackageInfo() const;
	/** Returns the instance of the package. Returns a null pointer if no instance is avaible.*/
	KInstance * getInstance() const;

	
	/** Appends a info to the log list.
		*	The component is selectable during log list view. If the component is part of a module
		* the module is selectable. */
	void logInfo(const QString & msg);	
	/** Appends a warning to the log list.
		*	The component is selectable during log list view. If the component is part of a module
		* the module is selectable. */
	void logWarning(const QString & msg);	
	/** Appends a error to the log list.
		*	The component is selectable during log list view. If the component is part of a module
		* the module is selectable. */
	void logError(const QString & msg);	
  /** Returns the actions object. In this object describes all of this addon required actions. */
	KSimAction & getAction() { return m_myActions; };
	
	/** Returns the component type.
	  * @see eComponentType */
	eComponentType getComponentType() const;
	
	/** Returns true, if this is a "Zero Delay" component.
	  * @see setComponentType. */
	bool isZeroDelayComponent() const;

		
	static const char * const sSerialNumber;
	static const char * const sType;
	static const char * const sName;

// Protected attributes
protected:
	/** Sets the component type.
	  * @see eComponentType */
	void setComponentType(eComponentType newType);
	
	/** Declare this component as a "Zero Delay" component.
	  * Zero Delay components have no internal delay like a @ref ExternalConnector or
	  * a converter.
	  * Regular components are no zero delay components. So use this function only if
	  * you know what are are doing. */
	void setZeroDelayComponent(bool zeroDelay);
	
private:
	/** The sheet view of the component */
	CompView * m_sheetView;
	/** The user view of the component */
	CompView * m_userView;
	/** the container of this conmponent */
	CompContainer * m_container;
	/** Component name */
	QString m_name;
	/** The serial number. This number is unique at the current container. */
	unsigned int m_serialNumber;
	/** List of all connectors of the conponent */
	ConnectorList * m_connList;
	/** Pointer to the info object */
	const ComponentInfo * m_info;
	
	/** The component type. */
	eComponentType m_componentType;
	/** The list of all component add ons. */
	ComponentAddOnList * m_addonList;
	/** The available actions of this component. */
	KSimAction m_myActions;
	
	/** Private attributes. */
	Private * m_p;

	/** Internal helper function. */
	static void addLog(Component * comp, unsigned int priority, const QString & msg);


signals:
	void signalSetName(const QString & newName);


};

class ComponentList : public QPtrList<Component>
{
	public:
	
	void insertComponent(Component * comp);
};



#define FOR_EACH_COMP(_it_,_compList_)	\
		for(QPtrListIterator<Component> _it_(_compList_);_it_.current();++_it_)


	
		
	
#endif
