/***************************************************************************
                          convertintegerfloat.cpp  -  description
                             -------------------
    begin                : Sun Sep 7 2003
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
#include <qpainter.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/wireproperty.h"

#include "convertintegerfloat.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{


//###############################################################
//###############################################################

Component * ConvertIntegerFloat::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertIntegerFloat(container, ci);
}


const ComponentInfo * ConvertIntegerFloat::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Integer to Floating Point"),
	                                QString::fromLatin1("Converter/Integer to Float"),
	                                i18n("Component", "Converter/Integer to Float"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-bool-float")*/);
	return &Info;
}



//###############################################################
//###############################################################


ConvertIntegerFloat::ConvertIntegerFloat(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_recursionLocked(false)
{
	setZeroDelayComponent(true);
	
	m_input = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"));
	CHECK_PTR(m_input);
	
	m_output = new ConnectorFloatOut(this,
	                                 QString::fromLatin1("Output"),
	                                 i18n("Integer-Connector", "Output"));
	CHECK_PTR(m_output);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW, "ConvertIntegerFloatSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void ConvertIntegerFloat::reset()
{
	Component::reset();
	
	m_recursionLocked = false;
}

/** Executes the simulation of this component */
void ConvertIntegerFloat::calculate()
{
	Component::calculate();
	
	if (!m_recursionLocked)
	{
		m_recursionLocked = true;
	
		getOutput()->setOutput((double)getInput()->getInput(), false);
		if (getOutput()->getWireProperty())
		{
			getOutput()->getWireProperty()->execute();
		}
		m_recursionLocked = false;
	}
	else
	{
		executeNext();
	}
}

//###############################################################
//###############################################################


ConvertIntegerFloat::View::View(ConvertIntegerFloat * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getConvertIntegerFloat()->getInput()->setGridPos(0,1);
	getConvertIntegerFloat()->getOutput()->setGridPos(4,1);
}
/*ConvertIntegerFloat::View::~View()
{
}*/

void ConvertIntegerFloat::View::draw(QPainter * p)
{
	CompView::draw(p);
		
	drawFrame(p);
	QRect place(getDrawingPlace());
	p->setPen(QPen(black, 1));
	p->drawLine(place.bottomLeft()+QPoint(1,0), place.topRight()+QPoint(0,1));
	
	if(getRotation() != 0)
	{
		int yMid = place.top() + place.bottom() / 2;
	
		p->drawLine(QPoint(place.left()+4, yMid) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid-3) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid+3) , QPoint(place.right()-4, yMid));
	}
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

