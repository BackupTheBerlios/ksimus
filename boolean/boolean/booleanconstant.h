/***************************************************************************
                          booleanconstant.h  -  description
                             -------------------
    begin                : Mon Dec 17 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

#ifndef BOOLEANCONSTANT_H
#define BOOLEANCONSTANT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentpropertygeneralwidget.h"



// Forward declaration
class ConnectorBoolOut;


namespace KSimLibBoolean
{

extern const ComponentInfo BooleanConstantTrue;
extern const ComponentInfo BooleanConstantFalse;


//###############################################################
//###############################################################


/**Boolean constant component.
  *@author Rasmus Diekenbrock
  */

class BooleanConstant : public Component
{
public:
	BooleanConstant(CompContainer * container, const ComponentInfo * ci);
//	~BooleanConstant();

	/** Resets the component state. */
	virtual void reset();
	
	ConnectorBoolOut * getOutputConnector() const { return m_out; };
	
private:
	ConnectorBoolOut * m_out;
};


//###############################################################
//###############################################################

}; //namespace KSimLibBoolean

#endif
