/***************************************************************************
                          convertbooltristate2bool.h  -  description
                             -------------------
    begin                : Mon Jul 1 2002
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

#ifndef CONVERTBOOLTRISTATE2BOOL_H
#define CONVERTBOOLTRISTATE2BOOL_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"


// Forward declaration
class ConnectorBoolOut;
class ConnectorBoolTristate;


/**Converts a boolean tristate into a boolean.
  *@author Rasmus Diekenbrock
  */

namespace KSimLibBoolean
{


const ComponentInfo * getConvertBoolTristate2BoolInfo();

class ConvertBoolTristate2Bool : public Component
{

	Q_OBJECT

public:
	ConvertBoolTristate2Bool(CompContainer * container, const ComponentInfo * ci);
	~ConvertBoolTristate2Bool();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();

	
private:
	ConnectorBoolTristate * m_connIn;
	ConnectorBoolOut * m_connDataOut;
	ConnectorBoolOut * m_connActiveOut;
	bool m_lockRecursion;

};


//###############################################################

class ConvertBoolTristate2BoolView : public CompView
{
public:
	ConvertBoolTristate2BoolView(ConvertBoolTristate2Bool * comp, eViewType viewType);
	
	virtual void draw(QPainter * p);
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
