/***************************************************************************
                          integeradd.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
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
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"
#include "integeradd.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerAdd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerAdd(container, ci);
}

const ComponentInfo * IntegerAdd::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Adder"),
	                                QString::fromLatin1("Integer/Arithmetic/Adder"),
	                                i18n("Component", "Integer/Arithmetic/Adder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null /* TODO,
	                                QString::fromLatin1("component-float-arithmetic-add")*/);
	return &Info;
}





//###############################################################
//###############################################################

void IntegerAdd::View::draw(QPainter * p)
{
	IntegerXIn1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Add");
}


//###############################################################
//###############################################################

IntegerAdd::IntegerAdd(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	getInputConnectorPack()->setConnectorName(i18n("Connector", "Summand %1"));

	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

/** Executes the simulation of this component */
void IntegerAdd::calculate()
{
//	IntegerXIn1Out::calculate();
	
	int result = 0;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result += ((ConnectorIntegerIn*)it.current())->getInput();
	}
	
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

