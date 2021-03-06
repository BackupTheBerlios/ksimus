/***************************************************************************
                          jkflipflop.h  -  description
                             -------------------
    begin                : Fri Dec 7 2001
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

#ifndef JKFLIPFLOP_H
#define JKFLIPFLOP_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimus/compview.h"

// Project-Includes
#include "flipflopbase.h"

// Forward declaration
class ConnectorBoolInEdge;
class ConnectorBoolIn;
class ComponentLayoutSimple;
class CompContainer;


namespace KSimLibBoolean
{


//###############################################################
//###############################################################

/** A single edge JK-FF component.
	*
	* @see JKFlipFlopBase
	* @see JKFlipFlopView
	*
  * @author Rasmus Diekenbrock
  */

class JKFlipFlop : public KSimLibBoolean::FlipFlopBase
{
	Q_OBJECT

public:
	JKFlipFlop(CompContainer * container, const ComponentInfo * ci);
//	~JKFlipFlop();

	/** Executes the simulation of this component */
	virtual void calculate();
	/** Resets the component state. */
	virtual void reset();

	/** save component properties */
	virtual void save(KSimData & file) const;
	/** load component properties
	*   copyLoad is true, if the load function is used as a copy function
	*	  Returns true if successful */
	virtual bool load(KSimData & file, bool copyLoad);
	
	/** Creates the general property page for the property dialog.
	  * This function creeates a @ref JKFlipFlopPropertyGeneralWidget.
	  * This function is called by @ref addGeneralProperty*/
	virtual ComponentPropertyBaseWidget * createGeneralProperty(QWidget *parent);
	
	/** Returns the "J" input connector.
	  */
	ConnectorBoolIn * getJInputConnector() const { return m_inJ; };
	/** Returns the "K" input connector.
	  */
	ConnectorBoolIn * getKInputConnector() const { return m_inK; };
	/** Returns the "Clk" input connector.
	  */
	ConnectorBoolInEdge * getClockInputConnector() const { return m_inClk; };
	
	/** Enables Master Slave. */
	void setMasterSlaveEnabled(bool ena) { m_isMasterSlave = ena; };
	/** Returns true, if Master Slave is enabled. */
	bool isMasterSlaveEnabled() const { return m_isMasterSlave; };

	static Component * create(CompContainer * container, const ComponentInfo * ci);
	static const ComponentInfo * getStaticJKInfo();
	static const ComponentInfo * getStaticJKMSInfo();

protected:
	/** Executes the simulation of a single edge JK FF */
	void calculateJK();
	/** Executes the simulation of a JK Master Slave FF */
	void calculateJKMS();

	ConnectorBoolIn * m_inJ;
	ConnectorBoolIn * m_inK;
	ConnectorBoolInEdge * m_inClk;
	
	bool m_isMasterSlave;
	bool m_lastClk;
	bool m_lastJ;
	bool m_lastK;
};

//###############################################################
//###############################################################

/** The view of the JK-FF.
	*
	* @see JKFlipFlop
	* @see JKMSFlipFlop
	*
  * @author Rasmus Diekenbrock
  */

class JKFlipFlopView : public CompView
{
public:
	JKFlipFlopView(JKFlipFlop * comp, eViewType viewType, const char * name);
//	~JKFlipFlopView();
//	virtual void draw(QPainter * p);

private:
	ComponentLayoutSimple * m_layout;

	JKFlipFlop * getJKFF() { return (JKFlipFlop *) getComponent(); };

};

//###############################################################
//###############################################################

class JKFlipFlopPropertyGeneralWidget : public KSimLibBoolean::FlipFlopBasePropertyGeneralWidget
{
	Q_OBJECT

public:
	JKFlipFlopPropertyGeneralWidget(JKFlipFlop * comp, QWidget *parent=0, const char *name=0);
//	~JKFlipFlopPropertyGeneralWidget();


	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this slot, if you add new properties.
	 */
	virtual void defaultPressed();


private:
	KSimBooleanBox * m_masterSlave;
	
	JKFlipFlop * getJKFF() { return (JKFlipFlop *) getComponent(); };
};



}; //namespace KSimLibBoolean

#endif
