/***************************************************************************
                          component.cpp  -  description
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

#include <qvbox.h>
#include <qpopupmenu.h>

#include <klocale.h>
#include <kmessagebox.h>

#include "resource.h"

#include "ksimdebug.h"
#include "ksimdata.h"
#include "component.h"
#include "compcontainer.h"
#include "componentinfo.h"
#include "componentaddon.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "ksimusdoc.h"
#include "ksimundo.h"
#include "compview.h"
#include "componentpropertydialog.h"
#include "componentpropertygeneralwidget.h"
#include "componentpropertyinfowidget.h"
#include "llicompsel.h"
#include "loglist.h"
#include "library.h"
#include "componentlibrary.h"
#include "packageinfo.h"



static const char * sSheet = "Sheet/";
static const char * sUser  = "User/";

const char * Component::sSerialNumber = "Serial Number";
const char * Component::sType = "Type";
const char * Component::sName = "Name";


Component::Component(CompContainer * container, const ComponentInfo * ci)
	: QObject(0, ci->getName()),
	  m_sheetView(0),
	  m_userView(0),
	  m_container(container),
	  m_name(i18n(ci->getName().latin1())),
	  m_serialNumber(0),
	  m_info(ci),
	  m_isWire(false),
	  m_isModule(false),
	  m_isExtConn(false),
	  m_isGroup(false),
	  m_addonList(0),
	  m_myActions(KSimAction::ALL)		//Default
{
	m_connList = new ConnectorList;
	m_connList->setAutoDelete(true);
}
	
Component::~Component()
{
	if (m_addonList)
		delete m_addonList;
	if (m_connList)
		delete m_connList;
	if (m_sheetView)
		delete m_sheetView;
	if (m_userView)
		delete m_userView;
}

/** Returns the name of the component.
* If component is member of a module, the returned name includes the module name
* The name is appended to the given QString name */
QString Component::requestTopLevelName() const
{
	if (getContainer()->isParentComponent())
	{
		// Parent is a Module
		return getContainer()->getParentComponent()->requestTopLevelName() + '/' + getName();
	}
	else
	{
		// Parent is the top level container
		return getName();
	}
}
	
/** Returns a pointer to the top level component.
* A top level component is member of the top level container
* If the component is a member of a module, the pointer of the top level� module is returned
*/
Component * Component::getTopLevelComponent()
{
	if (getContainer()->isParentComponent())
	{
		// Parent is a Module
		return getContainer()->getParentComponent()->getTopLevelComponent();
	}
	else
	{
		return this;
	}
}

	
/** Returns the component type */
const char * Component::getType() const
{
	return getInfo()->getLibName();
}	


/** Add a connector to the connector list and set the serial ID of the connector (if required). */
void Component::addConnector(ConnectorBase * conn)
{
	/** Add to list. Now the component is owner of the connector. */
	getConnList()->append(conn);
}



/** save component properties */
void Component::save(KSimData & file) const
{
	QString group;
	
	file.writeEntry(sType, getType());
	
	if (getName() != i18n(getInfo()->getName().latin1()))
	{
		file.writeEntry(sName, getName());
	}
	file.writeEntry(sSerialNumber, getSerialNumber());
	
	group = file.group();
	
	if (getSheetView())
	{
		QString sheetGroup;
		sheetGroup = group + sSheet;
		file.setGroup(sheetGroup);
		getSheetView()->save(file);
	}
	
	if (getUserView())
	{
		QString userGroup;
		userGroup = group + sUser;
		file.setGroup(userGroup);
		getUserView()->save(file);
	}
	
	if(m_addonList)
	{
		file.setGroup(group);
		m_addonList->save(file);
	}
	
	// If wire, save no connector properties
	if (!isWire())
	{
		unsigned int c = 0;
		FOR_EACH_CONNECTOR(it, *getConnList())
		{
			if (it.current()->getAction().isSaveEnabled())
			{
				QString connGroup;
				connGroup.sprintf("Conn%i/",c);
				connGroup = group + connGroup;
				file.setGroup(connGroup);
				it.current()->save(file);
				c++;
			}
		}
	}
	
	file.setGroup(group);
	
}

/** Load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Component::load(KSimData & file, bool)
{
	QString group;
	bool ok = true;
	
	setName(file.readEntry(sName, i18n(getInfo()->getName().latin1())));
	setSerialNumber(file.readUnsignedNumEntry(sSerialNumber, getSerialNumber()));
	
	group = file.group();
	
	if (getSheetView())
	{
		QString sheetGroup;
		sheetGroup = group + sSheet;
		file.setGroup(sheetGroup);
		ok &= getSheetView()->load(file);
	}
		
	if (getUserView())
	{
		QString userGroup;
		userGroup = group + sUser;
		file.setGroup(userGroup);
		ok &= getUserView()->load(file);
	}
		
	if(m_addonList)
	{
		file.setGroup(group);
		ok &= m_addonList->load(file);
	}
	
	// If wire, load no connector properties
	if (!isWire())
	{
		unsigned int c = 0;
		FOR_EACH_CONNECTOR(it, *getConnList())
		{
			if (it.current()->getAction().isLoadEnabled())
			{
				QString connGroup;
				connGroup.sprintf("Conn%i/",c);
				connGroup = group + connGroup;
				file.setGroup(connGroup);
				ok &= it.current()->load(file);
				c++;
			}
		}
	}
	
	file.setGroup(group);
	
	return ok;
}
	
/** Returns true, if component is a wire  */
bool Component::isWire() const
{
	return m_isWire;
}

/** Returns true, if component is a module  */
bool Component::isModule() const
{
	return m_isModule;
}

/** Returns true, if component is an external connector  */
bool Component::isExtConn() const
{
	return m_isExtConn;
}

/** Returns true, if component is a group container  */
bool Component::isGroup() const
{
	return m_isGroup;
}

/** Returns true, if simulation is running. */
bool Component::isRunning() const
{
	return getContainer()->isRunning();
}

/** Reset all simulation variables */
void Component::reset()
{
	FOR_EACH_CONNECTOR(it,*getConnList())
	{
		if (it.current()->getAction().isResetEnabled())
		{
			it.current()->reset();
		}
	}
	if(m_addonList)
	{
		m_addonList->reset();
	}
}

int Component::checkCircuit()
{
	int errors = 0;
	
	// If changes required, see also ExternalConnector !!!
	FOR_EACH_CONNECTOR(it,*getConnList())
	{
		if (it.current()->getAction().isCheckCircuitEnabled())
		{
			errors += it.current()->checkCircuit();
		}
	}
	if(m_addonList)
	{
		errors += m_addonList->checkCircuit();
	}
	return errors;
}

void Component::checkProperty(QStringList & errorMsg)
{
	unsigned int i,j;
	// Check unique connector names
  for(i=0; i < getConnList()->count(); i++)
  {
	  for(j=i+1; j < getConnList()->count(); j++)
  	{
			if (getConnList()->at(i)->getName() == getConnList()->at(j)->getName())
			{
				errorMsg.append(i18n("Connector names must be unique (%1).").arg(getConnList()->at(j)->getName()));
			}
		}
	}
	
	FOR_EACH_CONNECTOR(it,*getConnList())
	{
		if (it.current()->getAction().isCheckPropertyEnabled())
		{
			it.current()->checkProperty(errorMsg);
		}
	}
	
	if(m_addonList)
	{
		m_addonList->checkProperty(errorMsg);
	}
}

unsigned int Component::executePropertyCheck()
{
	QStringList errMsg;
	
	checkProperty(errMsg);
	
	if(errMsg.count())
	{	
		// Error detected
		QString errText(i18n("Property Errors"));
		errText += ":\n\n" + errMsg.join("\n");
		
		logError(errText);

		KMessageBox::error(0, errText, i18n("Property Errors"));

		// Restore data
		if(getUndo())
		{
			getUndo()->hiddenUndo();
		}
		
	}
	return errMsg.count();
}

void Component::calculate()
{
	if(m_addonList)
	{
		m_addonList->calculate();
	}
}

/** Shift the result of calculation to output */
void Component::updateOutput()
{
	if(m_addonList)
	{
		m_addonList->updateOutput();
	}
}


void Component::setContainer(CompContainer * parent)
{
	m_container = parent;
	
/*	if (getSheetView())
		getSheetView()->setComponentMap(getSheetMap());
	
	if (getUserView())
		getUserView()->setComponentMap(getUserMap());*/
}

void Component::setName(const QString & newName)
{
	m_name = newName.simplifyWhiteSpace();
	emit signalSetName(m_name);
};

/** Returns the related document */
KSimusDoc * Component::getDoc() const
{
	return getContainer()->getDoc();	
}
	

/** Returns the log window */
LogList * Component::getLogList() const
{
	return getContainer()->getLogList();
}

const KSimTimeServer & Component::getTimeServer() const
{
	return getContainer()->getTimeServer();
}

/** Returns the undo object */
KSimUndo * Component::getUndo() const
{

	return getDoc()->getUndo();
}
/** Executes the undo functionality (save this component) */
void Component::undoChangeProperty(const QString & description) const
{
	getUndo()->changeProperty(this, description);
}

/** Set the document modified */
void Component::setModified() const
{
	getDoc()->setModified();
}
/** Reroute all component and update views */
void Component::refresh() const
{
	getContainer()->refresh();
	
/*	getContainer()->routeComponents();
	getDoc()->slotUpdateAllViews(0);*/
}

/** Shows a status message in the application status line */
void Component::statusMsg(const QString &text) const
{
	getContainer()->statusMsg(text);
}

/** Shows a help message in the application status line */
void Component::statusHelpMsg(const QString &text) const
{
	getContainer()->statusHelpMsg(text);
}
	


/** Returns a pointer to the sheet map from the parent conatiner */
ComponentMap * Component::getSheetMap() const
{
	return getContainer()->getSheetMap();
}
/** Returns a pointer to the sheet map from the parent conatiner */
ComponentMap * Component::getUserMap() const
{
	return getContainer()->getUserMap();
}

ConnectorBase * Component::searchConnector(const QString & name) const
{
	ConnectorBase * conn = 0;
	
	FOR_EACH_CONNECTOR(it,*getConnList())
	{
		// equal names?
		if (it.current()->getWireName() == name)
		{
			conn = it.current();
			break;
		}
	}
	
	return conn;
}

const PackageInfo * Component::getPackageInfo() const
{
	return g_library->getComponentLib()->getPackageInfo(getInfo()->getLibName());
}

KInstance * Component::getInstance() const
{
	const PackageInfo * pi = getPackageInfo();
	
	if (pi)
	{
		KInstance * instance = pi->getInstance();
		
		if (!instance)
		{
			KSIMDEBUG_VAR("No KInstance found", getInfo()->getLibName());
		}
		
		return instance;
	}
	return 0;
}

static void addLog(Component * comp, unsigned int priority, const QString & msg)
{
	QString str = comp->requestTopLevelName() + " - " + msg;
	LLICompSel * lli;
	if(comp->isWire() && comp->getContainer()->isParentDoc())
	{
		ComponentList list;
		
		FOR_EACH_CONNECTOR(it, *comp->getConnList())
		{
			list.append(it.current()->getComponent());
		}
		lli = new LLICompSel(str, priority, comp->getDoc(), &list);
	}
	else
	{
		lli = new LLICompSel(str, priority, comp->getDoc(), comp->getTopLevelComponent());
	}

	comp->getLogList()->append(lli);	
}

void Component::logInfo(const QString & msg)
{
	addLog(this, LOG_INFO, msg);
}

void Component::logWarning(const QString & msg)
{
	addLog(this, LOG_WARNING, msg);
}

void Component::logError(const QString & msg)
{
	addLog(this, LOG_ERROR, msg);
}

//##########################################################
// Component Dialog

/** Init the property dialog */
void Component::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	addGeneralProperty(dialog);
	addConnectorProperty(dialog);
	addInfoProperty(dialog);

	// Don't forget ComponentStyle
		
	if(m_addonList)
	{
		m_addonList->initPropertyDialog(dialog);
	}
}
   	
/** Add the general property page to the property dialog
	This function is called by initPropertyDialog()*/
void Component::addGeneralProperty(ComponentPropertyDialog * dialog)
{
	QVBox * page;
	ComponentPropertyBaseWidget * wid;
	page = dialog->addVBoxPage(i18n("General"));
	wid = createGeneralProperty(this, page);
	dialog->connectSlots(wid);
}

/** Creates the general property page to the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * Component::createGeneralProperty(Component * comp, QWidget *parent)
{
	ComponentPropertyBaseWidget * wid = new ComponentPropertyGeneralWidget(comp, parent, "General Settings");
	CHECK_PTR(wid);
	return wid;
}

/** Adds the connector property pages to the property dialog
	This function is called by initPropertyDialog()*/
void Component::addConnectorProperty(ComponentPropertyDialog * dialog)
{
	if (getConnList()->count())
	{
		QStringList nameList;
		QStringList strList;
		strList.append(i18n("Connectors"));
		strList.append("");
		unsigned int i,j,c;
		
		FOR_EACH_CONNECTOR(it, *getConnList())
		{
			nameList.append(it.current()->getName());
		}
		
		for(i = 0; i < getConnList()->count(); i++)
		{
			if (getConnList()->at(i)->getAction().isInitPropertyDialogEnabled())
			{
				c = 1;
				for(j = i+1; j < getConnList()->count(); j++)
				{
					if (nameList[i] == nameList[j])
					{
						c++;
						nameList[j] = nameList[j]+ QString("[%1]").arg(c);
					}
				}
				if (c != 1)
				{
					nameList[i] = nameList[i]+ "[1]";
				}
	
				strList[1] = nameList[i];
				QVBox * page;
				QWidget * wid;
				page = dialog->addVBoxPage(strList);
				wid = getConnList()->at(i)->propertyWidget(page);
				dialog->connectSlots(wid);
			}
		}
	}
}

/** Add the info page to the property dialog
	This function is called by initPropertyDialog()*/
void Component::addInfoProperty(ComponentPropertyDialog * dialog)
{
	QVBox * page;
	ComponentPropertyBaseWidget * wid;
	page = dialog->addVBoxPage(i18n("Information"));
	wid = createInfoProperty(this, page);
	dialog->connectSlots(wid);
}
	
ComponentPropertyBaseWidget * Component::createInfoProperty(Component * comp, QWidget *parent)
{
	ComponentPropertyBaseWidget * wid = new ComponentPropertyInfoWidget(comp, parent, "Information");
	CHECK_PTR(wid);
	return wid;
}
		
//##########################################################
// Component Popup

	
/** Initialize the component popup menu */
bool Component::initPopupMenu(QPopupMenu * popup)
{
	bool insertSep = false;
	
	// User view available
	if ((getInfo()->getViewAttr() & VA_USERVIEW)			&&
		!(getInfo()->getViewAttr() & VA_SHEET_XOR_USER)	&&
		(getUserView()))
	{
		if (!insertSep)
		{
			popup->insertSeparator();
			insertSep = true;
		}
		int idx = popup->insertItem((const char*)i18n("&Hide User View"), getUserView(), SLOT(slotToggleHide()));
		popup->setItemChecked(idx, getUserView()->isHidden());
	}
	
	if(m_addonList)
	{
		insertSep |= m_addonList->initPopupMenu(popup);
	}
	return insertSep;
}


