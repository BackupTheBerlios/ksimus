/***************************************************************************
                          rsflipflop.cpp  -  description
                             -------------------
    begin                : Sun Aug 19 2001
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

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorlabel.h"

// Project-Includes
#include "rsflipflop.h"

// Forward declaration

namespace KSimLibBoolean
{


Component * RSFlipFlop::create(CompContainer * container, const ComponentInfo * ci)
{
	return new RSFlipFlop(container, ci);
}

const ComponentInfo * RSFlipFlop::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "RS Flip Flop"),
	                                QString::fromLatin1("Boolean/Flip Flop/RS-FF"),
	                                i18n("Component", "Boolean/Flip Flop/RS-FF"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-rs-ff"));
	return &Info;
}



RSFlipFlop::RSFlipFlop(CompContainer * container, const ComponentInfo * ci)
	: FlipFlopBase(container, ci)
{
	getSetInputConnector()->setEdgeSensitive(false, true);
	getResetInputConnector()->setEdgeSensitive(false, true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new RSFlipFlopView(this, SHEET_VIEW);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}

/*RSFlipFlop::~RSFlipFlop()
{
} */



void RSFlipFlop::calculate()
{
	FlipFlopBase::calculate();
	
	bool set = getSetInputConnector()->getInput();
	bool reset = getResetInputConnector()->getInput();
	
	if (set && getDominant())
	{
		setState(true);
	}
	else if (reset)
	{
		setState(false);
	}
	else if (set)
	{
		setState(true);
	}
}


//###############################################################
//###############################################################

RSFlipFlopView::RSFlipFlopView(RSFlipFlop * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);

	getRSFF()->getOutputConnector()->setGridPos(4,1);
	new ConnectorLabel(getRSFF()->getOutputConnector(), "Q");
	
	getRSFF()->getNotOutputConnector()->setGridPos(4,3);
	new ConnectorLabel(getRSFF()->getNotOutputConnector(), "/Q");
	
	getRSFF()->getSetInputConnector()->setGridPos(0,1);
	new ConnectorLabel(getRSFF()->getSetInputConnector(), "S");
	
	getRSFF()->getResetInputConnector()->setGridPos(0,3);
	new ConnectorLabel(getRSFF()->getResetInputConnector(), "R");
	
	
}
/*RSFlipFlopView::~RSFlipFlopView()
{
}*/

void RSFlipFlopView::draw(QPainter * p)
{
  drawFrame(p);

	CompView::draw(p);
}

//###############################################################
//###############################################################

	
}; //namespace


