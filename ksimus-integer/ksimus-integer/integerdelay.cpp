/***************************************************************************
                          integerdelay.cpp  -  description
                             -------------------
    begin                : Sat Aug 2 2003
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

// C-Includes

// QT-Includes
#include <qlabel.h>
#include <qhbox.h>


// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimtimeserver.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimtimespinbox.h"
#include "ksimus/ksimembfont.h"

// Project-Includes
#include "integerdelay.h"

// Forward declaration



namespace KSimLibInteger
{


Component * IntegerDelay::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerDelay(container, ci);
}

const ComponentInfo * IntegerDelay::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Delay"),
	                                QString::fromLatin1("Integer/Timing/Delay"),
	                                i18n("Component", "Integer/Timing/Delay"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-timing-delay")*/);
	return &Info;
}


//###############################################################
//###############################################################

const unsigned long IntegerDelay::maxDepth  = (1<<16);
const char * const IntegerDelay::strDelayTime  = "Delay Time/";

/*const unsigned long IntegerDelay_maxDepth  = (1<<16);
const char * const IntegerDelay_DelayTime  = "Delay Time/";*/


IntegerDelay::IntegerDelay(CompContainer * container, const ComponentInfo * ci)
	: Integer1In1Out(container, ci),
		m_delayTime(getTimeServer())
{
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW, "IntegerDelaySV");
	}
	
	m_delayTime.setValue(1, unit_sec);
	getAction().disable(KSimAction::UPDATEVIEW);
}
	
/*IntegerDelay::~IntegerDelay()
{
}*/

void IntegerDelay::calculate()
{
	Integer1In1Out::calculate();
	
	if (m_list.size() < 1)
	{
		setValue(getInputConnector()->getInput());
		return;
	}
	
	setValue(m_list[m_index]);
	
	const int in = getInputConnector()->getInput();
	m_list[m_index] = in;
	
	if (++m_index >= m_list.size())
	{
		m_index = 0;
	}
	
	if (in != m_lastValue)
	{
		m_lastValue = in;
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
void IntegerDelay::reset()
{
	Integer1In1Out::reset();
	
	unsigned long listLength = (unsigned long)(m_delayTime.getValue(unit_ticks) + 0.5);
	
	if(listLength > maxDepth)
	{
		listLength = maxDepth;
		KSimTime myTime(m_delayTime);
		myTime.setValue((double)listLength, unit_ticks);
		logWarning(i18n("Integer", "Delay limited: %1").arg(myTime.getAdjustValueString()));
	}
	else if(listLength > 0)
	{
		listLength--;
	}
	
	m_lastValue = getResetValue().value();
	bool res = m_list.fill(m_lastValue, listLength);
	if(!res)
	{
		KSIMDEBUG("Resize of QArray failed");
	}
	setValue(m_lastValue);
	m_counter = m_list.size();
	m_index = 0;
}

/** save component properties */
void IntegerDelay::save(KSimData & file) const
{
	Integer1In1Out::save(file);
	
	file.pushGroupRel(strDelayTime);
	
	m_delayTime.save(file);

	file.popGroup();
}


bool IntegerDelay::load(KSimData & file, bool copyLoad)
{
	bool ok;
	
	ok = Integer1In1Out::load(file, copyLoad);
	
	if (file.hasGroupRel(strDelayTime))
	{
		file.pushGroupRel(strDelayTime);
		m_delayTime.load(file);
		file.popGroup();
	}
	else
	{
		m_delayTime.setValue(1.0, unit_sec);
	}
	
	return ok;
}

ComponentPropertyBaseWidget * IntegerDelay::createGeneralProperty(QWidget *parent)
{
	IntegerDelay::PropertyGeneralWidget * wid;
	wid = new IntegerDelay::PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

IntegerDelay::View::View(IntegerDelay * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getIntegerDelay()->getInputConnector()->setGridPos(0,2);
	getIntegerDelay()->getOutputConnector()->setGridPos(4,2);
}

/*IntegerDelay::View::~View()
{
}*/

void IntegerDelay::View::draw(QPainter * p)
{
	drawFrame(p);
	KSimEmbFont::getFont10()->drawText(p, getDrawingPlace(), AlignCenter, QString::fromLatin1("Dly"));
	
	CompView::draw(p);
}

//###############################################################
//###############################################################



IntegerDelay::PropertyGeneralWidget::PropertyGeneralWidget(IntegerDelay * comp, QWidget *parent, const char *name)
	:	Integer1In1OutPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;

	lab = new QLabel(i18n("Integer", "Delay time:"), this);
	CHECK_PTR(lab);
	
	m_delayTime = new KSimTimeSpinBox(comp->getDelayTime(), this);
	CHECK_PTR(m_delayTime);
	lab->setBuddy(m_delayTime);
	KSimTime myTime(comp->getDelayTime());
	myTime.setValue((double)maxDepth, unit_ticks);
	m_delayTime->setMaxValue(myTime.getValue(unit_sec));
	myTime.setValue(1.0, unit_ticks);
	m_delayTime->setMinValue(myTime.getValue(unit_sec));
	
	str = i18n("Integer", "Set here the duration auf the delay.");
	addToolTip(str, m_delayTime, lab);
	addWhatsThis(str, m_delayTime, lab);
	
	QHBox * hintBox = newRowHBox("hintBox");

	m_delayTimeHint = new QLabel(hintBox, "m_delayTimeHint");
	CHECK_PTR(m_delayTimeHint);
	slotChanged(comp->getDelayTime());
	connect(m_delayTime, SIGNAL(changed(const KSimTime &)), this, SLOT(slotChanged(const KSimTime &)));
	
	str = i18n("Integer - Hint in dialog", "Shows the effective delay time.\nThis time depends on the value of 'Simulation time per tick'.");
	addToolTip(str, m_delayTimeHint);
	addWhatsThis(str, m_delayTimeHint);
	
}

/*IntegerDelay::PropertyGeneralWidget::~PropertyGeneralWidget()
{
} */

void IntegerDelay::PropertyGeneralWidget::acceptPressed()
{
	Integer1In1OutPropertyGeneralWidget::acceptPressed();

	if (getIntegerDelay()->getDelayTime() != m_delayTime->value())
	{
		changeData();
		getIntegerDelay()->setDelayTime(m_delayTime->value());
	}
}

void IntegerDelay::PropertyGeneralWidget::defaultPressed()
{
	Integer1In1OutPropertyGeneralWidget::defaultPressed();

	m_delayTime->setValue(KSimTimeBase(1.0, unit_sec));
}

void IntegerDelay::PropertyGeneralWidget::slotChanged(const KSimTime & time)
{
	unsigned int ticks = QMAX(qRound(time.getValue(unit_ticks)), 1);
	
	KSimTime myTime(time);
	myTime.setValue((double)ticks, unit_ticks);
	
	m_delayTimeHint->setText(i18n("Integer - prints delay in a suitable time unit and ticks",
	                              "Effective delay time: %1 or %2 ticks")
	                         .arg(myTime.getAdjustValueString())
	                         .arg(ticks));
}

//##########################################################################################
//##########################################################################################

	
}; //namespace

#include "integerdelay.moc"
