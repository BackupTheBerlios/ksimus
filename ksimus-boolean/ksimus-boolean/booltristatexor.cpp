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
#include "booltristatexor.h"

// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

Component * BoolTristateXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BoolTristateXor(container, ci);
}

const ComponentInfo * BoolTristateXor::getStaticXorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean XOR with Tristate output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/XOR Tristate"),
	                                i18n("Component", "Boolean/Tristate/Gates/XOR with Tristate output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-tristate-xor"));
	return &Info;
}

const ComponentInfo * BoolTristateXor::getStaticXnorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean XNOR with Tristate output"),
	                                QString::fromLatin1("Boolean/Tristate/Gates/XNOR Tristate"),
	                                i18n("Component", "Boolean/Tristate/Gates/XNOR with Tristate output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-tristate-xnor"));
	return &Info;
}

//###############################################################
//###############################################################

BoolTristateXor::BoolTristateXor(CompContainer * container, const ComponentInfo * ci)
	: BoolTristateXIn1Out1Ena(container, ci)
{

	// make XNOR
	if (ci == getStaticXnorInfo())
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BoolTristateXIn1Out1EnaView(this, SHEET_VIEW, QString::fromLatin1("=1"), "BoolTristateXorSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BoolTristateXor::calculate()
{
	BoolTristateXIn1Out1Ena::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnector()->getConnList())
	{
		result ^= ((ConnectorBoolIn*)it.current())->getInput();
	}
	
	setState(result);
}


//###############################################################

}; //namespace KSimLibBoolean

#include "booltristatexor.moc"
