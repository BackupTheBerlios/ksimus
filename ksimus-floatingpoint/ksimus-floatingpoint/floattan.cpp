/***************************************************************************
                          floattan.cpp  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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
#include <math.h>

// QT-Includes
#include <qpainter.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floattan.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * createTan(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatTan(container, ci);
}

const ComponentInfo FloatTanInfo(I18N_NOOP("Floating Point tan(x)"),
                                 I18N_NOOP("Floating Point/Arithmetic/Trigonometric/tan(x)"),
                                 QString::null,
                                 VA_SHEETVIEW,
                                 createTan,
                                 QString::null,
                                 "component-float-trigonometric-tan");


static Component * createATan(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatATan(container, ci);
}

const ComponentInfo FloatATanInfo(I18N_NOOP("Floating Point atan(x)"),
                                  I18N_NOOP("Floating Point/Arithmetic/Trigonometric/atan(x)"),
                                  QString::null,
                                  VA_SHEETVIEW,
                                  createATan,
                                 QString::null,
                                 "component-float-trigonometric-atan");




//###############################################################
//###############################################################


void FloatTanView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "tan");
}


//###############################################################
//###############################################################

FloatTan::FloatTan(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatTanView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatTan::calculate()
{
	Float1In1Out::calculate();
	
	setValue(tan(getInput()->getInput()));
}


//###############################################################
//###############################################################


//###############################################################
//###############################################################


void FloatATanView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "atan");
}


//###############################################################
//###############################################################

FloatATan::FloatATan(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatATanView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatATan::calculate()
{
	Float1In1Out::calculate();
	
	setValue(atan(getInput()->getInput()));
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

