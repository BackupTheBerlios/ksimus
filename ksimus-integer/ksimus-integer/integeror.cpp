/***************************************************************************
                          integeror.cpp  -  description
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
#include "integeror.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerOr::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerOr(container, ci);
}

const ComponentInfo * IntegerOr::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer OR"),
	                                QString::fromLatin1("Integer/Logic/OR"),
	                                i18n("Component", "Integer/Logic/OR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerOr::IntegerOr(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, QString::null + QChar(0x2265) + "1");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerOr::calculate()
{
	IntegerXIn1Out::calculate();
	
	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)result);
}


//###############################################################
//###############################################################

Component * IntegerNor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerNor(container, ci);
}

const ComponentInfo * IntegerNor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer NOR"),
	                                QString::fromLatin1("Integer/Logic/NOR"),
	                                i18n("Component", "Integer/Logic/NOR"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}

//###############################################################

IntegerNor::IntegerNor(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, QString("!") + QChar(0x2265) + "1");
	}
}

//###############################################################

/** Executes the simulation of this component */
void IntegerNor::calculate()
{
	IntegerXIn1Out::calculate();
	
	unsigned int result = 0;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= (unsigned int)((ConnectorIntegerIn*)it.current())->getInput();
	}

	setValue((int)~result);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

