/***************************************************************************
                        booltristate1out.h  -  description
                             -------------------
    begin                : Fri Nov 15 2001
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

BoolTristate1Out::BoolTristate1Out(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetState(KSIMBOOLTRISTATE_INACTIVE),
		m_resetStateInit(KSIMBOOLTRISTATE_INACTIVE)
{
	
	m_out = new ConnectorBoolTristate(this,
	                                  QString::fromLatin1("Output"),
	                                  i18n("Connector", "Output"));
	CHECK_PTR(m_out);
	
}

/*BoolTristate1Out::~BoolTristate1Out()
{
} */

void BoolTristate1Out::setState(KSimBoolTristate newState)
{
	getOutputConnector()->setOutput(newState);
}
	
KSimBoolTristate BoolTristate1Out::getState() const
{
	return getOutputConnector()->getOutput();
}

void BoolTristate1Out::setResetState(KSimBoolTristate resetState, bool init)
{
	m_resetState = resetState;
	if (init)
		m_resetStateInit = resetState;
}

KSimBoolTristate BoolTristate1Out::getResetState() const
{
	return m_resetState;
};
	
KSimBoolTristate BoolTristate1Out::getResetStateInit() const
{
	return m_resetStateInit;
};
	


void BoolTristate1Out::reset()
{
	Component::reset();
	
	setState( getResetState() );
//	getOutputConnector()->setOutput(getState());
}

/** save component properties */
void BoolTristate1Out::save(KSimData & file) const
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
bool BoolTristate1Out::load(KSimData & file, bool copyLoad)
{
	m_resetState.load(file, "Reset State", m_resetStateInit.text());
	return Component::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * BoolTristate1Out::createGeneralProperty(QWidget *parent)
{
	BoolTristate1OutPropertyGeneralWidget * wid;
	wid = new BoolTristate1OutPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


BoolTristate1OutView::BoolTristate1OutView(BoolTristate1Out * comp, eViewType viewType)
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
/*BoolTristate1OutView::~BoolTristate1OutView()
{
}*/

void BoolTristate1OutView::draw(QPainter * p)
{
	drawFrame(p);
	
	CompView::draw(p);
}


//###############################################################
//###############################################################

BoolTristate1OutPropertyGeneralWidget::BoolTristate1OutPropertyGeneralWidget(BoolTristate1Out * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetStateLabel = new QLabel(i18n("Reset State: "), this, "ResetStateLabel");
	CHECK_PTR(m_resetStateLabel);
	
	m_resetState = new KSimBoolTristateBox(getBoolTristate1Out()->getResetState(), this, "ResetState");
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
void BoolTristate1OutPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getBoolTristate1Out()->getResetState() != m_resetState->getValue())
	{
		changeData();
		getBoolTristate1Out()->setResetState( m_resetState->getValue() );
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void BoolTristate1OutPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetState->setValue(getBoolTristate1Out()->getResetStateInit());
}

//###############################################################
//###############################################################

BoolTristate1Out1Ena::BoolTristate1Out1Ena(CompContainer * container, const ComponentInfo * ci)
	:	BoolTristate1Out(container, ci)
{
	m_enableOutput = new ConnectorBoolIn(this,
	                                     QString::fromLatin1("Enable Output"),
	                                     i18n("Boolean-Connector", "Enable Output"));
	CHECK_PTR(m_enableOutput);
}

/*BoolTristate1Out1Ena::~BoolTristate1Out1Ena()
{}*/

void BoolTristate1Out1Ena::setState(bool newState)
{
	BoolTristate1Out::setState(KSimBoolTristate(newState, getEnableOutputConnector()->getInput()));
}

bool BoolTristate1Out1Ena::getState() const
{
	return BoolTristate1Out::getState().isTrue();
}

//###############################################################
//###############################################################

BoolTristate1Out1EnaView::BoolTristate1Out1EnaView(BoolTristate1Out1Ena * comp, eViewType viewType)
	:	BoolTristate1OutView(comp, viewType),
		m_label(QString::fromLatin1(""))
{
	init(viewType);
}

BoolTristate1Out1EnaView::BoolTristate1Out1EnaView(const QString & label, BoolTristate1Out1Ena * comp, eViewType viewType)
	:	BoolTristate1OutView(comp, viewType),
		m_label(label)
{
	init(viewType);
}


void BoolTristate1Out1EnaView::init(eViewType viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_ctrlBlock = new ComponentControlBlock(this, getComponentLayout());
		CHECK_PTR(m_ctrlBlock);

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(getBoolTristate1Out1Ena()->getEnableOutputConnector());
//		getComponentLayout()->getLeft()->addSpace(1);

		getComponentLayout()->setMinSize(5,5);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getBoolTristate1Out1Ena()->getEnableOutputConnector(), QString::fromLatin1("EO"));
	}
}

/*	BoolTristate1Out1EnaView::~BoolTristate1OutView()
{}*/

void BoolTristate1Out1EnaView::draw(QPainter * p)
{
	// Don't use the BoolTristate1OutView::draw(QPainter * p) !!!
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

BoolTristateXIn1Out1Ena::BoolTristateXIn1Out1Ena(CompContainer * container, const ComponentInfo * ci)
	:	BoolTristate1Out1Ena( container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             2, 26);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
}

/* BoolTristateXIn1Out1Ena::~BoolTristateXIn1Out1Ena()
{ }*/

//###############################################################
//###############################################################

BoolTristateXIn1Out1EnaView::BoolTristateXIn1Out1EnaView(BoolTristateXIn1Out1Ena * comp, eViewType viewType)
	:	CompView(comp, viewType),
		m_label(QString::fromLatin1(""))
{
	init(viewType);
}

BoolTristateXIn1Out1EnaView::BoolTristateXIn1Out1EnaView(const QString & label, BoolTristateXIn1Out1Ena * comp, eViewType viewType)
	:	CompView(comp, viewType),
		m_label(label)
{
	init(viewType);
}

void BoolTristateXIn1Out1EnaView::init(eViewType viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);

		m_ctrlBlock = new ComponentControlBlock(this, m_layout);
		CHECK_PTR(m_ctrlBlock);

		m_ctrlBlock->getLeft()->addSpace(1);
		m_ctrlBlock->getLeft()->addConnector(getBoolTristateXIn1Out1Ena()->getEnableOutputConnector());

		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addConnector(getBoolTristateXIn1Out1Ena()->getOutputConnector());
		m_layout->getRight()->addStretch(1);

		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnectorPack(getBoolTristateXIn1Out1Ena()->getInputConnector());

		m_layout->setMinSize(5,5);
		m_layout->updateLayout();

		new ConnectorLabel(getBoolTristateXIn1Out1Ena()->getEnableOutputConnector(), QString::fromLatin1("EO"));
	}
/*	if (viewType == SHEET_VIEW)
	{
//		enableRotation(true);

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnectorPack(getBoolTristateXIn1Out1Ena()->getInputConnector());
//		getComponentLayout()->getRight()->addSpace(1);

		getComponentLayout()->setMinSize(5,5);
		getComponentLayout()->updateLayout();
	}*/
}

/* BoolTristateXIn1Out1EnaView::~BoolTristateXIn1Out1EnaView()
{}*/


void BoolTristateXIn1Out1EnaView::draw(QPainter * p)
{
	// Don't use the BoolTristate1OutView::draw(QPainter * p) !!!
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

