/***************************************************************************
                          ksimbaseuint.cpp  -  description
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

// C-Includes
#include <limits.h>

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimbaseuint.h"
#include "ksimbaseint.h"
#include "ksimdebug.h"
#include "enumdict.h"

// Forward declaration


//##########################################################
//##########################################################


EnumDict<KSimBaseUInt::eBaseType>::tData EnumDict<KSimBaseUInt::eBaseType>::data[]
      = { {"Binary",      KSimBaseUInt::Binary},
          {"Octal",       KSimBaseUInt::Octal},
          {"Decimal",     KSimBaseUInt::Decimal},
          {"Hexadecimal", KSimBaseUInt::Hexadecimal},
          {0,             KSimBaseUInt::Decimal}};

static const EnumDict<KSimBaseUInt::eBaseType> & getKSimBaseUIntBaseTypeDict()
{
	static EnumDict<KSimBaseUInt::eBaseType> dict(5);
	return dict;
}


const char * KSimBaseUInt::convertBase(KSimBaseUInt::eBaseType base)
{
	return getKSimBaseUIntBaseTypeDict().find(base);
}
KSimBaseUInt::eBaseType KSimBaseUInt::convertBase(const char * base, KSimBaseUInt::eBaseType defaultBase)
{
	return getKSimBaseUIntBaseTypeDict().find(base, defaultBase);
}

//#######################################################################################
//#######################################################################################


KSimBaseUInt::KSimBaseUInt(const QString & text)
	:	KSimUInt(0),
		m_base(KSimBaseUInt::Decimal)
{	
	setText(text);
}


KSimBaseUInt::KSimBaseUInt(const KSimBaseInt & newValue)
	:	KSimUInt((unsigned int)newValue.value()),
		m_base((KSimBaseUInt::eBaseType)newValue.base())
{
}


void KSimBaseUInt::setText(const QString & str, bool * ok)
{
	KSimBaseUInt u(convert(str, ok));
	m_value = u.value();
	m_base = u.base();
}


QString KSimBaseUInt::text() const
{
	switch(base())
	{
		case KSimBaseUInt::Binary:
			return QString::fromLatin1("0b") + QString::number(value(), 2);
			break;
		
		case KSimBaseUInt::Octal:
			return QString::fromLatin1("0o") + QString::number(value(), 8);
			break;
		
		case KSimBaseUInt::Decimal:
			return QString::number(value(), 10);
			break;
		
		case KSimBaseUInt::Hexadecimal:
			return QString::fromLatin1("0x") + QString::number(value(), 16);
			break;

		default:
			KSIMDEBUG_VAR("Unknown base", base());
			return QString::fromLatin1("0");
	}
}

KSimBaseUInt KSimBaseUInt::convert(const QString & text, bool * ok)
{
	QString str(text.stripWhiteSpace());
	int myBase = 10;
	KSimBaseUInt::eBaseType baseEnum = KSimBaseUInt::Decimal;
	unsigned int idx = 0;
	unsigned int v = 0;
	bool myOk = true;
	
	
	if (str[idx].latin1() == '0')
	{
		idx++;
	}
	
	switch(str[idx].latin1())
	{
		case 'b':
		case 'B':
			myBase = 2;
			baseEnum = KSimBaseUInt::Binary;
			idx++;
			break;
	
		case 'o':
		case 'O':
			myBase = 8;
			baseEnum = KSimBaseUInt::Octal;
			idx++;
			break;
	
		case 'x':
		case 'X':
			myBase = 16;
			baseEnum = KSimBaseUInt::Hexadecimal;
			idx++;
			break;
	
		default:
			break;
	}
	
	if (str.length() > idx)
	{
		str = str.right(str.length()-idx); // leading chars

		v = str.toUInt(&myOk, myBase);

		if (!myOk)
		{
			KSIMDEBUG_VAR("Conversion error", text);
		}
	}
	
	if(ok)
	{
		*ok = myOk;
	}
	
	
	return KSimBaseUInt(v, baseEnum);
}

bool KSimBaseUInt::operator==(const KSimBaseUInt & u) const
{
	return ((this->value() == u.value()) && (this->base() == u.base()));
};

bool KSimBaseUInt::operator!=(const KSimBaseUInt & u) const
{
	return ((this->value() != u.value()) || (this->base() != u.base()));
};


