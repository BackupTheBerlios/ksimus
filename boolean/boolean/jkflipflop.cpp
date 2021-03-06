/***************************************************************************
                          jkflipflop.cpp  -  description
                             -------------------
    begin                : Fri Dec 7 2001
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

// KSimus-Includes
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbooleanbox.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/componentlayout.h"

// Project-Includes
#include "jkflipflop.h"

// Forward declaration

namespace KSimLibBoolean
{

//###############################################################
//###############################################################


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new JKFlipFlop(container, ci);
}

const ComponentInfo JKFlipFlopInfo(I18N_NOOP("JK Flip Flop"),
                                   I18N_NOOP("Boolean/Flip Flop/JK-FF"),
                                   QString::null,/*"JK Flip Flop",*/
                                   VA_SHEETVIEW,
                                   create,	
                                   QString::null,
                                   "component-boolean-jk-ff"
                                  );

const ComponentInfo JKMSFlipFlopInfo(I18N_NOOP("JK Master Slave Flip Flop"),
                                     I18N_NOOP("Boolean/Flip Flop/JK-MS-FF"),
                                     QString::null,/*"JK Flip Flop",*/
                                     VA_SHEETVIEW,
                                     create,	
                                     QString::null,
                                     "component-boolean-jk-ff"
                                   );


//###############################################################
//###############################################################




JKFlipFlopBase::JKFlipFlopBase(CompContainer * container, const ComponentInfo * ci)
	: FlipFlopBase(container, ci)
{
	m_inJ = new ConnectorBoolIn(this, "J");
	CHECK_PTR(m_inJ);

	m_inK = new ConnectorBoolIn(this, "K");
	CHECK_PTR(m_inK);

	m_inClk = new ConnectorBoolInEdge(this, I18N_NOOP("Clock"));
	CHECK_PTR(m_inClk);
	m_inClk->setEdgeSensitiveChangeEnable(false);
	
	getResetInputConnector()->setEdgeSensitive(false,true);
	getResetInputConnector()->setHideEnabled(true);
	getResetInputConnector()->setHide(true,true);
	getSetInputConnector()->setEdgeSensitive(false,true);
	getSetInputConnector()->setHideEnabled(true);
	getSetInputConnector()->setHide(true,true);
}

/*JKFlipFlopBase::~JKFlipFlopBase()
{
} */



//###############################################################
//###############################################################

JKFlipFlop::JKFlipFlop(CompContainer * container, const ComponentInfo * ci)
	: JKFlipFlopBase(container, ci),
		m_isMaterSlave(false),
		m_lastClk(false),
		m_lastJ(false),
		m_lastK(false)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new JKFlipFlopView(this, SHEET_VIEW);
	}
	
	if (ci == &JKMSFlipFlopInfo)
	{
		setMasterSlaveEnabled(true);
	}	

	getAction().disable(KSimAction::UPDATEVIEW);
}

ComponentPropertyBaseWidget * JKFlipFlop::createGeneralProperty(Component * comp, QWidget *parent)
{
	JKFlipFlopPropertyGeneralWidget * wid;
	wid = new JKFlipFlopPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

/** save component properties */
void JKFlipFlop::save(KSimData & file) const
{
	JKFlipFlopBase::save(file);
	
	if (isMasterSlaveEnabled())   // Save true state only (default is false!)
	{
		file.writeEntry("Master Slave", true);
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool JKFlipFlop::load(KSimData & file, bool copyLoad)
{
	setMasterSlaveEnabled( file.readBoolEntry("Master Slave", false) );
	
	return JKFlipFlopBase::load(file, copyLoad);
}

void JKFlipFlop::calculate()
{
	JKFlipFlopBase::calculate();

	if(isMasterSlaveEnabled())
	{
		calculateJKMS();
	}
	else
	{
		calculateJK();
	}
}
	

void JKFlipFlop::calculateJK()
{
	bool set = getSetInputConnector()->getInput() && !getSetInputConnector()->isHidden();
	bool reset = getResetInputConnector()->getInput() && !getResetInputConnector()->isHidden();
	bool clk = getClockInputConnector()->getInput();
	
	if (set || reset)
	{
		setState(set && (getDominant() || !reset));
	}
	else if(clk)
	{
		bool j = getJInputConnector()->getInput();
		bool k = getKInputConnector()->getInput();
		int jkState = (j ? 1 : 0) + (k ? 2 : 0);
		switch(jkState)
		{
			case 0:
				break;
			case 1:
				setState(true);
				break;
			case 2:
				setState(false);
				break;
			case 3:
				setState(!getState());
				break;
		}
	}
}

void JKFlipFlop::calculateJKMS()
{
	bool set = getSetInputConnector()->getInput() && !getSetInputConnector()->isHidden();
	bool reset = getResetInputConnector()->getInput() && !getResetInputConnector()->isHidden();
	bool clk = ((ConnectorBoolIn *)getClockInputConnector())->getInput();
	
	if (set || reset)
	{
		m_lastJ = m_lastK = false;
		setState(set && (getDominant() || !reset));
	}
	else if(clk && !m_lastClk)
	{
		// pos edge
		m_lastJ = getJInputConnector()->getInput();
		m_lastK = getKInputConnector()->getInput();
	}
	else if(!clk && m_lastClk)
	{
		// neg edge
		int jkState = (m_lastJ ? 1 : 0) + (m_lastK ? 2 : 0);
		switch(jkState)
		{
			case 0:
				break;
			case 1:
				setState(true);
				break;
			case 2:
				setState(false);
				break;
			case 3:
				setState(!getState());
				break;
		}
	}
	m_lastClk = clk;
}

void JKFlipFlop::reset()
{
	JKFlipFlopBase::reset();
	
	m_lastClk = getClockInputConnector()->isNegated();
	m_lastJ = getJInputConnector()->isNegated();
	m_lastK = getKInputConnector()->isNegated();
}


//###############################################################
//###############################################################

#define getJKFF() ((JKFlipFlop *) getComponent())

JKFlipFlopView::JKFlipFlopView(JKFlipFlopBase * comp, eViewType viewType)
	: CompView(comp, viewType)
{
//	setPlace(QRect(0, 0, 7*gridX, 7*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		new ConnectorLabel(getJKFF()->getOutputConnector(), "Q");
		new ConnectorLabel(getJKFF()->getNotOutputConnector(), "/Q");
		new ConnectorLabel(getJKFF()->getSetInputConnector(), "S");
		new ConnectorLabel(getJKFF()->getResetInputConnector(), "R");
		new ConnectorLabel(getJKFF()->getJInputConnector(), "J");
		new ConnectorLabel(getJKFF()->getKInputConnector(), "K");
		new ConnectorLabel(getJKFF()->getClockInputConnector(), "C");
	
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
		
		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnector(getJKFF()->getSetInputConnector());
		m_layout->getLeft()->addConnector(getJKFF()->getJInputConnector());
		m_layout->getLeft()->addConnector(getJKFF()->getClockInputConnector());
		m_layout->getLeft()->addConnector(getJKFF()->getKInputConnector());
		m_layout->getLeft()->addConnector(getJKFF()->getResetInputConnector());

		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnector(getJKFF()->getOutputConnector());
		m_layout->getRight()->addStretch(3);
		m_layout->getRight()->addConnector(getJKFF()->getNotOutputConnector());
		m_layout->getRight()->addStretch(1);
			
//		m_layout->setMinSize(5,6);
		m_layout->updateLayout();
	}
}

/*RSFlipFlopView::~RSFlipFlopView()
{
}*/

void JKFlipFlopView::draw(QPainter * p)
{
  drawFrame(p);
	
	CompView::draw(p);
}

#undef getJKFF()

//###############################################################
//###############################################################

//##########################################################################################
//##########################################################################################


JKFlipFlopPropertyGeneralWidget::JKFlipFlopPropertyGeneralWidget(JKFlipFlop * comp, QWidget *parent, const char *name)
	:	FlipFlopBasePropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Master Slave:"), getGrid());
	CHECK_PTR(lab);
	
	m_masterSlave = new KSimBooleanBox(comp->isMasterSlaveEnabled(), getGrid());
	CHECK_PTR(m_masterSlave);
	
	m_masterSlave->setTrueText(i18n("Enabled"));
	m_masterSlave->setFalseText(i18n("Disabled"));
	
	str = i18n("Enables the Master Salve functionality.\nIf disabled, the FF works in single edge mode.");
	QToolTip::add(m_masterSlave, str);
	QToolTip::add(lab, str);
	QWhatsThis::add(m_masterSlave, str);
	QWhatsThis::add(lab, str);
	
}

/*JKFlipFlopPropertyGeneralWidget::~JKFlipFlopPropertyGeneralWidget()
{
} */

void JKFlipFlopPropertyGeneralWidget::acceptPressed()
{
	FlipFlopBasePropertyGeneralWidget::acceptPressed();

	if (((JKFlipFlop*)getComponent())->isMasterSlaveEnabled() != m_masterSlave->getValue())
	{
		changeData();
		((JKFlipFlop*)getComponent())->setMasterSlaveEnabled(m_masterSlave->getValue());
	}
}

void JKFlipFlopPropertyGeneralWidget::defaultPressed()
{
	FlipFlopBasePropertyGeneralWidget::defaultPressed();

	m_masterSlave->setValue(false);
}


//###############################################################
	
}; //namespace

