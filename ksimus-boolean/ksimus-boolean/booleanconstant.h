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


/**Boolean constant false component.
  *@author Rasmus Diekenbrock
  */

class BooleanConstantFalse : public Component
{
class View;
public:
	BooleanConstantFalse(CompContainer * container, const ComponentInfo * ci);
//	~BooleanConstantFalse();

	/** Resets the component state. */
	virtual void reset();
	
	ConnectorBoolOut * getOutputConnector() const { return m_out; };
	

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorBoolOut * m_out;
};

//###############################################################
//###############################################################

/**Boolean constant component.
  *@author Rasmus Diekenbrock
  */

class BooleanConstantTrue : public Component
{
class View;
public:
	BooleanConstantTrue(CompContainer * container, const ComponentInfo * ci);
//	~BooleanConstantTrue();

	/** Resets the component state. */
	virtual void reset();
	
	ConnectorBoolOut * getOutputConnector() const { return m_out; };
	

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	ConnectorBoolOut * m_out;
	static QBitmap * m_bitmap;
	static unsigned int m_instanceCnt;
};

//###############################################################
//###############################################################

}; //namespace KSimLibBoolean

#endif
