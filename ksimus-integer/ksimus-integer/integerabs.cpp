/***************************************************************************
                          integerabs.cpp  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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
#include <stdlib.h>

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "integerabs.h"
#include "ksimus/connectorintegerin.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerAbs::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerAbs(container, ci);
}

const ComponentInfo * IntegerAbs::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Absolute Value"),
	                                QString::fromLatin1("Integer/Arithmetic/Absolute Value"),
	                                i18n("Component", "Integer/Arithmetic/Absolute Value"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-abs")*/);
	return &Info;
}

//###############################################################
//###############################################################

IntegerAbs::IntegerAbs(CompContainer * container, const ComponentInfo * ci)
	: Integer1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Integer1In1OutView(this, SHEET_VIEW, "Abs");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerAbs::calculate()
{
	Integer1In1Out::calculate();
	
	setValue(abs(getInputConnector()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

