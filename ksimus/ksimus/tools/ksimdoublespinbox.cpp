/***************************************************************************
                          ksimdoublespinbox.cpp  -  description
                             -------------------
    begin                : Mon Aug 20 2001
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
#include <math.h>
#include <stdlib.h>

// QT-Includes
#include <qlineedit.h>
#include <qpopupmenu.h>
#include <qintdict.h>

// KDE-Includes
#include <kdebug.h>
#include <klocale.h>

// KSimus-Includes
//#include <ksimus/ksimdebug.h>

// Project-Includes
#include "ksimdoublespinbox.h"
#include "ksimexponentstep.h"

// Forward declaration

#ifndef KSIMDEBUG
#ifdef DEBUG
#define __DEBUG_INFO__        __FILE__ << "(" << __LINE__ << ") "
#define KSIMDEBUG(str) 	 	 {kdDebug() << __DEBUG_INFO__ << str << endl;}
#define KSIMDEBUG_VAR(str,x) {kdDebug() << __DEBUG_INFO__ << str << " " << #x << " = " << x << endl;}
#define KSIMDEBUG_FUNCTION   {kdDebug() << k_funcinfo << endl;}
#else
#define KSIMDEBUG(str) 	
#define KSIMDEBUG_VAR(str,x)
#define KSIMDEBUG_FUNCTION
#endif
#endif

//#############################################################
//#############################################################

class KSimDoubleSpinBoxPrivate
{
	friend class KSimDoubleSpinBox;
	
	KSimDoubleSpinBoxPrivate(double value, double min, double max, double steps)
		:	m_value(value),
			m_min(min),
			m_max(max),
			m_step(steps),
			m_expEna(false),
			m_expSteps(0),
			m_fieldwidth(0),
			m_format('g'),
			m_precision(-1),
			m_updateRequired(true),
			m_extraZeroAllowed(false),
			m_lastButtonState(0),
			m_multiSteps(5)
	{
	};
	
	~KSimDoubleSpinBoxPrivate()
	{
		if (m_expSteps)
			delete m_expSteps;
	};
		
	double value() const { return m_value; };
	double min()   const { return m_min; };
	double max()   const { return m_max; };
	double step()  const { return m_step; };
	bool expStepsEnabled() const { return m_expEna; };
	int fieldwidth() const { return m_fieldwidth; };
	char format()    const { return m_format; };
	int precision()  const { return m_precision; };
  bool updateRequired() const { return m_updateRequired; };
  bool extraZeroAllowed() const { return m_extraZeroAllowed; };
  int lastButtonState() const { return m_lastButtonState; };
	int multiSteps() const { return m_multiSteps; }
		
	void setValue(double value) { m_value = value; };
	void setMin(double min)     { m_min = min; };
	void setMax(double max)     { m_max = max; };
	void setStep(double step)   { m_step = step; };
	void setExpStepsEnabled(bool ena) { m_expEna = ena; };
	void setFieldwidth(int fieldwidth) { m_fieldwidth = fieldwidth; };
	void setFormat(char format) { m_format = format; };
	void setPrecision(int precision) { m_precision = precision; };
	void setUpdateRequired(bool update) { m_updateRequired = update; };
	void setExtraZeroAllowed(bool allowed) { m_extraZeroAllowed = allowed; };
	void setLastButtonState(int state) { m_lastButtonState = state; };
	void setMultiSteps(int steps) { m_multiSteps = steps; };
	
	
	KSimExponentStep * expSteps()	{ return m_expSteps; };
	bool useExpSteps() const  { return (m_expEna && m_expSteps && (m_expSteps->countSteps() != 0)); };
	
	double m_value;
	double m_min;
	double m_max;
	double m_step;
	bool m_expEna;
	KSimExponentStep * m_expSteps;
	int m_fieldwidth;
	char m_format;
	int m_precision;
	bool m_updateRequired;
	bool m_extraZeroAllowed;
	int m_lastButtonState;
	int m_multiSteps;
	
	
	static int rmbExponentStepID;
};

int KSimDoubleSpinBoxPrivate::rmbExponentStepID = -1;

#define QSPINBOX_UPPER_LIMIT	2
#define QSPINBOX_TOGGLE_1			1
#define QSPINBOX_TOGGLE_0			0
#define QSPINBOX_LOWER_LIMIT	-1





KSimDoubleSpinBox::KSimDoubleSpinBox(QWidget * parent, const char * name)
	: QSpinBox(parent, name),
		m_p(new KSimDoubleSpinBoxPrivate(0.0, -10.0, 10.0, 0.1))
{
	init();
}

KSimDoubleSpinBox::KSimDoubleSpinBox(double value, double min, double max, double steps,
                                     QWidget * parent, const char * name)
	: QSpinBox(parent, name),
		m_p(new KSimDoubleSpinBoxPrivate(value, min, max, steps))
{
	init();
}

void KSimDoubleSpinBox::init()
{
	CHECK_PTR(m_p);
	setValidator(0);
  connect(this,SIGNAL(valueChanged(int)), this, SLOT(slotValueChanged(int)));
  QSpinBox::setMinValue(QSPINBOX_LOWER_LIMIT);
  QSpinBox::setMaxValue(QSPINBOX_UPPER_LIMIT);
  setFocusPolicy(QWidget::WheelFocus);
}


KSimDoubleSpinBox::~KSimDoubleSpinBox()
{
	delete m_p;
}
	
double KSimDoubleSpinBox::value() const
{
	return m_p->value();
}

void KSimDoubleSpinBox::setMinValue(double value)
{
	m_p->setMin(value);
	setUpdateRequired();
}

double KSimDoubleSpinBox::minValue() const
{
	return m_p->min();
}

void KSimDoubleSpinBox::setMaxValue(double value)
{
	m_p->setMax(value);
	setUpdateRequired();
}

double KSimDoubleSpinBox::maxValue() const
{
	return m_p->max();
}

void KSimDoubleSpinBox::setLineStep(double value)
{
	m_p->setStep(value);
}

double KSimDoubleSpinBox::lineStep() const
{
	return m_p->step();
}

void KSimDoubleSpinBox::setValue(double value)
{
	setUpdateRequired();
	int res = setValueInternal(value);
	QSpinBox::setValue(res);
}

int KSimDoubleSpinBox::setValueInternal(double newValue)
{
	bool toggle = true;
	int res = QSpinBox::value();
	if (isUpdateRequired() || (newValue != value()))
	{
//		KSIMDEBUG("KSimDoubleSpinBox::setValueInternal(double newValue)");
	
		setUpdateRequired(false);
		// Value changed
		if (wrapping())
		{
			if (newValue > maxValue())
			{
				if (extraZeroAllowed() && (minValue() > 0))
				{
					newValue = 0;
				}
				else
				{
					newValue = minValue();
				}
			}
			else if (newValue < minValue())
			{
				if (extraZeroAllowed() && (minValue() > 0))
				{
					if (newValue < 0)
						newValue = maxValue();
					else if (newValue < (minValue()*0.999))
						newValue = 0;
					else
						newValue = minValue();
				}
				else
				{
					newValue = maxValue();
				}
			}
			
			if (newValue >= maxValue())
			{
				res = QSPINBOX_UPPER_LIMIT;
				toggle = false;
			}
			else if (extraZeroAllowed() && (minValue() > 0))
			{
				if (newValue <= 0)
				{
					res = QSPINBOX_LOWER_LIMIT;
					toggle = false;
				}
			}
			else
			{
				if (newValue <= minValue())
				{
					res = QSPINBOX_LOWER_LIMIT;
					toggle = false;
				}
			}
		}
		else		// (wrapping())
		{
//			KSIMDEBUG(QString::number(newValue, 'e'));
			if (newValue >= maxValue())
			{
//				KSIMDEBUG("newValue >= maxValue()");
				newValue = maxValue();
				res = QSPINBOX_UPPER_LIMIT;
				toggle = false;
			}
			else if (newValue <= minValue())
			{
//				KSIMDEBUG("newValue <= minValue()");
				if (extraZeroAllowed() && (minValue() > 0))
				{
//					KSIMDEBUG("extraZeroAllowed() && (minValue() > 0)");
					if (newValue < (minValue()*0.999))
					{
//						KSIMDEBUG("newValue < (minValue()*0.999)");
						newValue = 0;
						res = QSPINBOX_LOWER_LIMIT;
						toggle = false;
					}
					else
					{
						newValue = minValue();
					}
				}
				else
				{
					// no extra zero
//					KSIMDEBUG("NO extraZeroAllowed()");
					newValue = minValue();
					res = QSPINBOX_LOWER_LIMIT;
					toggle = false;
				}
			}
		}
		
		if(toggle)
		{
//			KSIMDEBUG("toggle");
			res = (res == QSPINBOX_TOGGLE_0) ? QSPINBOX_TOGGLE_1 : QSPINBOX_TOGGLE_0;
		}
		
//		KSIMDEBUG(QString::number(newValue, 'e'));
		m_p->setValue(newValue);
	}
	return res;
}

void KSimDoubleSpinBox::stepUp()
{
	double newValue = value();
	int res;
	int steps = (m_p->lastButtonState() & ControlButton) ? multiSteps() : 1;
	
	if (extraZeroAllowed() && (minValue() > 0) && (value() == 0))
	{
		if (exponentStepPossible() && (m_p->useExpSteps() ^ (0 != (m_p->lastButtonState() & AltButton))))
		{
			newValue = minValue();
		}
		else
		{
			newValue = lineStep();
		}
			
		steps--;
	}
	
	if (exponentStepPossible() && (m_p->useExpSteps() ^ (0 != (m_p->lastButtonState() & AltButton))))
	{
		int i;
		for (i = 0; i < steps; i++)
		{
			newValue = m_p->m_expSteps->next(newValue);
		}
	}
	else
	{
		newValue = newValue / lineStep();
		newValue = floor(newValue + steps + 0.05) * lineStep();
	}
	
	res = setValueInternal(newValue);
	QSpinBox::setValue(res);
}

void KSimDoubleSpinBox::stepDown()
{
	double newValue = value();
	int res;
	int steps = (m_p->lastButtonState() & ControlButton) ? multiSteps() : 1;
	
	if (extraZeroAllowed() && (minValue() > 0) && (value() == 0))
	{
		newValue = -minValue(); // Any negative value
								// no further steps
	}
	else if (exponentStepPossible() && (m_p->useExpSteps() ^ (0 != (m_p->lastButtonState() & AltButton))))
	{
		int i;
		for (i = 0; i < steps; i++)
		{
			newValue = m_p->m_expSteps->previous(newValue);
		}
		if ((i>1) && (newValue > maxValue()) && (value() < maxValue()*0.999))
		{
			// Do not "multi step" over the upper limit
			newValue = maxValue();
		}
	}
	else
	{
		newValue = newValue / lineStep();
		newValue = ceil(newValue - steps - 0.05) * lineStep();
	}
	
	res = setValueInternal(newValue);
	QSpinBox::setValue(res);
}

QString KSimDoubleSpinBox::mapValueToText(int)
{
	return QString("%1").arg(value(),fieldwidth(),format(),precision());
}

int KSimDoubleSpinBox::mapTextToValue(bool *)
{
	bool myOk = true;
	double newVal;
	newVal = cleanText().toDouble(&myOk);
	
	if (!myOk)
	 newVal = value();
	
	return setValueInternal(newVal);
}
	

void KSimDoubleSpinBox::slotValueChanged(int val)
{
	if (val == QSPINBOX_LOWER_LIMIT)
	{
		if (extraZeroAllowed() && (minValue() > 0))
		{
			if (value() != 0)
				setValueInternal(0);
		}
		else
		{
			if (value() != minValue())
				setValueInternal(minValue());
		}
	}
	emit valueChanged(value());
}


KSimExponentStep * KSimDoubleSpinBox::getExponentStep()
{
	if (!m_p->m_expSteps)
	{
		m_p->m_expSteps = new KSimExponentStep;
		CHECK_PTR(m_p->m_expSteps);
	}
	return m_p->m_expSteps;
}

void KSimDoubleSpinBox::setNewExponentSteps(double step1, double step2, double step3,
                                            double step4, double step5, double step6,
                                            double step7, double step8, double step9,
                                            double step10)
{
	getExponentStep()->setNewSteps(step1, step2, step3, step4, step5,
                                 step6, step7, step8, step9, step10);
	
  // Enable exponent steps if possible
	if (getExponentStep()->countSteps())
	{
		setExponentStepEnabled(true);
	}
	else
	{
		setExponentStepEnabled(false);
	}
}

bool KSimDoubleSpinBox::exponentStepPossible() const
{
	return ((m_p->m_expSteps != 0) && (m_p->m_expSteps->countSteps() != 0));
}

void KSimDoubleSpinBox::setExponentStepEnabled(bool ena)
{
	if (ena && !exponentStepPossible())
	{
		KSIMDEBUG("No Exponent List available");
	}
	else
	{
		m_p->setExpStepsEnabled(ena);
	}
}

bool KSimDoubleSpinBox::exponentStepEnabled() const
{
	return m_p->expStepsEnabled();
}

void KSimDoubleSpinBox::setExtraZeroAllowed(bool allowed)
{
	m_p->setExtraZeroAllowed(allowed);
}

bool KSimDoubleSpinBox::extraZeroAllowed() const
{
	return m_p->extraZeroAllowed();
}

void KSimDoubleSpinBox::setMultiSteps(int steps)
{
	if(steps > 1)
		m_p->setMultiSteps(steps);
	else
		m_p->setMultiSteps(1);
}
	
int KSimDoubleSpinBox::multiSteps() const
{
	return m_p->multiSteps();
}

void KSimDoubleSpinBox::setFieldwidth(int fieldwidth)
{
	m_p->setFieldwidth(fieldwidth);
	setUpdateRequired();
}
int KSimDoubleSpinBox::fieldwidth() const
{
	return m_p->fieldwidth();
}
void KSimDoubleSpinBox::setFormat(char format)
{
	m_p->setFormat(format);
	setUpdateRequired();
}
char KSimDoubleSpinBox::format() const
{
	return m_p->format();
}
void KSimDoubleSpinBox::setPrecision(int precision)
{
	m_p->setPrecision(precision);
	setUpdateRequired();
}
int KSimDoubleSpinBox::precision() const
{
	return m_p->precision();
}

void KSimDoubleSpinBox::setFormat(int fieldwidth, char format, int precision)
{
	m_p->setFieldwidth(fieldwidth);
	m_p->setFormat(format);
	m_p->setPrecision(precision);
	setUpdateRequired();
}



void KSimDoubleSpinBox::setUpdateRequired(bool update)
{
	m_p->setUpdateRequired(update);
	if (update && !isHidden())
	{
//		setValue(value());  endless loop !!!!
		int res = setValueInternal(value());
		QSpinBox::setValue(res);
	}
}

bool KSimDoubleSpinBox::isUpdateRequired() const
{
	return m_p->updateRequired();
}

bool KSimDoubleSpinBox::eventFilter(QObject * obj, QEvent * ev)
{
	switch(ev->type())
	{
		case QEvent::MouseButtonPress:
		{
			m_p->setLastButtonState(((QMouseEvent*)ev)->state());
			if (((QMouseEvent*)ev)->button() == QMouseEvent::RightButton)
			{
				QPopupMenu * popup = new QPopupMenu(0,"KSimDoubleSpinBox RMB Popup");
				CHECK_PTR(popup);
		
				initRmbMenu(popup);
				if (popup->count() != 0)
				{
					int id = popup->exec(QCursor::pos());
					if (id != -1)
					{
						execRmbMenu(id);
					}
					delete popup;
				}
				return true;
			}
			break;
		}
		
		case QEvent::MouseButtonRelease:
		{
			m_p->setLastButtonState(((QMouseEvent*)ev)->state());
			break;
		}
				
		case QEvent::FocusOut:
		{
			m_p->setLastButtonState(0);
			emit undoRequest();
			break;
		}
		
		case QEvent::Wheel:
		{
			m_p->setLastButtonState(((QWheelEvent*)ev)->state());
			break;
		}
		
		case QEvent::KeyPress:
		case QEvent::KeyRelease:
		{
			m_p->setLastButtonState(((QKeyEvent*)ev)->state());
			break;
		}
		
		default:
			break;
	}
	return QSpinBox::eventFilter(obj,ev);
}

void KSimDoubleSpinBox::initRmbMenu(QPopupMenu * popup)
{
	if (exponentStepPossible())
	{
		KSimDoubleSpinBoxPrivate::rmbExponentStepID = popup->insertItem(i18n("Enable exponent steps"));
		popup->setItemChecked(KSimDoubleSpinBoxPrivate::rmbExponentStepID, exponentStepEnabled());
	}
	else
	{
		KSimDoubleSpinBoxPrivate::rmbExponentStepID = -1; // invalid value
	}
}

void KSimDoubleSpinBox::execRmbMenu(int ID)
{
	if (ID == KSimDoubleSpinBoxPrivate::rmbExponentStepID)
	{
		setExponentStepEnabled(!exponentStepEnabled());
	}
}

//#############################################################
//#############################################################

class KSimDoubleUnitSpinBoxPrivate
{
	friend class KSimDoubleUnitSpinBox;

	KSimDoubleUnitSpinBoxPrivate()
		: currentUnit(0),
			lastUnit(0),
			fixedUnit(0),
			currentUnitList("")
	{
		multiUnitList = new KSimMultiUnitList();
		CHECK_PTR(multiUnitList);
	};
	~KSimDoubleUnitSpinBoxPrivate()
	{
		delete multiUnitList;
	};
	
	KSimMultiUnitList * multiUnitList;
	const KSimUnitBase * currentUnit;
	const KSimUnitBase * lastUnit;
	const KSimUnitBase * fixedUnit;
	QString currentUnitList;
	
	
	static int rmbAutoUnitID;
	static QIntDict<QString> * rmbUnitIDList;
};

int KSimDoubleUnitSpinBoxPrivate::rmbAutoUnitID = -1;
QIntDict<QString> * KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList = 0;

//#############################################################
//#############################################################

KSimDoubleUnitSpinBox::KSimDoubleUnitSpinBox(QWidget * parent, const char * name)
	:	KSimDoubleSpinBox(parent, name),
		m_p(new KSimDoubleUnitSpinBoxPrivate())
{
	CHECK_PTR(m_p);
}

KSimDoubleUnitSpinBox::KSimDoubleUnitSpinBox(double min, double max, double steps,
                        										QWidget * parent, const char * name)
	:	KSimDoubleSpinBox(0.0, min, max, steps, parent, name),
		m_p(new KSimDoubleUnitSpinBoxPrivate())
{
	CHECK_PTR(m_p);
}

KSimDoubleUnitSpinBox::~KSimDoubleUnitSpinBox()
{
	delete m_p;
}
	
KSimMultiUnitList & KSimDoubleUnitSpinBox::getMultiUnitList()
{
	return *m_p->multiUnitList;
}

int KSimDoubleUnitSpinBox::setValueInternal(double newValue)
{
	int res;
	
//	KSIMDEBUG("KSimDoubleUnitSpinBox::setValueInternal(double newValue)");
	
	if (getMultiUnitList().count() &&
		 (isUpdateRequired() || (newValue != value()) || (m_p->lastUnit != m_p->currentUnit) ) )
	{
		const KSimUnitBase * unit;
		
		res = KSimDoubleSpinBox::setValueInternal(newValue);

    // Find prefered Unit
    if ((m_p->currentUnit && m_p->currentUnit->isPrefered(value())) || (m_p->fixedUnit))
    {
    	unit = m_p->currentUnit;
//			KSIMDEBUG("unit = m_p->currentUnit");
    }
    else
    {
//			KSIMDEBUG_VAR("",m_p->currentUnitList);
			unit = getMultiUnitList().findPrefered(value(), &m_p->currentUnitList);
//			KSIMDEBUG("unit = getMultiUnitList().findPrefered(value(), &m_p->currentUnitList)");
		}
		if (unit)
		{
//			KSIMDEBUG_VAR("", value());
//			KSIMDEBUG_VAR("", unit->getUnitString());
			m_p->currentUnit = unit;
			setSuffix(QString(" ") + m_p->currentUnit->getUnitString());
		}
		else
		{
			// No unit found
			KSIMDEBUG("No prefered unit found.");
			setSuffix(QString(" "));
		}
		m_p->lastUnit = unit;
	}
	else
	{
		res = KSimDoubleSpinBox::setValueInternal(newValue);
	}

	return res;

}

QString KSimDoubleUnitSpinBox::mapValueToText(int newValue)
{
	if (getMultiUnitList().count() && m_p->currentUnit)
	{
//		KSIMDEBUG_VAR("mapValueToText",value());
//		KSIMDEBUG_VAR("mapValueToText",m_p->currentUnit->toUnit(value()));
		return QString("%1").arg(m_p->currentUnit->toUnit(value()),fieldwidth(),format(),precision());
	}
	return KSimDoubleSpinBox::mapValueToText(newValue);
}

int KSimDoubleUnitSpinBox::mapTextToValue(bool * ok)
{
	bool convertOk;
	char * end;
	const char * end1;
	double val, newVal;

	
	QCString a(text().utf8());
	
//	KSIMDEBUG_VAR("QString",text());
//	KSIMDEBUG_VAR("QCString",a);
//	KSIMDEBUG_VAR("QCString",QCString(text()));

	val = newVal = strtod( a.data() ? a.data() : "", &end );
	
	convertOk = ( a && *a && (a.data() != end));
	
	if ( convertOk )
	{
		// With units ?
		if ( getMultiUnitList().count() )
		{	
			// Search unit
			end1 = end;		// char * --> const char *  (compiler warning)
			if (end1 == 0)	end1 = "";
			
			QCString suffix (QCString(end1).simplifyWhiteSpace());
	
//			KSIMDEBUG_VAR("QString",QString::fromUtf8(suffix));
//			KSIMDEBUG_VAR("QString",QString(suffix));
//			KSIMDEBUG_VAR("QCString",suffix);
			
			const KSimUnitBase * unit = getMultiUnitList().findUnit(QString::fromUtf8(suffix), &m_p->currentUnitList);
	
			if (unit)
			{
				// Unit found!
				m_p->currentUnit = unit;
				newVal = unit->fromUnit(val);
			}
			else
			{
				KSIMDEBUG("Unit not found. Skip input");
				convertOk = false;
			}
		}
		else
		{
			// Without unit
			newVal = val;
//			KSIMDEBUG("without unit");
		}
	}
	
	if ( ok )
		*ok = convertOk;
		
	if (!convertOk)
	{
		KSIMDEBUG("!convertOk");
		newVal = value();
	}
	
	return setValueInternal(newVal);
}

const KSimUnitBase * KSimDoubleUnitSpinBox::getUnit() const
{
	return m_p->currentUnit;
}

const QString & KSimDoubleUnitSpinBox::getUnitString() const
{
	static const QString empty = QString();
	
	if (getUnit())
		return getUnit()->getUnitString();
	return empty;
}

const QString & KSimDoubleUnitSpinBox::getUnitListString() const
{
	return m_p->currentUnitList;
}

void KSimDoubleUnitSpinBox::setUnitListString(const QString & unitListStr)
{
	const KSimUnitList * unitList = getMultiUnitList().findUnitList(unitListStr);
	if(unitList)
	{
		m_p->fixedUnit = 0;
		m_p->currentUnit = 0;
		m_p->currentUnitList = unitListStr;
		setUpdateRequired();
//		setValue(value());		// complete update
	}
	else
	{
		KSIMDEBUG_VAR("Unit list not found", unitListStr);
	}
}

void KSimDoubleUnitSpinBox::setFixedUnit(const KSimUnitBase * fixedUnit)
{
	if (fixedUnit)
	{
		setFixedUnit(fixedUnit->getUnitString());
	}
	else
	{
		setFixedUnit(QString());
	}	
}

void KSimDoubleUnitSpinBox::setFixedUnit(const QString & fixedUnitStr)
{
	QString unitList;
	const KSimUnitBase * unit = getMultiUnitList().findUnit(fixedUnitStr, &unitList);
	m_p->fixedUnit = unit;
	if(unit)
	{
		m_p->currentUnit = unit;
		m_p->currentUnitList = unitList;
	}
	setUpdateRequired();
//	setValue(value());		// complete update
}

const KSimUnitBase * KSimDoubleUnitSpinBox::fixedUnit() const
{
	return m_p->fixedUnit;
}


void KSimDoubleUnitSpinBox::initRmbMenu(QPopupMenu * popup)
{
	KSimDoubleSpinBox::initRmbMenu(popup);
	
	if (popup->count())
	{
		popup->insertSeparator();
	}
	
	KSimDoubleUnitSpinBoxPrivate::rmbAutoUnitID = popup->insertItem(i18n("Auto unit"));
	popup->setItemChecked(KSimDoubleUnitSpinBoxPrivate::rmbAutoUnitID, fixedUnit() == 0);
	
	//####  Add units
	
	KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList = new QIntDict<QString>;
	CHECK_PTR(KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList);
	
	QListIterator<KSimUnitList> mulitListIt(getMultiUnitList().getList());
	
	for (mulitListIt.toFirst(); mulitListIt.current(); ++mulitListIt)
	{
		switch (mulitListIt.current()->countVisible())
		{
			case 0:
				// nothing to do
				break;
				
			case 1:
			{
				// Find (first) not hidden unit
				QListIterator<KSimUnitBase> unitIt(*mulitListIt.current());
				for (; unitIt.current(); ++unitIt)
				{
					if (!unitIt.current()->isHidden())
					{
						break;
					}
				}
	    	if (unitIt.current())
  	  	{
	  	  	// Insert unit
	  	  	int ID = popup->insertItem(unitIt.current()->getUnitString());
  	  		KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList->insert(ID, &unitIt.current()->getUnitString());
  	  	}
  	  	break;
  	  }
  	  	
  	  default:
  	  {
				// Create own popup
				QPopupMenu * unitPopup = new QPopupMenu (popup, mulitListIt.current()->getListName());
				CHECK_PTR(unitPopup);
	
				QListIterator<KSimUnitBase> unitIt(*mulitListIt.current());
				for (; unitIt.current(); ++unitIt)
				{
    			if (!unitIt.current()->isHidden())
    			{
	    			int ID = unitPopup->insertItem(unitIt.current()->getUnitString());
  	  			KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList->insert(ID, &unitIt.current()->getUnitString());
  	  		}
    		}
				popup->insertItem(mulitListIt.current()->getListName(), unitPopup);
				break;
			}
		}		
	}
}

void KSimDoubleUnitSpinBox::execRmbMenu(int ID)
{
	KSimDoubleSpinBox::execRmbMenu(ID);
	if (ID == KSimDoubleUnitSpinBoxPrivate::rmbAutoUnitID)
	{
		if (fixedUnit())
		{
			setFixedUnit("");
		}
		else
		{
			setFixedUnit(m_p->currentUnit);
		}
	}
	else
	{
		if (KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList)
		{
			QString * unitStr = KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList->find(ID);
			if (unitStr)
			{
				setFixedUnit(*unitStr);
			}
		}
	}
	delete KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList;
	KSimDoubleUnitSpinBoxPrivate::rmbUnitIDList = 0;
}


void KSimDoubleUnitSpinBox::polish()
{
	KSimDoubleSpinBox::polish();
	
	if (isUpdateRequired())
	{
		setValue(value());
	}
}


//#############################################################
//#############################################################
