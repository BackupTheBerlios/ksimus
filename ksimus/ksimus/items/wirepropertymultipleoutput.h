/***************************************************************************
                          wirepropertymultipleoutput.h  -  description
                             -------------------
    begin                : Fri Jun 28 2002
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

#ifndef WIREPROPERTYMULTIPLEOUTPUT_H
#define WIREPROPERTYMULTIPLEOUTPUT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "wireproperty.h"


// Forward declaration
class ConnectorBase;
class ConnectorTriStateList;
class ConnectorInputList;



/**Base class for wire proeprties with multiple outputs.
  *@author Rasmus Diekenbrock
  */

class WirePropertyMultipleOutput : public WireProperty
{
   Q_OBJECT
public: 
	~WirePropertyMultipleOutput();

	/** Setup the Wire property for a new circuit execution.
	  * Copies this wire property into the connected connectors.
	  */
	virtual void setupCircuit();
	
	/** Returns the count of currently active connectors. */
	unsigned int getActiveConnectorCount() const;
	
	WirePropertyMultipleOutput * getMasterWireProperty() const;
	
protected:
	WirePropertyMultipleOutput(Wire * wire, const WirePropertyInfo * wirePropertyInfo);
	
	void setMasterWireProperty(WirePropertyMultipleOutput * masterWP);
	
	ConnectorTriStateList * m_connectorList;
	ConnectorInputList * m_connectorInputList;
	ComponentList * m_executeNextList;
	ComponentList * m_zeroDelayList;
	WirePropertyList * m_wirePropertyList;
	unsigned int m_activeConnectors;
	
private slots:
	void slotPreReset();

private:
	void setupInternal(WirePropertyMultipleOutput * wireProperty);
	void setupInternalAddWireProperty(ConnectorBase * conn);
	
	WirePropertyMultipleOutput * m_masterWP;
};

#endif
