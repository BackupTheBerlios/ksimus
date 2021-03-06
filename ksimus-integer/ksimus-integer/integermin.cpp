/***************************************************************************
                          integermin.cpp  -  description
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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "integermin.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

Component * IntegerMin::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerMin(container, ci);
}

const ComponentInfo * IntegerMin::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Minimum"),
	                                QString::fromLatin1("Integer/Control/Minimum"),
	                                i18n("Component", "Integer/Control/Minimum"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-minimum") */);
	return &Info;
}





//###############################################################
//###############################################################

IntegerMin::IntegerMin(CompContainer * container, const ComponentInfo * ci)
	: IntegerXIn1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerXIn1OutView(this, SHEET_VIEW, QString::fromLatin1("Min"), "IntegerMinSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerMin::calculate()
{
	IntegerXIn1Out::calculate();
	
	QPtrListIterator<ConnectorBase> it(*getInputConnectorPack()->getConnList());
	
	
	// Hint: Gate has min 2 inputs !!! No check is required.
	int result = ((ConnectorIntegerIn*)it.current())->getInput();
	++it;
		
	while (it.current())
	{
		result = QMIN(result, ((ConnectorIntegerIn*)it.current())->getInput());
		++it;
	}
		
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

