/***************************************************************************
                          floatadd.cpp  -  description
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
#include "floatadd.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatAdd(container, ci);
}

const ComponentInfo FloatAddInfo(I18N_NOOP("Floating Point Adder"),
                                 I18N_NOOP("Floating Point/Arithmetic/Adder"),
                                 QString::null,
                                 VA_SHEETVIEW,
                                 create
                                 );





//###############################################################
//###############################################################


void FloatAddView::draw(QPainter * p)
{
	FloatXIn1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Add");
}


//###############################################################
//###############################################################

FloatAdd::FloatAdd(CompContainer * container, const ComponentInfo * ci)
	: FloatXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatAddView(this, SHEET_VIEW);
	}

	getInputConnectorPack()->setConnectorName(i18n("Summand %1"));
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatAdd::calculate()
{
	FloatXIn1Out::calculate();
	
	double result = 0.0;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result += ((ConnectorFloatIn*)it.current())->getInput();
	}
	
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

