/***************************************************************************
                          convertbool2booltristate.cpp  -  description
                             -------------------
    begin                : Sun Jun 30 2002
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
#include "convertbool2booltristate.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/resource.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/wireproperty.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"

// Forward declaration

namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBool2BoolTristate(container, ci);
}

const ComponentInfo * getConvertBool2BoolTristateInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean to Boolean Tristate"),
	                                QString::fromLatin1("Converter/Boolean to Boolean Tristate"),
	                                i18n("Component", "Converter/Boolean to Boolean Tristate"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/*,
	                                QString::null,
	                                QString::fromLatin1("component-clock-generator")  TODO */);
	return &Info;
}

//###############################################################
//###############################################################


ConvertBool2BoolTristate::ConvertBool2BoolTristate(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_lockRecursion(false)
{
	setZeroDelayComponent(true);
	
	m_connDataIn = new ConnectorBoolIn(this,
	                           QString::fromLatin1("Input"),
	                           i18n("Connector", "Input"),
	                           QString::fromLatin1("Di"));
	CHECK_PTR(m_connDataIn);
	
	
	m_connActiveIn = new ConnectorBoolIn(this,
	                           QString::fromLatin1("Enable"),
	                           i18n("Connector", "Enable"),
	                           QString::fromLatin1("E"));
	CHECK_PTR(m_connActiveIn);
	
	m_connOut = new ConnectorBoolTristate(this,
	                           QString::fromLatin1("Output"),
	                           i18n("Connector", "Output"));
	CHECK_PTR(m_connOut);
	
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ConvertBool2BoolTristateView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
}

ConvertBool2BoolTristate::~ConvertBool2BoolTristate()
{
}

void ConvertBool2BoolTristate::reset()
{
	Component::reset();
	
	m_lockRecursion = false;
}

void ConvertBool2BoolTristate::calculate()
{
	if(!m_lockRecursion)
	{
		m_lockRecursion = true;
		m_connOut->setOutput(m_connDataIn->getInput(),
		                     m_connActiveIn->getInput(),
		                     false);
		if (m_connOut->getWireProperty())
		{
			m_connOut->getWireProperty()->execute();
		}
		m_lockRecursion = false;
	}
	else
	{
		executeNext();
	}
//	KSIMDEBUG("ConvertBool2BoolTristate::calculate()");
}

//###############################################################
//###############################################################


ConvertBool2BoolTristateView::ConvertBool2BoolTristateView(ConvertBool2BoolTristate * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);

		m_ctrlBlock = new ComponentControlBlock(this, m_layout);
		CHECK_PTR(m_ctrlBlock);

		m_ctrlBlock->getLeft()->addSpace(1);
		m_ctrlBlock->getLeft()->addConnector(getConverter()->getInputActiveIn());

		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnector(getConverter()->getInputDataIn());

		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnector(getConverter()->getOutput());

		m_layout->setMinSize(5,5);
		m_layout->updateLayout();

	}
}

void ConvertBool2BoolTristateView::draw(QPainter * p)
{
	CompView::draw(p);
	
	QRect rect(getDrawingPlace());
	rect.rLeft() ++;
	rect.rTop() += 1 + m_ctrlBlock->getRect(false).bottom();
	rect.rBottom() ++;

	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
}

//###############################################################

}; //namespace KSimLibBoolean

