/***************************************************************************
                          wirepropertybooltristate.h  -  description
                             -------------------
    begin                : Sat Jun 29 2002
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

#ifndef WIREPROPERTYBOOLTRISTATE_H
#define WIREPROPERTYBOOLTRISTATE_H

#include "wirepropertymultipleoutput.h"


//###############################################################



class WireStateBoolTriState
{
public:
	WireStateBoolTriState()
		: m_trueCount(0),
			m_falseCount(0)
	{};
	
//	~WireStateBoolTriState();

	void set(unsigned int trueCount, unsigned int falseCount);
	void setTrue(unsigned int trueCount);
	void setFalse(unsigned int falseCount);

	bool getState() const;
	unsigned int getTrue() const;
	unsigned int getFalse() const;
	bool isActive() const;
	bool isInactive() const;

	QString getText() const;
	QString getDetailedText() const;

private:

	unsigned int m_trueCount;
	unsigned int m_falseCount;
};



inline void WireStateBoolTriState::set(unsigned int trueCount, unsigned int falseCount)
{
	m_trueCount = trueCount;
	m_falseCount = falseCount;
};

inline void WireStateBoolTriState::setTrue(unsigned int trueCount)
{
	m_trueCount = trueCount;
};

inline void WireStateBoolTriState::setFalse(unsigned int falseCount)
{
	m_falseCount = falseCount;
};

inline bool WireStateBoolTriState::getState() const
{
	return (getFalse() == 0);
};

inline unsigned int WireStateBoolTriState::getTrue() const
{
	return m_trueCount;
};

inline unsigned int WireStateBoolTriState::getFalse() const
{
	return m_falseCount;
};

inline bool WireStateBoolTriState::isActive() const
{
	return (getTrue() != 0) || (getFalse() != 0);
};

inline bool WireStateBoolTriState::isInactive() const
{
	return (getTrue() == 0) && (getFalse() == 0);
};


//###############################################################



/**The wire property for boolean tristate connectors.
  *@author Rasmus Diekenbrock
  */

const WirePropertyInfo * getWirePropertyBoolTriStateInfo();

class WirePropertyBoolTriState : public WirePropertyMultipleOutput
{

	Q_OBJECT
	
public:
	WirePropertyBoolTriState(Wire * wire);
	virtual ~WirePropertyBoolTriState();

	/** Setup the Wire property for a new circuit execution.
	  * The sub class has to implement this function.
	  */
	virtual void setupCircuit();
	/** Executes the WireProperty. This means copies the data from the output connector
	  * to the input connector. */
	virtual void execute();
	
	
	/** Get the colors for the wire property. */
	virtual const WireColorScheme & getColorScheme() const;

	static const WireColorScheme & colorScheme();
	
	/** Returns a pointer to the data that's read from the wire.
	  * Reimplementations is required. */
	virtual const void * readoutData() const;

	/** Returns a @ref WatchItemBooleanWireProperty object. */
	virtual WatchItemBase * makeWatchItem();

private:
	WireStateBoolTriState m_state;
	bool m_lockRecursion;


};

#endif
