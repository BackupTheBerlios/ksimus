/***************************************************************************
                          convertintegerfloat.h  -  description
                             -------------------
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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

#ifndef CONVERTINTEGERFLOAT_H
#define CONVERTINTEGERFLOAT_H


// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimus/component.h"
#include "ksimus/compview.h"

// Forward declaration
class ConnectorIntegerIn;
class ConnectorFloatOut;

namespace KSimLibInteger
{

/**Component for converting an integer value in a floating point value.
  *@author Rasmus Diekenbrock
  */

class ConvertIntegerFloat : public Component
{
public:
	/** Constructs the component. */
	ConvertIntegerFloat(CompContainer * container, const ComponentInfo * ci);
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorIntegerIn * getInput() { return m_input; };
	ConnectorFloatOut * getOutput() { return m_output; };
	
	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticInfo();

private:
	
	ConnectorIntegerIn * m_input;
	ConnectorFloatOut * m_output;
	
	bool m_recursionLocked;

	class View;
};

//###############################################################

/** View class of @ref ConvertIntegerFloat.
	*
  * @author Rasmus Diekenbrock
  */
class ConvertIntegerFloat::View : public CompView
{
public:
	View(ConvertIntegerFloat * comp, eViewType viewType, const char * name);
//	~View();
	
	virtual void draw(QPainter * p);
	
	ConvertIntegerFloat* getConvertIntegerFloat() { return (ConvertIntegerFloat*) CompView::getComponent(); };
};

//###############################################################


};  //namespace KSimLibInteger
#endif
