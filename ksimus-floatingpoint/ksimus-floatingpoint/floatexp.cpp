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

static Component * create(CompContainer * container, const ComponentInfo * ci)
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
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-exp"));
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

};  //namespace KSimLibFloatingPoint

