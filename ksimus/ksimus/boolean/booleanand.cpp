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

const ComponentInfo BooleanAndInfo(I18N_NOOP("Boolean AND"),
                                   I18N_NOOP("Boolean/Gates/AND"),
                                   QString::null,
                                   VA_SHEETVIEW,
                                   create,	
                                   QString::null,
                                   "component-boolean-and");

const ComponentInfo BooleanNandInfo(I18N_NOOP("Boolean NAND"),
                                    I18N_NOOP("Boolean/Gates/NAND"),
                                    QString::null,
                                    VA_SHEETVIEW,
                                    create,	
                                    QString::null,
                                    "component-boolean-nand");

const ComponentInfoList BooleanAndList = { &BooleanAndInfo, &BooleanNandInfo, 0 };



//###############################################################
//###############################################################


void BooleanAndView::draw(QPainter * p)
{
	BooleanXIn1OutView::draw(p);
	
	p->drawText(getDrawingPlace(), AlignCenter, "&");
}


//###############################################################
//###############################################################

BooleanAnd::BooleanAnd(CompContainer * container, const ComponentInfo * ci)
	: BooleanXIn1Out(container, ci)
{
	
	// make NAND
	if (ci == &BooleanNandInfo)
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanAndView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BooleanAnd::calculate()
{
	BooleanXIn1Out::calculate();
	
	bool result = true;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= ((ConnectorBoolIn*)it.current())->getInput();
		if (!result) break;	//  No more changes possible
	}
	
	setState(result);
}

