/***************************************************************************
                          integercounter.cpp  -  description
                             -------------------
    begin                : Sun Aug 17 2003
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
#include <limits.h>

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qcheckbox.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentpropertydialog.h"
#include "ksimus/optionalconnector.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/ksimdata.h"
#include "integercounter.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{

//###############################################################
//###############################################################

IntegerCounterBase::IntegerCounterBase(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_cnt(0),
		m_wrapping(true),
		m_defaultMaxCount(KSimBaseInt(255, KSimBaseInt::Decimal)),
		m_defaultMinCount(KSimBaseInt(0, KSimBaseInt::Decimal)),
		m_defaultResetCount(KSimBaseInt(0, KSimBaseInt::Decimal)),
		m_maxCount(m_defaultMaxCount),
		m_minCount(m_defaultMinCount),
		m_resetCount(m_defaultResetCount)
{
	// Create connectors
	
	m_inClear = new ConnectorBoolInEdge(this,
	                                    QString::fromLatin1("Reset"),
	                                    i18n("Integer-Connector", "Reset"));
	CHECK_PTR(m_inClear);
	m_inClear->setEdgeSensitive(false,true);
	// make optional
	new OptionalConnector(m_inClear,
	                      QString::fromLatin1("Reset Input"),
	                      i18n("Integer-Connector", "Reset input:"));
	
	m_output = new ConnectorIntegerOut(this,
	                                   QString::fromLatin1("Output"),
	                                   i18n("Integer-Connector", "Output"));
	CHECK_PTR(m_output);

	m_outputMin = new ConnectorBoolOut(this,
	                                    QString::fromLatin1("Count Min"),
	                                    i18n("Integer-Connector", "Count Min"));
	CHECK_PTR(m_outputMin);
	// make optional
	new OptionalConnector(m_outputMin,
	                      QString::fromLatin1("Count Minimum Output"),
	                      i18n("Integer-Connector", "Count Minimum Output:"));

	m_outputMax = new ConnectorBoolOut(this,
	                                    QString::fromLatin1("Count Max"),
	                                    i18n("Integer-Connector", "Count Max"));
	CHECK_PTR(m_outputMax);
	// make optional
	new OptionalConnector(m_outputMax,
	                      QString::fromLatin1("Count Maximum Output"),
	                      i18n("Integer-Connector", "Count Maximum Output:"));
}

IntegerCounterBase::~IntegerCounterBase()
{
}

/** save component properties */
void IntegerCounterBase::save(KSimData & file) const
{
	Component::save(file);

	getMinCount().save(file, "Min Count");
	getMaxCount().save(file, "Max Count");
	getResetCount().save(file, "Reset Count");
	file.writeEntry("Wraparound", m_wrapping);
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerCounterBase::load(KSimData & file, bool copyLoad)
{
	if(!m_minCount.load(file, "Min Count"))
	{
		m_minCount = getDefaultMinCount();
	}
	if(!m_maxCount.load(file, "Max Count"))
	{
		m_maxCount = getDefaultMaxCount();
	}
	if(!m_resetCount.load(file, "Reset Count"))
	{
		m_resetCount = getDefaultResetCount();
	}
	m_wrapping = file.readBoolEntry("Wraparound", true);


	return Component::load(file, copyLoad);
}

void IntegerCounterBase::checkProperty(QStringList & errorMsg)
{
	Component::checkProperty(errorMsg);
	
	if ((getMinCount().value() <= getResetCount().value()) && (getResetCount().value() <= getMaxCount().value()))
	{
		// Okay
	}
	else
	{
		errorMsg.append(i18n("The reset, minimum and maximum values do not match!\n"
		                     "The values have to match following condition:\n"
		                     "\tMinimum Value <= Reset Value <= Maximum Value\n"));
	}
}

void IntegerCounterBase::reset()
{
	Component::reset();
	
	m_cnt = getResetCount();
	
	setOutput();
}


void IntegerCounterBase::setOutput() const
{
	getOutput()->setOutput(m_cnt);
	getOutputMin()->setOutput(m_cnt <= getMinCount().value());
	getOutputMax()->setOutput(m_cnt >= getMaxCount().value());
}


void IntegerCounterBase::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog (dialog);
	
	QVBox * page;
	IntegerCounterBasePropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Integer", "Counter"));
	wid = new IntegerCounterBasePropertyWidget(this, page, "Counter");
	CHECK_PTR(wid);
	dialog->connectSlots(wid);
}


//###############################################################
//###############################################################

IntegerCounterBaseView::IntegerCounterBaseView(IntegerCounterBase * comp, eViewType viewType)
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
		m_ctrlBlock->getLeft()->addConnector(getCounterBase()->getInputClear());

		m_ctrlBlock->getRight()->addSpace(1);
		m_ctrlBlock->getRight()->addConnector(getCounterBase()->getOutputMin());
		m_ctrlBlock->getRight()->addConnector(getCounterBase()->getOutputMax());

		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnector(getCounterBase()->getOutput());
		
		m_layout->setMinSize(7,7);
		m_layout->updateLayout();
	
		new ConnectorLabel(getCounterBase()->getInputClear(), "R");
		new ConnectorLabel(getCounterBase()->getOutputMin(), "Min");
		new ConnectorLabel(getCounterBase()->getOutputMax(), "Max");
	}
}


void IntegerCounterBaseView::draw(QPainter * p)
{
	CompView::draw(p);
	
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

//###############################################################
//###############################################################

IntegerCounterBasePropertyWidget::IntegerCounterBasePropertyWidget(IntegerCounterBase * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 2, parent, name)
{
//	setColStretch(0,0);
	setColStretch(1,1);
	
	QLabel * lab;
	QString str;	
	
	// Reset value
	lab = new QLabel(i18n("Integer", "Reset value:"), this);
	CHECK_PTR(lab);
	m_resetValue = new KSimBaseIntEdit(getCounterBase()->getResetCount(), this, "Reset value");
	CHECK_PTR(m_resetValue);
	str = i18n("Integer", "Sets the reset counter value.\nThis value is also used if the counter is reseted.");
	addToolTip(str, lab, m_resetValue);
	str += m_resetValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_resetValue);
	lab->setBuddy(m_resetValue);

	// Maximum count
	lab = new QLabel(i18n("Integer", "Maximum:"), this);
	CHECK_PTR(lab);
	m_maxValue = new KSimBaseIntEdit(getCounterBase()->getMaxCount(), this, "Maximum");
	CHECK_PTR(m_maxValue);
	str = i18n("Integer", "Sets the maximum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_maxValue);
	str += m_maxValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_maxValue);
	lab->setBuddy(m_maxValue);

	// Minimum count
	lab = new QLabel(i18n("Integer", "Minimum:"), this);
	CHECK_PTR(lab);
	m_minValue = new KSimBaseIntEdit(getCounterBase()->getMinCount(), this, "Minimum");
	CHECK_PTR(m_minValue);
	str = i18n("Integer", "Sets the minimum counter value.\nThis value is used if the counter wraps around.");
	addToolTip(str, lab, m_minValue);
	str += m_minValue->getWhatsThisHelp();
	addWhatsThis(str, lab, m_minValue);
	lab->setBuddy(m_minValue);

	// Swap a round
	QHBox * wrapBox = newRowHBox("wrapBox");
	CHECK_PTR(wrapBox);
	m_wrapping = new QCheckBox(i18n("Integer", "Wrap around"), wrapBox, "m_wrapping");
	CHECK_PTR(m_wrapping);
	m_wrapping->setChecked(getCounterBase()->isWrappingEnabled());
	str = i18n("Enables the wrap around function.\n"
	           "If enabled it is possible to step from 'Minimum value' to 'Maximum value' and vice versa.");
	addToolTip(str, m_wrapping);
	addWhatsThis(str, m_wrapping);

}

IntegerCounterBasePropertyWidget::~IntegerCounterBasePropertyWidget()
{
}

void IntegerCounterBasePropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();
                                         
	if (getCounterBase()->getResetCount() != getResetValueWidget()->value())
	{
		changeData();
		getCounterBase()->getResetCount() = getResetValueWidget()->value();
	}
	
	if (getCounterBase()->getMinCount() != getMinValueWidget()->value())
	{
		changeData();
		getCounterBase()->getMinCount() = getMinValueWidget()->value();
	}
	
	if (getCounterBase()->getMaxCount() != getMaxValueWidget()->value())
	{
		changeData();
		getCounterBase()->getMaxCount() = getMaxValueWidget()->value();
	}

	if (getCounterBase()->isWrappingEnabled() != getWrappingWidget()->isChecked())
	{
		changeData();
		getCounterBase()->setWrappingEnabled(getWrappingWidget()->isChecked());
	}
}

void IntegerCounterBasePropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	getMinValueWidget()->setValue(getCounterBase()->getDefaultMinCount());
	getMaxValueWidget()->setValue(getCounterBase()->getDefaultMaxCount());
	getResetValueWidget()->setValue(getCounterBase()->getDefaultResetCount());
	getWrappingWidget()->setChecked(true);
}

//###############################################################
//###############################################################


Component * IntegerCounterClkUpClkDown::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerCounterClkUpClkDown(container, ci);
}

const ComponentInfo * IntegerCounterClkUpClkDown::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Counter with Clock Up and Down"),
	                                QString::fromLatin1("Integer/Counter/with Clock Up and Clock Down"),
	                                i18n("Component", "Integer/Counter/with Clock Up and Clock Down"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-counter-rco-rbo")*/);
	return &Info;
}

IntegerCounterClkUpClkDown::IntegerCounterClkUpClkDown(CompContainer * container, const ComponentInfo * ci)
	:	IntegerCounterBase(container, ci)
{
	// Create connectors

	m_inClkUp = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Up"),
	                             i18n("Integer-Connector", "Clock Up"));
	CHECK_PTR(m_inClkUp);
	m_inClkUp->setEdgeSensitiveChangeEnable(false);
	// make optional
	new OptionalConnector(true, m_inClkUp,
	                      QString::fromLatin1("Clock Up input"),
	                      i18n("Integer-Connector", "Clock Up input:"));

	m_inClkDown = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock Down"),
	                             i18n("Integer-Connector", "Clock Down"));
	CHECK_PTR(m_inClkDown);
	m_inClkDown->setEdgeSensitiveChangeEnable(false);
	// make optional
	new OptionalConnector(true, m_inClkDown,
	                      QString::fromLatin1("Clock Down input"),
	                      i18n("Integer-Connector", "Clock Down input:"));

	// Initializes the sheet view
	if (getSheetMap() && (ci == getStaticInfo()))
	{
		new IntegerCounterClkUpClkDownView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

IntegerCounterClkUpClkDown::~IntegerCounterClkUpClkDown()
{
}

void IntegerCounterClkUpClkDown::calculate()
{
//	IntegerCounterBase::calculate();

	bool clear(getInputClear()->getInput() && !getInputClear()->isHidden());
	bool up(getInputClockUp()->getInput() && !getInputClockUp()->isHidden());
	bool down(getInputClockDown()->getInput() && !getInputClockDown()->isHidden());

	if (clear)
	{
		m_cnt = getMinCount();
		setOutput();
	}
	else if (up && !down)
	{
		if (m_cnt < getMaxCount())
		{
			m_cnt++;
			setOutput();
		}
		else if (m_wrapping)
		{
			m_cnt = getMinCount();
			setOutput();
		}
	}
	else if (!up && down)
	{
		if (m_cnt > getMinCount())
		{
			m_cnt--;
			setOutput();
		}
		else if (m_wrapping)
		{
			m_cnt = getMaxCount();
			setOutput();
		}
	}
}

void IntegerCounterClkUpClkDown::checkProperty(QStringList & errorMsg)
{
	IntegerCounterBase::checkProperty(errorMsg);

	if (getInputClockUp()->isHidden() && getInputClockDown()->isHidden())
	{
		errorMsg.append(i18n("Integer", "Connector 'Clock Up' or 'Clock Down' have to be visible."));
	}
}



//###############################################################
//###############################################################

IntegerCounterClkUpClkDownView::IntegerCounterClkUpClkDownView(IntegerCounterClkUpClkDown * comp, eViewType viewType)
	: IntegerCounterBaseView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getControlBlock()->getLeft()->addConnector(getCounterClkUpClkDown()->getInputClockUp());
		getControlBlock()->getLeft()->addConnector(getCounterClkUpClkDown()->getInputClockDown());
//		getControlBlock()->getLeft()->addSpace(1);

		getComponentLayout()->setMinSize(7,7);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getCounterClkUpClkDown()->getInputClockUp(), "Up");
		new ConnectorLabel(getCounterClkUpClkDown()->getInputClockDown(), "Do");
	}
}


//###############################################################
//###############################################################


Component * IntegerCounterClkUpClkDownLoad::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerCounterClkUpClkDownLoad(container, ci);
}

const ComponentInfo * IntegerCounterClkUpClkDownLoad::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Counter with Clock Up, Clock Down and Load"),
	                                QString::fromLatin1("Integer/Counter/with Clock Up, Clock Down and Load"),
	                                i18n("Component", "Integer/Counter/with Clock Up, Clock Down and Load"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-counter-rco-rbo")*/);
	return &Info;
}

IntegerCounterClkUpClkDownLoad::IntegerCounterClkUpClkDownLoad(CompContainer * container, const ComponentInfo * ci)
	:	IntegerCounterClkUpClkDown(container, ci)
{
	// Create connectors
	m_inLoadEnable = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Load Enable"),
	                             i18n("Integer-Connector", "Load Enable"));
	CHECK_PTR(m_inLoadEnable);

	m_inCounter = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Load Input"),
	                             i18n("Integer-Connector", "Load Input"));
	CHECK_PTR(m_inCounter);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerCounterClkUpClkDownLoadView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

IntegerCounterClkUpClkDownLoad::~IntegerCounterClkUpClkDownLoad()
{
}

void IntegerCounterClkUpClkDownLoad::calculate()
{
//	IntegerCounterClkUpClkDown::calculate();

	bool clear(getInputClear()->getInput() && !getInputClear()->isHidden());
	bool loadEna(getInputLoadEnable()->getInput());
	bool up(getInputClockUp()->getInput() && !getInputClockUp()->isHidden());
	bool down(getInputClockDown()->getInput() && !getInputClockDown()->isHidden());

	if (clear)
	{
		m_cnt = getMinCount();
		setOutput();
	}
	else if (loadEna)
	{
		int newCnt = getInputCounter()->getInput();
		if (newCnt < getMinCount())
		{
			m_cnt = getMinCount();
		}
		else if (newCnt > getMaxCount())
		{
			m_cnt = getMaxCount();
		}
		else
		{
			m_cnt = newCnt;
		}
		setOutput();
	}
	else if (up && !down)
	{
		if (m_cnt < getMaxCount())
		{
			m_cnt++;
			setOutput();
		}
		else if (m_wrapping)
		{
			m_cnt = getMinCount();
			setOutput();
		}
	}
	else if (!up && down)
	{
		if (m_cnt > getMinCount())
		{
			m_cnt--;
			setOutput();
		}
		else if (m_wrapping)
		{
			m_cnt = getMaxCount();
			setOutput();
		}
	}
}



//###############################################################
//###############################################################

IntegerCounterClkUpClkDownLoadView::IntegerCounterClkUpClkDownLoadView(IntegerCounterClkUpClkDownLoad * comp, eViewType viewType)
	: IntegerCounterClkUpClkDownView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
//		getControlBlock()->getLeft()->addSpace(1);
		getControlBlock()->getLeft()->addConnector(getCounterClkUpClkDownLoad()->getInputLoadEnable());

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(getCounterClkUpClkDownLoad()->getInputCounter());

		getComponentLayout()->setMinSize(7,7);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getCounterClkUpClkDownLoad()->getInputLoadEnable(), "Ld");
	}
}


//###############################################################
//###############################################################


Component * IntegerCounterClkDir::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerCounterClkDir(container, ci);
}

const ComponentInfo * IntegerCounterClkDir::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Counter with Clock and Direction"),
	                                QString::fromLatin1("Integer/Counter/with Clock and Direction"),
	                                i18n("Component", "Integer/Counter/with Clock and Direction"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-counter-rco-rbo")*/);
	return &Info;
}

IntegerCounterClkDir::IntegerCounterClkDir(CompContainer * container, const ComponentInfo * ci)
	:	IntegerCounterBase(container, ci)
{
	// Create connectors

	m_inDir = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Direction"),
	                             i18n("Integer-Connector", "Direction"));
	CHECK_PTR(m_inDir);
	// make optional
	new OptionalConnector(true, m_inDir,
	                      QString::fromLatin1("Direction"),
	                      i18n("Integer-Connector", "Direction input:"));

	m_inClk = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Clock"),
	                             i18n("Integer-Connector", "Clock"));
	CHECK_PTR(m_inClk);
	m_inClk->setEdgeSensitiveChangeEnable(false);

	// Initializes the sheet view
	if (getSheetMap() && (ci == getStaticInfo()))
	{
		new IntegerCounterClkDirView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

IntegerCounterClkDir::~IntegerCounterClkDir()
{
}

void IntegerCounterClkDir::calculate()
{
//	IntegerCounterBase::calculate();

	bool clear(getInputClear()->getInput() && !getInputClear()->isHidden());
	bool clk(getInputClock()->getInput());

	if (clear)
	{
		m_cnt = getMinCount();
		setOutput();
	}
	else if (clk)
	{
		if (getInputDir()->getInput())
		{
			// Down
			if (m_cnt > getMinCount())
			{
				m_cnt--;
				setOutput();
			}
			else if (m_wrapping)
			{
				m_cnt = getMaxCount();
				setOutput();
			}
		}
		else
		{
			// Up
			if (m_cnt < getMaxCount())
			{
				m_cnt++;
				setOutput();
			}
			else if (m_wrapping)
			{
				m_cnt = getMinCount();
				setOutput();
			}
		}
	}
}


//###############################################################
//###############################################################

IntegerCounterClkDirView::IntegerCounterClkDirView(IntegerCounterClkDir * comp, eViewType viewType)
	: IntegerCounterBaseView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getControlBlock()->getLeft()->addConnector(getCounterClkDir()->getInputDir());
		getControlBlock()->getLeft()->addConnector(getCounterClkDir()->getInputClock());
//		getControlBlock()->getLeft()->addSpace(1);

		getComponentLayout()->setMinSize(7,7);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getCounterClkDir()->getInputDir(), "Dir");
		new ConnectorLabel(getCounterClkDir()->getInputClock(), "Clk");
	}
}

//###############################################################
//###############################################################


Component * IntegerCounterClkDirLoad::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerCounterClkDirLoad(container, ci);
}

const ComponentInfo * IntegerCounterClkDirLoad::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Counter with Clock, Direction and Load"),
	                                QString::fromLatin1("Integer/Counter/with Clock, Direction and Load"),
	                                i18n("Component", "Integer/Counter/with Clock, Direction and Load"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-counter-rco-rbo")*/);
	return &Info;
}

IntegerCounterClkDirLoad::IntegerCounterClkDirLoad(CompContainer * container, const ComponentInfo * ci)
	:	IntegerCounterClkDir(container, ci)
{
	// Create connectors
	m_inLoadEnable = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Load Enable"),
	                             i18n("Integer-Connector", "Load Enable"));
	CHECK_PTR(m_inLoadEnable);

	m_inCounter = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Load Input"),
	                             i18n("Integer-Connector", "Load Input"));
	CHECK_PTR(m_inCounter);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerCounterClkDirLoadView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

IntegerCounterClkDirLoad::~IntegerCounterClkDirLoad()
{
}

void IntegerCounterClkDirLoad::calculate()
{
//	IntegerCounterClkDir::calculate();

	bool clear(getInputClear()->getInput() && !getInputClear()->isHidden());
	bool loadEna(getInputLoadEnable()->getInput());
	bool clk(getInputClock()->getInput());

	if (clear)
	{
		m_cnt = getMinCount();
		setOutput();
	}
	else if (loadEna)
	{
		int newCnt = getInputCounter()->getInput();
		if (newCnt < getMinCount())
		{
			m_cnt = getMinCount();
		}
		else if (newCnt > getMaxCount())
		{
			m_cnt = getMaxCount();
		}
		else
		{
			m_cnt = newCnt;
		}
		setOutput();
	}
	else if (clk)
	{
		if (getInputDir()->getInput())
		{
			// Down
			if (m_cnt > getMinCount())
			{
				m_cnt--;
				setOutput();
			}
			else if (m_wrapping)
			{
				m_cnt = getMaxCount();
				setOutput();
			}
		}
		else
		{
			// Up
			if (m_cnt < getMaxCount())
			{
				m_cnt++;
				setOutput();
			}
			else if (m_wrapping)
			{
				m_cnt = getMinCount();
				setOutput();
			}
		}
	}
}



//###############################################################
//###############################################################

IntegerCounterClkDirLoadView::IntegerCounterClkDirLoadView(IntegerCounterClkDirLoad * comp, eViewType viewType)
	: IntegerCounterClkDirView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
//		getControlBlock()->getLeft()->addSpace(1);
		getControlBlock()->getLeft()->addConnector(getCounterClkDirLoad()->getInputLoadEnable());

		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(getCounterClkDirLoad()->getInputCounter());

		getComponentLayout()->setMinSize(7,7);
		getComponentLayout()->updateLayout();

		new ConnectorLabel(getCounterClkDirLoad()->getInputLoadEnable(), "Ld");
	}
}


}; //namespace KSimLibInteger
