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
#include <klocale.h>

// Project-Includes
#include "booleanor.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"


// Forward declaration

// Project-Includes

namespace KSimLibBoolean
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanOr(container, ci);
}

const ComponentInfo BooleanOrInfo(I18N_NOOP("Boolean OR"),
                                  I18N_NOOP("Boolean/Gates/OR"),
                                  QString::null,
                                  VA_SHEETVIEW,
                                  create,	
                                  QString::null,
                                  "component-boolean-or"
                                  );

const ComponentInfo BooleanNorInfo(I18N_NOOP("Boolean NOR"),
                                   I18N_NOOP("Boolean/Gates/NOR"),
                                   QString::null,
                                   VA_SHEETVIEW,
                                   create,	
                                   QString::null,
                                   "component-boolean-nor"
                                   );




//###############################################################
//###############################################################


BooleanOrView::BooleanOrView(BooleanOr * comp, eViewType viewType)
	: BooleanXIn1OutView(comp, viewType)
{
}

void BooleanOrView::draw(QPainter * p)
{
	BooleanXIn1OutView::draw(p);
	
	p->drawText(getDrawingPlace(), AlignCenter, ">=1");
}


//###############################################################

BooleanOr::BooleanOr(CompContainer * container, const ComponentInfo * ci)
	: BooleanXIn1Out(container, ci)
{
	// make Nor
	if (ci == &BooleanNorInfo)
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanOrView(this, SHEET_VIEW);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void BooleanOr::calculate()
{
	BooleanXIn1Out::calculate();
	
	bool result = false;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result |= ((ConnectorBoolIn*)it.current())->getInput();
		if (result) break;	//  No more changes possible
	}
	
	setState(result);
}

}; //namespace KSimLibBoolean
