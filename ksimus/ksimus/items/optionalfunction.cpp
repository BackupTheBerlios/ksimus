/***************************************************************************
                          optionalfunction.cpp  -  description
                             -------------------
    begin                : Sun Jun 2 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include <qlabel.h>
#include <qcombobox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdebug.h"
#include "ksimdata.h"
#include "optionalfunction.h"
#include "propertywidget.h"
#include "rowlayoutwidget.h"
#include "componentpropertydialog.h"

// Forward declaration

typedef struct
{
	QLabel * label;
	QComboBox * comboBox;
	PropertyWidget * propertyWidget;
} tPropertyWidgetItem;



class OptionalFunction::Private
{
public:
	Private()
		: propertyWidgetItem((tPropertyWidgetItem *)0)
	{
		flags.enabled = 0;
		flags.defaultEnabled = 0;
		flags.changeable = 1;
	};
	
	struct
	{
		unsigned int enabled :1;
		unsigned int defaultEnabled :1;
		unsigned int changeable : 1;
	} flags;
	tPropertyWidgetItem * propertyWidgetItem;
};

//########################################################################
//########################################################################

OptionalFunction::OptionalFunction(Component * component, const QString & addOnName, bool unique)
	: ComponentAddOn(component, addOnName, unique)
{
	static const QString pageName(i18n("Component property dialog", "Optionals"));
	
	m_p = new Private();
	CHECK_PTR(m_p);
	setDialogPageName(pageName);
	getAction().setAction(KSimAction::STORAGE | KSimAction::INITPROPERTYDIALOG);
}

OptionalFunction::~OptionalFunction()
{
	delete m_p;
}

void OptionalFunction::save(KSimData & file) const
{
	file.writeEntry(getName(), isEnabled());
}

bool OptionalFunction::load(KSimData & file)
{
	setEnabled(file.readBoolEntry(getName(), isDefaultEnabled()));
	
	return true;
}

void OptionalFunction::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	if (!dialog)
	{
		KSIMDEBUG_VAR("dialog == 0", getName());
		return;
	}
		
	PropertyWidget * page = dialog->getPage(getDialogPageName());
	if (!page)
	{
		KSIMDEBUG_VAR("dialog page == 0", getName());
		return;
	}
	
	addToPropertyWidget(page);
}

void OptionalFunction::setEnabled(bool ena)
{
	m_p->flags.enabled = ena;
}
bool OptionalFunction::isEnabled() const
{
	return m_p->flags.enabled;
}


void OptionalFunction::setDefaultEnabled(bool ena)
{
	m_p->flags.defaultEnabled = ena;
	setEnabled(ena);
}
bool OptionalFunction::isDefaultEnabled() const
{
	return m_p->flags.defaultEnabled;
}


void OptionalFunction::setChangeable(bool changeable)
{
	m_p->flags.changeable = changeable;
}
bool OptionalFunction::isChangeable() const
{
	return m_p->flags.changeable;
}

void OptionalFunction::addToPropertyWidget(PropertyWidget * widget)
{
	if (!m_p->propertyWidgetItem)
	{
		m_p->propertyWidgetItem = new tPropertyWidgetItem;
		
	}
	else
	{
		KSIMDEBUG("FIXME: m_p->propertyWidgetItem != 0");
	}
	
		
	m_p->propertyWidgetItem->propertyWidget = widget;
	
	widget->newRow();
	
	m_p->propertyWidgetItem->label = new QLabel(getLabelText(), widget, "OptionalFunction::QLabel");
	CHECK_PTR(getLabel());
	
	m_p->propertyWidgetItem->comboBox = new QComboBox(widget, "OptionalFunction::QComboBox");
	CHECK_PTR(getComboBox());
	getComboBox()->insertItem(i18n("Disabled"), 0);
	getComboBox()->insertItem(i18n("Enabled"), 1);
	getLabel()->setBuddy(getComboBox());
	
	getComboBox()->setCurrentItem((int)isEnabled());
	getComboBox()->setEnabled(isChangeable());
	
	getLabel()->setBuddy(getComboBox());
	PropertyWidget::addToolTip(getToolTipText(), getLabel(), getComboBox());
	PropertyWidget::addWhatsThis(getWhatsThisText(), getLabel(), getComboBox());
	
	connect(widget, SIGNAL(signalAccept()), SLOT(slotAccept()));
	connect(widget, SIGNAL(signalDefault()), SLOT(slotDefault()));
	connect(widget, SIGNAL(destroyed()), SLOT(slotPropertyWidgetDestroyed()));
}


void OptionalFunction::slotAccept()
{
	if (!getComboBox())
	{
		KSIMDEBUG("FIXME: getComboBox() == 0");
		return;
	}
	if (!getPropertyWidget())
	{
		KSIMDEBUG("FIXME: getPropertyWidget() == 0");
		return;
	}
	
	if ((bool)getComboBox()->currentItem() != isEnabled())
	{
		getPropertyWidget()->changeData();
		setEnabled((bool)getComboBox()->currentItem());
	}
}

void OptionalFunction::slotDefault()
{
	// Do not change if Widget is disabled
	if (getComboBox()->isEnabled())
	{
		getComboBox()->setCurrentItem((int)isDefaultEnabled());
	}
}

void OptionalFunction::slotPropertyWidgetDestroyed()
{
	if (!m_p->propertyWidgetItem)
	{
		KSIMDEBUG("m_p->propertyWidgetItem is null");
	}
	delete m_p->propertyWidgetItem;
	m_p->propertyWidgetItem = (tPropertyWidgetItem *)0;
}

QLabel * OptionalFunction::getLabel() const
{
	if (m_p->propertyWidgetItem)
		return m_p->propertyWidgetItem->label;
	return (QLabel *)0;
}

QComboBox * OptionalFunction::getComboBox() const
{
	if (m_p->propertyWidgetItem)
		return m_p->propertyWidgetItem->comboBox;
	return (QComboBox *)0;
}

PropertyWidget * OptionalFunction::getPropertyWidget() const
{
	if (m_p->propertyWidgetItem)
		return m_p->propertyWidgetItem->propertyWidget;
	return (PropertyWidget *)0;
}

#include "optionalfunction.moc"
