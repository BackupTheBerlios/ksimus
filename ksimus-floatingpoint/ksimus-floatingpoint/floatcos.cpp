/***************************************************************************
                          floatcos.cpp  -  description
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
#include "floatcos.h"
#include "ksimus/connectorfloatin.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatCos::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatCos(container, ci);
}

const ComponentInfo * FloatCos::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point cos(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/cos(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/cos(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-cos"));
	return &Info;
}


Component * FloatACos::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatACos(container, ci);
}

const ComponentInfo * FloatACos::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point acos(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/acos(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/acos(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-acos"));
	return &Info;
}



Component * FloatCosh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatCosh(container, ci);
}

const ComponentInfo * FloatCosh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point cosh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/cosh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/cosh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-cosh"));
	return &Info;
}


Component * FloatACosh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatACosh(container, ci);
}

const ComponentInfo * FloatACosh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point acosh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/acosh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/acosh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-acosh"));
	return &Info;
}

//###############################################################
//###############################################################

FloatCos::FloatCos(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("cos"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatCos::calculate()
{
	Float1In1Out::calculate();
	
	setValue(cos(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatACos::FloatACos(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\ncos"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatACos::calculate()
{
	Float1In1Out::calculate();
	
	setValue(acos(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatCosh::FloatCosh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("cos\nhyp"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatCosh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(cosh(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatACosh::FloatACosh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\ncos\nhyp"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatACosh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(acosh(getInput()->getInput()));
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

