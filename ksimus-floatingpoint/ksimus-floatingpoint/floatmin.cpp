/***************************************************************************
                          floatmin.cpp  -  description
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
#include "floatmin.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatMin::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatMin(container, ci);
}

const ComponentInfo * FloatMin::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Minimum"),
	                                QString::fromLatin1("Floating Point/Control/Minimum"),
	                                i18n("Component", "Floating Point/Control/Minimum"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-minimum"));
	return &Info;
}

//###############################################################
//###############################################################

FloatMin::FloatMin(CompContainer * container, const ComponentInfo * ci)
	: FloatXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatXIn1OutView(this, SHEET_VIEW, QString::fromLatin1("Min"), "FloatMinSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatMin::calculate()
{
	FloatXIn1Out::calculate();
	
	QPtrListIterator<ConnectorBase> it(*getInputConnectorPack()->getConnList());
	
	// Hint: Gate has min 2 inputs !!! No check is required.
	double result = ((ConnectorFloatIn*)it.current())->getInput();
	++it;
		
	while (it.current())
	{
		const double in = ((ConnectorFloatIn*)it.current())->getInput();
		if (result > in)
		{
			result = in;
		}
		++it;
	}
		
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

