/***************************************************************************
                          floatadd.h  -  description
                             -------------------
    begin                : Wed Jan 02 2002
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

#ifndef FLOATADD_H
#define FLOATADD_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "float1out.h"


// Forward declaration
class ConnectorPack;

namespace KSimLibFloatingPoint
{

/**A floating point ADD gate with muliple inputs
	*
  * @author Rasmus Diekenbrock
  */

class FloatAdd : public Float1Out
{
public:
	/** Constructs a floating point ADD */
	FloatAdd(CompContainer * container, const ComponentInfo * ci);
	/** Executes the simulation of this component */
	virtual void calculate();
	
	/** Returns the input connector pack.
	  */
	ConnectorPack * getInputConnectorPack() const { return m_inPack; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorPack * m_inPack;
};

//###############################################################


};  //namespace KSimLibFloatingPoint
#endif
