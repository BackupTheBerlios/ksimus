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
#include <qvalidator.h>

// KDE-Includes

// Project-Includes
#include "ksimtype.h"
#include "ksimlineedit.h"

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
	
	KSimBaseUInt(unsigned int u, eKSimBaseTypes base)
		:	KSimUInt(u),
			m_base(base)
	{	};
	
	KSimBaseUInt(const KSimBaseUInt & u)
		:	KSimUInt(u.value()),
			m_base(u.base())
	{	};

	KSimBaseUInt(const QString & text);
	
	const KSimBaseUInt & operator=(unsigned int newValue) { m_value = newValue; return *this; };
	
	void setBase(eKSimBaseTypes base) { m_base = base; };
	eKSimBaseTypes base() const { return m_base; };

	virtual void setText(const QString & str, bool * ok = 0);
	virtual QString text() const;

	static unsigned int convert(const QString & text, eKSimBaseTypes * base = 0, bool * ok = 0);

private:
	eKSimBaseTypes m_base;
};

//#######################################################################################
//#######################################################################################

/** A validator for @ref KSimBaseUInt objects.
  *
  *@author Rasmus Diekenbrock
  */


class KSimBaseUIntValidator : public QValidator
{
public:
	/** Creates a validator with range 0 to UINT_MAX. */
	KSimBaseUIntValidator(QWidget * parent, const char * name = 0);
	/** Creates a validator with range bottom to top. */
	KSimBaseUIntValidator(unsigned int bottom, unsigned int top, QWidget * parent, const char * name = 0);
	/** The destructor. */
	~KSimBaseUIntValidator();

	/** Reimplemented for valdiate @ref KSimBaseUInt. */
	virtual State validate(QString & str, int & pos) const;
	
	/** Sets the minimum value. */
	void setBottom(unsigned int bottom);
	/** Sets the maximum value. */
	void setTop(unsigned int top);
	/** Sets the valid range. */
	void setRange(unsigned int bottom, unsigned int top);

	/** Returns the minimum value. */
	unsigned int bottom() const { return m_bottom; }
	/** Returns the maximum value. */
	unsigned int top() const { return m_top; }


private:
	unsigned int m_bottom, m_top;

};


//#######################################################################################
//#######################################################################################

/** A line edit for @ref KSimBaseUInt objects.
  *
  *@author Rasmus Diekenbrock
  */

class KSimBaseUIntLineEdit : public KSimLineEdit
{

	Q_OBJECT

public:
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is a empty string.
	  * The allowed range is from  0 to UINT_MAX. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntLineEdit(QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is a empty string.
	  * The allowed range is from bottom to top. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntLineEdit(unsigned int bottom, unsigned int top, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is given by value.
	  * The allowed range is from  0 to UINT_MAX. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntLineEdit(const KSimBaseUInt & value, QWidget * parent = 0, const char * name = 0);
	/** Constructs a @ref LineEdit widget for @ref KSimBaseUInt objects. The start value is given by value.
	  * The allowed range is from  bottom to top. The validator @ref KSimBaseUIntValidator is used. */
	KSimBaseUIntLineEdit(const KSimBaseUInt & value, unsigned int bottom, unsigned int top, QWidget * parent = 0, const char * name = 0);
	
	
	/** Returns the current value. */
	KSimBaseUInt value() const;

  /** Sets the minimum value. */
  void setBottom(unsigned int minimum);
  /** Sets the maximum value. */
  void setTop(unsigned int maximum);
	/** Sets the valid range. */
	void setRange(unsigned int bottom, unsigned int top);

  /** Returns the minimum value. */
  unsigned int bottom() const;
  /** Returns the maximum value. */
  unsigned int top() const;

	/** Returns a help text which explains the usage of this widget. Add this text to the whatsThis text. */
	static const QString & getWhatsThisHelp();

public slots:
	/** Sets a new value. */
	void setValue(const KSimBaseUInt & newValue);
	/** Sets a new value. Leave base unchanged. */
	void setValue(unsigned int newValue);


private:
	KSimBaseUIntValidator * m_validator;
};


#endif
