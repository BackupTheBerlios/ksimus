/***************************************************************************
                          convertbool2booltristate.h  -  description
                             -------------------
    begin                : Sun Jun 30 2002
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

#ifndef CONVERTBOOL2BOOLTRISTATE_H
#define CONVERTBOOL2BOOLTRISTATE_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"


// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolTristate;

namespace KSimLibBoolean
{



/**Converter from boolean to boolean tristate
  *@author Rasmus Diekenbrock
  */

class ConvertBool2BoolTristate : public Component
{

	Q_OBJECT

public:
	ConvertBool2BoolTristate(CompContainer * container, const ComponentInfo * ci);
	~ConvertBool2BoolTristate();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolIn * getInputDataIn() const { return m_connDataIn; };
	ConnectorBoolIn * getInputActiveIn() const { return m_connActiveIn; };
	ConnectorBoolTristate * getOutput() const { return m_connOut; };
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();


private:
	ConnectorBoolIn * m_connDataIn;
	ConnectorBoolIn * m_connActiveIn;
	ConnectorBoolTristate * m_connOut;
	bool m_lockRecursion;

};

//###############################################################

}; //namespace KSimLibBoolean

#endif
