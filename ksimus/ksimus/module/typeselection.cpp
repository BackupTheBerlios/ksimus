/***************************************************************************
                          typeselection.cpp  -  description
                             -------------------
    begin                : Sun Apr 22 2001
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
#include <qradiobutton.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "typeselection.h"
#include "moduledialog.h"

// Forward declaration

#define data (m_dialog->getData())


TypeSelection::TypeSelection(ModuleDialog * dialog, QWidget * parent, const char * name)
	:	QVButtonGroup(i18n("View selection"),parent,name),
		m_dialog(dialog)
{
	QString tooltip;
	
	m_rbNoView = new QRadioButton(i18n("&No Module"), this);
	CHECK_PTR(m_rbNoView);
 	m_rbNoView->setFocusPolicy(ClickFocus);
	insert(m_rbNoView,(int)MV_NONE);
	tooltip = i18n("Check, if this is not a module.");
  QToolTip::add(m_rbNoView,tooltip);
  QWhatsThis::add(m_rbNoView,tooltip);

	
	m_rbGeneric = new QRadioButton(i18n("&Generic View"), this);
	CHECK_PTR(m_rbGeneric);
 	m_rbGeneric->setFocusPolicy(ClickFocus);
	insert(m_rbGeneric,(int)MV_GENERIC);
	tooltip = i18n("Check, if this is a module with a default view.");
  QToolTip::add(m_rbGeneric,tooltip);
  QWhatsThis::add(m_rbGeneric,tooltip);
	
	m_rbUserView = new QRadioButton("&User View", this);
	CHECK_PTR(m_rbUserView);
 	m_rbUserView->setFocusPolicy(ClickFocus);
	insert(m_rbUserView,(int)MV_USERVIEW);
	tooltip = i18n("Check, if the module should have a view like the user interface.");
  QToolTip::add(m_rbUserView,tooltip);
  QWhatsThis::add(m_rbUserView,tooltip);
	
	m_rbPixmap = new QRadioButton("&Image", this);
	CHECK_PTR(m_rbPixmap);
 	m_rbPixmap->setFocusPolicy(ClickFocus);
	insert(m_rbPixmap,(int)MV_PIXMAP);
	tooltip = i18n("Check, if this is a module with a image view.");
  QToolTip::add(m_rbPixmap,tooltip);
  QWhatsThis::add(m_rbPixmap,tooltip);

	connect(this, SIGNAL(clicked(int)), this, SLOT(slotClicked(int)));
	setup();
}

TypeSelection::~TypeSelection()
{
}

void TypeSelection::setup()
{
	setButton((int)data->getModuleView());
}

void TypeSelection::slotClicked(int id)
{
	emit clicked((ModuleViewType) id);
}

