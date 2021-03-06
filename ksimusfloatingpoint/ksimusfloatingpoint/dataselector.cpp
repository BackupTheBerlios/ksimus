/***************************************************************************
                          dataselector.h  -  description
                             -------------------
    begin                : Fri Feb 15 2002
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
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qgrid.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimdoubleedit.h"

#include "dataselector.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new DataSelector(container, ci);
}


const ComponentInfo DataSelectorInfo(
                     I18N_NOOP("Floating Data Selector"),
                     I18N_NOOP("Floating Point/Control/Data Selector"),
                     QString::null,
                     VA_SHEETVIEW,
                     create,
                     QString::null,
                     "component-float-control-dataSelect");


#define DEFAULT_RESET_VALUE  0.0
#define MIN_CHANNELS         1
#define MAX_CHANNELS         16
#define DEFAULT_CHANNELS     2
#define MIN_ADRESS           1
#define MAX_ADRESS           4
#define DEFAULT_ADRESS       1

//###############################################################
//###############################################################


DataSelector::DataSelector(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci)
{
	setResetValue(DEFAULT_RESET_VALUE);
	
	m_latchOutput = new ConnectorBoolInEdge (this, I18N_NOOP("Latch Output"));
	CHECK_PTR(m_latchOutput);
	m_latchOutput->setEdgeSensitive(false,true);
	m_latchOutput->setHideEnabled(true);
	m_latchOutput->setHide(true,true);
	
	m_latchAddress = new ConnectorBoolInEdge (this, I18N_NOOP("Latch Address Input"));
	CHECK_PTR(m_latchAddress);
	m_latchAddress->setEdgeSensitive(false,true);
	m_latchAddress->setHideEnabled(true);
	m_latchAddress->setHide(true,true);
	
	m_inputPack = new ConnectorPack(this, I18N_NOOP("Input"), &ConnectorFloatInInfo, MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->setConnectorName(I18N_NOOP("Input %1"));
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_addressPack = new ConnectorPack(this, I18N_NOOP("Address"), &ConnectorBoolInInfo, MIN_ADRESS, MAX_ADRESS);
	CHECK_PTR(m_addressPack);
	m_addressPack->setConnectorCount(DEFAULT_ADRESS);
	m_addressPack->setConnectorName(I18N_NOOP("Address %1"));
	m_addressPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new DataSelectorView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void DataSelector::reset()
{
	Float1Out::reset();
	
	m_addressLatch = 0;
}


/** Executes the simulation of this component */
void DataSelector::calculate()
{
	Float1Out::calculate();
	
	
	if ((getLatchAddress()->isHidden()) || getLatchAddress()->getInput())
	{
		// Fetch address if "Latch Address Input" is hidden or enabled
		unsigned int mask = 1;
		m_addressLatch = 0;
		FOR_EACH_CONNECTOR(it, *getAddressPack()->getConnList())
		{
			if (((ConnectorBoolIn*)it.current())->getInput())
			{
				m_addressLatch += mask;
			}
			mask <<= 1;
		}
	}
	
	
	if ((getLatchOutput()->isHidden()) || getLatchOutput()->getInput())
	{
		// Fetch output value if "Latch Output" is hidden or enabled
		if (m_addressLatch < getInputPack()->getConnectorCount())
		{
			ConnectorFloatIn * in = (ConnectorFloatIn *)getInputPack()->getConnList()->at(m_addressLatch);
			ASSERT(in != 0);
			setValue(in->getInput());
		}
		else
		{
			setValue(getResetValue());
		}
	}
}

bool DataSelector::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("&Add Channel"), this, SLOT(slotAddChannel()));
	if (getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getConnectorCount() <= getMinChannelCount()))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * DataSelector::createGeneralProperty(Component * comp, QWidget *parent)
{
	DataSelectorPropertyGeneralWidget * wid;
	wid = new DataSelectorPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void DataSelector::slotAddChannel()
{
	setChannelCount(getChannelCount()+1);
}

void DataSelector::slotDeleteChannel()
{
	setChannelCount(getChannelCount()-1);
}

void DataSelector::setChannelCount(int count)
{
	if (count < MIN_CHANNELS) count = MIN_CHANNELS;
	if (count > MAX_CHANNELS) count = MAX_CHANNELS;
	getInputPack()->setConnectorCount(count);
	
	if (count <= 2)
		getAddressPack()->setConnectorCount(1);
	else if (count <= 4)
		getAddressPack()->setConnectorCount(2);
	else if (count <= 8)
		getAddressPack()->setConnectorCount(3);
	else if (count <= 16)
		getAddressPack()->setConnectorCount(4);
	else
	{
		ASSERT(count <= 16);
	}
}

unsigned int DataSelector::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

unsigned int DataSelector::getMinChannelCount() const
{
	unsigned int i = MAX_CHANNELS;
	
	switch(getAddressPack()->getNumberOfNotDeletableConnectors())
	{
		case 0: i = 2; break;
		case 1: i = 2; break;
		case 2: i = 3; break;
		case 3: i = 5; break;
		case 4: i = 9; break;
		default: ASSERT(getAddressPack()->getNumberOfNotDeletableConnectors() <= 4); break;
	}
	
	return QMAX(getInputPack()->getNumberOfNotDeletableConnectors(), i);
}

static unsigned int tempConnCount;
static DataSelector * tempConnCountOwner = (DataSelector *)0;

void DataSelector::setChannelCountDelayed(int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void DataSelector::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (DataSelector *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


DataSelectorView::DataSelectorView(DataSelector * comp, eViewType viewType)
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
		m_ctrlBlock->getLeft()->addConnector(getComponent()->getLatchOutput());
		m_ctrlBlock->getLeft()->addConnector(getComponent()->getLatchAddress());
		m_ctrlBlock->getLeft()->addConnectorPack(getComponent()->getAddressPack());
		
		
		m_layout->getLeft()->addSpace(1);
		m_layout->getLeft()->addConnectorPack(getComponent()->getInputPack());
		
		m_layout->getRight()->addStretch(1);
		m_layout->getRight()->addConnector(getComponent()->getOutputConnector(),0);
		m_layout->getRight()->addStretch(1);
		
		m_layout->setMinSize(6,5);
		m_layout->updateLayout();
	
		new ConnectorLabel(getComponent()->getLatchOutput(), "EO");
		new ConnectorLabel(getComponent()->getLatchAddress(), "EA");
	
		unsigned int i = 1;
		FOR_EACH_CONNECTOR(it, *getComponent()->getInputPack()->getConnList())
		{
			new ConnectorLabel(it.current(), QChar('A' - 1 + i));
			i ++;
		}
		connect(getComponent()->getInputPack(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addInConn(ConnectorBase *)));
		
		i = 1;
		FOR_EACH_CONNECTOR(it, *getComponent()->getAddressPack()->getConnList())
		{
			new ConnectorLabel(it.current(), QString("Adr %1"). arg(QChar('A' - 1 + i)));
			i ++;
		}
		connect(getComponent()->getAddressPack(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addAdrConn(ConnectorBase *)));
	}
}

void DataSelectorView::draw(QPainter * p)
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
	p->drawText(rect, (AlignHCenter | AlignTop), "MUX");
}


void DataSelectorView::addInConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QChar('A' - 1 + getComponent()->getInputPack()->getConnectorCount()));
}

void DataSelectorView::addAdrConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString("Adr %1"). arg(QChar('A' - 1 + getComponent()->getAddressPack()->getConnectorCount())));
}

//###############################################################
//###############################################################

DataSelectorPropertyGeneralWidget::DataSelectorPropertyGeneralWidget(DataSelector * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_channelsLabel = new QLabel(i18n("Channel count: "), getGrid(), "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new QSpinBox(comp->getMinChannelCount(), MAX_CHANNELS, 1, getGrid(), "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("Change count of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	
	
	m_resetValueLabel = new QLabel(i18n("Reset value: "), getGrid(), "m_resetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimDoubleEdit(getGrid(), "m_resetValue");
	CHECK_PTR(m_resetValue);
	tip = i18n("Change the reset value of the component here.");
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	
	
	// Setup value
	m_resetValue->setValue(comp->getResetValue());
	m_channels->setValue(comp->getInputPack()->getConnectorCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void DataSelectorPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	double reset = m_resetValue->value();
	if (getComponent()->getResetValue() != reset)
	{
		changeData();
		getComponent()->setResetValue(reset);
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
void DataSelectorPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
	m_channels->setValue(QMAX(DEFAULT_CHANNELS, getComponent()->getMinChannelCount()));
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint
