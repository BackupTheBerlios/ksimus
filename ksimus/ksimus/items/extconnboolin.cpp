/***************************************************************************
                          extconnboolin.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes

// Forward declaration
#include "extconnboolin.h"
#include "resource.h"
#include "connectorlist.h"
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "wireproperty.h"
#include "ksimdata.h"
#include "ksimbooleanbox.h"


#define DEFAULT_STATE   false

const char * const ExtConnBoolIn::sDefaultState = "Default State";

Component * ExtConnBoolIn::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolIn(container, ci);
}

const ComponentInfo * ExtConnBoolIn::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean Input"),
	                                QString::fromLatin1("External Connector/Bool Input"),
	                                i18n("Component", "External Connector/Bool Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-in"));
	
	return &Info;
}

//###############################################################

ExtConnBoolIn::ExtConnBoolIn(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, true),
		m_defaultState(DEFAULT_STATE)
{
	ConnectorBoolOut * out;
	ConnectorBoolIn * in;
	
	out = new ConnectorBoolOut(this,
	                           QString::fromLatin1("Output"),
	                           i18n("Connector", "Output"),
	                           QPoint(4,1));
	CHECK_PTR(out);
	setInternalConn(out);
	
	in = new ConnectorBoolIn(this,
	                         QString::fromLatin1("Input"),
	                         i18n("Connector", "Input"),
	                         QPoint(0,1));
	CHECK_PTR(in);
	setExternalConn(in);
}

ExtConnBoolIn::~ExtConnBoolIn()
{
}

void ExtConnBoolIn::reset()
{
	ExternalConnector::reset();

	
	if (!getUsedExternalConn()->isConnected())
	{
		ConnectorBoolOut * out = (ConnectorBoolOut *)getInternalConn();
		ConnectorBoolIn * in = (ConnectorBoolIn *)getUsedExternalConn();

		in->copyData(&m_defaultState);
		out->setOutput(in->getInput(), false);
	}
}

void ExtConnBoolIn::calculate()
{
	ConnectorBoolOut * out = (ConnectorBoolOut *)getInternalConn();
	ConnectorBoolIn * in = (ConnectorBoolIn *)getUsedExternalConn();
	
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
		ExternalConnector::calculate();
	
		out->setOutput(in->getInput(), false);
		if (out->getWireProperty())
		{
			out->getWireProperty()->execute();
		}
		setRecursionLocked(false);
	}
	else
	{
		executeNext();
	}
}

void ExtConnBoolIn::save(KSimData & file) const
{
	if (isDefaultState())
	{
		file.writeEntry(sDefaultState, true);
	}
	ExternalConnector::save(file);
}

bool ExtConnBoolIn::load(KSimData & file, bool copyLoad)
{
	setDefaultState(file.readBoolEntry(sDefaultState, false));

	return ExternalConnector::load(file, copyLoad);
}

ComponentPropertyBaseWidget * ExtConnBoolIn::createGeneralProperty(QWidget *parent)
{
	ExtConnBoolInPropertyGeneralWidget * wid;
	wid = new ExtConnBoolInPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

//##########################################################################################
//##########################################################################################


ExtConnBoolInPropertyGeneralWidget::ExtConnBoolInPropertyGeneralWidget(ExtConnBoolIn * comp, QWidget *parent, const char *name)
	:	ExternalConnectorPropertyGeneralWidget(comp, parent, name)
{
	QString str;

	QLabel * lab = new QLabel(i18n("Default State:"), this, "m_defaultStateLabel"); 
	CHECK_PTR(lab);
	
	m_defaultState = new KSimBooleanBox(this, "m_defaultState");
	CHECK_PTR(m_defaultState);

	str = i18n("Sets the default state of the external connector.\n"
	           "This state is used if the external connector is not connected.");
	addToolTip(str, lab, m_defaultState);
	addWhatsThis(str, lab, m_defaultState);

	m_defaultState->setValue(getExtConnIn()->isDefaultState());
}

/*ExtConnBoolInPropertyGeneralWidget::~ExtConnBoolInPropertyGeneralWidget()
{
} */

void ExtConnBoolInPropertyGeneralWidget::acceptPressed()
{
	ExternalConnectorPropertyGeneralWidget::acceptPressed();

	if (getExtConnIn()->isDefaultState() != m_defaultState->getValue())
	{
		changeData();
		getExtConnIn()->setDefaultState( m_defaultState->getValue() );
	}
}

void ExtConnBoolInPropertyGeneralWidget::defaultPressed()
{
	ExternalConnectorPropertyGeneralWidget::defaultPressed();

	m_defaultState->setValue(DEFAULT_STATE);
}

#undef DEFAULT_STATE

#include "extconnboolin.moc"

