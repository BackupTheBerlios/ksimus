/***************************************************************************
                          ksimdoubleedit.h  -  description
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

#ifndef KSIMDOUBLEEDIT_H
#define KSIMDOUBLEEDIT_H

#include "ksimlineedit.h"

class QDoubleValidator;

/**A QLineEdit for Double Type
  *@author Rasmus Diekenbrock
  */

class KSimDoubleEdit : public KSimLineEdit
{
   Q_OBJECT

class Private;
friend class KSimDoubleEditValidator;

public:
	KSimDoubleEdit(QWidget *parent, const char *name = 0);
	KSimDoubleEdit(double bottom, double top, int decimals, QWidget *parent, const char *name = 0);
	~KSimDoubleEdit();

	void setRange(double bottom, double top, int decimals);
	double bottom() const;
	double top() const;
	int decimals() const;
	double value();
	void setBottom(double bottom);
	void setTop(double top);
	void setDecimals(int decimals);
	
	/** Sets the convertion type. Allowed types are 'g', 'G', 'e', 'E', 'g' and 'G'. See the printf docu.
		*/
	void setConversionType(char type);
	/** Gets the convertion type. Allowed types are 'g', 'G', 'e', 'E', 'g' and 'G'. See the printf docu.
		*/
	char getConversionType() const;
	
	/** Enables the value tracking during the input. If the current input is a valid value
	  * and is in the given range the signal @ref valueChanged is emitted. If tracking is
	  * disabled the signal is emitted only after retrun pressed or the input line was leaved.
	  * The default is enabled.
	  */
	void setTrackingEnabled(bool tracking);
	/** Returns true if tracking is enabled.
	  * @see setTrackingEnabled
	  */
	bool isTrackingEnabled() const;
	
public slots:
	void setValue(double value);
	void restoreValue();

protected slots:
	void slotTextChanged(const QString & text);
	void slotReady();


private:
	QDoubleValidator * m_dVal;
	double m_lastValue;
	double m_trackedValue;
	Q_UINT32 m_flags;
	Private * m_p;
	

signals:
	void valueChangedTracking(double value);	
	void valueChanged(double value);	

};

#endif
