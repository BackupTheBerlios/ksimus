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

const ComponentInfo FloatCosInfo(I18N_NOOP("Floating Point cos(x)"),
                                 I18N_NOOP("Floating Point/Arithmetic/Trigonometric/cos(x)"),
                                 QString::null,
                                 VA_SHEETVIEW,
                                 createCos,
                                 QString::null,
                                 "component-float-trigonometric-cos");


static Component * createACos(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatACos(container, ci);
}

const ComponentInfo FloatACosInfo(I18N_NOOP("Floating Point acos(x)"),
                                  I18N_NOOP("Floating Point/Arithmetic/Trigonometric/acos(x)"),
                                  QString::null,
                                  VA_SHEETVIEW,
                                  createACos,
                                  QString::null,
                                  "component-float-trigonometric-acos");




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
	p->drawText(getDrawingPlace(), AlignCenter, "acos");
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


};  //namespace KSimLibFloatingPoint

