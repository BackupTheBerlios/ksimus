/***************************************************************************
                          integerand.cpp  -  description
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
#include <limits.h>

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"
#include "integerand.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerAnd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerAnd(container, ci);
}

const ComponentInfo * IntegerAnd::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer AND"),
	                                QString::fromLatin1("Integer/Logic/AND"),
	                                i18n("Component", "Integer/Logic/AND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerAnd::IntegerAnd(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, QString::fromLatin1("&"), "IntegerAndSV");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerAnd::calculate()
{
	IntegerXIn1Out::calculate();

	unsigned int result = UINT_MAX; // all ones

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerNand::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerNand(container, ci);
}

const ComponentInfo * IntegerNand::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer NAND"),
	                                QString::fromLatin1("Integer/Logic/NAND"),
	                                i18n("Component", "Integer/Logic/NAND"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerNand::IntegerNand(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, QString::fromLatin1("!&"), "IntegerNandSV");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerNand::calculate()
{
	IntegerXIn1Out::calculate();

	unsigned int result = UINT_MAX;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result &= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

