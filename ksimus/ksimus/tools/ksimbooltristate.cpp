/***************************************************************************
                      ksimbooltristate.cpp  -  description
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

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimbooltristate.h"
#include "ksimdata.h"
#include "enumdict.h"
#include "ksimdebug.h"

// Forward declaration


//#########################################################################################

EnumDict<eKSimBoolTriState>::tData EnumDict<eKSimBoolTriState>::data[]
      = { {"False",        KSIMBOOLTRISTATE_FALSE},
          {"True",         KSIMBOOLTRISTATE_TRUE},
          {"Inactive",     KSIMBOOLTRISTATE_INACTIVE},
          {0,              (eKSimBoolTriState)0}};

static const EnumDict<eKSimBoolTriState> & getTriStateDict()
{
	static EnumDict<eKSimBoolTriState> triStateDict;
	return triStateDict;
}

const char * KSimBoolTriState::convert(KSimBoolTriState state)
{
	return getTriStateDict().find(state);
}

KSimBoolTriState KSimBoolTriState::convert(const char * state, KSimBoolTriState eDefault)
{
	return KSimBoolTriState(getTriStateDict().find(state, eDefault));
}


QString KSimBoolTriState::text() const
{
	return convert(m_value);
}

void KSimBoolTriState::setText(const QString & text, bool * ok)
{
	eKSimBoolTriState e = convert(text, (eKSimBoolTriState) -1);
	if ( e == (eKSimBoolTriState) -1)
	{
		if (ok) *ok = false;
		KSIMDEBUG(QString::fromLatin1("FixMe: '%1' is no eKSimBoolTriState").arg(text));
	}
	else
	{
		m_value = e;
		if (ok) *ok = true;
	}
}








