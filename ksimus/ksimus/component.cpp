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
#include "componentaddon.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "ksimus.h"
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
#include "simulationexecute.h"
#include "module.h"
#include "propertywidget.h"



static const char * const sSheet = "Sheet/";
static const char * const sUser  = "User/";

const char * const Component::sSerialNumber = "Serial Number";
const char * const Component::sType = "Type";
const char * const Component::sName = "Name";


//#######################################################################
//#######################################################################

class Component::Private
{
public:
	Private()
	{
		flags.zeroDelay = 0;
		addonListCalculate = (ComponentAddOnList *)0;
	};

	~Private()
	{
		delete addonListCalculate;
	}
	
	struct
	{
		unsigned int zeroDelay :1;
	} flags;

	ComponentAddOnList * addonListCalculate;
};


//#######################################################################
//#######################################################################

KSIMDEBUG_INSTANCE_COUNTER(Component);

Component::Component(CompContainer * container, const ComponentInfo * ci)
	: QObject((QObject*)0, ci->getName().latin1()),
	  m_sheetView(0),
	  m_userView(0),
	  m_container(container),
	  m_name(),
	  m_serialNumber(0),
	  m_info(ci),
	  m_componentType(ci->getViewAttr() & VA_USERVIEW ? eGuiComponent : eComponent),
	  m_addonList(0),
	  m_myActions(KSimAction::ALL)		//Default
{
	m_connList = new ConnectorList;
	Q_CHECK_PTR(m_connList);
	m_connList->setAutoDelete(true);
	m_p = new Private();
	Q_CHECK_PTR(m_p);
	KSIMDEBUG_INSTANCE_INC(Component);
}
	
Component::~Component()
{
	delete m_addonList;
	delete m_connList;
//	delete m_sheetView;
//	delete m_userView;
	delete m_p;
	KSIMDEBUG_INSTANCE_DEC(Component);
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
* If the component is a member of a module, the pointer of the top levelö module is returned
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
const QString & Component::getType() const
{
	return getInfo()->getLibName();
}	

bool Component::isProperReloadType(const QString & type) const
{
	return (type == getType());
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
	
	if (!m_name.isEmpty())
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
	
	if (file.hasKey(sName))
	{
		setName(file.readEntry(sName, m_name));
	}
	else
	{
		setName(QString::null);
	}
//	Set in compContainer !!!
//	setSerialNumber(file.readUnsignedNumEntry(sSerialNumber, getSerialNumber()));
	
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
	

Component::eComponentType Component::getComponentType() const
{
	return m_componentType;
}

void Component::setComponentType(Component::eComponentType newType)
{
	m_componentType = newType;
}

/** Returns true, if component is a wire  */
bool Component::isWire() const
{
	return (getComponentType() == eWire);
}

/** Returns true, if component is a module  */
bool Component::isModule() const
{
	return (getComponentType() == eModule);
}

/** Returns true, if component is an external connector  */
bool Component::isExtConn() const
{
	return (getComponentType() == eExternalConnector);
}

/** Returns true, if component is a group container  */
bool Component::isGroup() const
{
	return (getComponentType() == eGroup);
}

/** Returns true, if component is a GUI component.  */
bool Component::isGuiComp() const
{
	return (getComponentType() == eGuiComponent);
}

void Component::setZeroDelayComponent(bool zeroDelay)
{
	m_p->flags.zeroDelay = zeroDelay;
}
bool Component::isZeroDelayComponent() const
{
	return m_p->flags.zeroDelay;
}

/** Returns true, if simulation is running. */
bool Component::isRunning() const
{
	return getContainer()->isRunning();
}

/** Reset all simulation variables */
void Component::reset()
{
	if (!isWire())
	{
		FOR_EACH_CONNECTOR(it,*getConnList())
		{
			if (it.current()->getAction().isResetEnabled())
			{
				it.current()->reset();
			}
		}
	}
	if(m_addonList)
	{
		m_addonList->reset();
		if (m_p->addonListCalculate)
		{
			m_p->addonListCalculate->clear();
		}
		FOR_EACH_COMPONENT_ADDON(it, *m_addonList)
		{
			if (it.current()->getAction().isCalculateEnabled())
			{
				if (!m_p->addonListCalculate)
					m_p->addonListCalculate = new ComponentAddOnList();
				Q_CHECK_PTR(m_p->addonListCalculate);
				m_p->addonListCalculate->append(it.current());
			}
		}
		if(m_p->addonListCalculate && (m_p->addonListCalculate->count() == 0))
		{
			delete m_p->addonListCalculate;
			m_p->addonListCalculate = (ComponentAddOnList *)0;
		}
	}
	else if (m_p->addonListCalculate)
	{
		delete m_p->addonListCalculate;
		m_p->addonListCalculate = (ComponentAddOnList *)0;
	}
}

void Component::executeNext()
{
	getDoc()->getExecute().executeComponentNext(this);
}

void Component::executeAt(unsigned int timerNo, const KSimTime & time)
{
	getDoc()->getExecute().executeComponentAt(this, timerNo, time);
}

void Component::executeAfter(unsigned int timerNo, const KSimTime & diffTime)
{
	getDoc()->getExecute().executeComponentAfter(this, timerNo, diffTime);
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

void Component::setupCircuit()
{
	FOR_EACH_CONNECTOR(it,*getConnList())
	{
		it.current()->setupCircuit();
	}
	if(m_addonList)
	{
		m_addonList->setupCircuit();
	}
}

void Component::checkProperty(QStringList & errorMsg)
{
	unsigned int i,j;
	// Check unique connector names
	for(i=0; i < getConnList()->count(); i++)
	{
		QString connName(getConnList()->at(i)->getName());
		for(j=i+1; j < getConnList()->count(); j++)
		{
			if (connName == getConnList()->at(j)->getName())
			{
				errorMsg.append(i18n("Connector names must be unique (%1).").arg(connName));
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
		QString caption((errMsg.count() == 1)
		                ? i18n("Property Error")
		                : i18n("Property Errors"));

		QString errText(caption
		                + QString::fromLatin1(":\n\n")
		                + errMsg.join(QString::fromLatin1("\n")));

//		logError(errText);

		KMessageBox::error(getDoc()->getApp(), errText, caption);

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
	if(m_p->addonListCalculate)
	{
		m_p->addonListCalculate->calculate();
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

QString Component::getName() const
{
	if (m_name.isEmpty())
	{
		//return getInfo()->getName();
		return getDefaultName();
	}
	return m_name;
};

void Component::setName(const QString & newName)
{
	if (newName == m_name) return;
	
	if (newName == getDefaultName())
	{
		m_name = QString::null;
	}
	else
	{
		m_name = newName.simplifyWhiteSpace();
	}
	emit signalSetName(getName());
};

QString Component::getDefaultName() const
{
	return QString::fromLatin1("%1 %2")
	                           .arg(getInfo()->getName())
	                           .arg(getSerialNumber());
};

bool Component::hasDefaultName() const
{
	return m_name.isEmpty();
}



/** Returns the related document */
KSimusDoc * Component::getDoc() const
{
	return getContainer()->getDoc();	
}
	

/** Returns the module depth. This means how many modules are 'between' this
  * component and the top level document.
  * Returns 0 if the component is not a member of  module.*/
unsigned int Component::getModuleDepth() const
{
	unsigned int u = 0;
	const CompContainer * container = getContainer();
	while (container->isParentComponent())
	{
		const Module * module = (const Module *)container->getParentComponent();
		container = module->getModuleContainer();
		u ++;
	};
	return u;
}


/** Returns the log window */
LogList * Component::getLogList() const
{
	return getContainer()->getLogList();
}

/** Returns a pointer to the watch widget */	
WatchWidget * Component::getWatchWidget() const
{
	return getContainer()->getWatchWidget();
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
	ConnectorBase * conn = (ConnectorBase *)0;
	
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
	return (KInstance *)0;
}

void Component::addLog(Component * comp, unsigned int priority, const QString & msg)
{
	QString str = comp->requestTopLevelName() + QString::fromLatin1(" - ") + msg;
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


void Component::menuExecuted()
{
	if(m_addonList)
	{
		m_addonList->menuExecuted();
	}
}


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
	static const QString i18nTitel(i18n("Component property dialog", "General"));
	
	QVBox * page = dialog->addVBoxPage(i18nTitel);
	ComponentPropertyBaseWidget * wid = createGeneralProperty(page);
	dialog->connectSlots(wid);
}

/** Creates the general property page to the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * Component::createGeneralProperty(QWidget *parent)
{
	ComponentPropertyBaseWidget * wid = new ComponentPropertyGeneralWidget(this, parent, "General Settings");
	Q_CHECK_PTR(wid);
	return wid;
}

/** Adds the connector property pages to the property dialog
	This function is called by initPropertyDialog()*/
void Component::addConnectorProperty(ComponentPropertyDialog * dialog)
{
	static const QString i18nTitel(i18n("Component property dialog", "Connectors"));
	
	if (getConnList()->count())
	{
		QStringList nameList;
		QStringList strList;
		strList.append(i18nTitel);
		strList.append(QString::fromLatin1(""));
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
						nameList[j] = nameList[j]+ QString::fromLatin1("[%1]").arg(c);
					}
				}
				if (c != 1)
				{
					nameList[i] = nameList[i]+ "[1]";
				}
				
				strList[1] = nameList[i];
				QVBox * page = dialog->addVBoxPage(strList);
				PropertyWidget * wid = getConnList()->at(i)->propertyWidget(page);
				dialog->connectSlots(wid);
			}
		}
	}
}

/** Add the info page to the property dialog
	This function is called by initPropertyDialog()*/
void Component::addInfoProperty(ComponentPropertyDialog * dialog)
{
	static const QString i18nTitel(i18n("Component property dialog", "Information"));
	
	QVBox * page = dialog->addVBoxPage(i18nTitel);
	ComponentPropertyBaseWidget * wid = createInfoProperty(page);
	dialog->connectSlots(wid);
}
	
ComponentPropertyBaseWidget * Component::createInfoProperty(QWidget *parent)
{
	ComponentPropertyBaseWidget * wid = new ComponentPropertyInfoWidget(this, parent, "Information");
	Q_CHECK_PTR(wid);
	return wid;
}


//##########################################################
// Component Popup

	
/** Initialize the component popup menu */
bool Component::initPopupMenu(QPopupMenu * popup)
{
	bool insertSep = false;
	
	// User view available
	if ( getUserView()
	  && (getInfo()->getViewAttr() == VA_SHEET_AND_USER)
	  && (!isModule() || (((Module*)this)->getModuleView() == MV_USERVIEW)))
	{
		if (!insertSep)
		{
			popup->insertSeparator();
			insertSep = true;
		}
		int idx = popup->insertItem(i18n("&Hide User View"), getUserView(), SLOT(slotToggleHide()));
		popup->setItemChecked(idx, getUserView()->isHidden());
	}
	
	if(m_addonList)
	{
		insertSep |= m_addonList->initPopupMenu(popup);
	}
	return insertSep;
}

//##########################################################
// Component List

void ComponentList::insertComponent(Component * comp)
{
	Component::eComponentType type = comp->getComponentType();
//	static unsigned int max = 0;
	unsigned int step = count() / 2;
	unsigned int i = step;
//	unsigned int cnt = 0, sum = 0;
	
//	KSIMDEBUG(QString::fromLatin1("#### %1  step=%2").arg(count()).arg(step));
	
	// Step fast to the position
	while(step >= 2)
	{
		step /= 2;
		if (type < at(i)->getComponentType())
		{
			i -= step;
		}
		else
		{
			i += step;
		}
//		cnt++;
	}
//	sum += cnt;
//	KSIMDEBUG(QString::fromLatin1("Step while %1  i=%2").arg(cnt).arg(i));
//	cnt = 0;
	
	while((i > 0) && (type < at(i)->getComponentType()))
	{
		i --;
//		cnt ++;
	}
//	sum += cnt;
//	KSIMDEBUG(QString::fromLatin1("Back while %1  i=%2").arg(cnt).arg(i));
//	cnt = 0;
	
	while((i < count()) && !(type < at(i)->getComponentType()))
	{
		i ++;
//		cnt ++;
	}
//	sum += cnt;
//	if (max<sum) max=sum;
//	KSIMDEBUG(QString::fromLatin1("Forward while %1  i=%2").arg(cnt).arg(i));
//	KSIMDEBUG(QString::fromLatin1("Summe %1  max=%2").arg(sum).arg(max));
	
	insert(i,comp);
}

#include "component.moc"
