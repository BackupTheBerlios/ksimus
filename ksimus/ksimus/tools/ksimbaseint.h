/***************************************************************************
                          ksimbaseint.h  -  description
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

#ifndef KSIMBASEINT_H
#define KSIMBASEINT_H

// C-Includes

// QT-Includes
#include <qstring.h>

// KDE-Includes

// Project-Includes
#include "ksimtype.h"
#include "types.h"

// Forward declaration
class KSimBaseUInt;


//#######################################################################################
//#######################################################################################

/**This class contains a int variable which can be stored or displayed as binary, octal
  * decimal or hexadecimal format.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseInt : public KSimInt
{
public:

	// Defintion must be same as KSimBaseUInt::eBaseType !!!
	enum eBaseType { Binary      = 2,
	                 Octal       = 8,
	                 Decimal     = 10,
	                 Hexadecimal = 16
	};

	KSimBaseInt()
		:	KSimInt((int)0),
			m_base(KSimBaseInt::Decimal)
	{	};
	
	KSimBaseInt(int i)
		:	KSimInt(i),
			m_base(KSimBaseInt::Decimal)
	{	};
	
	KSimBaseInt(int i, eBaseType base)
		:	KSimInt(i),
			m_base(base)
	{	};
	
	KSimBaseInt(const KSimBaseInt & i)
		:	KSimInt(i.value()),
			m_base(i.base())
	{	};

	KSimBaseInt(const QString & text);
	KSimBaseInt(const KSimBaseUInt & newValue);
	
	const KSimBaseInt & operator=(int newValue) { m_value = newValue; return *this; };
	
	void setBase(eBaseType base) { m_base = base; };
	eBaseType base() const { return m_base; };

	virtual void setText(const QString & str, bool * ok = 0);
	virtual QString text() const;

	bool operator==(const KSimBaseInt & i) const;
	bool operator!=(const KSimBaseInt & i) const;

	static KSimBaseInt convert(const QString & text, bool * ok = 0);
	static const char * convertBase(eBaseType base);
	static eBaseType convertBase(const char * base, eBaseType defaultBase = Decimal);

private:
	eBaseType m_base;
};


//#######################################################################################
//#######################################################################################

/**This class contains a int variable which can be stored or displayed as binary, octal
  * decimal or hexadecimal format. Additional the class contains also a name.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseIntNamed : public KSimBaseInt
{
public:

	KSimBaseIntNamed()
		:	KSimBaseInt(),
			m_name(QString::null)
	{	};

	KSimBaseIntNamed(int i, eBaseType base = Decimal, const QString & name = QString::null)
		:	KSimBaseInt(i, base),
			m_name(name)
	{	};

	KSimBaseIntNamed(const KSimBaseInt & i, const QString & name = QString::null)
		:	KSimBaseInt(i),
			m_name(name)
	{	};

	void setName(const QString & name);
	QString name() const { return m_name; };

	virtual void setText(const QString & str, bool * ok = 0);
	virtual QString text() const;

	bool operator==(const KSimBaseIntNamed & i) const;
	bool operator!=(const KSimBaseIntNamed & i) const;

	static KSimBaseIntNamed convert(const QString & text, bool * ok = 0);

private:
	QString m_name;
};

#endif
