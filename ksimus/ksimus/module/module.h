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

class QPainter;
class ExternalConnector;
class KSimWidgetList;

/**
  *@author Rasmus Diekenbrock
  */

extern const ComponentInfo ModuleBaseInfo;

//###############################################################

class ModuleSV : public CompView
{
public:
	ModuleSV(Component * comp, eViewType viewType);
	~ModuleSV();
	virtual void draw(QPainter * p);
	static void drawGeneric(QPainter * p, QRect & place);
	static void drawGeneric(QPainter * p, QSize & size);
	
	/** Creates a new Widget  */
	QWidget * createCompViewWidget(QWidget * parent);
	/** New position of the component view */
	virtual void setPos(const QPoint & pos);
	
	void reload();
	
protected:
	virtual void resize();
	KSimWidgetList * widgetList;

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
	virtual void save(KSimpleConfig & file) const;
	/** load module properties
	*   copyLoad is true, if the load function is used as a copy function
		Returns true if successful */
	virtual bool load(KSimpleConfig & file, bool copyLoad);
	
	/** Checks the component
	*   eg. all required inputs are connected.
	*   The implementation calls the checks functions of the connectors and container.
	*	Returns the number of errors
	*/
	virtual int checkCircuit();
	/** Executes the simulation of this component */
	virtual void calculate();
	/** Shift the result of calculation to output */
	virtual void updateOutput();
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

  /** Setup the component lists for calculation, updateOutput, updateView.
  	* Call during simulation start. */
  void setupSimulationList();

	/** Returns the component container. */
	CompContainer * getContainer() { return container; };

	/** Returns the component container. */
	const CompContainer * getContainer() const { return container; };

protected:
	CompContainer * container;
	QString moduleFile;
	ComponentList * extList;
	
	/** search the connector
	returns 0,if no connector is found */
	static ConnectorBase * searchConn(ExternalConnector * extConn, ConnectorList * connList);

	
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
