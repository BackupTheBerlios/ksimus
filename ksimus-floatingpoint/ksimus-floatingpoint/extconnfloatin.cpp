/***************************************************************************
                          extconnfloatin.cpp  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/wireproperty.h"
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/ksimdata.h"
#include "extconnfloatin.h"


// Forward declaration

namespace KSimLibFloatingPoint
{

#define DEFAULT_VALUE   0.0

static const char * sDefaultValue  = "Default Value";

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnFloatIn(container, ci);
}

const ComponentInfo * getExtConnFloatInInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Floating Point Input"),
	                                QString::fromLatin1("External Connector/Floating Point Input"),
	                                i18n("Component", "External Connector/Floating Point Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-extconn-float-in"));
	return &Info;
}

//###############################################################

ExtConnFloatIn::ExtConnFloatIn(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, true),
		m_defaultValue(DEFAULT_VALUE)
{
	ConnectorFloatOut * out;
	ConnectorFloatIn * in;
	
	out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"),
	                             QPoint(4,1));
	CHECK_PTR(out);
	setInternalConn(out);
	
	in = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"),
	                             QPoint(0,1));
	CHECK_PTR(in);
	setExternalConn(in);
	
}

/*ExtConnFloatIn::~ExtConnFloatIn()
{
} */

void ExtConnFloatIn::reset()
{
	ExternalConnector::reset();

	if (!getUsedExternalConn()->isConnected())
	{
		ConnectorFloatOut * out = (ConnectorFloatOut *)getInternalConn();
		ConnectorFloatIn * in = (ConnectorFloatIn *)getUsedExternalConn();

		in->copyData(&m_defaultValue);
		out->setOutput(in->getInput(), false);
	}
}

void ExtConnFloatIn::calculate()
{
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
		//ExternalConnector::calculate();
	
		ConnectorFloatOut * out = (ConnectorFloatOut *)getInternalConn();
		ConnectorFloatIn * in = (ConnectorFloatIn *)getUsedExternalConn();
	
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

void ExtConnFloatIn::save(KSimData & file) const
{
	if (getDefaultValue() != 0.0)
	{
		file.writeEntry(sDefaultValue, getDefaultValue());
	}
	ExternalConnector::save(file);
}

bool ExtConnFloatIn::load(KSimData & file, bool copyLoad)
{
	setDefaultValue(file.readDoubleNumEntry(sDefaultValue, 0.0));

	return ExternalConnector::load(file, copyLoad);
}

ComponentPropertyBaseWidget * ExtConnFloatIn::createGeneralProperty(QWidget *parent)
{
	ExtConnFloatInPropertyGeneralWidget * wid;
	wid = new ExtConnFloatInPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

//##########################################################################################
//##########################################################################################


ExtConnFloatInPropertyGeneralWidget::ExtConnFloatInPropertyGeneralWidget(ExtConnFloatIn * comp, QWidget *parent, const char *name)
	:	ExternalConnectorPropertyGeneralWidget(comp, parent, name)
{
	m_defaultValueLabel = new QLabel(i18n("FloatingPoint", "Default value:"), this, "m_defaultValueLabel");
	CHECK_PTR(m_defaultValueLabel);

	m_defaultValue = new KSimDoubleEdit(this, "m_defaultValue");
	CHECK_PTR(m_defaultValue);
	QString tip(i18n("FloatingPoint", "Sets the value state of the external connector.\n"
	                                   "This value is used if the external connector is not connected."));
	addToolTip(tip, m_defaultValue, m_defaultValueLabel);
	addWhatsThis(tip, m_defaultValue, m_defaultValueLabel);


	// Setup value
	m_defaultValue->setValue(getExtConnIn()->getDefaultValue());
}

/*ExtConnFloatInPropertyGeneralWidget::~ExtConnFloatInPropertyGeneralWidget()
{
} */

void ExtConnFloatInPropertyGeneralWidget::acceptPressed()
{
	ExternalConnectorPropertyGeneralWidget::acceptPressed();

	if (getExtConnIn()->getDefaultValue() != m_defaultValue->value())
	{
		changeData();
		getExtConnIn()->setDefaultValue( m_defaultValue->value() );
	}
}

void ExtConnFloatInPropertyGeneralWidget::defaultPressed()
{
	ExternalConnectorPropertyGeneralWidget::defaultPressed();

	m_defaultValue->setValue(DEFAULT_VALUE);
}

//###############################################################


};  //namespace KSimLibFloatingPoint
