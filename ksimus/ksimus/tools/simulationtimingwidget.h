/***************************************************************************
                          simulationtimingwidget.h  -  description
                             -------------------
    begin                : Fri Mar 9 2001
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

#ifndef SIMULATIONTIMINGWIDGET_H
#define SIMULATIONTIMINGWIDGET_H

// C-Includes

// QT-Includes

// KDE-Includes

// Project-Includes
#include "propertywidget.h"

// Forward declaration
class SimulationTiming;
class QCheckBox;
class KSimTimeSpinBox;

/**Widget for adjust the simulation timing
  *@author Rasmus Diekenbrock
  */

class SimulationTimingWidget : public PropertyWidget
{
   Q_OBJECT
public: 
	SimulationTimingWidget(SimulationTiming & timing, QWidget *parent=0, const char *name=0);
	~SimulationTimingWidget();

	/** The function acceptPressed() is called, if changes are accepted.
		You have to reimplement this function, if you add new properties.
		If you do so, then first call function changeData() and than changed data!
	 */
	virtual void acceptPressed();
	/** The function defaultPressed() is called, if user wants to set the default values.
		You have to reimplement this function, if you add new properties.
	 */
	virtual void defaultPressed();


protected:

private:
	
	SimulationTiming & m_timing;
	QCheckBox * m_execFastest;
	KSimTimeSpinBox * m_execTime;
	QCheckBox * m_tickSynchronized;
	KSimTimeSpinBox * m_tickTime;
	QCheckBox * m_updateSynchronized;
	KSimTimeSpinBox * m_updateTime;

private slots:
	void slotCheckBoxToggled();
	/** Stores data for undo mechanism */
	void slotUndo();

};



#endif
