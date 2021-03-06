/***************************************************************************
                          integerinverterarithmetic.cpp  -  description
                             -------------------
    begin                : Sun Aug 17 2003
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
#include "integerinverterarithmetic.h"
#include "ksimus/connectorintegerin.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerArithmeticInverter::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerArithmeticInverter(container, ci);
}

const ComponentInfo * IntegerArithmeticInverter::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Inverter (arithmetic)"),
	                                QString::fromLatin1("Integer/Arithmetic/Inverter"),
	                                i18n("Component", "Integer/Arithmetic/Inverter"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-abs")*/);
	return &Info;
}





//###############################################################
//###############################################################

IntegerArithmeticInverter::IntegerArithmeticInverter(CompContainer * container, const ComponentInfo * ci)
	: Integer1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Integer1In1OutView(this, SHEET_VIEW, QString::fromLatin1("Inv"), "IntegerArithmeticInverterSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerArithmeticInverter::calculate()
{
	Integer1In1Out::calculate();

	setValue(-getInputConnector()->getInput());
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

