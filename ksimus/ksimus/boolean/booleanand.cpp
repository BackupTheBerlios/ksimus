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
#include "componentlayout.h"



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
		CompView * sheetView = new CompView(this, SHEET_VIEW);
		Q_CHECK_PTR(sheetView);
		
		ComponentLayout * layout = new ComponentLayout(sheetView, false);
		Q_CHECK_PTR(layout);
		layout->setMinSize(3,3);

		ComponentLayoutBlock * block = new ComponentLayoutBlock(layout);
		Q_CHECK_PTR(block);

		block->getLeft()->addSpace(1);
		block->getLeft()->addConnectorPack(getInputConnectorPack());

		block->getRight()->addStretch(1);
		block->getRight()->addConnector(getOutputConnector(),0);
		block->getRight()->addStretch(1);

		new ComponentLayoutBlockContentText(block, QString::fromLatin1("&"));
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
		if (!((ConnectorBoolIn*)it.current())->getInput())
		{
			// One false input causes a false output
			result = false;
			break;
		}
	}

	setState(result);
}

