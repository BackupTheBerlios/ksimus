/***************************************************************************
                          ksimexponentstep.cpp  -  description
                             -------------------
    begin                : Tue Sep 11 2001
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

// QT-Includes
#include <qvaluelist.h>

// KDE-Includes
#include <kdebug.h>

// KSimus-Includes

// Project-Includes
#include "ksimexponentstep.h"

// Forward declaration

//#############################################################
//#############################################################

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

class KSimExponentStep::Private
{
	public:
	Private(double newBase)
		:	base(newBase)
	{};
	
	QValueList<double> list;
	double base;
};





//#############################################################
//#############################################################

#define stepList	(m_p->list)


KSimExponentStep::KSimExponentStep()
{
	m_p = new Private(10.0);
	CHECK_PTR(m_p);
	m_p->list.append(1.0);
	m_p->list.append(2.0);
	m_p->list.append(5.0);
}

KSimExponentStep::~KSimExponentStep()
{
	delete m_p;
}

void KSimExponentStep::setBase(double base)
{
	if (base <= 1)
	{
		KSIMDEBUG_VAR("Base lesser or equal as 1. Skip!",base);
	}
	else
	{
		m_p->base = base;
	}
}

double KSimExponentStep::getBase() const
{
	return m_p->base;
}
	
double KSimExponentStep::convert(unsigned int factor, int exp) const
{
	return stepList[factor] * pow(getBase(), exp);
}

double KSimExponentStep::convert(tParts & part) const
{
	return convert(part.factorIndex, part.exponent);
}

bool KSimExponentStep::findPart(double value, tParts & parts) const
{
	if (value > 0.0)
	{
		
		double exponent = floor(log(value)/log(getBase()));
		parts.exponent = (int)exponent;
		double baseExp = pow(getBase(), exponent);
		
		unsigned int step = 0;
		while((step < stepList.count()) && (value >= (stepList[step] * baseExp)))
		{
			step++;
		};
		double lower, upper;
		if (step == stepList.count())
		{
			// between max factor and next exponent
			lower = stepList[step-1] * baseExp;
			upper = stepList[0] * baseExp * getBase();
			if ((log(value)-log(lower)) < (log(upper)-log(value)))
			{
				// value is rather lower
				parts.result = lower;
				// parts.exponent is correct;
				parts.factorIndex = step-1;
			}
			else
			{
				// value is rather upper
				parts.result = upper;
				parts.exponent++;
				parts.factorIndex = 0;
			}
		}
		else
		{
			// between max factor and next exponent
			lower = stepList[step-1] * baseExp;
			upper = stepList[step] * baseExp;
			if ((log(value)-log(lower)) < (log(upper)-log(value)))
			{
				// value is rather lower
				parts.result = lower;
				// parts.exponent is correct;
				parts.factorIndex = step-1;
			}
			else
			{
				// value is rather upper
				parts.result = upper;
				// parts.exponent is correct;
				parts.factorIndex = step;
			}
		}
/*		KSIMDEBUG(QString("value:%1 exponent:%2 factor:%3 result:%4")
							.arg(value)
							.arg(pow(getBase(),parts.exponent))
							.arg(stepList.at(parts.factorIndex))
							.arg(parts.result));*/
		return true;
	}
	else
	{
		KSIMDEBUG_VAR("value <= 0", value);
		return false;
	}
}



double KSimExponentStep::adjust(double value) const
{
	tParts part;
	if(countSteps() && findPart(value, part))
	{
		return part.result;
	}
	else
	{
		return value;
	}
}

double KSimExponentStep::previous(double value) const
{
	tParts part;
	double res;
	
	if(countSteps() && findPart(value, part))
	{
		if (part.result < value)
		{
			res = part.result;
		}
		else if(part.factorIndex > 0)
		{
			res = convert (part.factorIndex-1,part.exponent);
		}
		else
		{
			res = convert (stepList.count()-1,part.exponent-1);
		}
	}
	else
	{
		res = value;
	}
	return res;
}

double KSimExponentStep::next(double value) const
{
	tParts part;
	double res;
	if(countSteps() && findPart(value, part))
	{
		if (part.result > value)
		{
			res = part.result;
		}
		else if(part.factorIndex < stepList.count()-1)
		{
			res = convert (part.factorIndex+1,part.exponent);
		}
		else
		{
			res = convert (0,part.exponent+1);
		}
	}
	else
	{
		res = value;
	}
	return res;
}
	
void KSimExponentStep::setNewSteps(double step1, double step2, double step3,
                                   double step4, double step5, double step6,
                                   double step7, double step8, double step9,
                                   double step10)
{
	clearSteps();
	
	if(step1  != -1) appendStepInternal(step1 ); else return;
	if(step2  != -1) appendStepInternal(step2 ); else return;
	if(step3  != -1) appendStepInternal(step3 ); else return;
	if(step4  != -1) appendStepInternal(step4 ); else return;
	if(step5  != -1) appendStepInternal(step5 ); else return;
	if(step6  != -1) appendStepInternal(step6 ); else return;
	if(step7  != -1) appendStepInternal(step7 ); else return;
	if(step8  != -1) appendStepInternal(step8 ); else return;
	if(step9  != -1) appendStepInternal(step9 ); else return;
	if(step10 != -1) appendStepInternal(step10); else return;
}

void KSimExponentStep::clearSteps()
{
	stepList.clear();
}
void KSimExponentStep::appendStep(double step)
{
	appendStepInternal(step);
}

void KSimExponentStep::appendStepInternal(double step)
{
	if (step < 1)
	{
		KSIMDEBUG_VAR("step < 1. Skip step!",step);
	}
	else if (step >= getBase())
	{
		KSIMDEBUG_VAR("step >= base. Skip step!",step);
	}
	else if (stepList.count() && (stepList.last() >= step))
	{
		KSIMDEBUG_VAR("step <= last step. Skip step!",step);
	}
	else
	{
		stepList.append(step);
	}
}

unsigned int KSimExponentStep::countSteps() const
{
	return stepList.count();
}

