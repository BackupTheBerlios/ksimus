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
#include "ksimus/resource.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorpack.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/componentlayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentpropertydialog.h"
#include "ksimus/optionalconnector.h"
#include "ksimus/ksimbaseuintedit.h"
#include "booleancounter.h"

// Forward declaration



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
#define DEFAULT_MIN    QString::fromLatin1("0x0")
#define DEFAULT_MAX    QString::fromLatin1("0xF")
#define DEFAULT_RESET  QString::fromLatin1("0x0")



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
	Q_CHECK_PTR(m_inClear);
	m_inClear->setEdgeSensitive(false,true);
	// make optional
	new OptionalConnector(m_inClear,
	                      QString::fromLatin1("Reset Input"),
	                      i18n("Boolean", "Reset input:"));
	
	
	m_inClkUp = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Up"),
	                             i18n("Boolean-Connector", "Clock Up"));
	Q_CHECK_PTR(m_inClkUp);
	m_inClkUp->setEdgeSensitiveChangeEnable(false);
	// make optional
	new OptionalConnector(true, m_inClkUp,
	                      QString::fromLatin1("Clock up input"),
	                      i18n("Boolean", "Clock up input:"));
	
	
	
	
	m_inClkDown = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Down"),
	                             i18n("Boolean-Connector", "Clock Down"));
	Q_CHECK_PTR(m_inClkDown);
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
	Q_CHECK_PTR(m_outCnt);
	m_outCnt->setConnectorCount(DEFAULT_BIT);
	
	m_outBorrow = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Ripple Borrow Out"),
	                             i18n("Boolean-Connector", "Ripple Borrow Out"));
	Q_CHECK_PTR(m_outBorrow);
	// make optional
	new OptionalConnector(true, m_outBorrow,
	                      QString::fromLatin1("Ripple borrow output"),
	                      i18n("Boolean", "Ripple borrow output:"));
	
	
	m_outCarry = new ConnectorBoolOut(this,
	                             QString::fromLatin1("Ripple Carry Out"),
	                             i18n("Boolean-Connector", "Ripple Carry Out"));
	Q_CHECK_PTR(m_outCarry);
	// make optional
	new OptionalConnector(true, m_outCarry,
	                      QString::fromLatin1("Ripple carry output"),
	                      i18n("Boolean", "Ripple carry output:"));
	
	
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanCounterView(this, SHEET_VIEW, "BooleanCounterSV");
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

BooleanCounterView::BooleanCounterView(BooleanCounter * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayoutVerticalCtrl(this);
		Q_CHECK_PTR(m_layout);

		m_layout->getCtrlBlock()->getLeft()->addSpace(1);
		m_layout->getCtrlBlock()->getLeft()->addConnector(getCounter()->getInputClear());
		m_layout->getCtrlBlock()->getLeft()->addConnector(getCounter()->getInputClockUp());
		m_layout->getCtrlBlock()->getLeft()->addConnector(getCounter()->getInputClockDown());
//		m_layout->getCtrlBlock()->getLeft()->addSpace(1);
		
		m_layout->getCtrlBlock()->getRight()->addSpace(1);
		m_layout->getCtrlBlock()->getRight()->addConnector(getCounter()->getOutputCarry());
		m_layout->getCtrlBlock()->getRight()->addConnector(getCounter()->getOutputBorrow());
//		m_layout->getCtrlBlock()->getRight()->addSpace(1);
		
		m_layout->getFuncBlock()->getRight()->addSpace(1);
		m_layout->getFuncBlock()->getRight()->addConnectorPack(getCounter()->getOutputCounter());
		
		new ComponentLayoutBlockContentText(m_layout->getFuncBlock(), QString::fromLatin1("CTR"), AlignTop|AlignHCenter);
		
		m_layout->setMinSize(5,7);
	
		new ConnectorLabel(getCounter()->getInputClear(), QString::fromLatin1("R"));
		new ConnectorLabel(getCounter()->getInputClockUp(), QString::fromLatin1("Up"));
		new ConnectorLabel(getCounter()->getInputClockDown(), QString::fromLatin1("Do"));

		new ConnectorLabel(getCounter()->getOutputCarry(), QString::fromLatin1("RC"));
		new ConnectorLabel(getCounter()->getOutputBorrow(), QString::fromLatin1("RB"));
		
		unsigned int i = 1;
		FOR_EACH_CONNECTOR(it, *getCounter()->getOutputCounter()->getConnList())
		{
			new ConnectorLabel(it.current(), QChar('A' - 1 + i));
			i ++;
		}
		connect(getCounter()->getOutputCounter(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addConn(ConnectorBase *)));
	}
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
	Q_CHECK_PTR(lab);
	m_bits = new KSimSpinBox(this, "Bits");
	Q_CHECK_PTR(m_bits);
	m_bits->setValue(getCounter()->getBits());
	m_bits->setRange(getCounter()->getMinBits() ,16);
	m_bits->setSuffix(i18n("Boolean", " bit"));
	str = i18n("Boolean", "Sets the counter width.");
	addToolTip(str, lab, m_bits);
	addWhatsThis(str, lab, m_bits);
	lab->setBuddy(m_bits);

	// Maximum count
	lab = new QLabel(i18n("Boolean", "Maximum:"), this);
	Q_CHECK_PTR(lab);
	m_maxValue = new KSimBaseUIntEdit(getCounter()->getMaxCount(), this, "Maximum");
	Q_CHECK_PTR(m_maxValue);
	str = i18n("Boolean", "Sets the maximum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_maxValue);
	str += m_maxValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_maxValue);
	lab->setBuddy(m_maxValue);

	// Minimum count
	lab = new QLabel(i18n("Boolean", "Minimum:"), this);
	Q_CHECK_PTR(lab);
	m_minValue = new KSimBaseUIntEdit(getCounter()->getMinCount(), this, "Minimum");
	Q_CHECK_PTR(m_minValue);
	str = i18n("Boolean", "Sets the minimum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_minValue);
	str += m_minValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_minValue);
	lab->setBuddy(m_minValue);

	// Reset value
	lab = new QLabel(i18n("Boolean", "Reset value:"), this);
	Q_CHECK_PTR(lab);
	m_resetValue = new KSimBaseUIntEdit(getCounter()->getResetCount(), this, "Reset value");
	Q_CHECK_PTR(m_resetValue);
	str = i18n("Boolean", "Sets the reset counter value.\nThis value is also used if the counter is reseted.");
	addToolTip(str, lab, m_resetValue);
	str += m_resetValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_resetValue);
	lab->setBuddy(m_resetValue);

	
	// Hint bit count
	str = i18n("Boolean", "Hint: minimum counter width is %1 bit").arg(getCounter()->getMinBits());
	lab = new QLabel(str, newRowVBox());
	Q_CHECK_PTR(lab);
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
	
	getMinValueWidget()->setText(QString::fromLatin1("0x0"));
	getResetValueWidget()->setText(QString::fromLatin1("0x0"));
	
	getMaxValueWidget()->setText(QString::fromLatin1("0x%1").arg((1 << getCounter()->getBits())-1, 0, 16));
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

#include "booleancounter.moc"
