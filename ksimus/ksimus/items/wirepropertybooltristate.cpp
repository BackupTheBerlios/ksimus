/***************************************************************************
                          wirepropertybooltristate.cpp  -  description
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

// C-Includes
#include <limits.h>

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "wirepropertybooltristate.h"
#include "wirepropertyinfo.h"
#include "connectorbase.h"
#include "ksimdebug.h"
#include "watchitembooltristate.h"
#include "wire.h"

// Forward declaration




QString WireStateBoolTristate::getText() const
{
	if (!isActive())
	{
		return i18n("BoolTristate", "Inactive");
	}
	else if (isTrue())
	{
		return i18n("BoolTristate", "True");
	}
	else
	{
		return i18n("BoolTristate", "False");
	}
}

QString WireStateBoolTristate::getDetailedText() const
{
	return getText() + i18n("BoolTristate", " (True: %1, False: %2)")
		                     .arg(getTrue())
		                     .arg(getFalse());
}


static WireProperty * create(Wire * wire)
{
	WireProperty * wp;
	wp = new WirePropertyBoolTristate(wire);
	CHECK_PTR(wp);
	return wp;
}


const WirePropertyInfo * getWirePropertyBoolTristateInfo()
{
	static const WirePropertyInfo Info(QString::fromLatin1("Wire Property Boolean Tristate"),
	                                   QString::fromLatin1("wire/property/boolean tristate"),
	                                   QString::fromLatin1("Boolean Tristate"),
	                                   create );
	return &Info;
}


WirePropertyBoolTristate::WirePropertyBoolTristate(Wire * wire)
	:	WirePropertyMultipleOutput(wire, getWirePropertyBoolTristateInfo()),
		m_lockRecursion(false)
{
}

WirePropertyBoolTristate::~WirePropertyBoolTristate()
{
}

void WirePropertyBoolTristate::setupCircuit()
{
	WirePropertyMultipleOutput::setupCircuit();

	// Set invalid
	m_state.set(UINT_MAX, UINT_MAX);
}

/** Executes the WireProperty. This means copies the data from the output connector
  * to the input connector. */
void WirePropertyBoolTristate::execute()
{
//	KSIMDEBUG_VAR("WirePropertyBoolTristate::execute()", getWire()->getName());
/*	if (this != getMasterWireProperty())
	{
		KSIMDEBUG_VAR("this != getMasterWireProperty()", getWire()->getName());
	}*/
	
	if (m_lockRecursion) return;
	
	m_lockRecursion = true;
	unsigned int trueCounter = 0;
	unsigned int falseCounter = 0;
	
	FOR_EACH_CONNECTORTRISTATE(itRd, *m_connectorList)
	{
		const WireStateBoolTristate * state = (const WireStateBoolTristate *)itRd.current()->readoutData();
		trueCounter += state->getTrue();
		falseCounter += state->getFalse();
	}
	
	if ((falseCounter != m_state.getFalse()) || (trueCounter != m_state.getTrue()))
	{
		// State changed. Update components
		
		m_activeConnectors = trueCounter + falseCounter;
		m_state.set(trueCounter, falseCounter);
	
		FOR_EACH_CONNECTORTRISTATE(itWr, *m_connectorList)
		{
			itWr.current()->putData(&m_state);
		}
		
		FOR_EACH_CONNECTORINPUT(itInput, *m_connectorInputList)
		{
			itInput.current()->putData(&m_state);
		}
	
		FOR_EACH_COMP(itNext, *m_executeNextList)
		{
			// Execute components at next cycle
			itNext.current()->executeNext();
		}
		
		FOR_EACH_COMP(itZero, *m_zeroDelayList)
		{
			// Execute zero delay components
			itZero.current()->calculate();
		}
	}
	
	m_lockRecursion = false;
}
	
	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyBoolTristate::colorScheme()
{
	static WireColorScheme colorScheme(black, darkGreen);
	
	return colorScheme;
}

	/** Get the colors for the wire property. */
const WireColorScheme & WirePropertyBoolTristate::getColorScheme() const
{
	return colorScheme();
}

const void * WirePropertyBoolTristate::readoutData() const
{
	return (const void *)&m_state;
}

WatchItemBase * WirePropertyBoolTristate::makeWatchItem()
{
	WatchItemBase * wi = new WatchItemBoolTristateWireProperty(this);
	CHECK_PTR(wi);
	return wi;
}

#include "wirepropertybooltristate.moc"
