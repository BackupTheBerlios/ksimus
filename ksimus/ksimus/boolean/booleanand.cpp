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
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "connectorpack.h"
#include "ksimembfont.h"


// Forward declaration

// Project-Includes

Component * BooleanAnd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanAnd(container, ci);
}

const ComponentInfo * BooleanAnd::getStaticAndInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean AND"),
	                                QString::fromLatin1("Boolean/Gates/AND"),
	                                i18n("Component", "Boolean/Gates/AND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-and"));
	return &Info;
}

const ComponentInfo * BooleanAnd::getStaticNandInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean NAND"),
	                                QString::fromLatin1("Boolean/Gates/NAND"),
	                                i18n("Component", "Boolean/Gates/NAND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-nand"));
	return &Info;
}




//###############################################################
//###############################################################

void BooleanAndView::draw(QPainter * p)
{
	BooleanXIn1OutView::draw(p);

	g_embFont10->drawText(p, getDrawingPlace(), AlignCenter, QString::fromLatin1("&"));
}


//###############################################################
//###############################################################

BooleanAnd::BooleanAnd(CompContainer * container, const ComponentInfo * ci)
	: BooleanXIn1Out(container, ci)
{
	
	// make NAND
	if (ci == getStaticNandInfo())
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
	
/*	bool result = true;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= ((ConnectorBoolIn*)it.current())->getInput();
		if (!result) break;	//  No more changes possible
	}
	
	setState(result);*/


	bool result = true;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (!((ConnectorBoolIn*)it.current())->getInput())
		{
			// One false input causes a false output
			result = false;
			break;
		}
	}

	setState(result);
}

