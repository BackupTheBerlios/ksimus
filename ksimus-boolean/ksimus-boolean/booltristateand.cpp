/***************************************************************************
                      booltristateand.h  -  description
                             -------------------
    begin                : Sun Nov 17 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentinfo.h"
#include "booltristateand.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BoolTristateAnd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BoolTristateAnd(container, ci);
}

const ComponentInfo * BoolTristateAnd::getStaticAndInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean AND with Tristate output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/AND Tristate"),
	                                i18n("Component", "Boolean/Tristate/Gates/AND with Tristate output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-tristate-and"));
	return &Info;
}

const ComponentInfo * BoolTristateAnd::getStaticNandInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean NAND with Tristate output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/NAND Tristate"),
	                                i18n("Component", "Boolean/Tristate/Gates/NAND with Tristate output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-tristate-nand"));
	return &Info;
}

//###############################################################
//###############################################################

BoolTristateAnd::BoolTristateAnd(CompContainer * container, const ComponentInfo * ci)
	: BoolTristateXIn1Out1Ena(container, ci)
{
	
	// make NAND
	if (ci == getStaticNandInfo())
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BoolTristateXIn1Out1EnaView(this, SHEET_VIEW, "&");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BoolTristateAnd::calculate()
{
	BoolTristateXIn1Out1Ena::calculate();
	
	bool result = true;
	
	FOR_EACH_CONNECTOR(it, *getInputConnector()->getConnList())
	{
		result &= ((ConnectorBoolIn*)it.current())->getInput();
		if (!result) break;	//  No more changes possible
	}
	
	setState(result);
}


//###############################################################

}; //namespace KSimLibBoolean

#include "booltristateand.moc"
