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
#include "ksimus/compview.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/resource.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/wireproperty.h"
#include "ksimus/componentlayout.h"
#include "convertbool2booltristate.h"

// Forward declaration

namespace KSimLibBoolean
{

Component * ConvertBool2BoolTristate::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBool2BoolTristate(container, ci);
}

const ComponentInfo * ConvertBool2BoolTristate::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean to Boolean Tristate"),
	                                QString::fromLatin1("Converter/Boolean to Boolean Tristate"),
	                                i18n("Component", "Converter/Boolean to Boolean Tristate"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-converter-bool-booltri"));
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
//		new ConvertBool2BoolTristateView(this, SHEET_VIEW);
		CompView * sheetView = new CompView(this, SHEET_VIEW);
		Q_CHECK_PTR(sheetView);
		
		ComponentLayout * layout = new ComponentLayout(sheetView, false);
		Q_CHECK_PTR(layout);
		layout->setMinSize(3,3);

		ComponentLayoutControlBlock * pCtrlBlock = new ComponentLayoutControlBlock(layout);
		Q_CHECK_PTR(pCtrlBlock);

		pCtrlBlock->getLeft()->addSpace(1);
		pCtrlBlock->getLeft()->addConnector(getInputActiveIn());

		ComponentLayoutBlock * pFuncBlock = new ComponentLayoutBlock(layout);
		Q_CHECK_PTR(pFuncBlock);

		pFuncBlock->getLeft()->addSpace(1);
		pFuncBlock->getLeft()->addConnector(getInputDataIn());

		pFuncBlock->getRight()->addSpace(1);
		pFuncBlock->getRight()->addConnector(getOutput());
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
	Component::calculate();
	
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

}; //namespace KSimLibBoolean

#include "convertbool2booltristate.moc"
