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

ExtConnBoolTristateBase::ExtConnBoolTristateBase(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput)
	: ExternalConnector(container, ci, input, multiOutput),
		m_resetState(KSIMBOOLTRISTATE_INACTIVE)
{
}

/*ExtConnBoolTristateBase::~ExtConnBoolTristateBase()
{
}*/

void ExtConnBoolTristateBase::reset()
{
	Component::reset();

	if (!isConnectedWithBoolTristate(getUsedExternalConn()))
	{
		((ConnectorBoolTristateSpecial *)getUsedExternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
		((ConnectorBoolTristateSpecial *)getInternalConn())->setOutput(m_resetState);
	}
	else if (!isConnectedWithBoolTristate(getInternalConn()))
	{
		((ConnectorBoolTristateSpecial *)getUsedExternalConn())->setOutput(m_resetState);
		((ConnectorBoolTristateSpecial *)getInternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
	}
	else
	{
		((ConnectorBoolTristateSpecial *)getUsedExternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
		((ConnectorBoolTristateSpecial *)getInternalConn())->setOutput(KSIMBOOLTRISTATE_INACTIVE);
	}
}


bool ExtConnBoolTristateBase::isConnectedWithBoolTristate(ConnectorBase * conn)
{
	bool connected = false;
	if (conn->getWire())
	{
		FOR_EACH_CONNECTOR(it, *conn->getWire()->getConnList())
		{
			if ((it.current() != conn) && (it.current()->inherits("ConnectorBoolTristate")))
			{
				connected = true;
				break;
			}
		}
	}
	return connected;
}


/** save component properties */
void ExtConnBoolTristateBase::save(KSimData & file) const
{
	ExternalConnector::save(file);

	if (m_resetState != KSimBoolTristate(KSIMBOOLTRISTATE_INACTIVE))
	{
		m_resetState.save(file, "Reset State");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool ExtConnBoolTristateBase::load(KSimData & file, bool copyLoad)
{
	m_resetState.load(file, "Reset State", KSimBoolTristate(KSIMBOOLTRISTATE_INACTIVE).text());
	return ExternalConnector::load(file, copyLoad);
}

void ExtConnBoolTristateBase::setResetState(KSimBoolTristate resetState)
{
	m_resetState = resetState;
}

KSimBoolTristate ExtConnBoolTristateBase::getResetState() const
{
	return m_resetState;
};


ComponentPropertyBaseWidget * ExtConnBoolTristateBase::createGeneralProperty(QWidget *parent)
{
	ExtConnBoolTristateBasePropertyGeneralWidget * wid;
	wid = new ExtConnBoolTristateBasePropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

//###############################################################
//##########################################################################################
//##########################################################################################


ExtConnBoolTristateBasePropertyGeneralWidget::ExtConnBoolTristateBasePropertyGeneralWidget(ExtConnBoolTristateBase * comp, QWidget *parent, const char *name)
	:	ExternalConnectorPropertyGeneralWidget(comp, parent, name)
{
	QString str;

	QLabel * lab = new QLabel(i18n("Default State:"), this, "m_defaultStateLabel");
	CHECK_PTR(lab);

	m_defaultState = new KSimBoolTristateBox(this, "m_defaultState");
	CHECK_PTR(m_defaultState);

	str = i18n("Sets the default state of the external connector.\n"
	           "This state is used if the external connector is not connected.");
	addToolTip(str, lab, m_defaultState);
	addWhatsThis(str, lab, m_defaultState);

	m_defaultState->setValue(getExtConn()->getResetState());
}

/*ExtConnBoolTristateBasePropertyGeneralWidget::~ExtConnBoolTristateBasePropertyGeneralWidget()
{
} */

void ExtConnBoolTristateBasePropertyGeneralWidget::acceptPressed()
{
	ExternalConnectorPropertyGeneralWidget::acceptPressed();

	if (getExtConn()->getResetState() != m_defaultState->getValue())
	{
		changeData();
		getExtConn()->setResetState( m_defaultState->getValue() );
	}
}

void ExtConnBoolTristateBasePropertyGeneralWidget::defaultPressed()
{
	ExternalConnectorPropertyGeneralWidget::defaultPressed();

	m_defaultState->setValue(KSimBoolTristate(KSIMBOOLTRISTATE_INACTIVE));
}


//###############################################################
//###############################################################
//###############################################################


//###############################################################
//###############################################################

Component * ExtConnBoolTristateIn::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTristateIn(container, ci);
}

const ComponentInfo * ExtConnBoolTristateIn::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean Tristate Input"),
	                                QString::fromLatin1("External Connector/Bool Tristate Input"),
	                                i18n("Component", "External Connector/Bool Tristate Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                ExtConnBoolTristateIn::create,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-booltri-in"));

	return &Info;
}

ExtConnBoolTristateIn::ExtConnBoolTristateIn(CompContainer * container, const ComponentInfo * ci)
	: ExtConnBoolTristateBase(container, ci, true, true)
{
	ConnectorBoolTristateSpecial * internal;
	ConnectorBoolTristateSpecial * external;

	internal = new ConnectorBoolTristateSpecial(this,
	                                            QString::fromLatin1("Internal"),
	                                            i18n("Connector", "Internal"),
	                                            QPoint(4,1));
	CHECK_PTR(internal);
	setInternalConn(internal);

	external = new ConnectorBoolTristateSpecial(this,
	                                            QString::fromLatin1("External"),
	                                            i18n("Connector", "External"),
	                                            QPoint(0,1));
	CHECK_PTR(external);
	setExternalConn(external);
	external->setOrientation(CO_LEFT); // is on the left side
}

/*ExtConnBoolTristateIn::~ExtConnBoolTristateIn()
{
}*/

//###############################################################

ExtConnBoolTristateOut::ExtConnBoolTristateOut(CompContainer * container, const ComponentInfo * ci)
	: ExtConnBoolTristateBase(container, ci, false, true)
{
	ConnectorBoolTristateSpecial * internal;
	ConnectorBoolTristateSpecial * external;
	
	internal = new ConnectorBoolTristateSpecial(this,
	                                            QString::fromLatin1("Internal"),
	                                            i18n("Connector", "Internal"),
	                                            QPoint(0,1));
	CHECK_PTR(internal);
	setInternalConn(internal);
	internal->setOrientation(CO_LEFT); // is on the left side
	
	external = new ConnectorBoolTristateSpecial(this,
	                                            QString::fromLatin1("External"),
	                                            i18n("Connector", "External"),
	                                            QPoint(4,1));
	CHECK_PTR(external);
	setExternalConn(external);
}

/*ExtConnBoolTristateOut::~ExtConnBoolTristateOut()
{
}*/


Component * ExtConnBoolTristateOut::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTristateOut(container, ci);
}

const ComponentInfo * ExtConnBoolTristateOut::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean Tristate Output"),
	                                QString::fromLatin1("External Connector/Bool Tristate Output"),
	                                i18n("Component", "External Connector/Bool Tristate Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                ExtConnBoolTristateOut::create,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-booltri-out"));

	return &Info;
}

//###############################################################

}; //namespace KSimLibBoolean

