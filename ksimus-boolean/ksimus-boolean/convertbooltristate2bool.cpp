/***************************************************************************
                          convertbooltristate2bool.cpp  -  description
                             -------------------
    begin                : Mon Jul 1 2002
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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "convertbooltristate2bool.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/resource.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/wireproperty.h"

// Forward declaration

namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBoolTriState2Bool(container, ci);
}

const ComponentInfo * getConvertBoolTriState2BoolInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean TriState to Boolean"),
	                                QString::fromLatin1("Converter/Boolean TriState to Boolean"),
	                                i18n("Component", "Converter/Boolean TriState to Boolean"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/*,
	                                QString::null,
	                                QString::fromLatin1("component-clock-generator")  TODO */);
	return &Info;
}

//###############################################################
//###############################################################


//###############################################################
//###############################################################


ConvertBoolTriState2Bool::ConvertBoolTriState2Bool(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_lockRecursion(false)
{
	setZeroDelayComponent(true);
	
	m_connIn = new ConnectorBoolTriState(this,
	                           QString::fromLatin1("Input"),
	                           i18n("Connector", "Input"),
	                           QPoint(0,2));
	CHECK_PTR(m_connIn);
	m_connIn->setOrientation(CO_LEFT); // is on the left side
	
	m_connDataOut = new ConnectorBoolOut(this,
	                           QString::fromLatin1("Data Output"),
	                           i18n("Connector", "Data Output"),
	                           QString::fromLatin1("Do"),
	                           QPoint(4,3));
	CHECK_PTR(m_connDataOut);
	
	
	m_connActiveOut = new ConnectorBoolOut(this,
	                           QString::fromLatin1("Wire active"),
	                           i18n("Connector", "Wire active"),
	                           QString::fromLatin1("Eo"),
	                           QPoint(4,1));
	CHECK_PTR(m_connActiveOut);
	
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ConvertBoolTriState2BoolView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
}

ConvertBoolTriState2Bool::~ConvertBoolTriState2Bool()
{
}

void ConvertBoolTriState2Bool::reset()
{
	Component::reset();
	
	m_lockRecursion = false;
}

void ConvertBoolTriState2Bool::calculate()
{
	if(!m_lockRecursion)
	{
		m_lockRecursion = true;
		m_connActiveOut->setOutput(m_connIn->isActive(), false);
		if (m_connActiveOut->getWireProperty())
		{
			m_connActiveOut->getWireProperty()->execute();
		}
		m_connDataOut->setOutput(m_connIn->getInput(), false);
		if (m_connDataOut->getWireProperty())
		{
			m_connDataOut->getWireProperty()->execute();
		}
		m_lockRecursion = false;
	}
	else
	{
		executeNext();
	}
//	KSIMDEBUG("ConvertBoolTriState2Bool::calculate()");
}

//###############################################################
//###############################################################


ConvertBoolTriState2BoolView::ConvertBoolTriState2BoolView(ConvertBoolTriState2Bool * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
//	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

void ConvertBoolTriState2BoolView::draw(QPainter * p)
{
	CompView::draw(p);
	
	drawFrame(p);
}

//###############################################################

}; //namespace KSimLibBoolean

