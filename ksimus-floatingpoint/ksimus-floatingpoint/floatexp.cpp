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
#include "floatexp.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * createExp(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatExp(container, ci);
}

const ComponentInfo * getFloatExpInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point exp(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Exponentiation & Logarithms/exp(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Exponentiation & Logarithms/exp(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createExp,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-exp"));
	return &Info;
}



static Component * createPow(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatPow(container, ci);
}

const ComponentInfo * getFloatPowInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point pow(base,power)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Exponentiation & Logarithms/pow(base,power)"),
	                                i18n("Component", "Floating Point/Arithmetic/Exponentiation & Logarithms/pow(base,power)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createPow,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-pow"));
	return &Info;
}



static Component * createSqrt(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatSqrt(container, ci);
}

const ComponentInfo * getFloatSqrtInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point square root"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Exponentiation & Logarithms/square root"),
	                                i18n("Component", "Floating Point/Arithmetic/Exponentiation & Logarithms/square root"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createSqrt,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-sqrt"));
	return &Info;
}





//###############################################################
//###############################################################


void FloatExpView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "exp");
}


//###############################################################
//###############################################################

FloatExp::FloatExp(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatExpView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatExp::calculate()
{
	Float1In1Out::calculate();
	
	setValue(exp(getInput()->getInput()));
}


//###############################################################
//###############################################################

//###############################################################
//###############################################################


void FloatPowView::draw(QPainter * p)
{
	Float2In1OutView::draw(p);

	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "pow");
}


//###############################################################
//###############################################################

FloatPow::FloatPow(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatPowView(this, SHEET_VIEW);
	}
	getInputA()->setName(i18n("FloatingPoint", "Power"));
	getInputB()->setName(i18n("FloatingPoint", "Base"));

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatPow::calculate()
{
	Float2In1Out::calculate();

	setValue(pow(getInputB()->getInput(), getInputA()->getInput()));
}


//###############################################################
//###############################################################

//###############################################################
//###############################################################


void FloatSqrtView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);

	QFont newFont("helvetica",8);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "sqrt");
}


//###############################################################
//###############################################################

FloatSqrt::FloatSqrt(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatSqrtView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatSqrt::calculate()
{
	Float1In1Out::calculate();

	setValue(sqrt(getInput()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

