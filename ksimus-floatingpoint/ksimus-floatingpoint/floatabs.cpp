/***************************************************************************
                          floatabs.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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
#include "floatabs.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatAbs::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatAbs(container, ci);
}

const ComponentInfo * FloatAbs::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Absolute Value"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Absolute Value"),
	                                i18n("Component", "Floating Point/Arithmetic/Absolute Value"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-abs"));
	return &Info;
}





//###############################################################
//###############################################################


void FloatAbsView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Abs");
}


//###############################################################
//###############################################################

FloatAbs::FloatAbs(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatAbsView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatAbs::calculate()
{
	Float1In1Out::calculate();
	
	setValue(fabs(getInput()->getInput()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

