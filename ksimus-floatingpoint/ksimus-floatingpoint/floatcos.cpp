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
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * createCos(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatCos(container, ci);
}

const ComponentInfo * getFloatCosInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point cos(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/cos(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/cos(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createCos,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-cos"));
	return &Info;
}


static Component * createACos(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatACos(container, ci);
}

const ComponentInfo * getFloatACosInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point acos(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/acos(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/acos(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createACos,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-acos"));
	return &Info;
}



static Component * createCosh(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatCosh(container, ci);
}

const ComponentInfo * getFloatCoshInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point cosh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/cosh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/cosh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createCosh,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-cosh"));
	return &Info;
}


static Component * createACosh(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatACosh(container, ci);
}

const ComponentInfo * getFloatACoshInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point acosh(x)"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Trigonometric/Hyperbolic/acosh(x)"),
	                                i18n("Component", "Floating Point/Arithmetic/Trigonometric/Hyperbolic/acosh(x)"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createACosh,
	                                QString::null,
	                                QString::fromLatin1("component-float-trigonometric-acosh"));
	return &Info;
}




//###############################################################
//###############################################################


void FloatCosView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "cos");
}


//###############################################################
//###############################################################

FloatCos::FloatCos(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatCosView(this, SHEET_VIEW);
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


//###############################################################
//###############################################################


void FloatACosView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "arc\ncos");
}


//###############################################################
//###############################################################

FloatACos::FloatACos(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatACosView(this, SHEET_VIEW);
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


//###############################################################
//###############################################################


void FloatCoshView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);

	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "cos\nhyp");
}


//###############################################################
//###############################################################

FloatCosh::FloatCosh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatCoshView(this, SHEET_VIEW);
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


//###############################################################
//###############################################################


void FloatACoshView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);

	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "arc\ncos\nhyp");
}


//###############################################################
//###############################################################

FloatACosh::FloatACosh(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatACoshView(this, SHEET_VIEW);
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

