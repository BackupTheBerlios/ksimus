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
	QRect rect(getPlace().topLeft()+QPoint(gridX+1,1),
							getPlace().bottomRight()-QPoint(gridX-0,0));
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
//	p->drawRect(getPlace().x()+gridX*1+1, getPlace().y()+1, gridX*3-1, gridY*5-1);
	p->drawRect(rect);
	
	p->drawText(rect, AlignCenter, "&");
/*	p->drawText(getPlace().x()+gridX+1, getPlace().y()+gridY+1,
				getPlace().width()-gridX*2-1, getPlace().height()-gridY*2-1,
				AlignCenter, "&");*/

	CompView::draw(p);
}


//###############################################################

BooleanAnd::BooleanAnd(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	out = new ConnectorBoolOut (this, i18n("Output"), QPoint(4,2));
	CHECK_PTR(out);
	inA = new ConnectorBoolIn (this, i18n("Input A"), QPoint(0,1));
	CHECK_PTR(inA);
	inB = new ConnectorBoolIn (this, i18n("Input B"), QPoint(0,3));
	CHECK_PTR(inB);
	
	// make Nand
	if (ci == &BooleanNandInfo)
	{
		out->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanAndView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

BooleanAnd::~BooleanAnd()
{
}

/** Executes the simulation of this component */
void BooleanAnd::calculate()
{
	Component::calculate();
	result = inA->getInput() && inB->getInput();
}

/** Shift the result of calculation to output */
void BooleanAnd::updateOutput()
{
	Component::updateOutput();
	out->setOutput(result);
}
/** Reset all simulation variables */
void BooleanAnd::reset()
{
	Component::reset();
	result = false;
}

