/***************************************************************************
                          inverter.h  -  description
                             -------------------
    begin                : Sun Oct 27 2002
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

#ifndef INVERTER_H
#define INVERTER_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/compview.h"
#include "ksimus/componentinfo.h"
#include "ksimus/boolean1out.h"

// Project-Includes

// Forward declaration
class ConnectorBoolIn;


namespace KSimLibBoolean
{

/**A Inverter and a Identity gate.
  *@author Rasmus Diekenbrock
  */

class Inverter : public Boolean1Out
{
	Q_OBJECT

public:
	Inverter(CompContainer * container, const ComponentInfo * ci);
//	~Inverter();

	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolIn * getInputConnector() const { return m_input; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInverterInfo();
	static const ComponentInfo * getStaticIdentityInfo();

private:
	ConnectorBoolIn * m_input;
};

//###############################################################
//###############################################################


class InverterView : public CompView
{
public:
	InverterView(Inverter * comp, eViewType viewType, const char * name);
//	~InverterView();
	virtual void draw(QPainter * p);
	
	Inverter * getInverter() const { return (Inverter*) getComponent(); };

};

}; //namespace KSimLibBoolean

#endif
