/***************************************************************************
                          clockgenerator.cpp  -  description
                             -------------------
    begin                : Sun Mar 4 2001
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

// QT-Includes
#include <qpainter.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qlayout.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "ksimdebug.h"
#include "clockgenerator.h"
#include "resource.h"
#include "connectorboolout.h"
#include "ksimtime.h"
#include "ksimtimeserver.h"
#include "componentpropertydialog.h"
#include "ksimtimespinbox.h"

// Forward declaration

// Project-Includes


const char * const ClockGenerator::sStartDelay = "Start Delay/";
const char * const ClockGenerator::sHighTime   = "High Time/";
const char * const ClockGenerator::sLowTime    = "Low Time/";

Component * ClockGenerator::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ClockGenerator(container, ci);
}

const ComponentInfo * ClockGenerator::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Clock Generator"),
	                                QString::fromLatin1("Boolean/Gates/Clock Generator"),
	                                i18n("Component", "Boolean/Gates/Clock Generator"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-clock-generator"));
	return &Info;
}

//###############################################################
//###############################################################


ClockGeneratorView::ClockGeneratorView(Component * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 4*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
//	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

void ClockGeneratorView::draw(QPainter * p)
{
	CompView::draw(p);
	
	drawFrame(p);

	// 8 Steps (width)
	#define XSTEP      (((4*gridX - 1*gridX) - 2) / 8)
	#define XPOS(step) ((XSTEP * step) + 0*gridX + gridX/2 -1)
	// heigth
	#define YPOS(high) ((high ? 1*gridY : 2*gridY) -1)
	
	
	p->setPen(QPen(black, 1));
	p->drawLine(XPOS(0),YPOS(0),XPOS(1),YPOS(0));
	p->drawLine(XPOS(1),YPOS(0),XPOS(1),YPOS(1));
	p->drawLine(XPOS(1),YPOS(1),XPOS(3),YPOS(1));
	p->drawLine(XPOS(3),YPOS(1),XPOS(3),YPOS(0));
	p->drawLine(XPOS(3),YPOS(0),XPOS(5),YPOS(0));
	p->drawLine(XPOS(5),YPOS(0),XPOS(5),YPOS(1));
	p->drawLine(XPOS(5),YPOS(1),XPOS(7),YPOS(1));
	p->drawLine(XPOS(7),YPOS(1),XPOS(7),YPOS(0));
	p->drawLine(XPOS(7),YPOS(0),XPOS(8),YPOS(0));
	
	#undef YPOS
	#undef XPOS
	#undef XSTEP
}


//###############################################################

ClockGenerator::ClockGenerator(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_startDelay(getTimeServer()),
		m_highTime(getTimeServer()),
		m_lowTime(getTimeServer()),
		m_time(getTimeServer())
{
	m_out = new ConnectorBoolOut (this,
	                              QString::fromLatin1("Output"),
	                              i18n("Connector", "Output"),
	                              QPoint(3,1));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ClockGeneratorView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	
	m_startDelay.setValue(1.0, unit_sec);
	m_highTime.setValue(0.5, unit_sec);
	m_lowTime.setValue(0.5, unit_sec);
	
}

ClockGenerator::~ClockGenerator()
{
}

/** Executes the simulation of this component */

void ClockGenerator::calculate()
{
	Component::calculate();
	if(getTimeServer() >= m_time)
	{
		if(m_result)
		{
			m_result = false;
			m_time += m_lowTime;
		}
		else
		{
			m_result = true;
			m_time += m_highTime;
		}
		m_out->setOutput(m_result);
	}
	executeAt(0, m_time);
//	KSIMDEBUG_VAR("executeAt", m_time.getValue(unit_msec));
//	executeNext();
}

/** Reset all simulation variables */
void ClockGenerator::reset()
{
	Component::reset();
	if (m_startDelay.getValue(unit_tenth_nsec) != 0.0)
	{
		m_time = m_startDelay;
		m_result = false;
	}
	else
	{
		m_time = m_highTime;
		m_result = true;
	}
	m_out->setOutput(m_result);
}

void ClockGenerator::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog (dialog);
	
	QVBox * page;
	ClockGeneratorPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Component property dialog","Timing"));
	wid = new ClockGeneratorPropertyWidget(this, page, "Timing");
	dialog->connectSlots(wid);
}

/** save component properties */
void ClockGenerator::save(KSimData & file) const
{
	Component::save(file);
	
	QString oldGroup(file.group());
	
	file.setGroup(oldGroup + QString::fromLatin1(sStartDelay));
	m_startDelay.save(file);

	file.setGroup(oldGroup + QString::fromLatin1(sHighTime));
	m_highTime.save(file);

	file.setGroup(oldGroup + QString::fromLatin1(sLowTime));
	m_lowTime.save(file);

	file.setGroup(oldGroup);
}
		

bool ClockGenerator::load(KSimData & file, bool copyLoad)
{
	QString oldGroup(file.group());
	QString newGroup;
	bool ok;
	
	ok = Component::load(file, copyLoad);
	
	newGroup = oldGroup + QString::fromLatin1(sStartDelay);
	if (file.hasGroup(newGroup))
	{
		file.setGroup(newGroup);
		m_startDelay.load(file);
	}
	else
	{
		m_startDelay.setValue(0.0, unit_msec);
	}
		
	newGroup = oldGroup + QString::fromLatin1(sHighTime);
	if (file.hasGroup(newGroup))
	{
		file.setGroup(newGroup);
		m_highTime.load(file);
	}
	else
	{
		m_highTime.setValue(500.0, unit_msec);
	}
		
	newGroup = oldGroup + QString::fromLatin1(sLowTime);
	if (file.hasGroup(newGroup))
	{
		file.setGroup(newGroup);
		m_lowTime.load(file);
	}
	else
	{
		m_lowTime.setValue(500.0, unit_msec);
	}
		
	file.setGroup(oldGroup);
	
	return ok;
}

//###############################################################
//###############################################################
//###############################################################


ClockGeneratorPropertyWidget::ClockGeneratorPropertyWidget(ClockGenerator * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 2, parent, name)
{
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QLabel * lab;
	QString str;	
	
	lab = new QLabel(i18n("Start delay: "), this);
	CHECK_PTR(lab);
	
	m_startDelay = new KSimTimeSpinBox(getClockGen()->m_startDelay, this, "StartDelay");
	CHECK_PTR(m_startDelay);
	m_startDelay->setExtraZeroAllowed(true);
	m_startDelay->setValue(getClockGen()->m_startDelay);
	str = i18n("Delay before clock starts.");
	addToolTip(str, lab, m_startDelay);
	addWhatsThis(str, lab, m_startDelay);

	lab = new QLabel(i18n("High time: "), this);
	CHECK_PTR(lab);
	m_highTime = new KSimTimeSpinBox(getClockGen()->m_highTime, this, "HighTime");
	CHECK_PTR(m_highTime);
	str = i18n("Time of high level.");
	addToolTip(str, lab, m_highTime);
	addWhatsThis(str, lab, m_highTime);

	lab = new QLabel(i18n("Low time: "), this);
	CHECK_PTR(lab);
	m_lowTime = new KSimTimeSpinBox(getClockGen()->m_lowTime, this, "LowTime");
	CHECK_PTR(m_lowTime);
	str = i18n("Time of low level.");
	addToolTip(str, lab, m_lowTime);
	addWhatsThis(str, lab, m_lowTime);
}

ClockGeneratorPropertyWidget::~ClockGeneratorPropertyWidget()
{
}

void ClockGeneratorPropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	if (getClockGen()->m_startDelay != m_startDelay->value())
	{
		changeData();
		getClockGen()->m_startDelay = m_startDelay->value();
	}
	if (getClockGen()->m_highTime != m_highTime->value())
	{
		changeData();
		getClockGen()->m_highTime = m_highTime->value();
	}
	if (getClockGen()->m_lowTime != m_lowTime->value())
	{
		changeData();
		getClockGen()->m_lowTime = m_lowTime->value();
	}
}

void ClockGeneratorPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	m_startDelay->setValue(getClockGen()->m_startDelay);
	m_highTime->setValue(getClockGen()->m_highTime);
	m_lowTime->setValue(getClockGen()->m_lowTime);
}

#include "clockgenerator.moc"
