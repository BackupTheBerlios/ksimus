/***************************************************************************
                          dflipflop.cpp  -  description
                             -------------------
    begin                : Tue Dec 11 2001
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


// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/componentlayout.h"
#include "ksimus/optionalconnector.h"
//#include "ksimus/componentpropertybasewidget.h"

// Project-Includes
#include "dflipflop.h"

// Forward declaration





namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new DFlipFlop(container, ci);
}

const ComponentInfo * getDFlipFlopInfo()
{
	static const ComponentInfo Info(i18n("Component", "D Latch"),
	                                QString::fromLatin1("Boolean/Flip Flop/D Latch"),
	                                i18n("Component", "Boolean/Flip Flop/D Latch"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-d-latch"));
	return &Info;
}


//###############################################################
//###############################################################



DFlipFlop::DFlipFlop(CompContainer * container, const ComponentInfo * ci)
	: FlipFlopBase(container, ci)
{
	
	m_inD = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Data"),
	                             i18n("Boolean-Connector", "Data"));
	CHECK_PTR(m_inD);

	m_inEna = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable"),
	                             i18n("Boolean-Connector", "Enable"));
	CHECK_PTR(m_inEna);
//	m_inEna->setEdgeSensitiveChangeEnable(false);
	m_inEna->setEdgeSensitive(false, true);
	
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
		new DFlipFlopView(this, SHEET_VIEW);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/*DFlipFlop::~DFlipFlop()
{
} */

void DFlipFlop::calculate()
{
	FlipFlopBase::calculate();
	
	bool set = getSetInputConnector()->getInput() && !getSetInputConnector()->isHidden();
	bool reset = getResetInputConnector()->getInput() && !getResetInputConnector()->isHidden();
	bool enable = getEnableInputConnector()->getInput();
	bool data = getDataInputConnector()->getInput();

	
	if (set || reset)
	{
		setState(set && (getDominant() || !reset));
	}
	else if (enable)
	{
		setState(data);
	}	
}

//###############################################################
//###############################################################

DFlipFlopView::DFlipFlopView(DFlipFlop * comp, eViewType viewType)
	: CompView(comp, viewType)
{
//	setPlace(QRect(0, 0, 7*gridX, 7*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		new ConnectorLabel(getDFF()->getOutputConnector(), "Q");
		new ConnectorLabel(getDFF()->getNotOutputConnector(), "/Q");
		new ConnectorLabel(getDFF()->getSetInputConnector(), "S");
		new ConnectorLabel(getDFF()->getResetInputConnector(), "R");
		new ConnectorLabel(getDFF()->getEnableInputConnector(), "E");
		new ConnectorLabel(getDFF()->getDataInputConnector(), "D");
	
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
		
		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnector(getDFF()->getSetInputConnector());
		m_layout->getLeft()->addConnector(getDFF()->getDataInputConnector());
		m_layout->getLeft()->addConnector(getDFF()->getEnableInputConnector());
		m_layout->getLeft()->addConnector(getDFF()->getResetInputConnector());

		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnector(getDFF()->getOutputConnector());
		m_layout->getRight()->addStretch(3);
		m_layout->getRight()->addConnector(getDFF()->getNotOutputConnector());
		m_layout->getRight()->addStretch(1);
			
//		m_layout->setMinSize(5,6);
		m_layout->updateLayout();
	}
}

/*DFlipFlopView::~DFlipFlopView()
{
}*/

void DFlipFlopView::draw(QPainter * p)
{
  drawFrame(p);

	CompView::draw(p);
}


//###############################################################
//###############################################################

//###############################################################
	
}; //namespace
