/***************************************************************************
                          ksimbaseuint.h  -  description
                             -------------------
    begin                : Sun Dec 23 2001
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

#ifndef KSIMBASEUINT_H
#define KSIMBASEUINT_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimtype.h"

// Forward declaration


//#######################################################################################
//#######################################################################################

/**This class contains a unsigned int variable which can be stored or displayed as binary, octal
  * decimal or hexadecimal format.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseUInt : public KSimUInt
{
public:

	enum eKSimBaseTypes { Binary, Octal, Decimal, Hexadecimal };

	KSimBaseUInt()
		:	KSimUInt((unsigned int)0),
			m_base(Decimal)
	{	};
	
	KSimBaseUInt(unsigned int u)
		:	KSimUInt(u),
			m_base(Decimal)
	{	};
	
	KSimBaseUInt(const KSimBaseUInt & u)
		:	KSimUInt(u.value()),
			m_base(u.base())
	{	};

	
	void setBase(eKSimBaseTypes base) { m_base = base; };
	eKSimBaseTypes base() const { return m_base; };

	virtual void setText(const QString & str, bool * ok = 0);
	virtual QString text() const;

	static unsigned int convert(const QString & text, eKSimBaseTypes * base = 0, bool * ok = 0);

private:
	eKSimBaseTypes m_base;
};

#endif
