/***************************************************************************
                          floatmax.cpp  -  description
                             -------------------
    begin                : Sat Feb 23 2002
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

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floatmax.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatMax(container, ci);
}

const ComponentInfo FloatMaxInfo(I18N_NOOP("Floating Point Maximum"),
                                 I18N_NOOP("Floating Point/Control/Maximum"),
                                 QString::null,
                                 VA_SHEETVIEW,
                                 create
                                 );





//###############################################################
//###############################################################


void FloatMaxView::draw(QPainter * p)
{
	FloatXIn1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Max");
}


//###############################################################
//###############################################################

FloatMax::FloatMax(CompContainer * container, const ComponentInfo * ci)
	: FloatXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatMaxView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatMax::calculate()
{
	FloatXIn1Out::calculate();
	
	QListIterator<ConnectorBase> it(*getInputConnectorPack()->getConnList());
	
	
	// Hint: Gate has min 2 inputs !!! No check is required.
	double result = ((ConnectorFloatIn*)it.current())->getInput();
	++it;
		
	while (it.current())		
	{
		result = QMAX(result, ((ConnectorFloatIn*)it.current())->getInput());
		++it;
	}
		
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

