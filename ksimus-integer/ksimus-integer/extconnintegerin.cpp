/***************************************************************************
                          extconnintegerin.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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
#include <float.h>

// QT-Includes
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/wireproperty.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/ksimdata.h"
#include "extconnintegerin.h"


// Forward declaration

namespace KSimLibInteger
{

const KSimBaseInt ExtConnIntegerIn::sInitDefaultValue(0, KSimBaseInt::Decimal);


Component * ExtConnIntegerIn::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnIntegerIn(container, ci);
}

const ComponentInfo * ExtConnIntegerIn::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Integer Input"),
	                                QString::fromLatin1("External Connector/Integer Input"),
	                                i18n("Component", "External Connector/Integer Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-extconn-float-in")*/);
	return &Info;
}

//###############################################################

ExtConnIntegerIn::ExtConnIntegerIn(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, true),
		m_defaultValue(sInitDefaultValue)
{
	ConnectorIntegerOut * out;
	ConnectorIntegerIn * in;
	
	out = new ConnectorIntegerOut(this,
	                              QString::fromLatin1("Output"),
	                              i18n("Integer-Connector", "Output"),
	                              QPoint(4,1));
	CHECK_PTR(out);
	setInternalConn(out);
	
	in = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"),
	                             QPoint(0,1));
	CHECK_PTR(in);
	setExternalConn(in);
	
}

/*ExtConnIntegerIn::~ExtConnIntegerIn()
{
} */

void ExtConnIntegerIn::reset()
{
	ExternalConnector::reset();

	if (!getUsedExternalConn()->isConnected())
	{
		ConnectorIntegerOut * out = (ConnectorIntegerOut *)getInternalConn();
		ConnectorIntegerIn * in = (ConnectorIntegerIn *)getUsedExternalConn();

		int value = m_defaultValue.value();
		in->copyData(&value);
		out->setOutput(in->getInput(), false);
	}
}

void ExtConnIntegerIn::calculate()
{
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
		//ExternalConnector::calculate();
	
		ConnectorIntegerOut * out = (ConnectorIntegerOut *)getInternalConn();
		ConnectorIntegerIn * in = (ConnectorIntegerIn *)getUsedExternalConn();
	
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

void ExtConnIntegerIn::save(KSimData & file) const
{
	if (m_defaultValue != sInitDefaultValue)
	{
		m_defaultValue.save(file, "Default Value");
	}
	ExternalConnector::save(file);
}

bool ExtConnIntegerIn::load(KSimData & file, bool copyLoad)
{
	if(!m_defaultValue.load(file, "Default Value"))
	{
		m_defaultValue = sInitDefaultValue;
	}

	return ExternalConnector::load(file, copyLoad);
}

ComponentPropertyBaseWidget * ExtConnIntegerIn::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

//##########################################################################################
//##########################################################################################


ExtConnIntegerIn::PropertyGeneralWidget::PropertyGeneralWidget(ExtConnIntegerIn * comp, QWidget *parent, const char *name)
	:	ExternalConnectorPropertyGeneralWidget(comp, parent, name)
{
	m_defaultValueLabel = new QLabel(i18n("Integer", "Default value:"), this, "m_defaultValueLabel");
	CHECK_PTR(m_defaultValueLabel);

	m_defaultValue = new KSimBaseIntEdit(this, "m_defaultValue");
	CHECK_PTR(m_defaultValue);
	QString tip(i18n("Integer", "Sets the value state of the external connector.\n"
	                            "This value is used if the external connector is not connected."));
	addToolTip(tip, m_defaultValue, m_defaultValueLabel);
	addWhatsThis(tip, m_defaultValue, m_defaultValueLabel);
	m_defaultValueLabel->setBuddy(m_defaultValue);



	// Setup value
	m_defaultValue->setValue(getExtConnIn()->getDefaultValue());
}

/*ExtConnIntegerIn::PropertyGeneralWidget::~PropertyGeneralWidget()
{
} */

void ExtConnIntegerIn::PropertyGeneralWidget::acceptPressed()
{
	ExternalConnectorPropertyGeneralWidget::acceptPressed();

	if (getExtConnIn()->getDefaultValue() != m_defaultValue->value())
	{
		changeData();
		getExtConnIn()->setDefaultValue( m_defaultValue->value() );
	}
}

void ExtConnIntegerIn::PropertyGeneralWidget::defaultPressed()
{
	ExternalConnectorPropertyGeneralWidget::defaultPressed();

	m_defaultValue->setValue(ExtConnIntegerIn::sInitDefaultValue);
}

//###############################################################

};  //namespace KSimLibInteger
