/***************************************************************************
                          simulationtimingwidget.cpp  -  description
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

// C-Includes
#include <limits.h>

// QT-Includes
#include <qlabel.h>
#include <qcheckbox.h>
#include <qvgroupbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "simulationtimingwidget.h"
#include "ksimtimeselectorwidget.h"
#include "ksimtimeserver.h"
#include "simulationtiming.h"
#include "ksimusdoc.h"
#include "ksimundo.h"

//#include "ksimdoublespinbox.h"
#include "ksimunit.h"
#include "ksimtimespinbox.h"
// Forward declaration


SimulationTimingWidget::SimulationTimingWidget(SimulationTiming & timing, QWidget *parent, const char *name )
	:	PropertyWidget(1, parent,name),
		m_timing(timing)
{
	QVGroupBox * box;
	
	box = new QVGroupBox(i18n("Execution speed:"), this);
	CHECK_PTR(box);
	m_execFastest = new QCheckBox(i18n("Fast as possible"), box);
	CHECK_PTR(m_execFastest);
	m_execFastest->setChecked(m_timing.m_execFastest);
	m_execTime = new KSimTimeSpinBox(*m_timing.m_execTime, 1e-3, 1e0, 1e-2, box);
	CHECK_PTR(m_execTime);
	connect(m_execFastest, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));
	
	box = new QVGroupBox(i18n("Simulation time per tick:"), this);
	CHECK_PTR(box);
	m_tickSynchronized = new QCheckBox(i18n("Sychronized to execution speed"), box);
	CHECK_PTR(m_tickSynchronized);
	m_tickSynchronized->setChecked(m_timing.m_tickSynchronized);
	m_tickTime = new KSimTimeSpinBox(*m_timing.m_tickTime, 1e-10, 1e0, 1e-2, box);
	CHECK_PTR(m_tickTime);
	connect(m_tickSynchronized, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));

	box = new QVGroupBox(i18n("View update speed:"), this);
	CHECK_PTR(box);
	m_updateSynchronized = new QCheckBox(i18n("Sychronized to execution speed"), box);
	CHECK_PTR(m_updateSynchronized);
	m_updateSynchronized->setChecked(m_timing.m_updateSynchronized);
	m_updateTime = new KSimTimeSpinBox(*m_timing.m_updateTime, 1e-3, 1e0, 1e-2, box);
	CHECK_PTR(m_updateTime);
	connect(m_updateSynchronized, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));

	// Fixes a problem with the truncated layout on the right side! Why? TODO
	setRightColSpacing(4 * margin());
	
	slotCheckBoxToggled();
	connect(this,SIGNAL(signalChangeData()), this, SLOT(slotUndo()));
}

SimulationTimingWidget::~SimulationTimingWidget()
{
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
*/
void SimulationTimingWidget::acceptPressed()
{
	if (*m_timing.m_execTime != m_execTime->value())
	{
		changeData();
		*m_timing.m_execTime = m_execTime->value();
	}
	if (*m_timing.m_updateTime != m_updateTime->value())
	{
		changeData();
		*m_timing.m_updateTime = m_updateTime->value();
	}
	if (*m_timing.m_tickTime != m_tickTime->value())
	{
		changeData();
		*m_timing.m_tickTime = m_tickTime->value();
	}
	if (m_timing.m_execFastest != m_execFastest->isChecked())
	{
		changeData();
		m_timing.m_execFastest = m_execFastest->isChecked();
	}
	if (m_timing.m_tickSynchronized != m_tickSynchronized->isChecked())
	{
		changeData();
		m_timing.m_tickSynchronized = m_tickSynchronized->isChecked();
	}
	if (m_timing.m_updateSynchronized != m_updateSynchronized->isChecked())
	{
		changeData();
		m_timing.m_updateSynchronized = m_updateSynchronized->isChecked();
	}

  m_timing.updateTickTime();
}

/** The function defaultPressed() is called, if user wants to set the default values.
	You have to reimplement this function, if you add new properties.
 */
void SimulationTimingWidget::defaultPressed()
{
	m_execTime->setValue(KSimTimeBase(0.01, unit_sec));
	m_execFastest->setChecked(false);
	
	m_tickTime->setValue(KSimTimeBase(0.01, unit_sec));
	m_tickSynchronized->setChecked(true);
	
	m_updateTime->setValue(KSimTimeBase(0.05, unit_sec));
	m_updateSynchronized->setChecked(false);
}

void SimulationTimingWidget::slotCheckBoxToggled()
{
	if (m_execFastest->isChecked())
	{
		m_tickSynchronized->setEnabled(false);
		m_updateSynchronized->setEnabled(false);
	}
	else
	{
		m_tickSynchronized->setEnabled(true);
		m_updateSynchronized->setEnabled(true);
	}
	
	if (m_tickSynchronized->isChecked() || m_updateSynchronized->isChecked())
	{
		m_execFastest->setEnabled(false);
	}
	else
	{
		m_execFastest->setEnabled(true);
	}

	m_execTime->setDisabled(m_execFastest->isChecked());
	m_tickTime->setDisabled(m_tickSynchronized->isChecked());
	m_updateTime->setDisabled(m_updateSynchronized->isChecked());
}

/** Stores data for undo mechanism */
void SimulationTimingWidget::slotUndo()
{
	KSimUndo * undo = m_timing.m_doc->getUndo();
	if(undo)
	{
		undo->changeDocumentProperty(i18n("Change Timing"));
		m_timing.m_doc->setModified();
	}
}

#include "simulationtimingwidget.moc"
