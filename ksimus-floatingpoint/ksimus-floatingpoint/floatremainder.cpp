/***************************************************************************
                          floatremainder.cpp  -  description
                             -------------------
    begin                : Fri Feb 1 2002
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
#include <math.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorfloatin.h"
#include "floatremainder.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

Component * FloatRemainder::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatRemainder(container, ci);
}

const ComponentInfo * FloatRemainder::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Remainder"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Remainder"),
	                                i18n("Component", "Floating Point/Arithmetic/Remainder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-mod"));
	return &Info;
}


//###############################################################
//###############################################################

FloatRemainder::FloatRemainder(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float2In1OutView(this, SHEET_VIEW, QString::fromLatin1("Mod"), "FloatRemainderSV");
	}

	getInputA()->setName(i18n("FloatingPoint", "Dividend"), true);
	getInputB()->setName(i18n("FloatingPoint", "Divisor"), true);
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatRemainder::calculate()
{
	Float2In1Out::calculate();
	
	setValue(fmod(getInputA()->getInput(), getInputB()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

