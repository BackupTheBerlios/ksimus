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
#include "ksimlineedit.h"

// Forward declaration
class KSimBaseInt;


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

	// Defintion must be same as KSimBaseInt::eBaseType !!!
	enum eBaseType { Binary      = 2,
	                 Octal       = 8,
	                 Decimal     = 10,
	                 Hexadecimal = 16
	};

	KSimBaseUInt()
		:	KSimUInt((unsigned int)0),
			m_base(Decimal)
	{ };
	
	KSimBaseUInt(unsigned int u)
		:	KSimUInt(u),
			m_base(Decimal)
	{ };
	
	KSimBaseUInt(unsigned int u, eBaseType base)
		:	KSimUInt(u),
			m_base(base)
	{ };
	
	KSimBaseUInt(const KSimBaseUInt & u)
		:	KSimUInt(u.value()),
			m_base(u.base())
	{ };

	KSimBaseUInt(const QString & text);
	KSimBaseUInt(const KSimBaseInt & newValue);
	
	const KSimBaseUInt & operator=(unsigned int newValue) { m_value = newValue; return *this; };
	
	void setBase(eBaseType base) { m_base = base; };
	eBaseType base() const { return m_base; };

	virtual void setText(const QString & str, bool * ok = 0);
	virtual QString text() const;

	bool operator==(const KSimBaseUInt & u) const;
	bool operator!=(const KSimBaseUInt & u) const;

	static KSimBaseUInt convert(const QString & text, bool * ok = 0);
	static const char * convertBase(eBaseType base);
	static eBaseType convertBase(const char * base, eBaseType defaultBase = Decimal);

private:
	eBaseType m_base;
};


#endif
