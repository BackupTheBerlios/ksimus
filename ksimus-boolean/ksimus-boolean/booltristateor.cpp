/***************************************************************************
                      booltristateor.cpp  -  description
                             -------------------
    begin                : Tue Nov 27 2002
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
#include "booltristateor.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BoolTriStateOr::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BoolTriStateOr(container, ci);
}

const ComponentInfo * BoolTriStateOr::getStaticOrInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean OR with TriState output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/OR TriState"),
	                                i18n("Component", "Boolean/Tristate/Gates/OR with TriState output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* todo,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-and")*/);
	return &Info;
}

const ComponentInfo * BoolTriStateOr::getStaticNorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean NOR with TriState output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/NOR TriState"),
	                                i18n("Component", "Boolean/Tristate/Gates/NOR with TriState output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-nand")*/);
	return &Info;
}

//###############################################################
//###############################################################

BoolTriStateOr::BoolTriStateOr(CompContainer * container, const ComponentInfo * ci)
	: BoolTriStateXIn1Out1Ena(container, ci)
{
	
	// make NOR
	if (ci == getStaticNorInfo())
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BoolTriStateOrView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BoolTriStateOr::calculate()
{
	BoolTriStateXIn1Out1Ena::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnector()->getConnList())
	{
		result |= ((ConnectorBoolIn*)it.current())->getInput();
		if (result) break;	//  No more changes possible
	}
	
	setState(result);
}


//###############################################################

}; //namespace KSimLibBoolean
