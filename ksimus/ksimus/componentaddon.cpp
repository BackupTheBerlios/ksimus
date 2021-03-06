/***************************************************************************
                          componentaddon.cpp  -  description
                             -------------------
    begin                : Thu May 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// KDE-Includes

// Project-Includes
#include "ksimdata.h"
#include "component.h"
#include "componentaddon.h"
#include "ksimaction.h"
#include "ksimdebug.h"

// Forward declaration


class ComponentAddOn::UIData
{
public:
	UIData()
		: i18nLabelText(QString::null),
		  i18nWhatsThisText(QString::null),
		  i18nToolTipText(QString::null),
		  propertyDialogPageName(QString::null)
	{
	};
	
	QString i18nLabelText;
	QString i18nWhatsThisText;
	QString i18nToolTipText;
	QStringList propertyDialogPageName;
};

//########################################################################
//########################################################################

KSIMDEBUG_INSTANCE_COUNTER(ComponentAddOn);


ComponentAddOn::ComponentAddOn(Component * component, const QString & addOnName, bool unique)
	: QObject(component),
		ComponentItem(component),
		m_myActions(KSimAction::ALL),
		m_addOnName(addOnName),
		m_uiData((UIData *)0)
{
	if (!component->m_addonList)
	{
		component->m_addonList = new ComponentAddOnList;
		Q_CHECK_PTR(component->m_addonList);
	}
	
	if (unique)
	{
		FOR_EACH_COMPONENT_ADDON(it, *component->m_addonList)
		{
			if (it.current()->getName() == m_addOnName)
			{
				KSIMDEBUG_VAR("Add on name not unique!", m_addOnName);
			}
		}
	}
	
	component->m_addonList->append(this);
	
	KSIMDEBUG_INSTANCE_INC(ComponentAddOn);
}

ComponentAddOn::~ComponentAddOn()
{
	if(-1 != getComponent()->m_addonList->findRef((ComponentAddOn*)this))
	{
		// Remove from list
		getComponent()->m_addonList->take();
	}
	delete m_uiData;
	
	KSIMDEBUG_INSTANCE_DEC(ComponentAddOn);
}
	
void ComponentAddOn::save(KSimData & ) const
{
}

bool ComponentAddOn::load(KSimData & )
{
	return true;
}

void ComponentAddOn::drawSheetView (QPainter *) const
{
}

void ComponentAddOn::drawUserView (QPainter *) const
{
}

int ComponentAddOn::checkCircuit()
{
	return 0;
}

void ComponentAddOn::setupCircuit()
{
}

void ComponentAddOn::checkProperty(QStringList & )
{
}

void ComponentAddOn::calculate()
{
}

void ComponentAddOn::reset()
{
}

void ComponentAddOn::menuExecuted()
{
}

void ComponentAddOn::initPropertyDialog(ComponentPropertyDialog * )
{
}

bool ComponentAddOn::initPopupMenu(QPopupMenu * )
{
	return false;
}


ComponentAddOn::UIData * ComponentAddOn::getUIData()
{
	if (!m_uiData)
	{
		m_uiData = new UIData();
		Q_CHECK_PTR(m_uiData);
	}
	return m_uiData;
}

void ComponentAddOn::setDialogPageName(const QStringList & dialogPageName)
{
	getUIData()->propertyDialogPageName = dialogPageName;
}
void ComponentAddOn::setDialogPageName(const QString & dialogPageName)
{
	setDialogPageName(QStringList::split('/', dialogPageName));
}
QStringList ComponentAddOn::getDialogPageName()
{
	return	getUIData()->propertyDialogPageName;
}

void ComponentAddOn::setLabelText(const QString & i18nLabelText)
{
	getUIData()->i18nLabelText = i18nLabelText;
}
QString ComponentAddOn::getLabelText()
{
	return getUIData()->i18nLabelText;
}


void ComponentAddOn::setToolTipText(const QString & i18nToolTipText)
{
	getUIData()->i18nLabelText = i18nToolTipText;
}
QString ComponentAddOn::getToolTipText()
{
	return getUIData()->i18nToolTipText;
}


void ComponentAddOn::setWhatsThisText(const QString & i18nWhatsThisText)
{
	getUIData()->i18nLabelText = i18nWhatsThisText;
}
QString ComponentAddOn::getWhatsThisText()
{
	return getUIData()->i18nWhatsThisText;
}



//#################################################################################
//#################################################################################

ComponentAddOnList::ComponentAddOnList()
	:	QPtrList<ComponentAddOn>()
{
	setAutoDelete(true);
}

ComponentAddOnList::~ComponentAddOnList()
{
	// Remove from last element to first element
	while (removeLast());
}


/** Save all component add on's properties.
	*/
void ComponentAddOnList::save(KSimData & file) const
{
	QString group(file.group());
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isSaveEnabled())
		{
			it.current()->save(file);
			file.setGroup(group);
		}
	}
}
	/** Load all component add on's properties.
		*	Returns true if successful.
		*/
bool ComponentAddOnList::load(KSimData & file)
{
	bool res = true;
	QString group(file.group());

	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isLoadEnabled())
		{
			res &= it.current()->load(file);
			file.setGroup(group);
		}
	}
	return res;
}
	
/** Draw the sheet view.
	* This function takes care about the KSimAction information.
	*/
void ComponentAddOnList::drawSheetView (QPainter *p) const
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isDrawSheetViewEnabled())
		{
			it.current()->drawSheetView(p);
		}
	}
}

/** Draw the user view.
	* This function takes care about the KSimAction information.
	*/
void ComponentAddOnList::drawUserView (QPainter *p) const
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isDrawUserViewEnabled())
		{
			it.current()->drawUserView(p);
		}
	}
}

/** Checks all component add on before simulation.
	* Returns the number of errors.
	*/
int ComponentAddOnList::checkCircuit()
{
	int error = 0;
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isCheckCircuitEnabled())
		{
			error += it.current()->checkCircuit();
		}
	}
	return error;
}

void ComponentAddOnList::setupCircuit()
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		it.current()->setupCircuit();
	}
}



/** Checks  all component property. The functions is called after the
	*	property dialog.
	*/
void ComponentAddOnList::checkProperty(QStringList & errorMsg)
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isCheckPropertyEnabled())
		{
			it.current()->checkProperty(errorMsg);
		}
	}
}

/** Executes the simulation of the addons.
	*/
void ComponentAddOnList::calculate()
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isCalculateEnabled())
		{
			it.current()->calculate();
		}
	}
}

/** Reset all simulation variables.
	*/
void ComponentAddOnList::reset()
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isResetEnabled())
		{
			it.current()->reset();
		}
	}
}

/** Init the property dialog.
	*/
void ComponentAddOnList::menuExecuted()
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		it.current()->menuExecuted();
	}
}

/** Init the property dialog.
	*/
void ComponentAddOnList::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isInitPropertyDialogEnabled())
		{
			it.current()->initPropertyDialog(dialog);
		}
	}
}

/** Initialize the component popup menu.
  *	Return true, if items are added.
	*/
bool ComponentAddOnList::initPopupMenu(QPopupMenu * popup)
{
	bool added = false;
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isInitPopupMenuEnabled())
		{
			added |= it.current()->initPopupMenu(popup);
		}
	}
	return added;
}


/** Returns all actions of the addons. */
KSimAction ComponentAddOnList::getAction() const
{
	KSimAction::eAction action(KSimAction::NO);
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		action = (KSimAction::eAction)(action | it.current()->getAction().getAction());
	}
	return KSimAction::componentAddOnFilter(KSimAction(action));
}

/** Find the by name given @ref ComponentAddOn. */
ComponentAddOn * ComponentAddOnList::findByName(const QString & addOnName) const
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getName() == addOnName)
			return it.current();
	}
	return (ComponentAddOn *)0;
}

#include "componentaddon.moc"
