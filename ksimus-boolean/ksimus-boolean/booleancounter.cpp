/***************************************************************************
                          booleancounter.cpp  -  description
                             -------------------
    begin                : Wed Dec 19 2001
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
#include <math.h>

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>
#include <qvbox.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "booleancounter.h"
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorpack.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentpropertydialog.h"
#include "ksimus/optionalconnector.h"

// Forward declaration

// Project-Includes


namespace KSimLibBoolean
{

Component * BooleanCounter::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanCounter(container, ci);
}

const ComponentInfo * BooleanCounter::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Counter with RCO and RBO"),
	                                QString::fromLatin1("Boolean/Counter/with RCO and RBO"),
	                                i18n("Component", "Boolean/Counter/with RCO and RBO"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-counter-rco-rbo"));
	return &Info;
}

//###############################################################
//###############################################################

#define MIN_BIT         2
#define MAX_BIT        16
#define DEFAULT_BIT     4
#define DEFAULT_MIN    "0x0"
#define DEFAULT_MAX    "0xF"
#define DEFAULT_RESET  "0x0"



//###############################################################
//###############################################################

BooleanCounter::BooleanCounter(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_cnt(0),
		m_maxCount(DEFAULT_MAX),
		m_minCount(DEFAULT_MIN),
		m_resetCount(DEFAULT_RESET)
{
	// Create connectors
	
	m_inClear = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Reset"),
	                             i18n("Boolean-Connector", "Reset"));
	CHECK_PTR(m_inClear);
	m_inClear->setEdgeSensitive(false,true);
	// make optional
	new OptionalConnector(m_inClear,
	                      QString::fromLatin1("Reset Input"),
	                      i18n("Boolean", "Reset input:"));
	
	
	m_inClkUp = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Up"),
	                             i18n("Boolean-Connector", "Clock Up"));
	CHECK_PTR(m_inClkUp);
	m_inClkUp->setEdgeSensitiveChangeEnable(false);
	// make optional
	new OptionalConnector(true, m_inClkUp,
	                      QString::fromLatin1("Clock up input"),
	                      i18n("Boolean", "Clock up input:"));
	
	
	
	
	m_inClkDown = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Down"),
	                             i18n("Boolean-Connector", "Clock Down"));
	CHECK_PTR(m_inClkDown);
	m_inClkDown->setEdgeSensitiveChangeEnable(false);
	// make optional
	new OptionalConnector(true, m_inClkDown,
	                      QString::fromLatin1("Clock down input"),
	                      i18n("Boolean", "Clock down input:"));
	
	
	
	m_outCnt = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output %1"),
	                             getConnectorBoolOutInfo(),
	                             MIN_BIT, MAX_BIT);
	CHECK_PTR(m_outCnt);
	m_outCnt->setConnectorCount(DEFAULT_BIT);
	
	m_outBorrow = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Ripple Borrow Out"),
	                             i18n("Boolean-Connector", "Ripple Borrow Out"));
	CHECK_PTR(m_outBorrow);
	// make optional
	new OptionalConnector(true, m_outBorrow,
	                      QString::fromLatin1("Ripple borrow output"),
	                      i18n("Boolean", "Ripple borrow output:"));
	
	
	m_outCarry = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Ripple Carry Out"),
	                             i18n("Boolean-Connector", "Ripple Carry Out"));
	CHECK_PTR(m_outCarry);
	// make optional
	new OptionalConnector(true, m_outCarry,
	                      QString::fromLatin1("Ripple carry output"),
	                      i18n("Boolean", "Ripple carry output:"));
	
	
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanCounterView(this, SHEET_VIEW);
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
	getOutputCounter()->getAction().disable(KSimAction::INITPOPUPMENU);
	
}

BooleanCounter::~BooleanCounter()
{
}

/** save component properties */
void BooleanCounter::save(KSimData & file) const
{
	Component::save(file);

	getMinCount().save(file, "Min Count");	
	getMaxCount().save(file, "Max Count");	
	getResetCount().save(file, "Reset Count");	
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool BooleanCounter::load(KSimData & file, bool copyLoad)
{
	bool res = Component::load(file, copyLoad);
	
	res &= getMinCount().load(file, "Min Count", DEFAULT_MIN);	
	res &= getMaxCount().load(file, "Max Count", KSimBaseUInt((1<<getBits())-1, KSimBaseUInt::Hexadecimal).text());	
	res &= getResetCount().load(file, "Reset Count", DEFAULT_RESET);	

	return res;
}

void BooleanCounter::checkProperty(QStringList & errorMsg)
{
	Component::checkProperty(errorMsg);
	
	if (getInputClockUp()->isHidden() && getInputClockDown()->isHidden())
	{
		errorMsg.append(i18n("Boolean", "Connector 'Clock Up' or 'Clock Down' must be visible."));
	}
}

void BooleanCounter::reset()
{
	Component::reset();
	
	m_cnt = getResetCount();
	
	m_borrow = false;
	m_carry = false;
	
	setOutput();
}


void BooleanCounter::calculate()
{
	Component::calculate();

	bool clear(getInputClear()->getInput() && !getInputClear()->isHidden());	
	bool up(getInputClockUp()->getInput() && !getInputClockUp()->isHidden());	
	bool down(getInputClockDown()->getInput() && !getInputClockDown()->isHidden());
	
	bool updateRequiered(m_borrow || m_carry);
	
	m_borrow = false;
	m_carry = false;
	
	if (clear)
	{
		m_cnt = getMinCount();
		updateRequiered = true;
	}
	else
	{
		if (up && down)
		{
			// do thing
		}
		else if (up)
		{
			if (m_cnt < getMaxCount())
			{
				m_cnt++;
				updateRequiered = true;
			}
			else
			{
				m_cnt = getMinCount();
				m_carry = true;
				updateRequiered = true;
			}
		}
		else if (down)
		{
			if (m_cnt > getMinCount())
			{
				m_cnt--;
				updateRequiered = true;
			}
			else
			{
				m_cnt = getMaxCount();
				m_borrow = true;
				updateRequiered = true;
			}
		}
	}
	if (updateRequiered)
	{
		setOutput();
	}
}


void BooleanCounter::setOutput() const
{
	unsigned int mask = 1;
	FOR_EACH_CONNECTOR(it, *getOutputCounter()->getConnList())
	{
		((ConnectorBoolOut*)it.current())->setOutput(mask & m_cnt);
		mask <<= 1;
	}
	
	getOutputBorrow()->setOutput(m_borrow);
	getOutputCarry()->setOutput(m_carry);
}


void BooleanCounter::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog (dialog);
	
	QVBox * page;
	BooleanCounterPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Boolean", "Counter"));
	wid = new BooleanCounterPropertyWidget(this, page, "Counter");
	dialog->connectSlots(wid);
}

unsigned int BooleanCounter::getBits() const
{
	return getOutputCounter()->getConnectorCount();
}

void BooleanCounter::setBits(unsigned int bits)
{
	getOutputCounter()->setConnectorCount(bits);
	if (getSheetView())
	{
		((BooleanCounterView*)getSheetView())->getComponentLayout()->updateLayout();
	}
}


unsigned int BooleanCounter::getMinBits() const
{
	return getOutputCounter()->getNumberOfNotDeletableConnectors();
}


//###############################################################
//###############################################################

BooleanCounterView::BooleanCounterView(BooleanCounter * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayout(this);
		CHECK_PTR(m_layout);
		
		m_ctrlBlock = new ComponentControlBlock(this, m_layout);
		CHECK_PTR(m_ctrlBlock);
		
		m_ctrlBlock->getLeft()->addSpace(1);
		m_ctrlBlock->getLeft()->addConnector(getCounter()->getInputClear());
		m_ctrlBlock->getLeft()->addConnector(getCounter()->getInputClockUp());
		m_ctrlBlock->getLeft()->addConnector(getCounter()->getInputClockDown());
//		m_ctrlBlock->getLeft()->addSpace(1);
		
		m_ctrlBlock->getRight()->addSpace(1);
		m_ctrlBlock->getRight()->addConnector(getCounter()->getOutputCarry());
		m_ctrlBlock->getRight()->addConnector(getCounter()->getOutputBorrow());
//		m_ctrlBlock->getRight()->addSpace(1);
		
		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnectorPack(getCounter()->getOutputCounter());
//		m_layout->getRight()->addSpace(1);
		
		m_layout->setMinSize(7,7);
		m_layout->updateLayout();
	
		new ConnectorLabel(getCounter()->getInputClear(), "R");
		new ConnectorLabel(getCounter()->getInputClockUp(), "Up");
		new ConnectorLabel(getCounter()->getInputClockDown(), "Do");

		new ConnectorLabel(getCounter()->getOutputCarry(), "RC");
		new ConnectorLabel(getCounter()->getOutputBorrow(), "RB");
		
/*		m_ctrlBlock->setText("CTR");
		m_ctrlBlock->setAlign((AlignmentFlags)(AlignHCenter | AlignTop));*/
		
		
		unsigned int i = 1;
		FOR_EACH_CONNECTOR(it, *getCounter()->getOutputCounter()->getConnList())
		{
			new ConnectorLabel(it.current(), QChar('A' - 1 + i));
			i ++;
		}
		connect(getCounter()->getOutputCounter(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addConn(ConnectorBase *)));
	}
}


void BooleanCounterView::draw(QPainter * p)
{
	CompView::draw(p);
	
//	QRect rect(getWidgetPlace());
	QRect rect(getDrawingPlace());
	rect.rLeft() ++;
	rect.rTop() += 1 + m_ctrlBlock->getRect(false).bottom();
	rect.rBottom() ++;
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawRect(rect);
	
	QFont newFont("helvetica",8);
	p->setFont(newFont);
	p->setPen(black);
	p->drawText(rect, (AlignHCenter | AlignTop), "CTR");
}
	
void BooleanCounterView::addConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QChar('A' - 1 + getCounter()->getOutputCounter()->getConnectorCount()));
}

//###############################################################
//###############################################################


//###############################################################
//###############################################################

BooleanCounterPropertyWidget::BooleanCounterPropertyWidget(BooleanCounter * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 2, parent, name),
		m_noBitChanges(false)
{
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QLabel * lab;
	QString str;	
	
	// Bits
	lab = new QLabel(i18n("Boolean", "Counter width:"), this);
	CHECK_PTR(lab);
	m_bits = new QSpinBox(this, "Bits");
	CHECK_PTR(m_bits);
	m_bits->setValue(getCounter()->getBits());
	m_bits->setRange(getCounter()->getMinBits() ,16);
	m_bits->setSuffix(i18n("Boolean", " bit"));
	str = i18n("Boolean", "Sets the counter width.");
	addToolTip(str, lab, m_bits);
	addWhatsThis(str, lab, m_bits);

	// Maximum count
	lab = new QLabel(i18n("Boolean", "Maximum:"), this);
	CHECK_PTR(lab);
	m_maxValue = new KSimBaseUIntLineEdit(getCounter()->getMaxCount(), this, "Maximum");
	CHECK_PTR(m_maxValue);
	str = i18n("Boolean", "Sets the maximum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_maxValue);
	str += m_maxValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_maxValue);

	// Minimum count
	lab = new QLabel(i18n("Boolean", "Minimum:"), this);
	CHECK_PTR(lab);
	m_minValue = new KSimBaseUIntLineEdit(getCounter()->getMinCount(), this, "Minimum");
	CHECK_PTR(m_minValue);
	str = i18n("Boolean", "Sets the minimum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_minValue);
	str += m_minValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_minValue);

	// Reset value
	lab = new QLabel(i18n("Boolean", "Reset value:"), this);
	CHECK_PTR(lab);
	m_resetValue = new KSimBaseUIntLineEdit(getCounter()->getResetCount(), this, "Reset value");
	CHECK_PTR(m_resetValue);
	str = i18n("Boolean", "Sets the reset counter value.\nThis value is also used if the counter is reseted.");
	addToolTip(str, lab, m_resetValue);
	str += m_resetValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_resetValue);

	
	// Hint bit count
	str = i18n("Boolean", "Hint: minimum counter width is %1 bit").arg(getCounter()->getMinBits());
	lab = new QLabel(str, newRowVBox());
	CHECK_PTR(lab);
	str = i18n("Boolean", "This is the minimal possible counter width.\nThis value depends on wired connectors.");
	addToolTip(str, lab);
	addWhatsThis(str, lab);
	
	connect(m_resetValue, SIGNAL(changed()), this, SLOT(slotResetValueChanged()));
	connect(m_minValue, SIGNAL(changed()), this, SLOT(slotMinValueChanged()));
	connect(m_maxValue, SIGNAL(changed()), this, SLOT(slotMaxValueChanged()));
	connect(m_bits, SIGNAL(valueChanged(int)), this, SLOT(slotBitsChanged()));
}

BooleanCounterPropertyWidget::~BooleanCounterPropertyWidget()
{
}

void BooleanCounterPropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();
                                         
	if (getCounter()->getResetCount() != getResetValueWidget()->value())
	{
		changeData();
		getCounter()->getResetCount() = getResetValueWidget()->value();
	}
	
	if (getCounter()->getMinCount() != getMinValueWidget()->value())
	{
		changeData();
		getCounter()->getMinCount() = getMinValueWidget()->value();
	}
	
	if (getCounter()->getMaxCount() != getMaxValueWidget()->value())
	{
		changeData();
		getCounter()->getMaxCount() = getMaxValueWidget()->value();
	}
	
	if (getCounter()->getBits() != (unsigned int)getBitsWidget()->value())
	{
		changeData();
		getCounter()->setBits(getBitsWidget()->value());
	}
}

void BooleanCounterPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();
	
	getBitsWidget()->setValue(getCounter()->getBits());
	
	getMinValueWidget()->setText("0x0");
	getResetValueWidget()->setText("0x0");
	
	getMaxValueWidget()->setText(QString("0x%1").arg((1 << getCounter()->getBits())-1, 16));
}


void BooleanCounterPropertyWidget::slotMinValueChanged()
{
//	KSIMDEBUG("BooleanCounterPropertyWidget::slotMinValueChanged()");
	// Limit values
	limit();
}

void BooleanCounterPropertyWidget::slotMaxValueChanged()
{
	unsigned int requiredBit = (unsigned int)ceil(log(getMaxValueWidget()->value()+1)/log(2.0));
	
//	KSIMDEBUG("BooleanCounterPropertyWidget::slotMaxValueChanged()");
	if (requiredBit <= MAX_BIT)
	{
		unsigned int newBit = QMAX(requiredBit, getCounter()->getMinBits());
		// Set bit count
		if (newBit != (unsigned int)getBitsWidget()->value())
		{
			m_noBitChanges = true;
			getBitsWidget()->setValue(newBit);
			m_noBitChanges = false;
		}
	}
	else
	{
		// limit max value
		unsigned int maxCnt = (1 << getBitsWidget()->value()) -1;
		getMaxValueWidget()->setValue(maxCnt);
	}
	
	// Limit values
	limit();
}

void BooleanCounterPropertyWidget::slotBitsChanged()
{
	if (!m_noBitChanges)
	{
		unsigned int maxCnt = (1 << getBitsWidget()->value()) -1;
		getMaxValueWidget()->setValue(maxCnt);
	}
	
	// Limit values
	limit();
}

void BooleanCounterPropertyWidget::slotResetValueChanged()
{
	// Limit values
	limit();
}

void BooleanCounterPropertyWidget::limit()
{
	if (getBitsWidget()->value() > MAX_BIT)
	{
		getBitsWidget()->setValue(MAX_BIT);
	}
	else if (getBitsWidget()->value() < MIN_BIT)
	{
		getBitsWidget()->setValue(MIN_BIT);
	}

	unsigned int maxCnt = (1 << getBitsWidget()->value()) -1;
	
	if (getMaxValueWidget()->value() > maxCnt)
	{
		getMaxValueWidget()->setValue(maxCnt);
	}
	else if (getMaxValueWidget()->value() < 1)
	{
		getMaxValueWidget()->setValue(1);
	}
	
	if (getMinValueWidget()->value() >= getMaxValueWidget()->value())
	{
		getMinValueWidget()->setValue(getMaxValueWidget()->value()-1);
	}


	if (getResetValueWidget()->value() > getMaxValueWidget()->value())
	{
		getResetValueWidget()->setValue(getMaxValueWidget()->value());
	}

	if (getResetValueWidget()->value() < getMinValueWidget()->value())
	{
		getResetValueWidget()->setValue(getMinValueWidget()->value());
	}
}
	
//###############################################################


#undef MIN_BIT
#undef MAX_BIT
#undef DEFAULT_BIT
#undef DEFAULT_MIN
#undef DEFAULT_MAX
#undef DEFAULT_RESET


}; //namespace KSimLibBoolean

