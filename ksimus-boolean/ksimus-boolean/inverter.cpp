/***************************************************************************
                          inverter.cpp  -  description
                             -------------------
    begin                : Sun Oct 27 2002
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

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"

// Project-Includes
#include "inverter.h"

// Forward declaration



namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Inverter(container, ci);
}

const ComponentInfo * getInverterInfo()
{
	static const ComponentInfo Info(i18n("Component", "Inverter"),
	                                QString::fromLatin1("Boolean/Gates/Inverter"),
	                                i18n("Component", "Boolean/Gates/Inverter"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-inverter"));
	return &Info;
}

const ComponentInfo * getIdentityInfo()
{
	static const ComponentInfo Info(i18n("Component", "Identity"),
	                                QString::fromLatin1("Boolean/Gates/Identity"),
	                                i18n("Component", "Boolean/Gates/Identity"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-identity"));
	return &Info;
}


//###############################################################
//###############################################################

Inverter::Inverter(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci)
{

	m_input = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input"));
	CHECK_PTR(m_input);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new InverterView(this, SHEET_VIEW);
	}
	
	// make inverter
	if (ci == getInverterInfo())
	{
		getOutputConnector()->setNegate(true, true);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
}
	
/*Identity::~Identity()
{
}*/

void Inverter::calculate()
{
	Boolean1Out::calculate();
	
	setState(getInputConnector()->getInput());
}


//###############################################################
//###############################################################

InverterView::InverterView(Inverter * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getInverter()->getInputConnector()->setGridPos(0,2);
	getInverter()->getOutputConnector()->setGridPos(4,2);
}

/*InverterView::~InverterView()
{
}*/

void InverterView::draw(QPainter * p)
{
	drawFrame(p);
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "1");
	
	CompView::draw(p);
}

}; //namespace