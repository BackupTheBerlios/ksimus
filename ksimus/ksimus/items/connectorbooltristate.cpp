/***************************************************************************
                          connectorbooltristate.cpp  -  description
                             -------------------
    begin                : Sat Jun 29 2002
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

// QT includes
#include <qpopupmenu.h>

// KDE includes
#include <klocale.h>

// Project includes
#include "connectorinfo.h"
#include "connectorbooltristate.h"
#include "connectorlabel.h"
#include "ksimdebug.h"
#include "component.h"
#include "wirepropertybooltristate.h"
#include "wire.h"
#include "watchitembooltristate.h"

// Forward declaration


static ConnectorBase * create(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolTristate(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolTristateInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean Tristate"),
	                                QString::fromLatin1("Boolean Tristate"),
	                                QString::fromLatin1("Boolean Tristate"),
	                                create );
	return &Info;
}

ConnectorBoolTristate::ConnectorBoolTristate(Component * comp, const QString & name,
		                                         const QString & i18nName, const QPoint & pos)
	:	ConnectorTristateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTristateInfo()),
		m_inData(),
		m_outData()
{
	init();
}

ConnectorBoolTristate::ConnectorBoolTristate(Component * comp, const QString & name, const QString & i18nName,
		                                         const QString & descr, const QPoint & pos)
	:	ConnectorTristateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTristateInfo()),
		m_inData(),
		m_outData()
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolTristate::ConnectorBoolTristate(Component * comp, const QString & name, const QString & i18nName,
		                                         const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorTristateBase(comp, name, i18nName, pos, orient, ci),
		m_inData(),
		m_outData()
{
	init();
}

void ConnectorBoolTristate::init()
{
	setNegateEnabled(true);
}

// Get the colors for the connector
const WireColorScheme & ConnectorBoolTristate::getColorScheme() const
{
	return WirePropertyBoolTristate::colorScheme();
}
	
/** Resets the connector
*/
void ConnectorBoolTristate::reset()
{
	ConnectorTristateBase::reset();
	
	m_inData.set(0,0);
	m_outData.set(0,0);
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorBoolTristate::copyData(const void * pData)
{
	m_inData = *(const WireStateBoolTristate*)pData;
}

/** Set the current output */
void ConnectorBoolTristate::setOutput(bool out, bool active, bool exeWirePropNext)
{
	KSimBoolTristate state(out ^ isNegated(), active);
	
	if (state != m_outData.getTristate())
	{
		// Value changed
		m_outData.setTristate(state);
		// No Wire connected ?
		if (!getWireProperty())
		{
			m_inData = m_outData;
		}
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

void ConnectorBoolTristate::setOutput(KSimBoolTristate state, bool executeWirePropertyNext)
{
	setOutput(state.isTrue(), state.isActive(), executeWirePropertyNext);
}

/** Return the current output */
KSimBoolTristate ConnectorBoolTristate::getOutput() const
{
	if (isNegated())
	{
		return KSimBoolTristate(m_outData.isFalse(), m_outData.isActive());
	}
	else
	{
		return m_outData.getTristate();
	}
};

/** Return the current wire state */
KSimBoolTristate ConnectorBoolTristate::getInput() const
{
	if (isNegated())
	{
		return KSimBoolTristate(m_inData.isFalse(), m_inData.isActive());
	}
	else
	{
		return m_inData.getTristate();
	}
}

KSimBoolTristate ConnectorBoolTristate::getInputWithoutOutput() const
{
	WireStateBoolTristate res(m_inData.getTrue() - m_outData.getTrue(), m_inData.getFalse() - m_outData.getFalse());
	return res.getTristate();
}



/** Returns a pointer to the data that's read from the component. */
const void * ConnectorBoolTristate::readoutData() const
{
	return &m_outData;
}

const void * ConnectorBoolTristate::readoutInData() const
{
	return &m_inData;
}

/** Returns a text which represents the current value. */
QString ConnectorBoolTristate::getValueText() const
{
	return m_inData.getText();
}

WatchItemBase * ConnectorBoolTristate::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTristateConnector(this);
	CHECK_PTR(wi);
	return wi;
}


//#################################################################
//#################################################################

static ConnectorBase * createNoNegatable(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolTristateSpecial(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolTristateSpecialInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean Tristate No Negatable"),
	                                QString::fromLatin1("Boolean Tristate No Negatable"),
	                                QString::fromLatin1("Boolean Tristate"),
	                                createNoNegatable );
	return &Info;
}

ConnectorBoolTristateSpecial::ConnectorBoolTristateSpecial(Component * comp, const QString & name,
		                                                               const QString & i18nName, const QPoint & pos)
	:	ConnectorBoolTristate(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTristateSpecialInfo())
{
	init();
}

ConnectorBoolTristateSpecial::ConnectorBoolTristateSpecial(Component * comp, const QString & name, const QString & i18nName,
		                                                              const QString & descr, const QPoint & pos)
	:	ConnectorBoolTristate(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTristateSpecialInfo())
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolTristateSpecial::ConnectorBoolTristateSpecial(Component * comp, const QString & name, const QString & i18nName,
		                                                               const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBoolTristate(comp, name, i18nName, pos, orient, ci)
{
	init();
}

void ConnectorBoolTristateSpecial::init()
{
	setNegateEnabled(false);
}

WatchItemBase * ConnectorBoolTristateSpecial::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTristateConnectorSpecial(this);
	CHECK_PTR(wi);
	return wi;
}

