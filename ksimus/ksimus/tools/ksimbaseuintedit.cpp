/***************************************************************************
                          ksimbaseuintedit.cpp  -  description
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
#include <qpopupmenu.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimbaseuintedit.h"
#include "ksimdebug.h"

// Forward declaration




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

QValidator::State KSimBaseUIntValidator::validate(QString & str, int & /*pos*/) const
{
	// Copied from QT and modifed

	QRegExp empty( QString::fromLatin1("^ *0?[bBoOxX]?$") );
	if ( empty.exactMatch( str ) )  // TODO in kde2 source was empty.match(str). 
		return QValidator::Intermediate;
	bool ok;
	unsigned int tmp = (unsigned int)KSimBaseUInt::convert(str, &ok);
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

class KSimBaseUIntEdit::Private
{
public:
	Private()
	{
		flags.tracking = 0; // Default disable
	}


	KSimBaseUInt trackedValue;
	KSimBaseUInt lastValue;
	KSimBaseUIntValidator * validator;
	struct
	{
		unsigned int tracking : 1;
	} flags;
};



KSimBaseUIntEdit::KSimBaseUIntEdit(QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(0, UINT_MAX);
}

KSimBaseUIntEdit::KSimBaseUIntEdit(unsigned int bottom, unsigned int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(bottom, top);
}

KSimBaseUIntEdit::KSimBaseUIntEdit(const KSimBaseUInt & value, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(0, UINT_MAX);
	setValue(value);
}

KSimBaseUIntEdit::KSimBaseUIntEdit(const KSimBaseUInt & value, unsigned int bottom, unsigned int top, QWidget * parent, const char * name)
	:	KSimLineEdit(parent, name)
{
	init(bottom, top);
	setValue(value);
}

KSimBaseUIntEdit::~KSimBaseUIntEdit()
{
	delete m_p->validator;
	delete m_p;
}

void KSimBaseUIntEdit::init(unsigned int bottom, unsigned int top)
{
	m_p = new Private();
	Q_CHECK_PTR(m_p);

	m_p->validator = new KSimBaseUIntValidator(bottom, top, this);
	Q_CHECK_PTR(m_p->validator);

	setAlignment(AlignRight);
	setValidator(m_p->validator);

	connect(this, SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
	connect(this, SIGNAL(changed()),SLOT(slotReady()));
	connect((KLineEdit*)this, SIGNAL(aboutToShowContextMenu(QPopupMenu *)),SLOT(slotAboutToShowContextMenu(QPopupMenu *)));
}

KSimBaseUInt KSimBaseUIntEdit::value() const
{
	return KSimBaseUInt(text());
}

void KSimBaseUIntEdit::setValue(const KSimBaseUInt & newValue)
{
	setText(newValue.text());
	m_p->trackedValue = newValue;
	m_p->lastValue = newValue;
}

void KSimBaseUIntEdit::setValue(unsigned int newValue)
{
	KSimBaseUInt v(text());

	v = newValue;

	setValue(v);
}

void KSimBaseUIntEdit::setBottom(unsigned int minimum)
{
	m_p->validator->setBottom(minimum);

	KSimBaseUInt value(text());
	if (value < minimum)
	{
		value = minimum;
		setText(value.text());
	}
}

void KSimBaseUIntEdit::setTop(unsigned int maximum)
{
	m_p->validator->setTop(maximum);

	KSimBaseUInt value(text());
	if (value > maximum)
	{
		value = maximum;
		setText(value.text());
	}
}

void KSimBaseUIntEdit::setRange(unsigned int bottom, unsigned int top)
{
	setBottom(bottom);
	setTop(top);
}

unsigned int KSimBaseUIntEdit::bottom() const
{
	return m_p->validator->bottom();
}

unsigned int KSimBaseUIntEdit::top() const
{
	return m_p->validator->top();
}

void KSimBaseUIntEdit::setTrackingEnabled(bool tracking)
{
	m_p->flags.tracking = tracking;
}

bool KSimBaseUIntEdit::isTrackingEnabled() const
{
	return m_p->flags.tracking;
}

void KSimBaseUIntEdit::restoreValue()
{
//	KSIMDEBUG("restoreValue()")
	m_p->trackedValue = m_p->lastValue;
	setText(m_p->lastValue.text());
}


void KSimBaseUIntEdit::slotTextChanged(const QString & text)
{
//	KSIMDEBUG("slotTextChanged()")
	if (isTrackingEnabled())
	{
		bool ok;
		KSimBaseUInt u = KSimBaseUInt::convert(text, &ok);
		if (ok && (u >= bottom()) && (u <= top()))
		{
			m_p->trackedValue = u;
			emit valueChangedTracking(u);
		}
	}
}

void KSimBaseUIntEdit::slotReady()
{
	bool ok;
	QString tmp(text());
	int dummy;

//	KSIMDEBUG("slotReady()")

	if (QValidator::Acceptable != m_p->validator->validate(tmp, dummy))
	{
		bool ok;
		KSimBaseUInt u = KSimBaseUInt::convert(tmp, &ok);
		if (!ok || (((unsigned int)u) < bottom()) || (((unsigned int)u) > top()))
		{
			tmp = m_p->lastValue.text();
		}
	}

	setText(tmp);
	setCursorPosition(text().length());

	KSimBaseUInt u = KSimBaseUInt::convert(text(), &ok);
	if (ok && (((unsigned int)u) >= bottom()) && (((unsigned int)u) <= top()))
	{
		m_p->lastValue = u;
		m_p->trackedValue = u;
		emit valueChanged(u);
	}
	else
	{
		restoreValue();
	}
}




void KSimBaseUIntEdit::slotAboutToShowContextMenu(QPopupMenu* popup)
{
	Q_CHECK_PTR(popup);

	bool ok;
	int id;

	KSimBaseUInt u = KSimBaseUInt::convert(text(), &ok);

	QPopupMenu * convertPopup = new QPopupMenu(popup, "convertPopup");
	Q_CHECK_PTR(convertPopup);

	id = convertPopup->insertItem(i18n("integer base", "Binary"), this, SLOT(slotConvertToBinary()));
	convertPopup->setItemEnabled(id, (u.base() != KSimBaseUInt::Binary));

	id = convertPopup->insertItem(i18n("integer base", "Octal"), this, SLOT(slotConvertToOctal()));
	convertPopup->setItemEnabled(id, (u.base() != KSimBaseUInt::Octal));

	id = convertPopup->insertItem(i18n("integer base", "Decimal"), this, SLOT(slotConvertToDecimal()));
	convertPopup->setItemEnabled(id, (u.base() != KSimBaseUInt::Decimal));

	id = convertPopup->insertItem(i18n("integer base", "Hexadecimal"), this, SLOT(slotConvertToHexadecimal()));
	convertPopup->setItemEnabled(id, (u.base() != KSimBaseUInt::Hexadecimal));


	popup->insertSeparator();
	id = popup->insertItem(i18n("integer base", "Convert into"), convertPopup);
	popup->setItemEnabled(id, ok);
}

void KSimBaseUIntEdit::slotConvertToBinary()
{
	KSimBaseUInt u(value());
	u.setBase(KSimBaseUInt::Binary);
	setValue(u);
}

void KSimBaseUIntEdit::slotConvertToOctal()
{
	KSimBaseUInt u(value());
	u.setBase(KSimBaseUInt::Octal);
	setValue(u);
}

void KSimBaseUIntEdit::slotConvertToDecimal()
{
	KSimBaseUInt u(value());
	u.setBase(KSimBaseUInt::Decimal);
	setValue(u);
}

void KSimBaseUIntEdit::slotConvertToHexadecimal()
{
	KSimBaseUInt u(value());
	u.setBase(KSimBaseUInt::Hexadecimal);
	setValue(u);
}


const QString & KSimBaseUIntEdit::getWhatsThisHelp()
{
	static const QString s(i18n("\nThe input line accepts binary, octal, decimal and hexadecimal values."
	                            "\nSee the following examples how to select the different bases:"
	                            "\nBinary: 0b11001"
	                            "\nOctal: 0o31"
	                            "\nDecimal: 25"
	                            "\nHexadecimal: 0x19" ));

	return s;
}

#include "ksimbaseuintedit.moc"
