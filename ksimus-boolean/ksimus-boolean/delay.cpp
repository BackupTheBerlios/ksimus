/***************************************************************************
                          delay.cpp  -  description
                             -------------------
    begin                : Tue Jun 18 2002
    copyright            : (C) 2002 by Rasmus Diekenbrock
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

// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimtimespinbox.h"

// Project-Includes
#include "delay.h"

// Forward declaration



namespace KSimLibBoolean
{


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Delay(container, ci);
}

const ComponentInfo * getDelayInfo()
{
	static const ComponentInfo Info(i18n("Component", "Delay"),
	                                QString::fromLatin1("Boolean/Timing/Delay"),
	                                i18n("Component", "Boolean/Timing/Delay"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null/*,TODO
	                                QString::fromLatin1("component-boolean-monoflop")*/);
	return &Info;
}


//###############################################################
//###############################################################

#define MAX_DEPTH    (1<<16)
static const char * sDelayTime   = "Delay Time/";


Delay::Delay(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci),
		m_delayTime(getTimeServer())
{

	m_input = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input"));
	CHECK_PTR(m_input);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new DelayView(this, SHEET_VIEW);
	}
	
	m_delayTime.setValue(1, unit_sec);
	getAction().disable(KSimAction::UPDATEVIEW);
}
	
/*Delay::~Delay()
{
}*/

void Delay::calculate()
{
	Boolean1Out::calculate();
	
	if (m_list.size() < 1)
	{
		setState(getInputConnector()->getInput());
		return;
	}
	
	setState(m_list[m_index]);
	
	bool in = getInputConnector()->getInput();
	if (in)
	{
		m_list.setBit(m_index);
	}
	else
	{
		m_list.clearBit(m_index);
	}
	
	if (++m_index >= m_list.size())
	{
		m_index = 0;
	}
	
	if (in != m_lastState)
	{
		m_lastState = in;
		m_counter = 0;
		executeNext();
	}
	else if (m_counter < m_list.size())
	{
		m_counter++;
		executeNext();
	}
	else
	{
		// m_list is filled with one value
	}
	
}

/** Reset all simulation variables */
void Delay::reset()
{
	Boolean1Out::reset();
	
	unsigned long listLength = (unsigned long)(m_delayTime.getValue(unit_ticks) + 0.5);
	
	if(listLength > MAX_DEPTH)
	{
		listLength = MAX_DEPTH;
		KSimTime myTime(m_delayTime);
		myTime.setValue((double)listLength, unit_ticks);
		logWarning(i18n("Boolean", "Delay limited: %1").arg(myTime.getAdjustValueString()));
	}
	else if(listLength > 0)
	{
		listLength--;
	}
	
	bool res = m_list.fill(getResetState(), listLength);
	if(!res)
	{
		KSIMDEBUG("Resize of QBitArray failed");
	}
	m_lastState = getResetState();
	setState(getResetState());
	m_counter = 0;
	m_index = 0;
}

/** save component properties */
void Delay::save(KSimData & file) const
{
	Boolean1Out::save(file);
	
	file.pushGroupRel(sDelayTime);
	
	m_delayTime.save(file);

	file.popGroup();
}
		

bool Delay::load(KSimData & file, bool copyLoad)
{
	bool ok;
	
	ok = Boolean1Out::load(file, copyLoad);
	
	if (file.hasGroupRel(sDelayTime))
	{
		file.pushGroupRel(sDelayTime);
		m_delayTime.load(file);
		file.popGroup();
	}
	else
	{
		m_delayTime.setValue(1.0, unit_sec);
	}
	
	return ok;
}

ComponentPropertyBaseWidget * Delay::createGeneralProperty(QWidget *parent)
{
	DelayPropertyGeneralWidget * wid;
	wid = new DelayPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

DelayView::DelayView(Delay * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getDelay()->getInputConnector()->setGridPos(0,1);
	getDelay()->getOutputConnector()->setGridPos(4,1);
}

/*DelayView::~DelayView()
{
}*/

void DelayView::draw(QPainter * p)
{
	drawFrame(p);
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	p->drawText(getDrawingPlace(), AlignCenter, "Dly");
	
	CompView::draw(p);
}

//###############################################################
//###############################################################



DelayPropertyGeneralWidget::DelayPropertyGeneralWidget(Delay * comp, QWidget *parent, const char *name)
	:	Boolean1OutPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;

	lab = new QLabel(i18n("Boolean", "Delay time:"), this);
	CHECK_PTR(lab);
	
	m_delayTime = new KSimTimeSpinBox(comp->getDelayTime(), this);
	CHECK_PTR(m_delayTime);
	lab->setBuddy(m_delayTime);
	KSimTime myTime(comp->getDelayTime());
	myTime.setValue((double)MAX_DEPTH, unit_ticks);
	m_delayTime->setMaxValue(myTime.getValue(unit_sec));
	myTime.setValue(1.0, unit_ticks);
	m_delayTime->setMinValue(myTime.getValue(unit_sec));
	
	str = i18n("Boolean", "Set here the duration auf the delay.");
	addToolTip(str, m_delayTime, lab);
	addWhatsThis(str, m_delayTime, lab);
	
	lab = new QLabel(i18n("Boolean - Hint in dialog", "Hint:"), this);
	CHECK_PTR(lab);
	
	m_delayTimeHint = new QLabel(this);
	CHECK_PTR(m_delayTimeHint);
	slotChanged(comp->getDelayTime());
	connect(m_delayTime, SIGNAL(changed(const KSimTime &)), this, SLOT(slotChanged(const KSimTime &)));
	
	str = i18n("Boolean - Hint in dialog", "Shows the effective delay time.\nThis time depends on the value of 'Simulation time per tick'.");
	addToolTip(str, m_delayTimeHint, lab);
	addWhatsThis(str, m_delayTimeHint, lab);
	
}

/*DelayPropertyGeneralWidget::~DelayPropertyGeneralWidget()
{
} */

void DelayPropertyGeneralWidget::acceptPressed()
{
	Boolean1OutPropertyGeneralWidget::acceptPressed();

	if (getDelay()->getDelayTime() != m_delayTime->value())
	{
		changeData();
		getDelay()->setDelayTime(m_delayTime->value());
	}
}

void DelayPropertyGeneralWidget::defaultPressed()
{
	Boolean1OutPropertyGeneralWidget::defaultPressed();

	m_delayTime->setValue(KSimTimeBase(1.0, unit_sec));
}

void DelayPropertyGeneralWidget::slotChanged(const KSimTime & time)
{
	unsigned int ticks = QMAX(qRound(time.getValue(unit_ticks)), 1);
	
	KSimTime myTime(time);
	myTime.setValue((double)ticks, unit_ticks);
	
	m_delayTimeHint->setText(i18n("Boolean - prints delay in a suitable time unit and ticks",
	                              "effective delay time %1 / %2 ticks")
	                         .arg(myTime.getAdjustValueString())
	                         .arg(ticks));
}

//##########################################################################################
//##########################################################################################

	
}; //namespace