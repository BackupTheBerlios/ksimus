/***************************************************************************
                          module.h  -  description
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

#ifndef MODULE_H
#define MODULE_H

// QT-Inlcudes
#include <qstring.h>

// Project-Includes
#include "component.h"
#include "compview.h"
#include "displaywidget.h"
#include "moduledata.h"

class QPainter;
class ExternalConnector;
class KSimWidgetList;

/**
  *@author Rasmus Diekenbrock
  */

const ComponentInfo * getModuleBaseInfo();

//###############################################################

class ModuleSV : public CompView
{

	Q_OBJECT


public:
	ModuleSV(Component * comp, eViewType viewType);
	~ModuleSV();
	virtual void draw(QPainter * p);
	static void drawGeneric(QPainter * p, QRect & place);
	static void drawGeneric(QPainter * p, QSize & size);

	/** load component properties
		Returns true if successful */
	virtual bool load(KSimData & file);
	/** Draw the boundary of the component */
	virtual void drawBound(QPainter * p);
	/** Draws the CompView to a printer. Widget are also drawn. */
	virtual void print(QPainter * paint);
	
	/** Creates a new Widget  */
	QWidget * createCompViewWidget(QWidget * parent);
	
	void reload();
	
protected:
	KSimWidgetList * widgetList;

private slots:
	void slotWidgetShow();
	void slotWidgetHide();

signals:
	void signalWidgetShow();
	void signalWidgetHide();

};

//###############################################################

class CompContainer;

class Module : public Component  {

	Q_OBJECT

friend class ModuleSV;
friend class ModuleWidget;

public:
	Module(CompContainer * container, const ComponentInfo * ci);
	~Module();

	/** save module properties */
	virtual void save(KSimData & file) const;
	/** load module properties
	*   copyLoad is true, if the load function is used as a copy function
		Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Checks the component
	*   eg. all required inputs are connected.
	*   The implementation calls the checks functions of the connectors and container.
	*	Returns the number of errors
	*/
	virtual int checkCircuit();
	/** Setup the Component for a new circuit execution.
	  * Calls the default implementation and the setup function of all containing components.
	  */
	virtual void setupCircuit();
	/** Reset all simulation variables */
	virtual void reset();
	/** Set the module file and load it */
	void setModuleFile(const QString & fileName);
	/** Returns the current module file */
	const QString & getModuleFile() const;

	/** Initialize the component popup menu
	  *	Return true, if items are added */
	virtual bool initPopupMenu(QPopupMenu * popup);
	
	/** reloads the module and recreates the view */
	void reloadModule();
	
	/** Check if this module file is already a part of the schematic.
	  * Returns true, if a parent or any module above this module has the same name.
	  */
	bool checkRecursion() const;

	/** Setup the component lists for calculation, updateView.
		* Call during simulation start. */
	void setupSimulationList();

	/** Returns the component container of this module. */
	CompContainer * getModuleContainer() { return m_moduleContainer; };

	/** Returns the component container of this module. */
	const CompContainer * getModuleContainer() const { return m_moduleContainer; };

	/** Searches the external connector which is represented by the given connector.
	  * Returns 0 if no external connector will be found. */
	ExternalConnector * searchExtConn(const ConnectorBase * conn) const;

	/** returns the module view type.
		Types are: MV_NONE, MV_GENERIC, MV_USERVIEW, MV_PIXMAP. */
	ModuleViewType getModuleView() const { return m_moduleView; };

	/** search the connector
	returns 0,if no connector is found */
	static ConnectorBase * searchConn(const ExternalConnector * extConn, const ConnectorList * connList);

protected:
	CompContainer * m_moduleContainer;
	QString moduleFile;
	ComponentList * extList;
	ModuleViewType m_moduleView;
	

	
protected slots:
	/** Executes the reload function */
	void slotReload();

};

//###############################################################

class ModuleWidget : public DisplayWidget
{
public:
	ModuleWidget(Module * module, CompViewList * viewList, QWidget *parent=0, const char *name=0);
	~ModuleWidget();

	/** The event filter has to be installed by compViews based on widgets */
	virtual bool eventFilter( QObject * obj, QEvent * ev);
	void reload();

protected:
	Module * m_module;
};


#endif
