/***************************************************************************
                          floatsin.cpp  -  description
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
#include "floatsin.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorpack.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{

static Component * createSin(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatSin(container, ci);
}

const ComponentInfo FloatSinInfo(I18N_NOOP("Floating Point sin(x)"),
                                 I18N_NOOP("Floating Point/Arithmetic/Trigonometric/sin(x)"),
                                 QString::null,
                                 VA_SHEETVIEW,
                                 createSin
                                 );


static Component * createASin(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatASin(container, ci);
}

const ComponentInfo FloatASinInfo(I18N_NOOP("Floating Point asin(x)"),
                                  I18N_NOOP("Floating Point/Arithmetic/Trigonometric/asin(x)"),
                                  QString::null,
                                  VA_SHEETVIEW,
                                  createASin
                                  );




//###############################################################
//###############################################################


void FloatSinView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "sin");
}


//###############################################################
//###############################################################

FloatSin::FloatSin(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatSinView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatSin::calculate()
{
	Float1In1Out::calculate();
	
	setValue(sin(getInput()->getInput()));
}


//###############################################################
//###############################################################


//###############################################################
//###############################################################


void FloatASinView::draw(QPainter * p)
{
	Float1In1OutView::draw(p);
	
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "asin");
}


//###############################################################
//###############################################################

FloatASin::FloatASin(CompContainer * container, const ComponentInfo * ci)
	: Float1In1Out(container, ci)
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatASinView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatASin::calculate()
{
	Float1In1Out::calculate();
	
	setValue(asin(getInput()->getInput()));
}


//###############################################################
//###############################################################


};  //namespace KSimLibFloatingPoint

