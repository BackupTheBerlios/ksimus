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
#include "ksimus/compview.h"


// Forward declaration
class ConnectorBoolIn;
class ConnectorBoolTriState;
class ComponentControlBlock;
class ComponentLayout;



namespace KSimLibBoolean
{



/**Converter from boolean to boolean tristate
  *@author Rasmus Diekenbrock
  */

const ComponentInfo * getConvertBool2BoolTriStateInfo();

class ConvertBool2BoolTriState : public Component
{

	Q_OBJECT

public:
	ConvertBool2BoolTriState(CompContainer * container, const ComponentInfo * ci);
	~ConvertBool2BoolTriState();
	
	/** Resets the component state. */
	virtual void reset();
	
	/** Executes the simulation of this component */
	virtual void calculate();

	ConnectorBoolIn * getInputDataIn() const { return m_connDataIn; };
	ConnectorBoolIn * getInputActiveIn() const { return m_connActiveIn; };
	ConnectorBoolTriState * getOutput() const { return m_connOut; };
	
private:
	ConnectorBoolIn * m_connDataIn;
	ConnectorBoolIn * m_connActiveIn;
	ConnectorBoolTriState * m_connOut;
	bool m_lockRecursion;

};

//###############################################################

class ConvertBool2BoolTriStateView : public CompView
{
public:
	ConvertBool2BoolTriStateView(ConvertBool2BoolTriState * comp, eViewType viewType);
	
	virtual void draw(QPainter * p);

	ConvertBool2BoolTriState * getConverter() const { return (ConvertBool2BoolTriState*) getComponent(); };

	ComponentControlBlock * getControlBlock() const { return m_ctrlBlock; };
	ComponentLayout * getComponentLayout() const { return m_layout; };

private:
	ComponentControlBlock * m_ctrlBlock;
	ComponentLayout * m_layout;
};

//###############################################################

}; //namespace KSimLibBoolean

#endif
