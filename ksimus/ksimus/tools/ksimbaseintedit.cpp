/***************************************************************************
                          ksimbaseintedit.cpp  -  description
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
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimbaseintedit.h"
#include "ksimdebug.h"

// Forward declaration


KSimBaseIntValidator::KSimBaseIntValidator(QWidget * parent, const char * name)
	:	QValidator(parent, name),
		m_bottom(INT_MIN),
		m_top(INT_MAX)
{
}

KSimBaseIntValidator::KSimBaseIntValidator(int bottom, int top,
                                           QWidget * parent, const char * name)
	:	QValidator(parent, name),
		m_bottom(bottom),
		m_top(top)
{
}


KSimBaseIntValidator::~KSimBaseIntValidator()
{
}

QValidator::State KSimBaseIntValidator::validate(QString & str, int & /*pos*/) const
{
	// Copied from QT and modifed
	
	QRegExp empty( QString::fromLatin1("^ *-?0?[bBoOxX]?$") );
	if ( empty.match( str ) >= 0 )
		return QValidator::Intermediate;
	bool ok;
	int tmp = KSimBaseInt::convert(str, &ok);
	if ( !ok )
		return QValidator::Invalid;
	else if ((tmp < bottom()) || (tmp > top()))
		return QValidator::Intermediate;
	else
		return QValidator::Acceptable;
}
	
void KSimBaseIntValidator::setBottom(int bottom)
{
	m_bottom = bottom;
}

void KSimBaseIntValidator::setTop(int top)
{
	m_top = top;
}

void KSimBaseIntValidator::setRange(int bottom, int top)
{
	setBottom(bottom);
	setTop(top);
}



//#######################################################################################
//#######################################################################################

class KSimBaseIntEdit::Private
{
public:
	Private()
	{
		flags.tracking = 0; // Default disable
	}

	KSimBaseInt trackedValue;
	KSimBaseInt lastValue;
	KSimBaseIntValidator * validator;
	struct
	{
		unsigned int tracking : 1;
	} flags;
};



KSimBaseIntEdit::KSimBaseIntEdit(QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(INT_MIN, INT_MAX);
	setValue(KSimBaseInt(0, KSimBaseInt::Decimal));
}

KSimBaseIntEdit::KSimBaseIntEdit(int bottom, int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(bottom, top);
	setValue(KSimBaseInt(0, KSimBaseInt::Decimal));
}

KSimBaseIntEdit::KSimBaseIntEdit(const KSimBaseInt & value, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(INT_MIN, INT_MAX);
	setValue(value);
}

KSimBaseIntEdit::KSimBaseIntEdit(const KSimBaseInt & value, int bottom, int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(bottom, top);
	setValue(value);
}


KSimBaseIntEdit::~KSimBaseIntEdit()
{
	delete m_p;
}


void KSimBaseIntEdit::init(int bottom, int top)
{
	m_p = new Private();
	CHECK_PTR(m_p);
	
	m_p->validator = new KSimBaseIntValidator(bottom, top, this);
	CHECK_PTR(m_p->validator);

	setAlignment(AlignRight);
	setValidator(m_p->validator);

	connect(this, SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
	connect(this, SIGNAL(changed()),SLOT(slotReady()));
	connect((KLineEdit*)this, SIGNAL(aboutToShowContextMenu(QPopupMenu *)),SLOT(slotAboutToShowContextMenu(QPopupMenu *)));
}


KSimBaseInt KSimBaseIntEdit::value() const
{
	return KSimBaseInt(text());
}

void KSimBaseIntEdit::setValue(const KSimBaseInt & newValue)
{
	setText(newValue.text());
	m_p->trackedValue = newValue;
	m_p->lastValue = newValue;
}

void KSimBaseIntEdit::setValue(int newValue)
{
	KSimBaseInt v(text());

	v = newValue;
	
	setValue(v);
}

void KSimBaseIntEdit::setBottom(int minimum)
{
	m_p->validator->setBottom(minimum);
	
	KSimBaseInt value(text());
	if (value < minimum)
	{
		value = minimum;
		setText(value.text());
	}
}

void KSimBaseIntEdit::setTop(int maximum)
{
	m_p->validator->setTop(maximum);
	
	KSimBaseInt value(text());
	if (value > maximum)
	{
		value = maximum;
		setText(value.text());
	}
}

void KSimBaseIntEdit::setRange(int bottom, int top)
{
	setBottom(bottom);
	setTop(top);
}

int KSimBaseIntEdit::bottom() const
{
	return m_p->validator->bottom();
}

int KSimBaseIntEdit::top() const
{
	return m_p->validator->top();
}

void KSimBaseIntEdit::setTrackingEnabled(bool tracking)
{
	m_p->flags.tracking = tracking;
}

bool KSimBaseIntEdit::isTrackingEnabled() const
{
	return m_p->flags.tracking;
}

void KSimBaseIntEdit::restoreValue()
{
//	KSIMDEBUG("restoreValue()")
	m_p->trackedValue = m_p->lastValue;
	setText(m_p->lastValue.text());
}


void KSimBaseIntEdit::slotTextChanged(const QString & text)
{
//	KSIMDEBUG("slotTextChanged()")
	if (isTrackingEnabled())
	{
		bool ok;
		KSimBaseInt i = KSimBaseInt::convert(text, &ok);
		if (ok && (i >= bottom()) && (i <= top()))
		{
			m_p->trackedValue = i;
			emit valueChangedTracking(i);
		}
	}
}

void KSimBaseIntEdit::slotReady()
{
	bool ok;
	QString tmp(text());
	int dummy;

//	KSIMDEBUG("slotReady()")

	if (QValidator::Acceptable != m_p->validator->validate(tmp, dummy))
	{
		bool ok;
		KSimBaseInt i = KSimBaseInt::convert(tmp, &ok);
		if (!ok || (((int)i) < bottom()) || (((int)i) > top()))
		{
			tmp = m_p->lastValue.text();
		}
	}

	setText(tmp);
	setCursorPosition(text().length());

	KSimBaseInt i = KSimBaseInt::convert(text(), &ok);
	if (ok && (((int)i) >= bottom()) && (((int)i) <= top()))
	{
		m_p->lastValue = i;
		m_p->trackedValue = i;
		emit valueChanged(i);
	}
	else
	{
		restoreValue();
	}
}


void KSimBaseIntEdit::slotAboutToShowContextMenu(QPopupMenu* popup)
{
	CHECK_PTR(popup);

	bool ok;
	int id;

	KSimBaseInt i = KSimBaseInt::convert(text(), &ok);

	QPopupMenu * convertPopup = new QPopupMenu(popup, "convertPopup");
	CHECK_PTR(convertPopup);
	
	id = convertPopup->insertItem(i18n("integer base", "Binary"), this, SLOT(slotConvertToBinary()));
	convertPopup->setItemEnabled(id, (i.base() != KSimBaseInt::Binary));

	id = convertPopup->insertItem(i18n("integer base", "Octal"), this, SLOT(slotConvertToOctal()));
	convertPopup->setItemEnabled(id, (i.base() != KSimBaseInt::Octal));

	id = convertPopup->insertItem(i18n("integer base", "Decimal"), this, SLOT(slotConvertToDecimal()));
	convertPopup->setItemEnabled(id, (i.base() != KSimBaseInt::Decimal));

	id = convertPopup->insertItem(i18n("integer base", "Hexadecimal"), this, SLOT(slotConvertToHexadecimal()));
	convertPopup->setItemEnabled(id, (i.base() != KSimBaseInt::Hexadecimal));

	
	popup->insertSeparator();
	id = popup->insertItem(i18n("integer base", "Convert into"), convertPopup);
	popup->setItemEnabled(id, ok);
}

void KSimBaseIntEdit::slotConvertToBinary()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Binary);
	setValue(i);
}

void KSimBaseIntEdit::slotConvertToOctal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Octal);
	setValue(i);
}

void KSimBaseIntEdit::slotConvertToDecimal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Decimal);
	setValue(i);
}

void KSimBaseIntEdit::slotConvertToHexadecimal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Hexadecimal);
	setValue(i);
}



const QString & KSimBaseIntEdit::getWhatsThisHelp()
{
	static const QString s(i18n("\nThe input line accepts binary, octal, decimal and hexadecimal values."
	                            "\nSee the following examples how to select the different bases:"
	                            "\nBinary: 0b11001"
	                            "\nOctal: 0o31"
	                            "\nDecimal: 25"
	                            "\nHexadecimal: 0x19" ));
	
	return s;
}


#include "ksimbaseintedit.moc"
