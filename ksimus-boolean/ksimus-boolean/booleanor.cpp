/***************************************************************************
                          booleanor.cpp  -  description
                             -------------------
    begin                : Mon May 14 2001
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
#include <klocale.h>

// Project-Includes
#include "booleanor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"


// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BooleanOr::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanOr(container, ci);
}

const ComponentInfo * BooleanOr::getStaticOrInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean OR"),
	                                QString::fromLatin1("Boolean/Gates/OR"),
	                                i18n("Component", "Boolean/Gates/OR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-or"));
	return &Info;
}

const ComponentInfo * BooleanOr::getStaticNorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean NOR"),
	                                QString::fromLatin1("Boolean/Gates/NOR"),
	                                i18n("Component", "Boolean/Gates/NOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-nor"));
	return &Info;
}


//###############################################################

BooleanOr::BooleanOr(CompContainer * container, const ComponentInfo * ci)
	: BooleanXIn1Out(container, ci)
{
	// make Nor
	if (ci == BooleanOr::getStaticNorInfo())
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

		new ComponentLayoutBlockContentText(block, QString::null + QChar(0x2265) + "1");
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BooleanOr::calculate()
{
	BooleanXIn1Out::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= ((ConnectorBoolIn*)it.current())->getInput();
		if (result) break;	//  No more changes possible
	}
	
	setState(result);
}

}; //namespace KSimLibBoolean
