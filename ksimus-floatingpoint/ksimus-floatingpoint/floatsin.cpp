/***************************************************************************
                          floatsin.cpp  -  description
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
#include "floatsin.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatSin::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatSin(container, ci);
}

const ComponentInfo * FloatSin::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point sin(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/sin(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/sin(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-sin"));
	return &Info;
}


Component * FloatASin::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatASin(container, ci);
}

const ComponentInfo * FloatASin::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point asin(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/asin(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/asin(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-asin"));
	return &Info;
}

Component * FloatSinh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatSinh(container, ci);
}

const ComponentInfo * FloatSinh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point sinh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/sinh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/sinh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-sinh"));
	return &Info;
}

Component * FloatASinh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatASinh(container, ci);
}

const ComponentInfo * FloatASinh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point asinh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/asinh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/asinh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-asinh"));
	return &Info;
}

//###############################################################
//###############################################################

FloatSin::FloatSin(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("sin"), "FloatSinSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatSin::calculate()
{
	Float1In1Out::calculate();

	setValue(sin(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatASin::FloatASin(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\nsin"), "FloatASinSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatASin::calculate()
{
	Float1In1Out::calculate();
	
	setValue(asin(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatSinh::FloatSinh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("sin\nhyp"), "FloatSinhSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatSinh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(sinh(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatASinh::FloatASinh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\nsin\nhyp"), "FloatASinhSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatASinh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(asinh(getInput()->getInput()));
}


//###############################################################
//###############################################################



};  //namespace KSimLibFloatingPoint

