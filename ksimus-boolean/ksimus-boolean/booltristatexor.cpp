/***************************************************************************
                      booltristatexor.h  -  description
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
#include "booltristatexor.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BoolTriStateXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BoolTriStateXor(container, ci);
}

const ComponentInfo * BoolTriStateXor::getStaticXorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean XOR with TriState output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/XOR TriState"),
	                                i18n("Component", "Boolean/Tristate/Gates/XOR with TriState output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-and")*/);
	return &Info;
}

const ComponentInfo * BoolTriStateXor::getStaticXnorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean XNOR with TriState output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/XNOR TriState"),
	                                i18n("Component", "Boolean/Tristate/Gates/XNOR with TriState output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-nand")*/);
	return &Info;
}

//###############################################################
//###############################################################

BoolTriStateXor::BoolTriStateXor(CompContainer * container, const ComponentInfo * ci)
	: BoolTriStateXIn1Out1Ena(container, ci)
{

	// make XNOR
	if (ci == getStaticXnorInfo())
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BoolTriStateXorView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BoolTriStateXor::calculate()
{
	BoolTriStateXIn1Out1Ena::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnector()->getConnList())
	{
		result ^= ((ConnectorBoolIn*)it.current())->getInput();
	}
	
	setState(result);
}


//###############################################################

}; //namespace KSimLibBoolean

