/***************************************************************************
                      ksimbooltristate.h  -  description
                             -------------------
    begin                : Thu Nov 14 2002
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

#ifndef KSIMBOOLTRISTATE_H
#define KSIMBOOLTRISTATE_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimtype.h"

// Forward declaration
class KSimData;


//#######################################################################################
//#######################################################################################

enum eKSimBoolTriState { KSIMBOOLTRISTATE_FALSE = 0,
                         KSIMBOOLTRISTATE_TRUE,
                         KSIMBOOLTRISTATE_INACTIVE };

/** This class gives a eKSimBoolTriState variable the common interface like defined in @ref KSimTypeBase and
  * @ref KSimTypeTemplate.
  *
  * @author Rasmus Diekenbrock
  */


class KSimBoolTriState : public KSimTypeTemplate<eKSimBoolTriState>
{
public:


	/** Constructs a new object with the value zero.*/
	KSimBoolTriState()
		: KSimTypeTemplate<eKSimBoolTriState>()
	{	
		m_value = KSIMBOOLTRISTATE_FALSE;
	};
	
	/** Constructs a new object with the value u.*/
	KSimBoolTriState(eKSimBoolTriState u)
	{	
		m_value = u;
	};
	
	/** Constructs a new object with the value like u.*/
	KSimBoolTriState(const KSimBoolTriState & u)
		: KSimTypeTemplate<eKSimBoolTriState>()
	{	
		m_value = u.value();
	};
	
	/** Constructs a new object with the value like u.*/
	KSimBoolTriState(bool state, bool active)
		: KSimTypeTemplate<eKSimBoolTriState>()
	{
		m_value = active ? (state ? KSIMBOOLTRISTATE_TRUE : KSIMBOOLTRISTATE_FALSE) : KSIMBOOLTRISTATE_INACTIVE;
	};

	bool isFalse() const  { return m_value == KSIMBOOLTRISTATE_FALSE; };
	bool isTrue() const   { return m_value == KSIMBOOLTRISTATE_TRUE; };
	bool isActive() const { return m_value != KSIMBOOLTRISTATE_INACTIVE; };
	
	bool operator==(const KSimBoolTriState & state) const { return (m_value == state.m_value);};
	bool operator!=(const KSimBoolTriState & state) const { return (m_value != state.m_value);};

	/** Returns the current value as string.*/
	virtual QString text() const;
	/** Sets a new value. */
	virtual void setText(const QString & text, bool * ok = 0);

	static const char * convert(KSimBoolTriState state);
	static KSimBoolTriState convert(const char * state, KSimBoolTriState eDefault = KSimBoolTriState(KSIMBOOLTRISTATE_FALSE));

};

#endif
