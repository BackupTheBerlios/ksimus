/***************************************************************************
                          ksimdoubleedit.cpp  -  description
                             -------------------
    begin                : Sun Nov 26 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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
#include <math.h>

// QT-Includes
#include <qvalidator.h>

// KDE-Includes

// Project-Includes
#include "ksimdoubleedit.h"
#include "ksimdebug.h"

// Forward declaration

//###############################################################
//###############################################################


class KSimDoubleEditValidator : public QDoubleValidator
{

public:
	KSimDoubleEditValidator(KSimDoubleEdit * parent, const char *name);
	KSimDoubleEditValidator(double bottom, double top, int decimals,
	                        KSimDoubleEdit * parent, const char *name );
	
	
	virtual void fixup(QString & input) const;
};

KSimDoubleEditValidator::KSimDoubleEditValidator(KSimDoubleEdit * parent, const char *name)
	:	QDoubleValidator(-HUGE_VAL, HUGE_VAL, 10, parent, name)
{};

KSimDoubleEditValidator::KSimDoubleEditValidator(double bottom, double top, int decimals,
                                                 KSimDoubleEdit * parent, const char *name )
	:	QDoubleValidator(bottom, top, decimals, parent, name)
{};

void KSimDoubleEditValidator::fixup(QString & input) const
{
//	KSIMDEBUG("fixup()");
	bool ok;
	
	double d = input.toDouble(&ok);
	if (!ok || (d < bottom()) || (d > top()))
	{
		d = ((KSimDoubleEdit *)parent())->m_lastValue;
	}
	
	input = QString::number(d,((KSimDoubleEdit *)parent())->getConversionType(), decimals());

}


//###############################################################
//###############################################################


#define FLAGS_TYPE_MASK				0x00FF
#define FLAGS_ENA_TRACKING		0x0100


#define DEFAULT_FLAGS		(FLAGS_ENA_TRACKING | ((Q_UINT32)'g') )


//###############################################################
//###############################################################


KSimDoubleEdit::KSimDoubleEdit(QWidget *parent, const char *name)
	:	KSimLineEdit(parent,name),
		m_dVal(new KSimDoubleEditValidator(this,name)),
		m_lastValue(-1),
		m_trackedValue(-1),
		m_flags(DEFAULT_FLAGS)
{
	setAlignment(AlignRight);
	setValidator(m_dVal);
	setValue(0);
	connect(this, SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
	connect(this, SIGNAL(changed()),SLOT(slotReady()));
}

KSimDoubleEdit::KSimDoubleEdit(double bottom, double top, int decimals,
                               QWidget *parent, const char *name)
	:	KSimLineEdit(parent,name),
		m_dVal(new KSimDoubleEditValidator(bottom,top,decimals,this,name)),
		m_lastValue(-1),
		m_trackedValue(-1),
		m_flags(DEFAULT_FLAGS)
{
	setAlignment(AlignRight);
	setValidator(m_dVal);
	setValue(0);
	connect(this, SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
	connect(this, SIGNAL(changed()),SLOT(slotReady()));
}

KSimDoubleEdit::~KSimDoubleEdit()
{
}

void KSimDoubleEdit::setRange(double bottom, double top, int decimals)
{
	m_dVal->setRange(bottom, top, decimals);
}

double KSimDoubleEdit::bottom() const
{
	return m_dVal->bottom();
}

double KSimDoubleEdit::top() const
{
	return m_dVal->top();
}

int KSimDoubleEdit::decimals() const
{
	return m_dVal->decimals();
}

void KSimDoubleEdit::setBottom(double bottom)
{
	m_dVal->setBottom(bottom);
}

void KSimDoubleEdit::setTop(double top)
{
	m_dVal->setTop(top);
}

void KSimDoubleEdit::setDecimals(int decimals)
{
	m_dVal->setDecimals(decimals);
}

void KSimDoubleEdit::setValue(double value)
{
//	KSIMDEBUG("setValue()")
//	if (m_trackedValue != value)
	{
		m_lastValue = value;
		m_trackedValue = value;
		setText(QString::number(value,getConversionType(), decimals()));
	}
}

void KSimDoubleEdit::setTrackingEnabled(bool tracking)
{
	if (tracking)
	{
		m_flags |= FLAGS_ENA_TRACKING;
	}
	else
	{
		m_flags &= ~FLAGS_ENA_TRACKING;
	}
}

bool KSimDoubleEdit::isTrackingEnabled() const
{
	return m_flags & FLAGS_ENA_TRACKING;
}

void KSimDoubleEdit::setConversionType(char type)
{
	switch(type)
	{
		case 'e':
		case 'f':
		case 'g':
		case 'E':
		case 'F':
		case 'G':
			m_flags = (m_flags & ~FLAGS_TYPE_MASK) | type;
			break;
			
		default:
			m_flags = (m_flags & ~FLAGS_TYPE_MASK) | 'g';
			KSIMDEBUG_VAR("Unknown ",type);
			break;
	}
}

char KSimDoubleEdit::getConversionType() const
{
	return m_flags & FLAGS_TYPE_MASK;
}


void KSimDoubleEdit::restoreValue()
{
//	KSIMDEBUG("restoreValue()")
	m_trackedValue = m_lastValue;
	setText(QString::number(m_lastValue, getConversionType(), decimals()));
}

double KSimDoubleEdit::value()
{
	if (isTrackingEnabled())
		return m_trackedValue;
	else
		return m_lastValue;
}

void KSimDoubleEdit::slotTextChanged(const QString & text)
{
	bool ok;
	
//	KSIMDEBUG("slotTextChanged()")
	if (isTrackingEnabled())
	{
		double d = text.toDouble(&ok);
		if (ok && (d >= bottom()) && (d <= top()))
		{
			m_trackedValue = d;
			emit valueChangedTracking(d);
		}
	}
}

void KSimDoubleEdit::slotReady()
{
	bool ok;
	QString tmp(text());
	int dummy;
	
//	KSIMDEBUG("slotReady()")
	
	if (QValidator::Acceptable != m_dVal->validate(tmp, dummy))
	{
		m_dVal->fixup(tmp);
	}
	
	setText(tmp);
	setCursorPosition(text().length());
	
	double d = text().toDouble(&ok);
	if (ok && (d >= bottom()) && (d <= top()))
	{
		m_lastValue = d;
		m_trackedValue = d;
		emit valueChanged(d);
	}
	else
	{
		restoreValue();
	}
}
