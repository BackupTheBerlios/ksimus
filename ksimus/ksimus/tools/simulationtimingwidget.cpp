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
#include <qlayout.h>
#include <qlabel.h>
#include <qcheckbox.h>

// KDE-Includes
#include <kdialog.h>
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
	:	PropertyWidget(parent,name),
		m_timing(timing)
{
  QLabel * execLabel;
  QLabel * tickLabel;
  QLabel * updateLabel;
	QGridLayout * layout;
	
	execLabel = new QLabel (i18n("Execution speed:"), this);
	m_execFastest = new QCheckBox(i18n("Fast as possible"), this);
	m_execFastest->setChecked(m_timing.m_execFastest);
/*	m_execTime = new KSimDoubleUnitSpinBox(1e-3, 1e0, 1e-2, this);
	m_execTime->getMultiUnitList().append(KSimUnitListStatic::getTimeUnitList());
	m_execTime->setNewExponentSteps(1.0,2.0,5.0);
	m_execTime->setValue(m_timing.m_execTime->getValue(unit_sec));*/
	m_execTime = new KSimTimeSpinBox(*m_timing.m_execTime, 1e-3, 1e0, 1e-2, this);
	connect(m_execFastest, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));

	tickLabel = new QLabel (i18n("Simulation time per tick:"), this);
	m_tickSynchronized = new QCheckBox(i18n("Sychronized to execution speed"), this);
	m_tickSynchronized->setChecked(m_timing.m_tickSynchronized);
/*	m_tickTime = new KSimDoubleUnitSpinBox(1e-10, 1e0, 1e-2, this);
	m_tickTime->getMultiUnitList().append(KSimUnitListStatic::getTimeUnitList());
	m_tickTime->setNewExponentSteps(1.0,2.0,5.0);
	m_tickTime->setValue(m_timing.m_tickTime->getValue(unit_sec));*/
	m_tickTime = new KSimTimeSpinBox(*m_timing.m_tickTime, 1e-10, 1e0, 1e-2, this);
	connect(m_tickSynchronized, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));

	updateLabel = new QLabel (i18n("View update speed:"), this);
	m_updateSynchronized = new QCheckBox(i18n("Sychronized to execution speed"), this);
	m_updateSynchronized->setChecked(m_timing.m_updateSynchronized);
/*	m_updateTime = new KSimDoubleUnitSpinBox(1e-3, 1e0, 1e-2, this);
	m_updateTime->getMultiUnitList().append(KSimUnitListStatic::getTimeUnitList());
	m_updateTime->setNewExponentSteps(1.0,2.0,5.0);
	m_updateTime->setValue(m_timing.m_updateTime->getValue(unit_sec));*/
	m_updateTime = new KSimTimeSpinBox(*m_timing.m_updateTime, 1e-3, 1e0, 1e-2, this);
	connect(m_updateSynchronized, SIGNAL(toggled(bool)), this, SLOT(slotCheckBoxToggled()));

	layout = new QGridLayout(this,6,2);	
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	layout->colStretch(1);

	layout->addWidget(execLabel,0,0);
	layout->addWidget(m_execFastest,0,1);
	layout->addWidget(m_execTime,1,1);

	layout->addWidget(tickLabel,2,0);
	layout->addWidget(m_tickSynchronized,2,1);
	layout->addWidget(m_tickTime,3,1);

	layout->addWidget(updateLabel,4,0);
	layout->addWidget(m_updateSynchronized,4,1);
	layout->addWidget(m_updateTime,5,1);
	
	slotCheckBoxToggled();
	connect(this,SIGNAL(signalChangeData()), this, SLOT(slotUndo()));
}

SimulationTimingWidget::~SimulationTimingWidget()
{
}

/** The function slotAccept() is called, if changes are accepted.
	You have to reimplement this slot, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
*/
void SimulationTimingWidget::acceptPressed()
{
//	if (m_timing.m_execTime->getValue(unit_sec) != m_execTime->value())
	if (*m_timing.m_execTime != m_execTime->value())
	{
		changeData();
//		m_timing.m_execTime->setValue(m_execTime->value(), unit_sec);
		*m_timing.m_execTime = m_execTime->value();
	}
//	if (m_timing.m_updateTime->getValue(unit_sec) != m_updateTime->value())
	if (*m_timing.m_updateTime != m_updateTime->value())
	{
		changeData();
//		m_timing.m_updateTime->setValue(m_updateTime->value(), unit_sec);
		*m_timing.m_updateTime = m_updateTime->value();
	}
//	if (m_timing.m_tickTime->getValue(unit_sec) != m_tickTime->value())
	if (*m_timing.m_tickTime != m_tickTime->value())
	{
		changeData();
//		m_timing.m_tickTime->setValue(m_tickTime->value(), unit_sec);
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

/** The function slotDefault() is called, if user wants to set the default values.
	You have to reimplement this slot, if you add new properties.
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
