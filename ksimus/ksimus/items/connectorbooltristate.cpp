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
	return new ConnectorBoolTriState(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolTriStateInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean TriState"),
	                                QString::fromLatin1("Boolean TriState"),
	                                QString::fromLatin1("Boolean TriState"),
	                                create );
	return &Info;
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name,
		                                         const QString & i18nName, const QPoint & pos)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateInfo()),
		m_inData(),
		m_outData()
{
	init();
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name, const QString & i18nName,
		                                         const QString & descr, const QPoint & pos)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateInfo()),
		m_inData(),
		m_outData()
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolTriState::ConnectorBoolTriState(Component * comp, const QString & name, const QString & i18nName,
		                                         const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorTriStateBase(comp, name, i18nName, pos, orient, ci),
		m_inData(),
		m_outData()
{
	init();
}

void ConnectorBoolTriState::init()
{
	setNegateEnabled(true);
}

// Get the colors for the connector
const WireColorScheme & ConnectorBoolTriState::getColorScheme() const
{
	return WirePropertyBoolTriState::colorScheme();
}
	
/** Resets the connector
*/
void ConnectorBoolTriState::reset()
{
	ConnectorTriStateBase::reset();
	
	m_inData.set(0,0);
	m_outData.set(0,0);
}

/** The function copyData() has to copy data to the output variable
  * The default implementation does nothing
  * Reimplementations is required for all output connectors  */
void ConnectorBoolTriState::copyData(const void * pData)
{
	m_inData = *(const WireStateBoolTriState*)pData;
}

/** Set the current output */
void ConnectorBoolTriState::setOutput(bool out, bool active, bool exeWirePropNext)
{
	KSimBoolTriState state(out ^ isNegated(), active);
	
	if (state != m_outData.getTriState())
	{
		// Value changed
		m_outData.setTriState(state);
		// No Wire connected ?
		if (!getWireProperty())
		{
			m_inData = m_outData;
		}
		if (exeWirePropNext) executeWirePropertyNext();
	}
}

void ConnectorBoolTriState::setOutput(KSimBoolTriState state, bool executeWirePropertyNext)
{
	setOutput(state.isTrue(), state.isActive(), executeWirePropertyNext);
}

/** Return the current output */
KSimBoolTriState ConnectorBoolTriState::getOutput() const
{
	if (isNegated())
	{
		return KSimBoolTriState(m_outData.isFalse(), m_outData.isActive());
	}
	else
	{
		return m_outData.getTriState();
	}
};

/** Return the current wire state */
KSimBoolTriState ConnectorBoolTriState::getInput() const
{
	if (isNegated())
	{
		return KSimBoolTriState(m_inData.isFalse(), m_inData.isActive());
	}
	else
	{
		return m_inData.getTriState();
	}
}

/** Returns a pointer to the data that's read from the component. */
const void * ConnectorBoolTriState::readoutData() const
{
	return &m_outData;
}

const void * ConnectorBoolTriState::readoutInData() const
{
	return &m_inData;
}

/** Returns a text which represents the current value. */
QString ConnectorBoolTriState::getValueText() const
{
	return m_inData.getText();
}

WatchItemBase * ConnectorBoolTriState::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTriStateConnector(this);
	CHECK_PTR(wi);
	return wi;
}


//#################################################################
//#################################################################

static ConnectorBase * createNoNegatable(Component * comp, const QString & name, const QString & i18nName, const QPoint & pos)
{
	return new ConnectorBoolTriStateSpecial(comp, name, i18nName, pos);
}

const ConnectorInfo * getConnectorBoolTriStateSpecialInfo()
{
	static const ConnectorInfo Info(QString::fromLatin1("Boolean TriState No Negatable"),
	                                QString::fromLatin1("Boolean TriState No Negatable"),
	                                QString::fromLatin1("Boolean TriState"),
	                                createNoNegatable );
	return &Info;
}

ConnectorBoolTriStateSpecial::ConnectorBoolTriStateSpecial(Component * comp, const QString & name,
		                                                               const QString & i18nName, const QPoint & pos)
	:	ConnectorBoolTriState(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateSpecialInfo())
{
	init();
}

ConnectorBoolTriStateSpecial::ConnectorBoolTriStateSpecial(Component * comp, const QString & name, const QString & i18nName,
		                                                              const QString & descr, const QPoint & pos)
	:	ConnectorBoolTriState(comp, name, i18nName, pos, CO_RIGHT, getConnectorBoolTriStateSpecialInfo())
{
	init();
	new ConnectorLabel(this, descr);
}

ConnectorBoolTriStateSpecial::ConnectorBoolTriStateSpecial(Component * comp, const QString & name, const QString & i18nName,
		                                                               const QPoint & pos, ConnOrientationType orient, const ConnectorInfo * ci)
	:	ConnectorBoolTriState(comp, name, i18nName, pos, orient, ci)
{
	init();
}

void ConnectorBoolTriStateSpecial::init()
{
	setNegateEnabled(false);
}

WatchItemBase * ConnectorBoolTriStateSpecial::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTriStateConnectorSpecial(this);
	CHECK_PTR(wi);
	return wi;
}
