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
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatDivider(container, ci);
}

const ComponentInfo FloatDividerInfo(I18N_NOOP("Floating Point Divider"),
                                     I18N_NOOP("Floating Point/Arithmetic/Divider"),
                                     QString::null,
                                     VA_SHEETVIEW,
                                     create,
                                     QString::null,
                                     "component-float-arithmetic-div");





//###############################################################
//###############################################################


void FloatDividerView::draw(QPainter * p)
{
	Float2In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Div");
}


//###############################################################
//###############################################################

FloatDivider::FloatDivider(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatDividerView(this, SHEET_VIEW);
	}

	getInputA()->setName(I18N_NOOP("Dividend"), true);
	getInputB()->setName(I18N_NOOP("Divisor"), true);
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

