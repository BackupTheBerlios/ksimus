/***************************************************************************
                          boolean1out.cpp  -  description
                             -------------------
    begin                : Thu Dec 6 2001
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
#include <qpainter.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "boolean1out.h"
#include "resource.h"
#include "ksimdebug.h"
#include "connectorboolout.h"
#include "componentinfo.h"
#include "componentlayout.h"
#include "ksimdata.h"
#include "ksimbooleanbox.h"

// Forward declaration


//###############################################################
//###############################################################


Boolean1Out::Boolean1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_state(false),
		m_resetState(false)
{
	
	m_out = new ConnectorBoolOut (this, "Output");
	CHECK_PTR(m_out);
	
}

/*BooleanXIn1Out::~BooleanXIn1Out()
{
} */

void Boolean1Out::updateOutput()
{
	Component::updateOutput();
	getOutputConnector()->setOutput(getState());
}

void Boolean1Out::reset()
{
	Component::reset();
	
	setState( getResetState() );
	getOutputConnector()->setOutput(getState());
}

/** save component properties */
void Boolean1Out::save(KSimData & file) const
{
	Component::save(file);
	
	if (getResetState())   // Save true state only (defualt is false!)
	{
		file.writeEntry("Reset State", true);
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Boolean1Out::load(KSimData & file, bool copyLoad)
{
	setResetState( file.readBoolEntry("Reset State", false) );
	
	return Component::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * Boolean1Out::createGeneralProperty(Component * comp, QWidget *parent)
{
	Boolean1OutPropertyGeneralWidget * wid;
	wid = new Boolean1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


Boolean1OutView::Boolean1OutView(Boolean1Out * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	
	if (viewType == SHEET_VIEW)
	{
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
	
		m_layout->getRight()->addStretch(2);
		m_layout->getRight()->addConnector(comp->getOutputConnector(),0);
		m_layout->getRight()->addStretch(2);
	
		m_layout->updateLayout();
	}
	else
	{
		m_layout = 0;
	}
}
/*Boolean1OutView::~Boolean1OutView()
{
}*/

void Boolean1OutView::draw(QPainter * p)
{
	QRect rect(getWidgetPlace().topLeft()+QPoint(1,1),
							getWidgetPlace().bottomRight());
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
	
	CompView::draw(p);
}


//###############################################################
//###############################################################

#define getBoolean1Out()  ((Boolean1Out*) getComponent())


Boolean1OutPropertyGeneralWidget::Boolean1OutPropertyGeneralWidget(Boolean1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetStateLabel = new QLabel(i18n("Reset State: "), getGrid(), "ResetStateLabel");
	CHECK_PTR(m_resetStateLabel);
	
	m_resetState = new KSimBooleanBox(getBoolean1Out()->getResetState(), getGrid(), "ResetState");
	CHECK_PTR(m_resetState);
	
	QString tip(i18n("Change the reset state of the component here"));
	QToolTip::add(m_resetState, tip);
	QWhatsThis::add(m_resetState, tip);

	QToolTip::add(m_resetStateLabel, tip);
	QWhatsThis::add(m_resetStateLabel, tip);
}

/** The function slotAccept() is called, if changes are accepted.
	You have to reimplement this slot, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void Boolean1OutPropertyGeneralWidget::slotAccept()
{
	ComponentPropertyGeneralWidget::slotAccept();
	
	if (getBoolean1Out()->getResetState() != m_resetState->getValue())
	{
		changeData();
		getBoolean1Out()->setResetState( m_resetState->getValue() );
	}
}


/** The function slotDefault() is called, if user wants to set the default values.
    You have to reimplement this slot, if you add new properties.
  */
void Boolean1OutPropertyGeneralWidget::slotDefault()
{
	ComponentPropertyGeneralWidget::slotDefault();

	m_resetState->setValue(false);
}

#undef getBoolean1Out()

//###############################################################
//###############################################################

