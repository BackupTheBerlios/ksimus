/***************************************************************************
                          integerlatch.cpp  -  description
                             -------------------
    begin                : Sun Aug 31 2003
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
#include <qpainter.h>
#include <qpopupmenu.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/optionalconnector.h"

#include "integerlatch.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{


//###############################################################
//###############################################################


Component * IntegerLatch::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerLatch(container, ci);
}


const ComponentInfo * IntegerLatch::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Latch"),
	                                QString::fromLatin1("Integer/Control/Latch"),
	                                i18n("Component", "Integer/Control/Latch"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-latch")*/);
	return &Info;
}


#define DEFAULT_RESET_VALUE  KSimBaseInt(0, KSimBaseInt::Decimal)
#define MIN_CHANNELS         1
#define MAX_CHANNELS         32
#define DEFAULT_CHANNELS     MIN_CHANNELS

//###############################################################
//###############################################################


IntegerLatch::IntegerLatch(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetValue(DEFAULT_RESET_VALUE)
{
	
	m_inputReset = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Reset"),
	                             i18n("Integer-Connector", "Reset"));
	CHECK_PTR(m_inputReset);
	m_inputReset->setEdgeSensitive(false,true);
	// make Reset optional
	new OptionalConnector(m_inputReset,
	                      QString::fromLatin1("Reset input"),
	                      i18n("Integer", "Reset input:"));
	
	m_inputEnable = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable"),
	                             i18n("Integer-Connector", "Enable"));
	CHECK_PTR(m_inputEnable);
	
	m_inputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             ConnectorIntegerIn::getStaticInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_outputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Connector", "Output %1"),
	                             ConnectorIntegerOut::getStaticInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_outputPack);
	m_outputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_outputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void IntegerLatch::calculate()
{
//	Component::calculate();
	
	if (getInputReset()->getInput() && !getInputReset()->isHidden())
	{
		m_values.fill(getResetValue().value());
		setOutput();
	}
	else if (getInputEnable()->getInput())
	{
		QListIterator<ConnectorBase> it(*getInputPack()->getConnList());
		int i = 0;
		while(it.current())
		{
			m_values[i++] = ((ConnectorIntegerIn*)it.current())->getInput();
			++it;
		};
		setOutput();
	}
}

void IntegerLatch::setOutput()
{
	QListIterator<ConnectorBase> it(*getOutputPack()->getConnList());
	int i = 0;
	while(it.current())
	{
		((ConnectorIntegerOut*)it.current())->setOutput(m_values[i++]);
		++it;
	};
}
	

void IntegerLatch::reset()
{
	Component::reset();
	
	m_values.resize(QMAX(getInputPack()->getConnectorCount(),getOutputPack()->getConnectorCount()));
	m_values.fill(getResetValue().value());
	
	setOutput();
}

void IntegerLatch::save(KSimData & file) const
{
	Component::save(file);
	
	getResetValue().save(file, "Reset Value");
}

bool IntegerLatch::load(KSimData & file, bool copyLoad)
{
	if(!m_resetValue.load(file, "Reset Value"))
	{
		m_resetValue = DEFAULT_RESET_VALUE;
	}

	return Component::load(file, copyLoad);
}


bool IntegerLatch::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("Integer", "&Add Channel"), this, SLOT(slotAddChannel()));
	if ((getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	 || (getOutputPack()->getConnectorCount() >= MAX_CHANNELS))
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("Integer", "&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getNumberOfDeletableConnectors() == 0)
	 || (getOutputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getOutputPack()->getNumberOfDeletableConnectors() == 0))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * IntegerLatch::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void IntegerLatch::slotAddChannel()
{
	getInputPack()->slotAddConnector();
	getOutputPack()->slotAddConnector();
}

void IntegerLatch::slotDeleteChannel()
{
	getInputPack()->slotDeleteConnector();
	getOutputPack()->slotDeleteConnector();
}

void IntegerLatch::setResetValue(KSimBaseInt resetValue)
{
	m_resetValue = resetValue;
// crashes perhaps	reset(); // Update values
}
	
KSimBaseInt IntegerLatch::getResetValue() const
{
	return m_resetValue;
};

void IntegerLatch::setChannelCount(unsigned int count)
{
	if (count < MIN_CHANNELS) count = MIN_CHANNELS;
	if (count > MAX_CHANNELS) count = MAX_CHANNELS;
	getInputPack()->setConnectorCount(count);
	getOutputPack()->setConnectorCount(count);
}

int IntegerLatch::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

int IntegerLatch::tempConnCount;
IntegerLatch * IntegerLatch::tempConnCountOwner = 0;

void IntegerLatch::setChannelCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void IntegerLatch::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = 0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


IntegerLatch::View::View(IntegerLatch * comp, eViewType viewType)
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
		m_ctrlBlock->getLeft()->addConnector(getComponent()->getInputReset());
		m_ctrlBlock->getLeft()->addConnector(getComponent()->getInputEnable());
		
		
		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnectorPack(getComponent()->getInputPack());
		
		m_layout->getRight()->addSpace(1);
		m_layout->getRight()->addConnectorPack(getComponent()->getOutputPack());
		
		m_layout->setMinSize(6,5);
		m_layout->updateLayout();
	
		new ConnectorLabel(getComponent()->getInputReset(), "R");
		new ConnectorLabel(getComponent()->getInputEnable(), "E");
	}
}

void IntegerLatch::View::draw(QPainter * p)
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
	p->drawText(rect, (AlignHCenter | AlignTop), "Latch");
}


//###############################################################
//###############################################################

IntegerLatch::PropertyGeneralWidget::PropertyGeneralWidget(IntegerLatch * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	m_min = QMAX ( comp->getInputPack()->getNumberOfNotDeletableConnectors(),
	               comp->getOutputPack()->getNumberOfNotDeletableConnectors());

	
	m_channelsLabel = new QLabel(i18n("Integer", "Number of channels: "), this, "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new KSimSpinBox(m_min, MAX_CHANNELS, 1, this, "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("Integer", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	m_channelsLabel->setBuddy(m_channels);
	
	
	m_resetValueLabel = new QLabel(i18n("Integer", "Reset value: "), this, "m_resetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimBaseIntEdit(this, "m_resetValue");
	CHECK_PTR(m_resetValue);
	tip = i18n("Integer", "Change the reset value of the channels here.");
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	tip += m_resetValue->getWhatsThisHelp();
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	m_resetValueLabel->setBuddy(m_resetValue);
	
	
	// Setup value
	m_resetValue->setValue(comp->getResetValue());
	m_channels->setValue(comp->getInputPack()->getConnectorCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void IntegerLatch::PropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();

	if (getComponent()->getResetValue() != m_resetValue->value())
	{
		changeData();
		getComponent()->setResetValue(m_resetValue->value());
	}

	if (getComponent()->getInputPack()->getConnectorCount() != (unsigned int)m_channels->value())
	{
		changeData();
		getComponent()->setChannelCountDelayed(m_channels->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void IntegerLatch::PropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
	m_channels->setValue(QMAX(DEFAULT_CHANNELS, m_min));
}


//###############################################################
//###############################################################

#undef DEFAULT_RESET_VALUE
#undef MIN_CHANNELS
#undef MAX_CHANNELS
#undef DEFAULT_CHANNELS

};  //namespace KSimLibInteger
