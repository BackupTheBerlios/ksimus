/***************************************************************************
                          floatremainder.cpp  -  description
                             -------------------
    begin                : Fri Feb 1 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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
#include "floatremainder.h"
#include "ksimus/resource.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatRemainder(container, ci);
}

const ComponentInfo FloatRemainderInfo(I18N_NOOP("Floating Point Remainder"),
                                       I18N_NOOP("Floating Point/Arithmetic/Remainder"),
                                       QString::null,
                                       VA_SHEETVIEW,
                                       create
                                       );





//###############################################################
//###############################################################


void FloatRemainderView::draw(QPainter * p)
{
	Float2In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Mod");
}


//###############################################################
//###############################################################

FloatRemainder::FloatRemainder(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatRemainderView(this, SHEET_VIEW);
	}

	getInputA()->setName(I18N_NOOP("Dividend"), true);
	getInputB()->setName(I18N_NOOP("Divisor"), true);
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatRemainder::calculate()
{
	Float2In1Out::calculate();
	
	setValue(fmod(getInputA()->getInput(), getInputB()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

