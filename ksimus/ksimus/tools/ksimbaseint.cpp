/***************************************************************************
                          ksimbaseint.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include <stdlib.h>

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimbaseint.h"
#include "ksimbaseuint.h"
#include "ksimdebug.h"
#include "enumdict.h"

// Forward declaration


//##########################################################
//##########################################################


EnumDict<KSimBaseInt::eBaseType>::tData EnumDict<KSimBaseInt::eBaseType>::data[]
      = { {"Binary",      KSimBaseInt::Binary},
          {"Octal",       KSimBaseInt::Octal},
          {"Decimal",     KSimBaseInt::Decimal},
          {"Hexadecimal", KSimBaseInt::Hexadecimal},
          {0,             KSimBaseInt::Decimal}};

static const EnumDict<KSimBaseInt::eBaseType> & getKSimBaseIntBaseTypeDict()
{
	static EnumDict<KSimBaseInt::eBaseType> dict(5);
	return dict;
}


const char * KSimBaseInt::convertBase(KSimBaseInt::eBaseType base)
{
	return getKSimBaseIntBaseTypeDict().find(base);
}
KSimBaseInt::eBaseType KSimBaseInt::convertBase(const char * base, KSimBaseInt::eBaseType defaultBase)
{
	return getKSimBaseIntBaseTypeDict().find(base, defaultBase);
}

//##########################################################
//##########################################################


KSimBaseInt::KSimBaseInt(const QString & text)
	:	KSimInt(0),
		m_base(Decimal)
{	
	setText(text);
}

KSimBaseInt::KSimBaseInt(const KSimBaseUInt & newValue)
	:	KSimInt((int)newValue.value()),
		m_base((KSimBaseInt::eBaseType)newValue.base())
{
}


void KSimBaseInt::setText(const QString & str, bool * ok)
{
	KSimBaseInt i(convert(str, ok));
	m_value = i.value();
	m_base = i.base();
}


QString KSimBaseInt::text() const
{
	QString sign(value()<0
	             ? QString::fromLatin1("-")
	             : QString::null);

	switch(base())
	{
		case KSimBaseInt::Binary:
			return sign + QString::fromLatin1("0b") + QString::number((unsigned int)abs(value()), 2);
			break;
		
		case KSimBaseInt::Octal:
			return sign + QString::fromLatin1("0o") + QString::number((unsigned int)abs(value()), 8);
			break;
		
		case KSimBaseInt::Decimal:
			return QString::number(value(), 10);
			break;
		
		case KSimBaseInt::Hexadecimal:
			return sign + QString::fromLatin1("0x") + QString::number((unsigned int)abs(value()), 16);
			break;

		default:
			KSIMDEBUG_VAR("Unknown base", base());
			return QString::fromLatin1("0");
	}
}

KSimBaseInt KSimBaseInt::convert(const QString & text, bool * ok)
{
	QString str(text.stripWhiteSpace());
	unsigned int myBase = 10;
	KSimBaseInt::eBaseType baseEnum = KSimBaseInt::Decimal;
	bool negative = false;
	unsigned int idx = 0;
	int v = 0;
	bool myOk = true;
	
	if (str.isEmpty())
	{
		myOk = false;
	}
	else
	{
		if (str[idx].latin1() == '-')
		{
			negative = true;
			idx++;
		}

		if (str[idx].latin1() == '0')
		{
			idx++;
		}
	
		switch(str[idx].latin1())
		{
			case 'b':
			case 'B':
				myBase = 2;
				baseEnum = KSimBaseInt::Binary;
				idx++;
				break;
	
			case 'o':
			case 'O':
				myBase = 8;
				baseEnum = KSimBaseInt::Octal;
				idx++;
				break;
	
			case 'x':
			case 'X':
				myBase = 16;
				baseEnum = KSimBaseInt::Hexadecimal;
				idx++;
				break;
	
			default:
				break;
		}
	
		if (str.length() > idx)
		{
			str = str.right(str.length()-idx); // leading chars

			v = (int)str.toUInt(&myOk, myBase);

			if (negative)
			{
				v = -v;
			}

			if (!myOk)
			{
				KSIMDEBUG_VAR("Conversion error", text);
			}
		}
	}
	
	if(ok)
	{
		*ok = myOk;
	}
	
	return KSimBaseInt(v,baseEnum);
}

bool KSimBaseInt::operator==(const KSimBaseInt & i) const
{
	return ((this->value() == i.value()) && (this->base() == i.base()));
};

bool KSimBaseInt::operator!=(const KSimBaseInt & i) const
{
	return ((this->value() != i.value()) || (this->base() != i.base()));
};



//#######################################################################################
//#######################################################################################


void KSimBaseIntNamed::setName(const QString & name)
{
	m_name = name.isEmpty() ? QString::null : name;
};


void KSimBaseIntNamed::setText(const QString & str, bool * ok)
{
	bool myOk;

	KSimBaseIntNamed i(convert(str, &myOk));

	if (myOk)
	{
		*this = i;
	}
	if (ok)
	{
		*ok = myOk;
	}
}

QString KSimBaseIntNamed::text() const
{
	if (name().isEmpty())
	{
		return ((const KSimBaseInt*)this)->text();
	}
	else
	{
		return QString::fromLatin1("%1 %2").arg(((const KSimBaseInt*)this)->text()).arg(name());
	}
}

bool KSimBaseIntNamed::operator==(const KSimBaseIntNamed & i) const
{
	return ((const KSimBaseInt)*this == (const KSimBaseInt)i) && (name() == i.name());
}

bool KSimBaseIntNamed::operator!=(const KSimBaseIntNamed & i) const
{
	return !(*this == i);
}

KSimBaseIntNamed KSimBaseIntNamed::convert(const QString & text, bool * ok)
{
	bool myOk;
	KSimBaseInt i;
	QString name(QString::null);
	int idx = text.find(' ');
	if (idx == -1)
	{
		i = KSimBaseInt::convert(text, &myOk);
	}
	else
	{
		i = KSimBaseInt::convert(text.left(idx), &myOk);

		if (myOk)
		{
			name = text.right(text.length()-idx);
		}
	}

	if (ok)
		*ok = myOk;

	return KSimBaseIntNamed(i, name);
}


