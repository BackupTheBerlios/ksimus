/***************************************************************************
                          integerdivider.cpp  -  description
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
#include <limits.h>

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "integerdivider.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes

namespace KSimLibInteger
{

Component * IntegerDivider::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerDivider(container, ci);
}

const ComponentInfo * IntegerDivider::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Divider"),
	                                QString::fromLatin1("Integer/Arithmetic/Divider"),
	                                i18n("Component", "Integer/Arithmetic/Divider"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-div")*/);
	return &Info;
}

//###############################################################
//###############################################################

IntegerDivider::IntegerDivider(CompContainer * container, const ComponentInfo * ci)
	: Integer2In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Integer2In1OutView(this, SHEET_VIEW, QString::fromLatin1("Div"), "IntegerDividerSV");
	}

	getInputA()->setName(i18n("Integer", "Dividend"), true);
	getInputB()->setName(i18n("Integer", "Divisor"), true);
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerDivider::calculate()
{
	Integer2In1Out::calculate();

	const int a(getInputA()->getInput());
	const int b(getInputB()->getInput());

	if (b != 0)
	{
		const div_t res = div(a, b);
		setValue(res.quot);
	}
	else if (a == 0)
	{
		setValue(1);
	}
	else if (a > 0)
	{
		setValue(INT_MAX);
	}
	else
	{
		setValue(INT_MIN);
	}
}


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

