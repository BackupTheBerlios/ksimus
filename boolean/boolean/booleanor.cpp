/***************************************************************************
                          booleanor.cpp  -  description
                             -------------------
    begin                : Mon May 14 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
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
//#include <klocale.h>

// Project-Includes
#include "booleanor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/componentinfo.h"


// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanOr(container, ci);
}

const ComponentInfo BooleanOrInfo (	"Boolean OR",
									"Boolean/Gates/OR",
									QString::null,//"Boolean OR",
									VA_SHEETVIEW,
									create
								);

const ComponentInfo BooleanNorInfo (	"Boolean NOR",
									"Boolean/Gates/NOR",
									QString::null,//"Boolean NOR",
									VA_SHEETVIEW,
									create
								);




//###############################################################
//###############################################################


BooleanOrView::BooleanOrView(Component * comp, eViewType viewType)
	: Boolean2In1OutView(comp, viewType)
{
}

void BooleanOrView::draw(QPainter * p)
{
	Boolean2In1OutView::draw(p);
	
	p->drawText(getPlace(), AlignCenter, ">=1");
}


//###############################################################

BooleanOr::BooleanOr(CompContainer * container, const ComponentInfo * ci)
	: Boolean2In1Out(container, ci)
{
	// make Nor
	if (ci == &BooleanNorInfo)
	{
		m_out->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanOrView(this, SHEET_VIEW);
	}
}

/** Executes the simulation of this component */
void BooleanOr::calculate()
{
	Boolean2In1Out::calculate();
	
	m_result = m_inA->getInput() || m_inB->getInput();
}

}; //namespace KSimLibBoolean
