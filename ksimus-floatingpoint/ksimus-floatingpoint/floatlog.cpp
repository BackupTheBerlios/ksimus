/***************************************************************************
                          floatexp.cpp  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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
#include <math.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floatlog.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatLog::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLog(container, ci);
}

const ComponentInfo * FloatLog::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point log(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Exponentiation & Logarithms/log(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Exponentiation & Logarithms/log(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-log"));
	return &Info;
}



Component * FloatLog10::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLog10(container, ci);
}

const ComponentInfo * FloatLog10::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point log10(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Exponentiation & Logarithms/log10(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Exponentiation & Logarithms/log10(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-log10"));
	return &Info;
}

//###############################################################
//###############################################################

FloatLog::FloatLog(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("log"), "FloatLogSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatLog::calculate()
{
	Float1In1Out::calculate();
	
	setValue(log(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatLog10::FloatLog10(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("log\n10"), "FloatLog10SV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatLog10::calculate()
{
	Float1In1Out::calculate();
	
	setValue(log10(getInput()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

