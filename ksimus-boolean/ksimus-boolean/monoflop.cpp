/***************************************************************************
                          monoflop.cpp  -  description
                             -------------------
    begin                : Tue Dec 11 2001
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
#include <qlabel.h>
#include <qtooltip.h>
#include <qwhatsthis.h>


// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimtimespinbox.h"
#include "ksimus/ksimbooleanbox.h"

// Project-Includes
#include "monoflop.h"

// Forward declaration



namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new MonoFlop(container, ci);
}

const ComponentInfo * getMonoFlopInfo()
{
	static const ComponentInfo Info(i18n("Component", "Mono Flop"),
	                                QString::fromLatin1("Boolean/Flip Flop/Mono Flop"),
	                                i18n("Component", "Boolean/Flip Flop/Mono Flop"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-monoflop"));
	return &Info;
}


//###############################################################
//###############################################################

static const char * sHighTime   = "High Time/";
static const char * sRetrigger  = "Retrigger";


MonoFlop::MonoFlop(CompContainer * container, const ComponentInfo * ci)
	: FlipFlopBase(container, ci),
		m_retrigger(true),
		m_highTime(getTimeServer()),
		m_time(getTimeServer())
{

//	getSetInputConnector()->setEdgeSensitive(true,true);
	getSetInputConnector()->setName(i18n("Boolean", "Trigger"), true);

	getResetInputConnector()->setEdgeSensitive(false,true);
	getResetInputConnector()->setHideEnabled(true);
	getResetInputConnector()->setHide(true,true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new MonoFlopView(this, SHEET_VIEW);
	}
	
	m_highTime.setValue(0.5, unit_sec);
	getAction().disable(KSimAction::UPDATEVIEW);
}
	
/*MonoFlop::~MonoFlop()
{
}*/

void MonoFlop::calculate()
{
	FlipFlopBase::calculate();
	
	bool trigger = getSetInputConnector()->getInput();
	bool reset = getResetInputConnector()->getInput() && !getResetInputConnector()->isHidden();

	if(getTimeServer().getTime() >= m_time)
	{
		setState(false);
	}
	
	if (trigger && (getDominant() || !reset) && (getRetrigger() || !getState()))
	{
		setState(true);
		m_time = getTimeServer().getTime();
		m_time += m_highTime;
	}
	else if (reset)
	{
		setState(false);
		m_time = getTimeServer().getTime();
	}
	
	executeNext();
}

/** Reset all simulation variables */
void MonoFlop::reset()
{
	FlipFlopBase::reset();
	
	if (getResetState())
	{
		m_time = m_highTime;
	}
	else
	{
		m_time.setValue(0.0, unit_sec);
	}
}

/** save component properties */
void MonoFlop::save(KSimData & file) const
{
	FlipFlopBase::save(file);
	
	QString oldGroup(file.group());
	
	file.setGroup(oldGroup + sHighTime);
	m_highTime.save(file);

	file.setGroup(oldGroup);
	
	if (!getRetrigger())   // Save false state only (default is true!)
	{
		file.writeEntry(sRetrigger, false);
	}
}
		

bool MonoFlop::load(KSimData & file, bool copyLoad)
{
	QString oldGroup(file.group());
	QString newGroup;
	bool ok;
	
	ok = FlipFlopBase::load(file, copyLoad);
	
	setRetrigger( file.readBoolEntry(sRetrigger, true) );
	
	newGroup = oldGroup + sHighTime;
	if (file.hasGroup(newGroup))
	{
		file.setGroup(newGroup);
		m_highTime.load(file);
	}
	else
	{
		m_highTime.setValue(500.0, unit_msec);
	}
		
	file.setGroup(oldGroup);
	
	return ok;
}

ComponentPropertyBaseWidget * MonoFlop::createGeneralProperty(QWidget *parent)
{
	MonoFlopPropertyGeneralWidget * wid;
	wid = new MonoFlopPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

#define getMF() ((MonoFlop *) getComponent())

MonoFlopView::MonoFlopView(MonoFlop * comp, eViewType viewType)
	: CompView(comp, viewType)
{
//	setPlace(QRect(0, 0, 7*gridX, 7*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		new ConnectorLabel(getMF()->getOutputConnector(), "Q");
		new ConnectorLabel(getMF()->getNotOutputConnector(), "/Q");
		new ConnectorLabel(getMF()->getSetInputConnector(), "T");
		new ConnectorLabel(getMF()->getResetInputConnector(), "R");
	
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
		
		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnector(getMF()->getSetInputConnector());
		m_layout->getLeft()->addConnector(getMF()->getResetInputConnector());

		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnector(getMF()->getOutputConnector());
		m_layout->getRight()->addStretch(3);
		m_layout->getRight()->addConnector(getMF()->getNotOutputConnector());
		m_layout->getRight()->addStretch(1);
			
//		m_layout->setMinSize(5,6);
		m_layout->updateLayout();
	}
}

/*MonoFlopView::~MonoFlopView()
{
}*/

void MonoFlopView::draw(QPainter * p)
{
  drawFrame(p);
	
	CompView::draw(p);
}

//###############################################################
//###############################################################



MonoFlopPropertyGeneralWidget::MonoFlopPropertyGeneralWidget(MonoFlop * comp, QWidget *parent, const char *name)
	:	FlipFlopBasePropertyGeneralWidget(comp, parent, name)
{
	
	getDominantBox()->setTrueText(i18n("Boolean", "Trigger Input"));
	getDominantBox()->setFalseText(i18n("Boolean", "Reset Input"));
	
	
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Boolean", "Retrigger:"), this);
	CHECK_PTR(lab);
	
	m_retrigger = new KSimBooleanBox(comp->getRetrigger(), this);
	CHECK_PTR(m_retrigger);
	
	m_retrigger->setTrueText(i18n("Boolean", "On"));
	m_retrigger->setFalseText(i18n("Boolean", "Off"));
	
	str = i18n("Boolean", "Set here the retrigger mode.");
	QToolTip::add(m_retrigger, str);
	QToolTip::add(lab, str);
	QWhatsThis::add(m_retrigger, str);
	QWhatsThis::add(lab, str);
	
	lab = new QLabel(i18n("Boolean", "Time:"), this);
	CHECK_PTR(lab);
	
	m_highTime = new KSimTimeSpinBox(comp->getHighTime(), this);
	CHECK_PTR(m_highTime);
	
	str = i18n("Boolean", "Set here the duration.");
	QToolTip::add(m_highTime, str);
	QToolTip::add(lab, str);
	QWhatsThis::add(m_highTime, str);
	QWhatsThis::add(lab, str);

}

/*MonoFlopPropertyGeneralWidget::~MonoFlopPropertyGeneralWidget()
{
} */

void MonoFlopPropertyGeneralWidget::acceptPressed()
{
	FlipFlopBasePropertyGeneralWidget::acceptPressed();

	if (getMF()->getRetrigger() != m_retrigger->getValue())
	{
		changeData();
		getMF()->setRetrigger(m_retrigger->getValue());
	}
	
	if (getMF()->getHighTime() != m_highTime->value())
	{
		changeData();
		getMF()->setHighTime(m_highTime->value());
	}
}

void MonoFlopPropertyGeneralWidget::defaultPressed()
{
	FlipFlopBasePropertyGeneralWidget::defaultPressed();

	m_retrigger->setValue(false);
	m_highTime->setValue(KSimTimeBase(0.5, unit_sec));
}

#undef getMF()

//##########################################################################################
//##########################################################################################

	
}; //namespace
