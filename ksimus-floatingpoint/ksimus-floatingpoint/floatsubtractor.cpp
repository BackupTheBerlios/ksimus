/***************************************************************************
                          floatsubtractor.cpp  -  description
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
#include "floatsubtractor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatSubtractor(container, ci);
}

const ComponentInfo * getFloatSubtractorInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Subtractor"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Subtractor"),
	                                i18n("Component", "Floating Point/Arithmetic/Subtractor"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-sub"));
	return &Info;
}





//###############################################################
//###############################################################


void FloatSubtractorView::draw(QPainter * p)
{
	Float2In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Sub");
}


//###############################################################
//###############################################################

FloatSubtractor::FloatSubtractor(CompContainer * container, const ComponentInfo * ci)
	: Float2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatSubtractorView(this, SHEET_VIEW);
	}

	getInputA()->setName(i18n("FloatingPoint", "Minuend"));
	getInputB()->setName(i18n("FloatingPoint", "Subtrahend"));
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatSubtractor::calculate()
{
	Float2In1Out::calculate();
	
	setValue(getInputA()->getInput() - getInputB()->getInput());
	
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

