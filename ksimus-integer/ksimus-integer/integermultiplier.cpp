/***************************************************************************
                          integermultiplier.cpp  -  description
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
#include "integermultiplier.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerMultiplier::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerMultiplier(container, ci);
}

const ComponentInfo * IntegerMultiplier::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Multiplier"),
	                                QString::fromLatin1("Integer/Arithmetic/Multiplier"),
	                                i18n("Component", "Integer/Arithmetic/Multiplier"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-mul")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerMultiplier::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Mul");
}


//###############################################################
//###############################################################

IntegerMultiplier::IntegerMultiplier(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	getInputConnectorPack()->setConnectorName(i18n("Connector", "Factor %1"));

	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerMultiplier::calculate()
{
//	IntegerXIn1Out::calculate();
	
	int result = 1;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result *= ((ConnectorIntegerIn*)it.current())->getInput();
	}
	
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

