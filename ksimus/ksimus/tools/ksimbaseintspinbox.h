/***************************************************************************
                          ksimbaseintspinbox.h  -  description
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

#ifndef KSIMBASEINTSPINBOX_H
#define KSIMBASEINTSPINBOX_H

// C-Includes

// QT-Includes

// KDE-Includes

// KSimus-Includes
#include "ksimspinbox.h"
#include "ksimbaseint.h"

// Project-Includes

// Forward declaration
class QPopupMenu;

/** This class provides a spinbox for integer values with different bases. The usage is same as @ref QSpinBox.
	* Values are @ref KSimBaseInt based.
	*
	* The wrapping and special value text feature of the @ref QSpinBox is also useable.
	*
	*@short Spinbox for integer values with different bases.
  *@author Rasmus Diekenbrock
  */


//#############################################################
//#############################################################

class KSimBaseIntSpinBox : public KSimSpinBox
{
class Private;
	Q_OBJECT

public:
	/** Creates a spinbox for integer values with different bases.
		*
	  * Value defaults  : value = 0 decimal, min = MIN_INT, max = MAX_INT
	  *
	  * Step defaults   : stepWidth(linear Mode) = 1
	  *
	  * Display defaults: decimal
	  */
	KSimBaseIntSpinBox(QWidget * parent = 0, const char * name = 0);
	/** Creates a spinbox for integer values with different bases.
		*
	  * Step default    : no exponent steps
	  */
	KSimBaseIntSpinBox(const KSimBaseInt & value, int min, int max, int steps,
                    QWidget * parent = 0, const char * name = 0);
	/** The destructor.
	 */
	~KSimBaseIntSpinBox();
	
	/** Returns the current value.
	*/
	const KSimBaseInt & value() const;
	/** Sets the minimum allowed value.
	*/
	void setMinValue(int minValue);
	/** Returns the minimum allowed value.
	*/
	int minValue() const;
	/** Sets the minimum allowed value.
	*/
	void setMaxValue(int maxValue);
	/** Returns the minimum allowed value.
	*/
	int maxValue() const;

	/** Enables the value tracking during the input. If the current input is a valid value
	  * and is in the given range the signal @ref valueChanged is emitted. If tracking is
	  * disabled the signal is emitted only after retrun pressed or the input line was leaved.
	  * The default is disabled.
	  */
	void setTrackingEnabled(bool tracking);
	/** Returns true if tracking is enabled.
	  * @see setTrackingEnabled
	  */
	bool isTrackingEnabled() const;

	/** Adds a constant to the constant list. */
	void appendConstant(const KSimBaseInt & value, const QString & name);
	/** Deletes the constant list. */
	void clearConstantList();
	/** Adds the current minimum value as constant to the constant list. */
	void appendMinConstant(const QString & name = QString::null);
	/** Adds the current maximum value as constant to the constant list. */
	void appendMaxConstant(const QString & name = QString::null);

public slots:	
	/** Sets the current value.
	*/
	virtual void setValue(const KSimBaseInt & value);
	/** Increase the value. If the exponent mechanism is enabled, the exponent steps are used else
		* the value if lineStep().
		*/
	virtual void stepUp();
	/** Decrease the value. If the exponent mechanism is enabled, the exponent steps are used else
		* the value if lineStep().
		*/
	virtual void stepDown();

	
protected:
	/** Overloaded function to convert the current value to text.
	*/
	virtual QString mapValueToText(int value);
	/** Overloaded function to convert the current text to the proper value.
	*/
	virtual int mapTextToValue(bool * ok);
	/** Internal function. Set the current value.
	*/
	virtual int setValueInternal(const KSimBaseInt & value);
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
	Private * m_p;

private slots:
	/** Internal slot.
	*/
	void slotValueChanged(int val);
	void slotTextChanged(const QString & text);
	void slotConvertToBinary();
	void slotConvertToOctal();
	void slotConvertToDecimal();
	void slotConvertToHexadecimal();

signals:
	/** This signal is emitted, if the value is changed.
	*/
	void valueChanged(const KSimBaseInt & value);
	void valueChangedTracking(const KSimBaseInt & value);
	/** This signal is emitted, if undo is possible required.
			(Leave event and timeout)
	*/
	void undoRequest();
};


//#############################################################
//#############################################################



#endif
