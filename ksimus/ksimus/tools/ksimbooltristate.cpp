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

EnumDict<eKSimBoolTristate>::tData EnumDict<eKSimBoolTristate>::data[]
      = { {"False",        KSIMBOOLTRISTATE_FALSE},
          {"True",         KSIMBOOLTRISTATE_TRUE},
          {"Inactive",     KSIMBOOLTRISTATE_INACTIVE},
          {0,              (eKSimBoolTristate)0}};

static const EnumDict<eKSimBoolTristate> & getTristateDict()
{
	static EnumDict<eKSimBoolTristate> triStateDict;
	return triStateDict;
}

const char * KSimBoolTristate::convert(KSimBoolTristate state)
{
	return getTristateDict().find(state);
}

KSimBoolTristate KSimBoolTristate::convert(const char * state, KSimBoolTristate eDefault)
{
	return KSimBoolTristate(getTristateDict().find(state, eDefault));
}


QString KSimBoolTristate::text() const
{
	return QString::fromLatin1(convert(m_value));
}

void KSimBoolTristate::setText(const QString & text, bool * ok)
{
	eKSimBoolTristate e = convert(text, (eKSimBoolTristate) -1);
	if ( e == (eKSimBoolTristate) -1)
	{
		if (ok) *ok = false;
		KSIMDEBUG(QString::fromLatin1("FixMe: '%1' is no eKSimBoolTristate").arg(text));
	}
	else
	{
		m_value = e;
		if (ok) *ok = true;
	}
}








