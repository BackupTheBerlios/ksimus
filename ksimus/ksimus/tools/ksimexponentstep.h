/***************************************************************************
                          ksimexponentstep.h  -  description
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

#ifndef KSIMEXPONENTSTEP_H
#define KSIMEXPONENTSTEP_H

// C-Includes

// QT-Includes
//#include <qptrlist.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes

// Forward declaration


/**	Adjust double values to exponential steps.
	*
	*	A given value is splitted up occording following formula:
	*
	*	value = factor * (base ^ exponent)
	*
	* value: to be represent (double, value > 0.0)
	*
	* factor: the factor (double, 1.0 <= factor < base)
	*
	*	base: the base for the exponent (double, base > 1, default 10.0)
	*
	* exponent: the exponent (unsigned int)
	*
	* For using this class you have to provide a number of factors (so called steps) and the base.
	*	Example gain adjaust at oscillograph: Provide the steps 1.0, 2.0, 5.0 and as base 10.0.
	* Allowed exponential steps are now: 1.0eX, 2.0eX and 5.0eX, where X is an integer.
	*
	* The function @ref adjust returns the exponential steps, hich is (linear) nearest to the
	* given parameter. The function @ref previous and @ref next returns the exponential steps beloew
	*	or above the given parameter.
	*
	*	@short Manages exponential steppings with @ref KSimDoubleSpinBox.
  *	@author Rasmus Diekenbrock
  *	@see KSimDoubleSpinBox
  */

class KSimExponentStep
{
class Private;
public:
	/** Creates an object with the base 10.0 and without any steps.
	*/
	KSimExponentStep();
	/** The destructor.
	*/
	~KSimExponentStep();
	
	/** Sets the base.
		*	@param base The new base (> 1)
		*/
	void setBase(double base);
	/** Returns the current base. */
	double getBase() const;
	
	/** Returns the linear nearest exponential step to the given value.
		*	@param value The given value (> 0)
		*/
	double adjust(double value) const;
	/** Returns the prevoius exponential step to the given value.
		*	@param value The given value (> 0)
		*/
	double previous(double value) const;
	/** Returns the next exponential step to the given value.
		*	@param value The given value (> 0)
		*/
	double next(double value) const;
	
	/** Sets new steps. The functions clears all current steps and add all steps up to the
		*	first parameter equals -1.0.
		*	All steps have to be in range 1.0 <= step < base. Steps have to be in ascending order.
		*	The base is unchanged.
		*/
	void setNewSteps(double step1, double step2 = -1.0, double step3 = -1.0,
	                 double step4 = -1.0, double step5 = -1.0, double step6 = -1.0,
	                 double step7 = -1.0, double step8 = -1.0, double step9 = -1.0,
	                 double step10 = -1.0);
	/** Remove all current steps.
		*/
	void clearSteps();
	/** Appends a new steps to the current steps.
		*	The step has to be in range 1.0 <= step < base. Steps have to be added in ascending order.
		*	The base is unchanged.
		*/
	void appendStep(double step);
	/** Returns the number of the current steps. */
	unsigned int countSteps() const;

private:
	/** For internal use. */
	typedef struct tParts
	{
		double result;
		int exponent;
		unsigned int factorIndex;
	};

	/** For internal use. */
	void appendStepInternal(double step);
	/** For internal use. */
	bool findPart(double value, tParts & parts) const;
	/** For internal use. */
	double convert(unsigned int factor, int exp) const;
	/** For internal use. */
	double convert(tParts & part)const;
	
	/** For internal use. */
	Private * m_p;
};

#endif
