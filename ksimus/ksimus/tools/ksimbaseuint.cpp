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
#include <klocale.h>

// Project-Includes
#include "ksimbaseuint.h"
#include "ksimdebug.h"

// Forward declaration


//#######################################################################################
//#######################################################################################


KSimBaseUInt::KSimBaseUInt(const QString & text)
	:	KSimUInt(0),
		m_base(Decimal)
{	
	setText(text);
};


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



//#######################################################################################
//#######################################################################################



KSimBaseUIntValidator::KSimBaseUIntValidator(QWidget * parent, const char * name)
	:	QValidator(parent, name),
		m_bottom(0),
		m_top(UINT_MAX)
{
}

KSimBaseUIntValidator::KSimBaseUIntValidator(unsigned int bottom, unsigned int top,
                                                     QWidget * parent, const char * name)
	:	QValidator(parent, name),
		m_bottom(bottom),
		m_top(top)
{
}


KSimBaseUIntValidator::~KSimBaseUIntValidator()
{
}

QValidator::State KSimBaseUIntValidator::validate(QString & str, int & pos) const
{
	// Copied from QT and modifed
	
	QRegExp empty( QString::fromLatin1("^ *0?[bBoOxX]?$") );
	if ( empty.match( str ) >= 0 )
		return QValidator::Intermediate;
	bool ok;
	unsigned int tmp = KSimBaseUInt::convert(str, (KSimBaseUInt::eKSimBaseTypes *) 0, &ok);
	if ( !ok )
		return QValidator::Invalid;
	else if ((tmp < bottom()) || (tmp > top()))
		return QValidator::Intermediate;
	else
		return QValidator::Acceptable;
}
	
void KSimBaseUIntValidator::setBottom(unsigned int bottom)
{
	m_bottom = bottom;
}

void KSimBaseUIntValidator::setTop(unsigned int top)
{
	m_top = top;
}

void KSimBaseUIntValidator::setRange(unsigned int bottom, unsigned int top)
{
	setBottom(bottom);
	setTop(top);
}



//#######################################################################################
//#######################################################################################

KSimBaseUIntLineEdit::KSimBaseUIntLineEdit(QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	m_validator = new KSimBaseUIntValidator(this);
	CHECK_PTR(m_validator);
	
	setValidator(m_validator);
}

KSimBaseUIntLineEdit::KSimBaseUIntLineEdit(unsigned int bottom, unsigned int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	m_validator = new KSimBaseUIntValidator(bottom, top, this);
	CHECK_PTR(m_validator);
	
	setValidator(m_validator);
}

KSimBaseUIntLineEdit::KSimBaseUIntLineEdit(const KSimBaseUInt & value, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	m_validator = new KSimBaseUIntValidator(this);
	CHECK_PTR(m_validator);
	
	setValidator(m_validator);
	
	setValue(value);
}

KSimBaseUIntLineEdit::KSimBaseUIntLineEdit(const KSimBaseUInt & value, unsigned int bottom, unsigned int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	m_validator = new KSimBaseUIntValidator(bottom, top, this);
	CHECK_PTR(m_validator);
	
	setValidator(m_validator);
	
	setValue(value);
}


KSimBaseUInt KSimBaseUIntLineEdit::value() const
{
	return KSimBaseUInt(text());	
}

void KSimBaseUIntLineEdit::setValue(const KSimBaseUInt & newValue)
{
	setText(newValue.text());
}

void KSimBaseUIntLineEdit::setValue(unsigned int newValue)
{
	KSimBaseUInt v(text());

	v = newValue;
	
	setValue(v);
}

void KSimBaseUIntLineEdit::setBottom(unsigned int minimum)
{
	m_validator->setBottom(minimum);
	
	KSimBaseUInt value(text());
	if (value < minimum)
	{
		value = minimum;
		setText(value.text());
	}
}

void KSimBaseUIntLineEdit::setTop(unsigned int maximum)
{
	m_validator->setTop(maximum);
	
	KSimBaseUInt value(text());
	if (value > maximum)
	{
		value = maximum;
		setText(value.text());
	}
}

void KSimBaseUIntLineEdit::setRange(unsigned int bottom, unsigned int top)
{
	setBottom(bottom);
	setTop(top);
}

unsigned int KSimBaseUIntLineEdit::bottom() const
{
	return m_validator->bottom();
}

unsigned int KSimBaseUIntLineEdit::top() const
{
	return m_validator->top();
}

const QString & KSimBaseUIntLineEdit::getWhatsThisHelp()
{
	static const QString s(i18n("\nThe input line accepts binary, octal, decimal and hexadecimal values."
	                            "\nSee the following examples how to select the different bases:"
	                            "\nBinary: 0b11001"
	                            "\nOctal: 0o31"
	                            "\nDecimal: 25"
	                            "\nHexadecimal: 0x19" ));
	
	return s;
}

