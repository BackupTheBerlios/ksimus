/***************************************************************************
                          wireproperty.cpp  -  description
                             -------------------
    begin                : Sun Oct 15 2000
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

#include <klocale.h>

#include "wireproperty.h"

#include "ksimdata.h"
#include "wirepropertyinfo.h"
#include "wire.h"
#include "ksimdebug.h"
#include "connectorbase.h"
#include "connectorlist.h"

WireProperty::WireProperty(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	QObject(wire),
		ComponentItem(wire),
		m_wirePropertyInfo(wirePropertyInfo),
		m_inputCounter(0),
		m_outputCounter(0),
		m_triStateCounter(0)
{
}

WireProperty::~WireProperty()
{
}

/** save wire properties */
void WireProperty::save(KSimData &) const
{
	// nothing to do
}

/** load wire properties
	Returns true if successful */
bool WireProperty::load(KSimData &)
{
	// nothing to do
	return true;
}

/** Checks the connected component
	eg. No output connected or more than one connected
	Returns the number of errors
*/
int WireProperty::checkCircuit()
{
	int errors = 0;
	
	
	//*** Count inputs, outputs and triStates
	m_inputCounter = 0;
	m_outputCounter = 0;
	m_triStateCounter = 0;
	
	FOR_EACH_CONNECTOR(it,*getWire()->getConnList())
	{
		if (it.current()->isInput())
		{
			m_inputCounter++;
		}
		else if (it.current()->isOutput())
		{
			m_outputCounter++;
		}
		else if (it.current()->isTriState())
		{
			m_triStateCounter++;
		}
		else
			KSIMDEBUG("unknown connector direction");
	}

	//*** min 1 Output or TriState required
	if ((m_outputCounter == 0) && (m_triStateCounter == 0))
	{
		// Wire without controlling output
		logError(i18n("Wire without controlling output"));
		errors += 1;
		
	}
	
	return errors;
}

	
/** Add menu items depend on wire properties
  *	Return true, if items are added */
bool WireProperty::initPopupMenu(QPopupMenu *) const
{
	return false;
}
	
/** Returns the associated WirePropertyInfo */
const WirePropertyInfo * WireProperty::getInfo() const
{
	return m_wirePropertyInfo;
}


WirePropertySingleOutput::WirePropertySingleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo)
	:	WireProperty(wire, wirePropertyInfo)
{
}

/** Checks the connected component
	eg. No output connected or more than one connected
	Returns the number of errors
*/
int WirePropertySingleOutput::checkCircuit()
{
	int errors = WireProperty::checkCircuit();
	
	//*** min 1 Output or TriState required
	if (m_outputCounter > 1)
	{
		logError(i18n("Wire has more than one contected output"));
		errors += 1;
	}
	
	if (m_triStateCounter != 0)
	{
		logError(i18n("TriStates outputs are not allowed"));
		errors += 1;
	}
	
	return errors;
}
