/***************************************************************************
                          ksimdoublespinbox.h  -  description
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

#ifndef KSIMDOUBLESPINBOX_H
#define KSIMDOUBLESPINBOX_H

// C-Includes

// QT-Includes
#include <qspinbox.h>

// KDE-Includes

// KSimus-Includes

// Project-Includes
#include "ksimunit.h"

// Forward declaration
class KSimMultiUnitList;
class KSimDoubleSpinBoxPrivate;
class KSimDoubleUnitSpinBoxPrivate;
class KSimExponentStep;
class QPopupMenu;

/** This class provides a spinbox for double values. The usage is same as @ref QSpinBox.
	*
	* The up / down steps can be linear or exponential.
  *
	* The wrapping and special value text feature of the @ref QSpinBox is also useable.
	*
	*@short Spinbox for double numbers.
  *@author Rasmus Diekenbrock
  */


//#############################################################
//#############################################################

class KSimDoubleSpinBox : public QSpinBox
{
	Q_OBJECT

public:
	/** Creates a spinbox for double numbers.
		*
	  * Value defaults  : value = 0.0, min = -10.0, max = 10.0
	  *
	  * Step defaults   : stepWidth(linear Mode) = 0.1, no exponent steps
	  *
	  * Display defaults: fieldwidth = 0, format = 'g', precision = -1
	  */
	KSimDoubleSpinBox(QWidget * parent = 0, const char * name = 0);
	/** Creates a spinbox for double numbers.
		*
	  * Step default    : no exponent steps
	  *
	  * Display defaults: fieldwidth = 0, format = 'g', precision = -1
	  */
	KSimDoubleSpinBox(double value, double min = -10.0, double max = 10.0, double steps = 0.1,
                    QWidget * parent = 0, const char * name = 0);
	/** The destructor.
	 */
	~KSimDoubleSpinBox();
	
	/** Returns the current value.
	*/
	double value() const;
	/** Sets the minimum allowed value.
	*/
	void setMinValue(double value);
	/** Returns the minimum allowed value.
	*/
	double minValue() const;
	/** Sets the minimum allowed value.
	*/
	void setMaxValue(double value);
	/** Returns the minimum allowed value.
	*/
	double maxValue() const;
	/** Sets the line step in linear mode (not exponent steps). This value is used if the user
			presses the up or down button.
	*/
	void setLineStep(double value);
	/** Returns the line step value.
	*/
	double lineStep() const;
	/** Enables or disables the exponent step mechanism. You have to set exponent step before
			enable the mechanism.
		*/
	void setExponentStepEnabled(bool ena);
	/** Returns true, if the exponent mechanism is enabled.
	*/
	bool exponentStepEnabled() const;
	/** Returns the exponent step object.
	*/
	KSimExponentStep * getExponentStep();
	/** Sets new exponent steps. This steps are used, if the user press the up or down buttons
		*	and the exponent mechanism is enabled.
		*
		* The function clears all steps from the exponent object and adds up to 10 new steps.
		* The steps have to be in the range from 1.0 (including) to base of the exponent object
		*	(excluding). All parameters up to the frist parameter equals -1 are used.
		*
		*	The base of the exponent object is not changed by this function.
		*/
	void setNewExponentSteps(double step1, double step2 = -1, double step3 = -1,
	                         double step4 = -1, double step5 = -1, double step6 = -1,
	                         double step7 = -1, double step8 = -1, double step9 = -1,
	                         double step10 = -1);

	/** With enabled exponent steps only number greater than zero are allowed. With this switch you can
		*	additional use the zero.
		*
		* Default zero is not allowed.
		*/
	void setExtraZeroAllowed(bool allowed);
	/** With enabled exponent steps only number greater than zero are allowed. With this switch you can
		*	additional use the zero.
		*
		* Default zero is not allowed.
		*/
  bool extraZeroAllowed() const;
	
	/** Sets the number of steps which are executed if the ALT key is pressed during wheel or up/down event.
		*	Numbers lesser than 2 disables the multisteps.
		*
		* The default are 5 steps.
		*/
	void setMultiSteps(int steps);
	
	/** Gets the number of steps which are executed if the ALT key is pressed during wheel or up/down event.
		*	The function returns 1, if multisteps are disabled.
		*
		* The default are 5 steps.
		*/
	int multiSteps() const;
	
	/** Sets the fieldwidth of the output.
		*
		* @see QString #arg
	*/
	void setFieldwidth(int fieldwidth);
	/** Returns the fieldwidth of the output. See also QString::arg().
	*/
	int fieldwidth() const;
	/** Sets the number format of the output. See also QString::arg().
	*/
	void setFormat(char format);
	/** Returns the number format of the output. See also QString::arg().
	*/
	char format() const;
	/** Sets the precision of the output. See also QString::arg().
	*/
	void setPrecision(int precision);
	/** Returns the precision of the output. See also QString::arg().
	*/
	int precision() const;
	/** Sets the format of the output. See also QString::arg().
	*/
	void setFormat(int fieldwidth=0, char format = 'g', int precision = -1);

	/** Sets the alignment of the editor. */
	void setAlignment(int flag);
	
	/** Returns the alignment of the editor. */
	int alignment() const;

public slots:	
	/** Sets the current value.
	*/
	virtual void setValue(double value);
	/** Increase the value. If the exponent mechanism is enabled, the exponent steps are used else
		* the value if lineStep().
		*/
	virtual void stepUp();
	/** Decrease the value. If the exponent mechanism is enabled, the exponent steps are used else
		* the value if lineStep().
		*/
	virtual void stepDown();

	
protected:
	/** Returns true, if exponent steps are properly set.
	*/
	bool exponentStepPossible() const;
	/** Overloaded function to convert the current value to text.
	*/
	virtual QString mapValueToText(int value);
	/** Overloaded function to convert the current text to the proper value.
	*/
	virtual int mapTextToValue(bool * ok);
	/** Internal function. Set the current value.
	*/
	virtual int setValueInternal(double value);
	/** Internal function.
	*/
	void setUpdateRequired(bool update = true);
	/** Internal function.
	*/
	bool isUpdateRequired() const;
	/** Overloaded function. Fetches the right mouse button press event.
	*/
	virtual bool eventFilter(QObject * obj, QEvent * ev);
	/** Initialize the right mouse button menu.
	*/
	virtual void initRmbMenu(QPopupMenu * popup);
	/** Evaluates the return code of the right mouse button menu.
	*/
	virtual void execRmbMenu(int ID);
	

private:
	/** Internal function.
	*/
	void init();
	KSimDoubleSpinBoxPrivate * m_p;

private slots:
	/** Internal slot.
	*/
	void slotValueChanged(int val);

signals:
	/** This signal is emitted, if the value is changed.
	*/
	void valueChanged(double value);
	/** This signal is emitted, if undo is possible required.
			(Leave event and timeout)
	*/
	void undoRequest();
};

//#############################################################
//#############################################################

/** This calls provides a spinbox for double number with editable units.
	*
	*	Useable units are given via the class KSimMultiUnitList
	*
	*	@short Spinbox for double numbers with units.
  * @author Rasmus Diekenbrock
  *
  */

class KSimDoubleUnitSpinBox : public KSimDoubleSpinBox
{
	Q_OBJECT

public:
	/** Creates a spinbox for double numbers with units.
		*
	  * Value defaults: value = 0.0, min = -10.0, max = 10.0
		*
	  * Step defaults: stepWidth(linear Mode) = 0.1, no exponent steps
		*
	  * Display defaults: fieldwidth = 0, format = 'g', precision = -1
		*
	  * Unit defaults: no units
		*
	  */
	KSimDoubleUnitSpinBox(QWidget * parent = 0, const char * name = 0);
	/** Creates a spinbox for double numbers with units.
		*
	  * Step default: no exponent steps
		*
	  * Display defaults: fieldwidth = 0, format = 'g', precision = -1
		*
	  * Unit defaults: no units
		*
	  */
	KSimDoubleUnitSpinBox(double min, double max, double steps,
                        QWidget * parent = 0, const char * name = 0);
	/** The destructor.
	 */
	~KSimDoubleUnitSpinBox();
	
	/** Returns the MultiUnitList. This list contains all available units.
		*/
	KSimMultiUnitList & getMultiUnitList();
	
	/** Returns the current unit or a null pointer if no unit is present.
	*/
	const KSimUnitBase * getUnit() const;
  /** Returns the text string of the current unit or a null string if no unit is present.
  */
  const QString & getUnitString() const;

	/** Sets the current unit list. A selected fixed unit is disabled. */
	void setUnitListString(const QString & unitListStr);
	
  /** Returns the text string of the current unit list or a null string if no unit list is present.
  */
  const QString & getUnitListString() const;

  /** Set a fixed unit. All inputs will be converted into this unit. A null pointer
  	*	resets a fixed unit.
	  */
  void setFixedUnit(const KSimUnitBase * fixedUnit);
  /** Set a fixed unit. All inputs will be converted into this unit. A null string or any
  	*	unknown unit resets the fixed unit.
	  */
	void setFixedUnit(const QString & fixedUnitStr);
  /** Returns a pointer to the fixed unit or a null pointer if no fixed unit is selected.
  */
  const KSimUnitBase * fixedUnit() const;

public slots:
	/** For internal settings. */
	virtual void polish();

protected:
	/** Overloaded function to convert the current value to text.
	*/
	virtual QString mapValueToText(int value);
	/** Overloaded function to convert the current text to the proper value.
	*/
	virtual int mapTextToValue(bool * ok);
	/** Internal function. Set the current value.
	*/
	virtual int setValueInternal(double value);
	/** Overloaded function to initialize the right mouse button menu.
	*/
	virtual void initRmbMenu(QPopupMenu * popup);
	/** Overloaded function to evaluates the return code of the right mouse button menu.
	*/
	virtual void execRmbMenu(int ID);

private:
	KSimDoubleUnitSpinBoxPrivate * m_p;
};


//#############################################################
//#############################################################



#endif
