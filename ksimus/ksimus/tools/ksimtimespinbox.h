/***************************************************************************
                          ksimtimespinbox.h  -  description
                             -------------------
    begin                : Sun Oct 14 2001
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

#ifndef KSIMTIMESPINBOX_H
#define KSIMTIMESPINBOX_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "ksimdoublespinbox.h"
#include "ksimtime.h"

// Forward declaration

/**Widget for KSimTime objects.
  *@author Rasmus Diekenbrock
  */


class KSimTimeSpinBox : public KSimDoubleUnitSpinBox
{
Q_OBJECT

public:
	/** Creates a spinbox for a @ref KSimTime object. */
	KSimTimeSpinBox(const KSimTimeBase & time, QWidget * parent = 0, const char * name = 0);
	/** Creates a spinbox for a @ref KSimTime object. */
	KSimTimeSpinBox(const KSimTimeBase & time, double min, double max, double steps, QWidget * parent = 0, const char * name = 0);
	/** The destructor. */
	~KSimTimeSpinBox();
	
	/** Set the current time. */
	void setValue(const KSimTimeBase & time);
	/** Returns the current time. */
	const KSimTimeBase & value() const;

	
private:
	/** @internal */
	void init();
	/** Stores the current time. */
	KSimTimeBase m_time;
	

private slots:
	/** Connected to the @ref KSimDoubleUnitSpinBox::valueChanged(double).
			@internal
		*/
	void slotChanged(double sec);
	
signals:
	/** This signal is emitted, if the time is changed.*/
	void changed();
	/** This signal is emitted, if the time is changed.*/
	void changed(const KSimTime & time);
};


#endif
