/***************************************************************************
                          extconnbooltrisate.cpp  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#include <qlabel.h>

#include <klocale.h>

#include "extconnbooltristate.h"
#include "ksimus/resource.h"
#include "ksimus/connectorlist.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbooltristate.h"
#include "ksimus/ksimbooltristatebox.h"
#include "ksimus/wire.h"


namespace KSimLibBoolean
{


//###############################################################
//###############################################################

ExtConnBoolTriStateBase::ExtConnBoolTriStateBase(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput)
	: ExternalConnector(container, ci, input, multiOutput),
		m_resetState(KSIMBOOLTRISTATE_INACTIVE)
{
}

/*ExtConnBoolTriStateBase::~ExtConnBoolTriStateBase()
{
}*/

void ExtConnBoolTriStateBase::reset()
{
	Component::reset();

	if (!isConnectedWithBoolTriState(getUsedExternalConn()))
	{
		((ConnectorBoolTriStateSpecial *)getUsedExternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
		((ConnectorBoolTriStateSpecial *)getInternalConn())->setOutput(m_resetState);
	}
	else if (!isConnectedWithBoolTriState(getInternalConn()))
	{
		((ConnectorBoolTriStateSpecial *)getUsedExternalConn())->setOutput(m_resetState);
		((ConnectorBoolTriStateSpecial *)getInternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
	}
	else
	{
		((ConnectorBoolTriStateSpecial *)getUsedExternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
		((ConnectorBoolTriStateSpecial *)getInternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
	}
}


bool ExtConnBoolTriStateBase::isConnectedWithBoolTriState(ConnectorBase * conn)
{
	bool connected = false;
	if (conn->getWire())
	{
		FOR_EACH_CONNECTOR(it, *conn->getWire()->getConnList())
		{
			if ((it.current() != conn) && (it.current()->inherits("ConnectorBoolTriState")))
			{
				connected = true;
				break;
			}
		}
	}
	return connected;
}


/** save component properties */
void ExtConnBoolTriStateBase::save(KSimData & file) const
{
	ExternalConnector::save(file);

	if (m_resetState != KSimBoolTriState(KSIMBOOLTRISTATE_INACTIVE))
	{
		m_resetState.save(file, "Reset State");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool ExtConnBoolTriStateBase::load(KSimData & file, bool copyLoad)
{
	m_resetState.load(file, "Reset State", KSimBoolTriState(KSIMBOOLTRISTATE_INACTIVE).text());
	return ExternalConnector::load(file, copyLoad);
}

void ExtConnBoolTriStateBase::setResetState(KSimBoolTriState resetState)
{
	m_resetState = resetState;
}

KSimBoolTriState ExtConnBoolTriStateBase::getResetState() const
{
	return m_resetState;
};


ComponentPropertyBaseWidget * ExtConnBoolTriStateBase::createGeneralProperty(QWidget *parent)
{
	ExtConnBoolTriStateBasePropertyGeneralWidget * wid;
	wid = new ExtConnBoolTriStateBasePropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

//###############################################################
//##########################################################################################
//##########################################################################################


ExtConnBoolTriStateBasePropertyGeneralWidget::ExtConnBoolTriStateBasePropertyGeneralWidget(ExtConnBoolTriStateBase * comp, QWidget *parent, const char *name)
	:	ExternalConnectorPropertyGeneralWidget(comp, parent, name)
{
	QString str;

	QLabel * lab = new QLabel(i18n("Default State:"), this, "m_defaultStateLabel");
	CHECK_PTR(lab);

	m_defaultState = new KSimBoolTristateBox(this, "m_defaultState");
	CHECK_PTR(m_defaultState);

	str = i18n("Sets the default state of the external connector. Used if it is unconnected.");
	addToolTip(str, lab, m_defaultState);
	addWhatsThis(str, lab, m_defaultState);

	m_defaultState->setValue(getExtConn()->getResetState());
}

/*ExtConnBoolTriStateBasePropertyGeneralWidget::~ExtConnBoolTriStateBasePropertyGeneralWidget()
{
} */

void ExtConnBoolTriStateBasePropertyGeneralWidget::acceptPressed()
{
	ExternalConnectorPropertyGeneralWidget::acceptPressed();

	if (getExtConn()->getResetState() != m_defaultState->getValue())
	{
		changeData();
		getExtConn()->setResetState( m_defaultState->getValue() );
	}
}

void ExtConnBoolTriStateBasePropertyGeneralWidget::defaultPressed()
{
	ExternalConnectorPropertyGeneralWidget::defaultPressed();

	m_defaultState->setValue(KSimBoolTriState(KSIMBOOLTRISTATE_INACTIVE));
}


//###############################################################
//###############################################################
//###############################################################


//###############################################################
//###############################################################

Component * ExtConnBoolTriStateIn::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTriStateIn(container, ci);
}

const ComponentInfo * ExtConnBoolTriStateIn::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean TriState Input"),
	                                QString::fromLatin1("External Connector/Bool TriState Input"),
	                                i18n("Component", "External Connector/Bool TriState Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                ExtConnBoolTriStateIn::create/*,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-in") TODO */);

	return &Info;
}

ExtConnBoolTriStateIn::ExtConnBoolTriStateIn(CompContainer * container, const ComponentInfo * ci)
	: ExtConnBoolTriStateBase(container, ci, true, true)
{
	ConnectorBoolTriStateSpecial * internal;
	ConnectorBoolTriStateSpecial * external;

	internal = new ConnectorBoolTriStateSpecial(this,
	                                            QString::fromLatin1("Internal"),
	                                            i18n("Connector", "Internal"),
	                                            QPoint(4,1));
	CHECK_PTR(internal);
	setInternalConn(internal);

	external = new ConnectorBoolTriStateSpecial(this,
	                                            QString::fromLatin1("External"),
	                                            i18n("Connector", "External"),
	                                            QPoint(0,1));
	CHECK_PTR(external);
	setExternalConn(external);
	external->setOrientation(CO_LEFT); // is on the left side
}

/*ExtConnBoolTriStateIn::~ExtConnBoolTriStateIn()
{
}*/

//###############################################################

ExtConnBoolTriStateOut::ExtConnBoolTriStateOut(CompContainer * container, const ComponentInfo * ci)
	: ExtConnBoolTriStateBase(container, ci, false, true)
{
	ConnectorBoolTriStateSpecial * internal;
	ConnectorBoolTriStateSpecial * external;
	
	internal = new ConnectorBoolTriStateSpecial(this,
	                                            QString::fromLatin1("Internal"),
	                                            i18n("Connector", "Internal"),
	                                            QPoint(0,1));
	CHECK_PTR(internal);
	setInternalConn(internal);
	internal->setOrientation(CO_LEFT); // is on the left side
	
	external = new ConnectorBoolTriStateSpecial(this,
	                                            QString::fromLatin1("External"),
	                                            i18n("Connector", "External"),
	                                            QPoint(4,1));
	CHECK_PTR(external);
	setExternalConn(external);
}

/*ExtConnBoolTriStateOut::~ExtConnBoolTriStateOut()
{
}*/


Component * ExtConnBoolTriStateOut::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTriStateOut(container, ci);
}

const ComponentInfo * ExtConnBoolTriStateOut::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean TriState Output"),
	                                QString::fromLatin1("External Connector/Bool TriState Output"),
	                                i18n("Component", "External Connector/Bool TriState Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                ExtConnBoolTriStateOut::create/*,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-in") TODO */);

	return &Info;
}

//###############################################################

}; //namespace KSimLibBoolean

