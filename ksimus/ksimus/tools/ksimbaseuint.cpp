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

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimbaseuint.h"
#include "ksimdebug.h"

// Forward declaration


//#######################################################################################
//#######################################################################################


void KSimBaseUInt::setText(const QString & str, bool * ok)
{
	m_value = convert(str, &m_base, ok);
};


QString KSimBaseUInt::text() const
{
	switch(base())
	{
		case Binary:
			return "0b" + QString::number(value(), 2);
			break;
		
		case Octal:
			return "0o" + QString::number(value(), 8);
			break;
		
		case Decimal:
			return QString::number(value());
			break;
		
		case Hexadecimal:
			return "0x" + QString::number(value(), 16);
			break;

		default:
			KSIMDEBUG_VAR("Unknown base", base());
			return QString("0");
	}
}

unsigned int KSimBaseUInt::convert(const QString & text, eKSimBaseTypes * base, bool * ok)
{
	QString str(text.stripWhiteSpace());
	int myBase = 10;
	eKSimBaseTypes baseEnum = Decimal;
	
	
	if (str[0] == '0')
	{
		str = str.right(str.length()-1); // Remove first
	}
	
	switch((QChar)str[0])
	{
		case 'b':
		case 'B':
			myBase = 2;
			baseEnum = Binary;
			str = str.right(str.length()-1); // Remove first
			break;
	
		case 'o':
		case 'O':
			myBase = 8;
			baseEnum = Octal;
			str = str.right(str.length()-1); // Remove first
			break;
	
		case 'x':
		case 'X':
			myBase = 16;
			baseEnum = Hexadecimal;
			str = str.right(str.length()-1); // Remove first
			break;
	
		default:
			break;
	}
	
	bool myOk;
	
	unsigned int v = str.toUInt(&myOk, myBase);
	
	if(ok)
	{
		*ok = myOk;
	}
	
	if (myOk)
	{
		if (base)
		{
			*base = baseEnum;
		}
	}
	else
	{
		KSIMDEBUG_VAR("Conversion error", text);
	}
	
	return v;
}
