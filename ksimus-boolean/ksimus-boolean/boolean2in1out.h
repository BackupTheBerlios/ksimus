/***************************************************************************
                          boolean2in1out.h  -  description
                             -------------------
    begin                : Mon May 14 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
    email                : radie@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BOOLEAN2IN1OUT_H
#define BOOLEAN2IN1OUT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"

// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolOut;

/**Base class vor boolean gates with 2 inputs and 1 output.
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{

class Boolean2In1Out : public Component
{
	Q_OBJECT
	
public:
	Boolean2In1Out(CompContainer * container, const ComponentInfo * ci);
//	~Boolean2In1Out();
	
	/** Reset all simulation variables */
	virtual void reset();
	
	/** Returns the output connector. */
	ConnectorBoolOut * getOutputConnector() const { return m_out; };
	/** Returns the intput connector A. */
	ConnectorBoolIn * getInputConnectorA() const { return m_inA; };
	/** Returns the intput connector A. */
	ConnectorBoolIn * getInputConnectorB() const { return m_inB; };
	/** Sets the current component state. */
	void setState(bool newState);
	/** Returns the current component state. */
	bool getState() const;
	

private:
	bool m_result;
	ConnectorBoolOut * m_out;
	ConnectorBoolIn * m_inA;
	ConnectorBoolIn * m_inB;
};

//###############################################################

class Boolean2In1OutView : public CompView
{
public:
	Boolean2In1OutView(Component * comp, eViewType viewType);
//	~Boolean2In1OutView();
	virtual void draw(QPainter * p);
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
