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
#include "ksimus/optionalconnector.h"
//#include "ksimus/componentpropertybasewidget.h"

// Project-Includes
#include "jkflipflop.h"

// Forward declaration

namespace KSimLibBoolean
{

//###############################################################
//###############################################################


Component * JKFlipFlop::create(CompContainer * container, const ComponentInfo * ci)
{
	return new JKFlipFlop(container, ci);
}

const ComponentInfo * JKFlipFlop::getStaticJKInfo()
{
	static const ComponentInfo Info(i18n("Component", "JK Flip Flop"),
	                                QString::fromLatin1("Boolean/Flip Flop/JK-FF"),
	                                i18n("Component", "Boolean/Flip Flop/JK-FF"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-jk-ff"));
	return &Info;
}

const ComponentInfo * JKFlipFlop::getStaticJKMSInfo()
{
	static const ComponentInfo Info(i18n("Component", "JK Master Slave Flip Flop"),
	                                QString::fromLatin1("Boolean/Flip Flop/JK-MS-FF"),
	                                i18n("Component", "Boolean/Flip Flop/JK-MS-FF"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-jk-ff"));
	return &Info;
}


//###############################################################
//###############################################################




JKFlipFlop::JKFlipFlop(CompContainer * container, const ComponentInfo * ci)
	: FlipFlopBase(container, ci),
		m_isMasterSlave(false),
		m_lastClk(false),
		m_lastJ(false),
		m_lastK(false)
{
	m_inJ = new ConnectorBoolIn(this,
	                             QString::fromLatin1("J"),
	                             i18n("Boolean-Connector JK-FF", "J"));
	Q_CHECK_PTR(m_inJ);

	m_inK = new ConnectorBoolIn(this,
	                             QString::fromLatin1("K"),
	                             i18n("Boolean-Connector JK-FF", "K"));
	Q_CHECK_PTR(m_inK);

	m_inClk = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock"),
	                             i18n("Boolean-Connector", "Clock"));
	Q_CHECK_PTR(m_inClk);
	m_inClk->setEdgeSensitiveChangeEnable(false);
	
	getResetInputConnector()->setEdgeSensitive(false,true);
	getSetInputConnector()->setEdgeSensitive(false,true);
	
	// make Reset optional
	new OptionalConnector(getResetInputConnector(),
	                      QString::fromLatin1("Reset input"),
	                      i18n("Boolean", "Reset input:"));
	// make Set optional
	new OptionalConnector(getSetInputConnector(),
	                      QString::fromLatin1("Set input"),
	                      i18n("Boolean", "Set input:"));
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new JKFlipFlopView(this, SHEET_VIEW, "JKFlipFlopSV");
	}
	
	if (ci == JKFlipFlop::getStaticJKMSInfo())
	{
		setMasterSlaveEnabled(true);
	}	

	getAction().disable(KSimAction::UPDATEVIEW);
}

ComponentPropertyBaseWidget * JKFlipFlop::createGeneralProperty(QWidget *parent)
{
	JKFlipFlopPropertyGeneralWidget * wid;
	wid = new JKFlipFlopPropertyGeneralWidget(this, parent);
	Q_CHECK_PTR(wid);
	
	return wid;
}

/** save component properties */
void JKFlipFlop::save(KSimData & file) const
{
	FlipFlopBase::save(file);
	
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
	
	return FlipFlopBase::load(file, copyLoad);
}

void JKFlipFlop::calculate()
{
	FlipFlopBase::calculate();

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
	FlipFlopBase::reset();
	
	m_lastClk = getClockInputConnector()->isNegated();
	m_lastJ = getJInputConnector()->isNegated();
	m_lastK = getKInputConnector()->isNegated();
}


//###############################################################
//###############################################################


JKFlipFlopView::JKFlipFlopView(JKFlipFlop * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
//	setPlace(QRect(0, 0, 7*gridX, 7*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		new ConnectorLabel(getJKFF()->getOutputConnector(), QString::fromLatin1("Q"));
		new ConnectorLabel(getJKFF()->getNotOutputConnector(), QString::fromLatin1("Q"));
		new ConnectorLabel(getJKFF()->getSetInputConnector(), QString::fromLatin1("S"));
		new ConnectorLabel(getJKFF()->getResetInputConnector(), QString::fromLatin1("R"));
		new ConnectorLabel(getJKFF()->getJInputConnector(), QString::fromLatin1("J"));
		new ConnectorLabel(getJKFF()->getKInputConnector(), QString::fromLatin1("K"));
		new ConnectorLabel(getJKFF()->getClockInputConnector(), QString::fromLatin1("C"));
	
		m_layout = new ComponentLayoutSimple(this);
		Q_CHECK_PTR(m_layout);
		
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

/*void JKFlipFlopView::draw(QPainter * p)
{
  drawFrame(p);
	
	CompView::draw(p);
}*/


//###############################################################
//###############################################################

//##########################################################################################
//##########################################################################################


JKFlipFlopPropertyGeneralWidget::JKFlipFlopPropertyGeneralWidget(JKFlipFlop * comp, QWidget *parent, const char *name)
	:	FlipFlopBasePropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Boolean", "Master Slave:"), this);
	Q_CHECK_PTR(lab);
	
	m_masterSlave = new KSimBooleanBox(getJKFF()->isMasterSlaveEnabled(), this);
	Q_CHECK_PTR(m_masterSlave);
	
	m_masterSlave->setTrueText(i18n("Boolean", "Enabled"));
	m_masterSlave->setFalseText(i18n("Boolean", "Disabled"));
	
	str = i18n("Boolean", "Enables the Master Salve functionality.\nIf disabled, the FF works in single edge mode.");
	addToolTip(str, lab, m_masterSlave);
	addWhatsThis(str, lab, m_masterSlave);
	lab->setBuddy(m_masterSlave);
}

/*JKFlipFlopPropertyGeneralWidget::~JKFlipFlopPropertyGeneralWidget()
{
} */

void JKFlipFlopPropertyGeneralWidget::acceptPressed()
{
	FlipFlopBasePropertyGeneralWidget::acceptPressed();

	if (getJKFF()->isMasterSlaveEnabled() != m_masterSlave->getValue())
	{
		changeData();
		getJKFF()->setMasterSlaveEnabled(m_masterSlave->getValue());
	}
}

void JKFlipFlopPropertyGeneralWidget::defaultPressed()
{
	FlipFlopBasePropertyGeneralWidget::defaultPressed();

	m_masterSlave->setValue(false);
}


//###############################################################
	
}; //namespace

#include "jkflipflop.moc"
