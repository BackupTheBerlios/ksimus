/***************************************************************************
                          booleanxor.cpp  -  description
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
#include "booleanxor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BooleanXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanXor(container, ci);
}

const ComponentInfo * BooleanXor::getStaticXorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Exclusive OR"),
	                                QString::fromLatin1("Boolean/Gates/XOR"),
	                                i18n("Component", "Boolean/Gates/XOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-xor"));
	return &Info;
}

const ComponentInfo * BooleanXor::getStaticXnorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Exclusive NOR"),
	                                QString::fromLatin1("Boolean/Gates/XNOR"),
	                                i18n("Component", "Boolean/Gates/XNOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-xnor"));
	return &Info;
}


//###############################################################


BooleanXor::BooleanXor(CompContainer * container, const ComponentInfo * ci)
	: BooleanXIn1Out(container, ci)
{
	// make Xnor
	if (ci == BooleanXor::getStaticXnorInfo())
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

		new ComponentLayoutBlockContentText(block, "=1");
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}
/*BooleanXor::~BooleanXor()
{
} */

void BooleanXor::calculate()
{
	BooleanXIn1Out::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result ^= ((ConnectorBoolIn*)it.current())->getInput();
	}
	
	setState(result);
}


}; //namespace KSimLibBoolean

