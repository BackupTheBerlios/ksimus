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

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatAdd(container, ci);
}

const ComponentInfo * getFloatAddInfo()
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


FloatAddView::FloatAddView(FloatAdd * comp, eViewType viewType)
	: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnectorPack(comp->getInputConnectorPack());
	
		getComponentLayout()->updateLayout();
	}
}

void FloatAddView::draw(QPainter * p)
{
	Float1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Add");
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
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatAddView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatAdd::calculate()
{
	Float1Out::calculate();
	
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

