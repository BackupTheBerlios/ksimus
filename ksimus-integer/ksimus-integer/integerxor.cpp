/***************************************************************************
                          integerxor.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"
#include "integerxor.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerXor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerXor(container, ci);
}

const ComponentInfo * IntegerXor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer XOR"),
	                                QString::fromLatin1("Integer/Logic/XOR"),
	                                i18n("Component", "Integer/Logic/XOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerXor::IntegerXor(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, "=1");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerXor::calculate()
{
	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result ^= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerXnor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerXnor(container, ci);
}

const ComponentInfo * IntegerXnor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer XNOR"),
	                                QString::fromLatin1("Integer/Logic/XNOR"),
	                                i18n("Component", "Integer/Logic/XNOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerXnor::IntegerXnor(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, "!=1");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerXnor::calculate()
{
	IntegerXIn1Out::calculate();

	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result ^= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

