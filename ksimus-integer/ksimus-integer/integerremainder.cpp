/***************************************************************************
                          integerremainder.cpp  -  description
                             -------------------
    begin                : Fri Aug 1 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include <stdlib.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "integerremainder.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibInteger
{

Component * IntegerRemainder::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerRemainder(container, ci);
}

const ComponentInfo * IntegerRemainder::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Remainder"),
	                                QString::fromLatin1("Integer/Arithmetic/Remainder"),
	                                i18n("Component", "Integer/Arithmetic/Remainder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-integer-rem")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerRemainder::View::draw(QPainter * p)
{
	Integer2In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Mod");
}


//###############################################################
//###############################################################

IntegerRemainder::IntegerRemainder(CompContainer * container, const ComponentInfo * ci)
	: Integer2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getInputA()->setName(i18n("Integer", "Dividend"), true);
	getInputB()->setName(i18n("Integer", "Divisor"), true);
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerRemainder::calculate()
{
//	Integer2In1Out::calculate();

	const int a(getInputA()->getInput());
	const int b(getInputB()->getInput());

	if (b != 0)
	{
		const div_t res = div(a, b);
		setValue(res.rem);
	}
	else
	{
		setValue(0);
	}
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

