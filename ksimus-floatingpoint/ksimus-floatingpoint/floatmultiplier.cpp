/***************************************************************************
                          floatmultiplier.cpp  -  description
                             -------------------
    begin                : Sat Feb 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include "floatmultiplier.h"
#include "ksimus/resource.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatMultiplier::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatMultiplier(container, ci);
}

const ComponentInfo * FloatMultiplier::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Multiplier"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Multiplier"),
	                                i18n("Component", "Floating Point/Arithmetic/Multiplier"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-mul"));
	return &Info;
}

//###############################################################
//###############################################################

FloatMultiplier::FloatMultiplier(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci)
{
	
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Factor"),
	                             i18n("Connector", "Factor %1"),
	                             getConnectorFloatInInfo(),
	                             2, 10);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
	m_inPack->setStoreName(QString::fromLatin1("Input"));
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		Float1OutView * view = new Float1OutView(this, SHEET_VIEW);
		
		view->getComponentLayout()->getLeft()->addSpace(1);
		view->getComponentLayout()->getLeft()->addConnectorPack(getInputConnectorPack());
		
		new ComponentLayoutBlockContentText(view->getComponentLayout()->getBlock(), "Mul");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatMultiplier::calculate()
{
	Float1Out::calculate();
	
	double result = 1.0;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result *= ((ConnectorFloatIn*)it.current())->getInput();
	}
	
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

