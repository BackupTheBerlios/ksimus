/***************************************************************************
                          booleanand.cpp  -  description
                             -------------------
    begin                : Sat Feb 17 2001
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

// Project-Includes
#include "booleanand.h"
#include "resource.h"
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "componentinfo.h"
#include "connectorpack.h"
#include "componentlayout.h"

// Forward declaration

// Project-Includes

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanAnd(container, ci);
}

const ComponentInfo BooleanAndInfo (	"Boolean AND",
									"Boolean/Gates/AND",
									QString::null,//"Boolean AND",
									VA_SHEETVIEW,
									create
								);

const ComponentInfo BooleanNandInfo (	"Boolean NAND",
									"Boolean/Gates/NAND",
									QString::null,//"Boolean Nand",
									VA_SHEETVIEW,
									create
								);

const ComponentInfoList BooleanAndList = { &BooleanAndInfo, &BooleanNandInfo, 0 };



//###############################################################
//###############################################################


BooleanAndView::BooleanAndView(Component * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
}
BooleanAndView::~BooleanAndView()
{
}

void BooleanAndView::draw(QPainter * p)
{
	QRect rect(getWidgetPlace().topLeft()+QPoint(1,1),
							getWidgetPlace().bottomRight()-QPoint(0,0));
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
	
	p->drawText(rect, AlignCenter, "&");

	CompView::draw(p);
}


//###############################################################

BooleanAnd::BooleanAnd(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorBoolOut (this, i18n("Output"));
	CHECK_PTR(m_out);
	
	m_inPack = new ConnectorPack(this, QString("Input"), &ConnectorBoolInInfo, 2, 10);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
	
	// make Nand
	if (ci == &BooleanNandInfo)
	{
		m_out->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanAndView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
	
	ComponentLayout * lay = new ComponentLayout(this);
	
	lay->getLeft()->addSpace(1);
	lay->getLeft()->addConnectorPack(m_inPack);
	
	lay->getRight()->addStretch(2);
	lay->getRight()->addConnector(m_out,0);
	lay->getRight()->addStretch(2);
	
	lay->updateLayout();
}

/** Executes the simulation of this component */
void BooleanAnd::calculate()
{
	Component::calculate();
	m_result = true;
	
	FOR_EACH_CONNECTOR(it, *m_inPack->getConnList())
	{
		m_result &= ((ConnectorBoolIn*)it.current())->getInput();
	}
}

/** Shift the result of calculation to output */
void BooleanAnd::updateOutput()
{
	Component::updateOutput();
	m_out->setOutput(m_result);
}
/** Reset all simulation variables */
void BooleanAnd::reset()
{
	Component::reset();
	m_result = false;
}

