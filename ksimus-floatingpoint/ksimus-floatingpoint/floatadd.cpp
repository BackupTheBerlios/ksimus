/***************************************************************************
                          floatadd.cpp  -  description
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
#include "floatadd.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

Component * FloatAdd::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatAdd(container, ci);
}

const ComponentInfo * FloatAdd::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Adder"),
	                                QString::fromLatin1("Floating Point/Arithmetic/Adder"),
	                                i18n("Component", "Floating Point/Arithmetic/Adder"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-arithmetic-add"));
	return &Info;
}

//###############################################################
//###############################################################

FloatAdd::FloatAdd(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci)
{
	
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Summand"),
	                             i18n("Connector", "Summand %1"),
	                             getConnectorFloatInInfo(),
	                             2, 10);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
	m_inPack->setStoreName(QString::fromLatin1("Input"));
	
	getAction().disable(KSimAction::UPDATEVIEW);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		Float1OutView * view = new Float1OutView(this, SHEET_VIEW);
		
		view->getComponentLayout()->getLeft()->addSpace(1);
		view->getComponentLayout()->getLeft()->addConnectorPack(getInputConnectorPack());
		
		new ComponentLayoutBlockContentText(view->getComponentLayout()->getBlock(), "Add");
	}

}

/** Executes the simulation of this component */
void FloatAdd::calculate()
{
	double result = 0.0;
	
	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		result += ((ConnectorFloatIn*)it.current())->getInput();
	}
	
	setValue(result);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

