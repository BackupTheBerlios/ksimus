/***************************************************************************
                          ksimbaseintspinbox.cpp  -  description
                             -------------------
    begin                : Thu Jul 31 2003
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

// QT-Includes
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qintdict.h>
#include <qcursor.h>
#include <qvaluelist.h>

// KDE-Includes
#include <klocale.h>

// KSimus-Includes

// Project-Includes
#include "ksimdebug.h"
#include "ksimbaseintspinbox.h"

// Forward declaration

//#############################################################
//#############################################################

class KSimBaseIntSpinBox::Private
{
public:

	class Constant
	{
		public:
		Constant() {};
		Constant(const KSimBaseInt & value, const QString & name)
		:	m_value(value),
			m_name(name)
		{};

		const KSimBaseInt & value() const { return m_value; };

		void setName(const QString & name) { m_name = name; };
		const QString & name() const { return m_name; };

		void setIndex(int idx) { m_idx = idx; };
		int getIndex() const { return m_idx; };

		KSimBaseInt m_value;
		QString m_name;
		int m_idx;
	};

	class ConstantList : public QValueList<Constant>
	{};

	Private(const KSimBaseInt & value, int min, int max, int steps)
		:	m_value(value),
			m_min(min),
			m_max(max),
			m_tracking(false)
	{
	};
	
	const KSimBaseInt & value() const { return m_value; };
	int min()                   const { return m_min; };
	int max()                   const { return m_max; };
	bool tracking()             const { return m_tracking; };
	ConstantList & constantList()     { return m_constantList; };

	void setValue(const KSimBaseInt & value) { m_value = value; };
	void setMin(int min)                     { m_min = min; };
	void setMax(int max)                     { m_max = max; };
	void setTracking(bool ena)               { m_tracking = ena; };

	
	KSimBaseInt m_value;
	int m_min;
	int m_max;
	bool m_tracking;
	ConstantList m_constantList;

};

//#############################################################
//#############################################################

// Some statics

/*static const int KSimBaseIntSpinBox_QSPINBOX_UPPER_LIMIT =  2;
static const int KSimBaseIntSpinBox_QSPINBOX_TOGGLE_1    =  1;
static const int KSimBaseIntSpinBox_QSPINBOX_TOGGLE_0    =  0;
static const int KSimBaseIntSpinBox_QSPINBOX_LOWER_LIMIT = -1;*/

#define KSimBaseIntSpinBox_QSPINBOX_UPPER_LIMIT  2
#define KSimBaseIntSpinBox_QSPINBOX_TOGGLE_1     1
#define KSimBaseIntSpinBox_QSPINBOX_TOGGLE_0     0
#define KSimBaseIntSpinBox_QSPINBOX_LOWER_LIMIT -1


//#############################################################
//#############################################################

KSimBaseIntSpinBox::KSimBaseIntSpinBox(QWidget * parent, const char * name)
	: QSpinBox(parent, name)
{
	m_p = new Private(0, INT_MIN, INT_MAX, 1);
	CHECK_PTR(m_p);
	init();
}

KSimBaseIntSpinBox::KSimBaseIntSpinBox(const KSimBaseInt & value, int min, int max,
                                       int steps, QWidget * parent, const char * name)
	: QSpinBox(parent, name)
{
	m_p = new Private(value, min, max, steps);
	CHECK_PTR(m_p);
	init();
}

void KSimBaseIntSpinBox::init()
{
	setValidator(0);
	connect(this,SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
	connect(editor(), SIGNAL(textChanged(const QString &)),SLOT(slotTextChanged(const QString &)));
	QSpinBox::setMinValue(KSimBaseIntSpinBox_QSPINBOX_LOWER_LIMIT);
	QSpinBox::setMaxValue(KSimBaseIntSpinBox_QSPINBOX_UPPER_LIMIT);
	setFocusPolicy(QWidget::WheelFocus);
	setAlignment(AlignRight);
}


KSimBaseIntSpinBox::~KSimBaseIntSpinBox()
{
	delete m_p;
}
	
const KSimBaseInt & KSimBaseIntSpinBox::value() const
{
	return m_p->value();
}

void KSimBaseIntSpinBox::setMinValue(int value)
{
	m_p->setMin(value);
}

int KSimBaseIntSpinBox::minValue() const
{
	return m_p->min();
}

void KSimBaseIntSpinBox::setMaxValue(int value)
{
	m_p->setMax(value);
}

int KSimBaseIntSpinBox::maxValue() const
{
	return m_p->max();
}

void KSimBaseIntSpinBox::setValue(const KSimBaseInt & newValue)
{
	KSimBaseInt myNew(newValue);
//	KSIMDEBUG(QString::fromLatin1(">>> KSimBaseIntSpinBox::setValue() this=%1 newValue=%2").arg((unsigned int)this,0, 16).arg(newValue.text()));
	if (myNew != value())
	{
		int res = setValueInternal(myNew);
		QSpinBox::setValue(res);
	}
	else
	{
//		KSIMDEBUG("skipped");
	}
//	KSIMDEBUG("<<< KSimBaseIntSpinBox::setValue()");
}

int KSimBaseIntSpinBox::setValueInternal(const KSimBaseInt & newValue)
{
//	KSIMDEBUG(QString::fromLatin1(">>> KSimBaseIntSpinBox::setValueInternal() this=%1 newValue=%2").arg((unsigned int)this,0, 16).arg(newValue.text()));
	bool toggle = true;
	int res = QSpinBox::value();
	int val = newValue.value();
	if (newValue != value())
	{
//		KSIMDEBUG_VAR("KSimBaseIntSpinBox::setValueInternal()", val);
	
		// Value changed
		if (wrapping())
		{
			if (val > maxValue())
			{
				val = minValue();
			}
			else if (val < minValue())
			{
				val = maxValue();
			}

			if (val >= maxValue())
			{
				res = KSimBaseIntSpinBox_QSPINBOX_UPPER_LIMIT;
				toggle = false;
			}
			else if (val <= minValue())
			{
				res = KSimBaseIntSpinBox_QSPINBOX_LOWER_LIMIT;
				toggle = false;
			}
		}
		else		// (wrapping())
		{
			if (val >= maxValue())
			{
//				KSIMDEBUG("newValue >= maxValue()");
				val = maxValue();
				res = KSimBaseIntSpinBox_QSPINBOX_UPPER_LIMIT;
				toggle = false;
			}
			else if (val <= minValue())
			{
//				KSIMDEBUG("newValue <= minValue()");
				val = minValue();
				res = KSimBaseIntSpinBox_QSPINBOX_LOWER_LIMIT;
				toggle = false;
			}
		}
		
		if(toggle)
		{
//			KSIMDEBUG("toggle");
			res = (res == KSimBaseIntSpinBox_QSPINBOX_TOGGLE_0)
			      ? KSimBaseIntSpinBox_QSPINBOX_TOGGLE_1
			      : KSimBaseIntSpinBox_QSPINBOX_TOGGLE_0;
		}

		KSimBaseInt i(val, newValue.base());
		
//		KSIMDEBUG_VAR("KSimBaseIntSpinBox::setValueInternal", i.text());
		m_p->setValue(i);
	}
//	KSIMDEBUG("<<< KSimBaseIntSpinBox::setValueInternal()");
	return res;
}

void KSimBaseIntSpinBox::stepUp()
{
	KSimBaseInt newValue(value());
	int res;

	newValue = newValue.value() + lineStep(); // Base is not changed!


	res = setValueInternal(newValue);
	QSpinBox::setValue(res);
}

void KSimBaseIntSpinBox::stepDown()
{
	KSimBaseInt newValue(value());
	int res;
	newValue = newValue.value() - lineStep(); // Base is not changed!

	res = setValueInternal(newValue);
	QSpinBox::setValue(res);
}

QString KSimBaseIntSpinBox::mapValueToText(int)
{
	return value().text();
}

int KSimBaseIntSpinBox::mapTextToValue(bool * ok)
{
	bool myOk;
	KSimBaseInt i = KSimBaseInt::convert(cleanText(), &myOk);

	if (!myOk)
	{
		i = value();
	}

	if (ok)
	{
		*ok = myOk;
	}

	return setValueInternal(i);
}
	

void KSimBaseIntSpinBox::slotValueChanged(int /*val*/)
{
//	KSIMDEBUG("KSimBaseIntSpinBox::slotValueChanged()")
	emit valueChanged(value());
}


void KSimBaseIntSpinBox::slotTextChanged(const QString & /*text*/)
{
//	KSIMDEBUG("KSimBaseIntSpinBox::slotTextChanged()")
	if (isTrackingEnabled())
	{
		bool ok;
		KSimBaseInt i = KSimBaseInt::convert(cleanText(), &ok);
		if (ok && (i >= minValue()) && (i <= maxValue()))
		{
			emit valueChangedTracking(i);
		}
	}
}


void KSimBaseIntSpinBox::setAlignment(int flag)
{
	editor()->setAlignment(flag);
}
	
int KSimBaseIntSpinBox::alignment() const
{
	return editor()->alignment();
}

void KSimBaseIntSpinBox::setTrackingEnabled(bool tracking)
{
	m_p->setTracking(tracking);
}

bool KSimBaseIntSpinBox::isTrackingEnabled() const
{
	return m_p->tracking();
}


void KSimBaseIntSpinBox::appendConstant(const KSimBaseInt & value, const QString & name)
{
	m_p->constantList().append(Private::Constant(value, name));
}

void KSimBaseIntSpinBox::clearConstantList()
{
	m_p->constantList().clear();
}

void KSimBaseIntSpinBox::appendMinConstant(const QString & name)
{
	appendConstant(KSimBaseInt(minValue()), name.isEmpty() ? i18n("integer base", "Minimum") : name);
}

void KSimBaseIntSpinBox::appendMaxConstant(const QString & name)
{
	appendConstant(KSimBaseInt(maxValue()), name.isEmpty() ? i18n("integer base", "Maximum") : name);
}

bool KSimBaseIntSpinBox::eventFilter(QObject * obj, QEvent * ev)
{
	switch(ev->type())
	{
		case QEvent::MouseButtonPress:
		{
			if (((QMouseEvent*)ev)->button() == QMouseEvent::RightButton)
			{
				QPopupMenu * popup = new QPopupMenu(0,"KSimBaseIntSpinBox RMB Popup");
				CHECK_PTR(popup);
		
				initRmbMenu(popup);
				if (popup->count() != 0)
				{
					int id = popup->exec(QCursor::pos());
					if (id != -1)
					{
						execRmbMenu(id);
					}
				}
				delete popup;
				return true;
			}
			break;
		}
		
		case QEvent::FocusOut:
		{
			emit undoRequest();
			break;
		}
		
		default:
			break;
	}
	return QSpinBox::eventFilter(obj,ev);
}

void KSimBaseIntSpinBox::initRmbMenu(QPopupMenu * popup)
{
	CHECK_PTR(popup);

	bool ok;
	int id;

	KSimBaseInt i = KSimBaseInt::convert(cleanText(), &ok);

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


//	popup->insertSeparator();
	id = popup->insertItem(i18n("integer base", "Convert into"), convertPopup);
	popup->setItemEnabled(id, ok);



	if (m_p->constantList().count())
	{
		QPopupMenu * constantPopup = new QPopupMenu(popup, "constantPopup");
		CHECK_PTR(constantPopup);

		Private::ConstantList::Iterator it(m_p->constantList().begin());

		for(; it != m_p->constantList().end(); ++it)
		{
			if ((*it).name().isEmpty())
			{
				(*it).setName((*it).value().text());
			}
			id = constantPopup->insertItem((*it).name());
			(*it).setIndex(id);
		}
		popup->insertSeparator();
		popup->insertItem(i18n("integer base", "Constants"), constantPopup);
	}
}

void KSimBaseIntSpinBox::execRmbMenu(int id)
{
	Private::ConstantList::ConstIterator it(m_p->constantList().begin());

	for(; it != m_p->constantList().end(); ++it)
	{
		if ((*it).getIndex() == id)
		{
			setValue((*it).value());
		}
	}
}

void KSimBaseIntSpinBox::slotConvertToBinary()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Binary);
	setValue(i);
}

void KSimBaseIntSpinBox::slotConvertToOctal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Octal);
	setValue(i);
}

void KSimBaseIntSpinBox::slotConvertToDecimal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Decimal);
	setValue(i);
}

void KSimBaseIntSpinBox::slotConvertToHexadecimal()
{
	KSimBaseInt i(value());
	i.setBase(KSimBaseInt::Hexadecimal);
	setValue(i);
}

