/***************************************************************************
                        booltristate1out.h  -  description
                             -------------------
    begin                : Fre Nov 15 2001
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
#include <qpainter.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbooltristatebox.h"
#include "booltristate1out.h"

// Forward declaration


//###############################################################
//###############################################################

namespace KSimLibBoolean
{

BoolTriState1Out::BoolTriState1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetState(KSIMBOOLTRISTATE_INACTIVE),
		m_resetStateInit(KSIMBOOLTRISTATE_INACTIVE)
{
	
	m_out = new ConnectorBoolTriState(this,
	                                  QString::fromLatin1("Output"),
	                                  i18n("Connector", "Output"));
	CHECK_PTR(m_out);
	
}

/*BoolTriState1Out::~BoolTriState1Out()
{
} */

void BoolTriState1Out::setState(KSimBoolTriState newState)
{
	getOutputConnector()->setOutput(newState);
}
	
KSimBoolTriState BoolTriState1Out::getState() const
{
	return getOutputConnector()->getOutput();
}

void BoolTriState1Out::setResetState(KSimBoolTriState resetState, bool init)
{
	m_resetState = resetState;
	if (init)
		m_resetStateInit = resetState;
}

KSimBoolTriState BoolTriState1Out::getResetState() const
{
	return m_resetState;
};
	
KSimBoolTriState BoolTriState1Out::getResetStateInit() const
{
	return m_resetStateInit;
};
	


void BoolTriState1Out::reset()
{
	Component::reset();
	
	setState( getResetState() );
//	getOutputConnector()->setOutput(getState());
}

/** save component properties */
void BoolTriState1Out::save(KSimData & file) const
{
	Component::save(file);
	
	if (m_resetState != m_resetStateInit)
	{
		m_resetState.save(file, "Reset State");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool BoolTriState1Out::load(KSimData & file, bool copyLoad)
{
	m_resetState.load(file, "Reset State", m_resetStateInit.text());
	return Component::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * BoolTriState1Out::createGeneralProperty(QWidget *parent)
{
	BoolTriState1OutPropertyGeneralWidget * wid;
	wid = new BoolTriState1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


BoolTriState1OutView::BoolTriState1OutView(BoolTriState1Out * comp, eViewType viewType)
	: CompView(comp, viewType)
{
//	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
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
/*BoolTriState1OutView::~BoolTriState1OutView()
{
}*/

void BoolTriState1OutView::draw(QPainter * p)
{
	drawFrame(p);
	
	CompView::draw(p);
}


//###############################################################
//###############################################################

BoolTriState1OutPropertyGeneralWidget::BoolTriState1OutPropertyGeneralWidget(BoolTriState1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetStateLabel = new QLabel(i18n("Reset State: "), this, "ResetStateLabel");
	CHECK_PTR(m_resetStateLabel);
	
	m_resetState = new KSimBoolTristateBox(getBoolTriState1Out()->getResetState(), this, "ResetState");
	CHECK_PTR(m_resetState);
	
	QString tip(i18n("Changes the reset state of the component to true, false or inactive."));
	QToolTip::add(m_resetState, tip);
	QWhatsThis::add(m_resetState, tip);

	QToolTip::add(m_resetStateLabel, tip);
	QWhatsThis::add(m_resetStateLabel, tip);
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void BoolTriState1OutPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getBoolTriState1Out()->getResetState() != m_resetState->getValue())
	{
		changeData();
		getBoolTriState1Out()->setResetState( m_resetState->getValue() );
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void BoolTriState1OutPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetState->setValue(getBoolTriState1Out()->getResetStateInit());
}

//###############################################################
//###############################################################

BoolTriState1Out1Ena::BoolTriState1Out1Ena(CompContainer * container, const ComponentInfo * ci)
	:	BoolTriState1Out(container, ci)
{
	m_enableOutput = new ConnectorBoolIn(this,
	                                     QString::fromLatin1("Enable Output"),
	                                     i18n("Boolean-Connector", "Enable Output"));
	CHECK_PTR(m_enableOutput);
}

/*BoolTriState1Out1Ena::~BoolTriState1Out1Ena()
{}*/

void BoolTriState1Out1Ena::setState(bool newState)
{
	BoolTriState1Out::setState(KSimBoolTriState(newState, getEnableOutputConnector()->getInput()));
}

bool BoolTriState1Out1Ena::getState() const
{
	return BoolTriState1Out::getState().isTrue();
}

//###############################################################
//###############################################################

BoolTriState1Out1EnaView::BoolTriState1Out1EnaView(BoolTriState1Out1Ena * comp, eViewType viewType)
	:	BoolTriState1OutView(comp, viewType),
		m_label(QString::fromLatin1(""))
{
	init(viewType);
}

BoolTriState1Out1EnaView::BoolTriState1Out1EnaView(const QString & label, BoolTriState1Out1Ena * comp, eViewType viewType)
	:	BoolTriState1OutView(comp, viewType),
		m_label(label)
{
	init(viewType);
}


void BoolTriState1Out1EnaView::init(eViewType viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_ctrlBlock = new ComponentControlBlock(this, getComponentLayout());
		CHECK_PTR(m_ctrlBlock);

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(getBoolTriState1Out1Ena()->getEnableOutputConnector());
//		getComponentLayout()->getLeft()->addSpace(1);

		getComponentLayout()->setMinSize(5,5);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getBoolTriState1Out1Ena()->getEnableOutputConnector(), QString::fromLatin1("EO"));
	}
}

/*	BoolTriState1Out1EnaView::~BoolTriState1OutView()
{}*/

void BoolTriState1Out1EnaView::draw(QPainter * p)
{
	// Don't use the BoolTriState1OutView::draw(QPainter * p) !!!
	CompView::draw(p);

	QRect rect(getDrawingPlace());
	rect.rLeft() ++;
	rect.rTop() += 1 + m_ctrlBlock->getRect(false).bottom();
	rect.rBottom() ++;

	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);

	p->drawText(rect, (AlignHCenter | AlignTop), m_label);
}

//###############################################################
//###############################################################

BoolTriStateXIn1Out1Ena::BoolTriStateXIn1Out1Ena(CompContainer * container, const ComponentInfo * ci)
	:	BoolTriState1Out1Ena( container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             2, 26);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
}

/* BoolTriStateXIn1Out1Ena::~BoolTriStateXIn1Out1Ena()
{ }*/

//###############################################################
//###############################################################

BoolTriStateXIn1Out1EnaView::BoolTriStateXIn1Out1EnaView(BoolTriStateXIn1Out1Ena * comp, eViewType viewType)
	:	CompView(comp, viewType),
		m_label(QString::fromLatin1(""))
{
	init(viewType);
}

BoolTriStateXIn1Out1EnaView::BoolTriStateXIn1Out1EnaView(const QString & label, BoolTriStateXIn1Out1Ena * comp, eViewType viewType)
	:	CompView(comp, viewType),
		m_label(label)
{
	init(viewType);
}

void BoolTriStateXIn1Out1EnaView::init(eViewType viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);

		m_ctrlBlock = new ComponentControlBlock(this, m_layout);
		CHECK_PTR(m_ctrlBlock);

		m_ctrlBlock->getLeft()->addSpace(1);
		m_ctrlBlock->getLeft()->addConnector(getBoolTriStateXIn1Out1Ena()->getEnableOutputConnector());

		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addConnector(getBoolTriStateXIn1Out1Ena()->getOutputConnector());
		m_layout->getRight()->addStretch(1);

		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnectorPack(getBoolTriStateXIn1Out1Ena()->getInputConnector());

		m_layout->setMinSize(5,5);
		m_layout->updateLayout();

		new ConnectorLabel(getBoolTriStateXIn1Out1Ena()->getEnableOutputConnector(), QString::fromLatin1("EO"));
	}
/*	if (viewType == SHEET_VIEW)
	{
//		enableRotation(true);

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnectorPack(getBoolTriStateXIn1Out1Ena()->getInputConnector());
//		getComponentLayout()->getRight()->addSpace(1);

		getComponentLayout()->setMinSize(5,5);
		getComponentLayout()->updateLayout();
	}*/
}

/* BoolTriStateXIn1Out1EnaView::~BoolTriStateXIn1Out1EnaView()
{}*/


void BoolTriStateXIn1Out1EnaView::draw(QPainter * p)
{
	// Don't use the BoolTriState1OutView::draw(QPainter * p) !!!
	CompView::draw(p);

	QRect rect(getDrawingPlace());
	rect.rLeft() ++;
	rect.rTop() += 1 + m_ctrlBlock->getRect(false).bottom();
	rect.rBottom() ++;

	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);

	p->drawText(rect, AlignCenter, m_label);
}

//###############################################################

}; //namespace KSimLibBoolean

