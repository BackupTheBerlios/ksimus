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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "wirepropertymultipleoutput.h"
#include "ksimbooltristate.h"

// Forward declaration


//###############################################################



class WireStateBoolTristate
{
public:
	WireStateBoolTristate()
		: m_trueCount(0),
			m_falseCount(0)
	{};
	
	WireStateBoolTristate(unsigned int trueCount, unsigned int falseCount)
		: m_trueCount(trueCount),
			m_falseCount(falseCount)
	{};

//	~WireStateBoolTristate();

	void set(unsigned int trueCount, unsigned int falseCount);
	void setTrue(unsigned int trueCount);
	void setFalse(unsigned int falseCount);
	void setTristate(KSimBoolTristate state);

	unsigned int getTrue() const;
	unsigned int getFalse() const;
	bool isTrue() const;
	bool isFalse() const;
	bool isActive() const;
	bool isInactive() const;
	KSimBoolTristate getTristate() const;

	QString getText() const;
	QString getDetailedText() const;

private:

	unsigned int m_trueCount;
	unsigned int m_falseCount;
};



inline void WireStateBoolTristate::set(unsigned int trueCount, unsigned int falseCount)
{
	m_trueCount = trueCount;
	m_falseCount = falseCount;
};

inline void WireStateBoolTristate::setTrue(unsigned int trueCount)
{
	m_trueCount = trueCount;
};

inline void WireStateBoolTristate::setFalse(unsigned int falseCount)
{
	m_falseCount = falseCount;
};

inline unsigned int WireStateBoolTristate::getTrue() const
{
	return m_trueCount;
};

inline unsigned int WireStateBoolTristate::getFalse() const
{
	return m_falseCount;
};

inline bool WireStateBoolTristate::isTrue() const
{
	return (getTrue() != 0) && (getFalse() == 0);
}

inline bool WireStateBoolTristate::isFalse() const
{
	return getFalse() != 0;
}

inline bool WireStateBoolTristate::isActive() const
{
	return (getTrue() != 0) || (getFalse() != 0);
};

inline bool WireStateBoolTristate::isInactive() const
{
	return (getTrue() == 0) && (getFalse() == 0);
};

inline void WireStateBoolTristate::setTristate(KSimBoolTristate state)
{
	m_trueCount = state.isTrue() ? 1 : 0;
	m_falseCount = state.isFalse() ? 1 : 0;
}

inline KSimBoolTristate WireStateBoolTristate::getTristate() const
{
	return KSimBoolTristate(!isFalse(), isActive());
}

//###############################################################



/**The wire property for boolean tristate connectors.
  *@author Rasmus Diekenbrock
  */

const WirePropertyInfo * getWirePropertyBoolTristateInfo();

class WirePropertyBoolTristate : public WirePropertyMultipleOutput
{

	Q_OBJECT
	
public:
	WirePropertyBoolTristate(Wire * wire);
	virtual ~WirePropertyBoolTristate();

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
	WireStateBoolTristate m_state;
	bool m_lockRecursion;


};

#endif
