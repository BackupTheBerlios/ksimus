/***************************************************************************
                          floatdivider.cpp  -  description
                             -------------------
    begin                : Sat Feb 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floatdivider.h"
#include "ksimus/connectorfloatin.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

Component * FloatDivider::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatDivider(container, ci);
}

const ComponentInfo * FloatDivider::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Divider"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Divider"),
	                                i18n("Component", "Floating Point/Arithmetic/Divider"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-div"));
	return &Info;
}

//###############################################################
//###############################################################

FloatDivider::FloatDivider(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float2In1OutView(this, SHEET_VIEW, QString::fromLatin1("Div"), "FloatDividerSV");
	}

	getInputA()->setName(i18n("FloatingPoint", "Dividend"), true);
	getInputB()->setName(i18n("FloatingPoint", "Divisor"), true);
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatDivider::calculate()
{
	Float2In1Out::calculate();
	
	setValue(getInputA()->getInput() / getInputB()->getInput());
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

