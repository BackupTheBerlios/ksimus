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

#include <qvalidator.h>
#include "ksimdoubleedit.h"

KSimDoubleEdit::KSimDoubleEdit(double bottom, double top, int decimals,
								QWidget *parent, const char *name)
	:	QLineEdit(parent,name),
		m_dVal(new QDoubleValidator(bottom,top,decimals,this,name)),
		m_lastValue(-1)
{
	setValidator(m_dVal);
	setValue(0);
	connect(this, SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
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

void KSimDoubleEdit::setValue(double value)
{
	if (m_lastValue != value)
	{
		m_lastValue = value;
		QString s;
		s.setNum(value,'g',decimals());
		setText(s);
	}
}

void KSimDoubleEdit::restoreValue()
{
	setValue(m_lastValue);
}

double KSimDoubleEdit::value()
{
	bool ok;
	
	double d = text().toDouble(&ok);
	if(ok)
	{
		m_lastValue = d;
	}
	else
	{
		d = m_lastValue;
		restoreValue();
	}
	return d;
}

void KSimDoubleEdit::slotTextChanged(const QString & text)
{
	bool ok;
	
	double d = text.toDouble(&ok);
	if(ok)
	{
		m_lastValue = d;
		emit valueChanged(d);
	}
}
