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

ComponentAddOn::ComponentAddOn(Component * component, const QString & addOnName)
	: QObject(component),
		ComponentItem(component),
		m_myActions(KSimAction::ALL),
		m_addOnName(addOnName)
{
	if (!component->m_addonList)
	{
		component->m_addonList = new ComponentAddOnList;
		CHECK_PTR(component->m_addonList);
	}
	
	FOR_EACH_COMPONENT_ADDON(it, *component->m_addonList)
	{
		if (it.current()->getName() == m_addOnName)
		{
			KSIMDEBUG_VAR("Add on name not unique!", m_addOnName);
		}
	}
	
	component->m_addonList->append(this);
}

ComponentAddOn::~ComponentAddOn()
{
	getComponent()->m_addonList->removeRef(this);
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

void ComponentAddOn::checkProperty(QStringList & )
{
}

void ComponentAddOn::calculate()
{
}

void ComponentAddOn::updateOutput()
{
}

void ComponentAddOn::reset()
{
}

void ComponentAddOn::initPropertyDialog(ComponentPropertyDialog * )
{
}

bool ComponentAddOn::initPopupMenu(QPopupMenu * )
{
	return false;
}

//#################################################################################
//#################################################################################

ComponentAddOnList::ComponentAddOnList()
	:	QList<ComponentAddOn>()
{
	setAutoDelete(true);
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

/** Shift the result of calculation to output.
	*/
void ComponentAddOnList::updateOutput()
{
	FOR_EACH_COMPONENT_ADDON(it, *this)
	{
		if (it.current()->getAction().isUpdateOutputEnabled())
		{
			it.current()->updateOutput();
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
