/***************************************************************************
                          integermax.cpp  -  description
                             -------------------
    begin                : Sun Aug 17 2003
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
#include "integermax.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerMax::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerMax(container, ci);
}

const ComponentInfo * IntegerMax::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Maximum"),
	                                QString::fromLatin1("Integer/Control/Maximum"),
	                                i18n("Component", "Integer/Control/Maximum"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-maximum")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerMax::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Max");
}


//###############################################################
//###############################################################

IntegerMax::IntegerMax(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerMax::calculate()
{
//	IntegerXIn1Out::calculate();
	
	QListIterator<ConnectorBase> it(*getInputConnectorPack()->getConnList());
	
	
	// Hint: Gate has min 2 inputs !!! No check is required.
	int result = ((ConnectorIntegerIn*)it.current())->getInput();
	++it;
		
	while (it.current())
	{
		result = QMAX(result, ((ConnectorIntegerIn*)it.current())->getInput());
		++it;
	}
		
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

