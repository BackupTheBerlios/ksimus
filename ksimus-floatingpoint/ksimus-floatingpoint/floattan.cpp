/***************************************************************************
                          floattan.cpp  -  description
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
#include "floattan.h"
#include "ksimus/connectorfloatin.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatTan::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatTan(container, ci);
}

const ComponentInfo * FloatTan::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point tan(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/tan(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/tan(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-tan"));
	return &Info;
}


Component * FloatATan::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatATan(container, ci);
}

const ComponentInfo * FloatATan::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point atan(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/atan(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/atan(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-atan"));
	return &Info;
}



Component * FloatATan2::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatATan2(container, ci);
}

const ComponentInfo * FloatATan2::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point atan2(x,y)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/atan2(x,y)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/atan2(x,y)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-atan2"));
	return &Info;
}



Component * FloatTanh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatTanh(container, ci);
}

const ComponentInfo * FloatTanh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point tanh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/tan(hx)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/tanh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-tanh"));
	return &Info;
}


Component * FloatATanh::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatATanh(container, ci);
}

const ComponentInfo * FloatATanh::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point atanh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/atanh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/atanh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-atanh"));
	return &Info;
}




//###############################################################
//###############################################################

FloatTan::FloatTan(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("tan"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatTan::calculate()
{
	Float1In1Out::calculate();
	
	setValue(tan(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatATan::FloatATan(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\ntan"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatATan::calculate()
{
	Float1In1Out::calculate();
	
	setValue(atan(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatATan2::FloatATan2(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float2In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\ntan\n2"));
	}
	getInputA()->setName(i18n("FloatingPoint", "Input X"));
	getInputB()->setName(i18n("FloatingPoint", "Input Y"));

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatATan2::calculate()
{
	Float2In1Out::calculate();
	
	setValue(atan2(getInputA()->getInput(), getInputB()->getInput()));
}

//###############################################################
//###############################################################

FloatTanh::FloatTanh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("tan\nhyp"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatTanh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(tanh(getInput()->getInput()));
}

//###############################################################
//###############################################################

FloatATanh::FloatATanh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Float1In1OutView(this, SHEET_VIEW, QString::fromLatin1("arc\ntan\nhyp"));
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatATanh::calculate()
{
	Float1In1Out::calculate();
	
	setValue(atanh(getInput()->getInput()));
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

