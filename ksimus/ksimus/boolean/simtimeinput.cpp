/***************************************************************************
                          simtimeinput.cpp  -  description
                             -------------------
    begin                : Thu Feb 21 2002
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

// Project-Includes
#include "simtimeinput.h"
#include "resource.h"
#include "connectorfloatout.h"
#include "componentinfo.h"
#include "ksimtime.h"
#include "ksimtimeserver.h"

// Forward declaration

// Project-Includes


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new SimTimeInput(container, ci);
}

const ComponentInfo * getSimTimeInputInfo()
{
	static const ComponentInfo Info(i18n("Simulation time"),
	                                QString::fromLatin1("Control/Simulation Time"),
	                                i18n("Component", "Control/Simulation Time"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-sim-time"));
	return &Info;
}

//###############################################################
//###############################################################


SimTimeInputView::SimTimeInputView(SimTimeInput * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
//	enableConnectorSpacingRight(bool enable = true);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

void SimTimeInputView::draw(QPainter * p)
{
	CompView::draw(p);
	
	drawFrame(p);

	QFont newFont(QString::fromLatin1("helvetica"),10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, QString::fromLatin1("Sim\nTime"));
}


//###############################################################

SimTimeInput::SimTimeInput(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorFloatOut (this,
	                               QString::fromLatin1("Output"),
	                               i18n("Connector", "Output"),
	                               QPoint(4,2));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new SimTimeInputView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
	
}

SimTimeInput::~SimTimeInput()
{
}

void SimTimeInput::calculate()
{
	Component::calculate();
	m_out->setOutput(getTimeServer().getTime().getValue(unit_sec));
	executeNext();
}
/** Reset all simulation variables */
void SimTimeInput::reset()
{
	Component::reset();
	m_out->setOutput(0.0);
}

//###############################################################
//###############################################################

static Component * createTick(CompContainer * container, const ComponentInfo * ci)
{
	return new TickTimeInput(container, ci);
}


const ComponentInfo * getTickTimeInputInfo()
{
	static const ComponentInfo Info(i18n("Tick time"),
	                                QString::fromLatin1("Control/Tick Time"),
	                                i18n("Component", "Control/Tick Time"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createTick,
	                                QString::null,
	                                QString::fromLatin1("component-tick-time"));
	return &Info;
}

//###############################################################
//###############################################################


TickTimeInputView::TickTimeInputView(TickTimeInput * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
//	enableConnectorSpacingRight(bool enable = true);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

void TickTimeInputView::draw(QPainter * p)
{
	CompView::draw(p);
	
	drawFrame(p);

	QFont newFont(QString::fromLatin1("helvetica"),10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, QString::fromLatin1("Tick\nTime"));
}


//###############################################################

TickTimeInput::TickTimeInput(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_out = new ConnectorFloatOut (this,
	                               QString::fromLatin1("Output"),
	                               i18n("Connector", "Output"),
	                               QPoint(4,2));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new TickTimeInputView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
	
}

TickTimeInput::~TickTimeInput()
{
}

/** Reset all simulation variables */
void TickTimeInput::reset()
{
	Component::reset();
	m_out->setOutput(getTimeServer().getTick().getValue(unit_sec));
}

