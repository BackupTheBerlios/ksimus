/***************************************************************************
                          integerinverter.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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
#include "integerinverter.h"
#include "ksimus/connectorintegerin.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerBitwiseInverter::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerBitwiseInverter(container, ci);
}

const ComponentInfo * IntegerBitwiseInverter::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Bitwise Inverter"),
	                                QString::fromLatin1("Integer/Logic/Bitwise Inverter"),
	                                i18n("Component", "Integer/Logic/Bitwise Inverter"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-abs")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerBitwiseInverter::View::draw(QPainter * p)
{
	Integer1In1OutView::draw(p);
	
	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "bool\nNand");
}


//###############################################################
//###############################################################

IntegerBitwiseInverter::IntegerBitwiseInverter(CompContainer * container, const ComponentInfo * ci)
	: Integer1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerBitwiseInverter::calculate()
{
//	Integer1In1Out::calculate();
	
	setValue(~getInputConnector()->getInput());
}


//###############################################################
//###############################################################


Component * IntegerLogicalInverter::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLogicalInverter(container, ci);
}

const ComponentInfo * IntegerLogicalInverter::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Logical Inverter"),
	                                QString::fromLatin1("Integer/Logic/Logical Inverter"),
	                                i18n("Component", "Integer/Logic/Logical Inverter"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-abs")*/);
	return &Info;
}





//###############################################################
//###############################################################


void IntegerLogicalInverter::View::draw(QPainter * p)
{
	Integer1In1OutView::draw(p);

	QFont newFont("helvetica",9);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "log\nInv");
}


//###############################################################
//###############################################################

IntegerLogicalInverter::IntegerLogicalInverter(CompContainer * container, const ComponentInfo * ci)
	: Integer1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerLogicalInverter::calculate()
{
//	Integer1In1Out::calculate();

	setValue(!getInputConnector()->getInput());
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

