/***************************************************************************
                          rsflipflop.cpp  -  description
                             -------------------
    begin                : Sun Aug 19 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorlabel.h"

// Project-Includes
#include "rsflipflop.h"

// Forward declaration

namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new RSFlipFlop(container, ci);
}

const ComponentInfo RSFlipFlopInfo("RS Flip Flop",
                                   "Boolean/Flip Flop/RS-FF",
                                   QString::null,//"RS Flip Flop",
                                   VA_SHEETVIEW,
                                   create
                                  );




RSFlipFlop::RSFlipFlop(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorBoolOut (this, "Output", QPoint(4,1));
	CHECK_PTR(m_out);
	new ConnectorLabel(m_out, "Q");
	
	m_outNot = new ConnectorBoolOut (this, "/Output", QPoint(4,3));
	CHECK_PTR(m_outNot);
	m_outNot->setNegate(true, true);
	new ConnectorLabel(m_outNot, "Q");
	
	m_inSet = new ConnectorBoolIn (this, "Set", QPoint(0,1));
	CHECK_PTR(m_inSet);
	new ConnectorLabel(m_inSet, "S");
	
	m_inReset = new ConnectorBoolIn (this, "Reset", QPoint(0,3));
	CHECK_PTR(m_inReset);
	new ConnectorLabel(m_inReset, "R");
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new RSFlipFlopView(this, SHEET_VIEW);
	}
}

/*RSFlipFlop::~RSFlipFlop()
{
} */

	/** Shift the result of calculation to output */
void RSFlipFlop::updateOutput()
{
	Component::updateOutput();
	
	m_out->setOutput(m_FFstate);
	m_outNot->setOutput(m_FFstate);
}

	/** Reset all simulation variables */
void RSFlipFlop::reset()
{
	Component::reset();
	
	m_FFstate = false;
}


void RSFlipFlop::calculate()
{
	Component::calculate();
	
	if (m_inReset->getInput())
	{
		m_FFstate = false;
	}
	else if (m_inSet->getInput())
	{
		m_FFstate = true;
	}
}


//###############################################################
//###############################################################



RSFlipFlopView::RSFlipFlopView(Component * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
}
/*RSFlipFlopView::~RSFlipFlopView()
{
}*/

void RSFlipFlopView::draw(QPainter * p)
{
	QRect rect(getPlace().topLeft()+QPoint(gridX+1,1),
							getPlace().bottomRight()-QPoint(gridX-0,0));
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
	

	CompView::draw(p);
}





	
}; //namespace


