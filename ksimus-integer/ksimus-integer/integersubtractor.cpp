/***************************************************************************
                          integersubtractor.cpp  -  description
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

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "integersubtractor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibInteger
{

Component * IntegerSubtractor::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerSubtractor(container, ci);
}

const ComponentInfo * IntegerSubtractor::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Subtractor"),
	                                QString::fromLatin1("Integer/Arithmetic/Subtractor"),
	                                i18n("Component", "Integer/Arithmetic/Subtractor"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-sub")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerSubtractor::View::draw(QPainter * p)
{
	Integer2In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Sub");
}


//###############################################################
//###############################################################

IntegerSubtractor::IntegerSubtractor(CompContainer * container, const ComponentInfo * ci)
	: Integer2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getInputA()->setName(i18n("Integer", "Minuend"));
	getInputB()->setName(i18n("Integer", "Subtrahend"));
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerSubtractor::calculate()
{
//	Integer2In1Out::calculate();
	
	setValue(getInputA()->getInput() - getInputB()->getInput());
	
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

