/***************************************************************************
                          integerdataselector.h  -  description
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
#include <qpopupmenu.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/optionalconnector.h"

#include "integerdataselector.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{


//###############################################################
//###############################################################


Component * IntegerDataSelector::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerDataSelector(container, ci);
}


const ComponentInfo * IntegerDataSelector::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Data Selector"),
	                                QString::fromLatin1("Integer/Control/Data Selector"),
	                                i18n("Component", "Integer/Control/Data Selector"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-dataSelect")*/);
	return &Info;
}


#define DEFAULT_RESET_VALUE  0
#define MIN_CHANNELS         1
#define MAX_CHANNELS         16
#define DEFAULT_CHANNELS     2
#define MIN_ADRESS           1
#define MAX_ADRESS           4
#define DEFAULT_ADRESS       1

//###############################################################
//###############################################################

unsigned int IntegerDataSelector::tempConnCount;
IntegerDataSelector * IntegerDataSelector::tempConnCountOwner = (IntegerDataSelector *)0;

IntegerDataSelector::IntegerDataSelector(CompContainer * container, const ComponentInfo * ci)
	: Integer1Out(container, ci)
{
	setResetValue(DEFAULT_RESET_VALUE);
	
	m_latchOutput = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Latch Output"),
	                             i18n("Integer-Connector", "Latch Output"));
	Q_CHECK_PTR(m_latchOutput);
	m_latchOutput->setEdgeSensitive(false,true);
	// make Output Latch optional
	new OptionalConnector(m_latchOutput,
	                      QString::fromLatin1("Output Latch"),
	                      i18n("Integer-Connector", "Output Latch:"));
	
	m_latchAddress = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Latch Address Input"),
	                             i18n("Integer-Connector", "Latch Address Input"));
	Q_CHECK_PTR(m_latchAddress);
	m_latchAddress->setEdgeSensitive(false,true);
	// make Address Latch optional
	new OptionalConnector(m_latchAddress,
	                      QString::fromLatin1("Address Latch"),
	                      i18n("Integer-Connector", "Address Latch:"));
	
	m_inputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input %1"),
	                             ConnectorIntegerIn::getStaticInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	Q_CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_addressPack = new ConnectorPack(this,
	                             QString::fromLatin1("Address"),
	                             i18n("Integer-Connector", "Address %1"),
	                             getConnectorBoolInInfo(),
	                             MIN_ADRESS, MAX_ADRESS);
	Q_CHECK_PTR(m_addressPack);
	m_addressPack->setConnectorCount(DEFAULT_ADRESS);
	m_addressPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerDataSelectorView(this, SHEET_VIEW, "IntegerDataSelectorSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void IntegerDataSelector::reset()
{
	Integer1Out::reset();
	
	m_addressLatch = 0;
}


/** Executes the simulation of this component */
void IntegerDataSelector::calculate()
{
	Integer1Out::calculate();
	
	
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
			ConnectorIntegerIn * in = (ConnectorIntegerIn *)getInputPack()->getConnList()->at(m_addressLatch);
			Q_ASSERT(in != 0);
			setValue(in->getInput());
		}
		else
		{
			setValue(getResetValue().value());
		}
	}
}

bool IntegerDataSelector::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Integer1Out::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("Integer", "&Add Channel"), this, SLOT(slotAddChannel()));
	if (getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("Integer", "&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getConnectorCount() <= getMinChannelCount()))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * IntegerDataSelector::createGeneralProperty(QWidget *parent)
{
	IntegerDataSelectorPropertyGeneralWidget * wid;
	wid = new IntegerDataSelectorPropertyGeneralWidget(this, parent);
	Q_CHECK_PTR(wid);
	
	return wid;
}


void IntegerDataSelector::slotAddChannel()
{
	undoChangeProperty(i18n("Integer", "Add Channel"));
	setChannelCount(getChannelCount()+1);
}

void IntegerDataSelector::slotDeleteChannel()
{
	undoChangeProperty(i18n("Integer", "Delete Channel"));
	setChannelCount(getChannelCount()-1);
}

void IntegerDataSelector::setChannelCount(unsigned int count)
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
		Q_ASSERT(count <= 16);
	}
}

unsigned int IntegerDataSelector::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

unsigned int IntegerDataSelector::getMinChannelCount() const
{
	unsigned int i = MAX_CHANNELS;
	
	switch(getAddressPack()->getNumberOfNotDeletableConnectors())
	{
		case 0: i = 2; break;
		case 1: i = 2; break;
		case 2: i = 3; break;
		case 3: i = 5; break;
		case 4: i = 9; break;
		default: Q_ASSERT(getAddressPack()->getNumberOfNotDeletableConnectors() <= 4); break;
	}
	
	return QMAX(getInputPack()->getNumberOfNotDeletableConnectors(), i);
}

void IntegerDataSelector::setChannelCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void IntegerDataSelector::menuExecuted()
{
	Integer1Out::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (IntegerDataSelector *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


IntegerDataSelectorView::IntegerDataSelectorView(IntegerDataSelector * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 6*gridX, 5*gridY));
		enableRotation(true);
		ComponentLayoutVerticalCtrl * layout = new ComponentLayoutVerticalCtrl(this);
		Q_CHECK_PTR(layout);
		
		layout->getCtrlBlock()->getLeft()->addSpace(1);
		layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getLatchOutput());
		layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getLatchAddress());
		layout->getCtrlBlock()->getLeft()->addConnectorPack(getComponent()->getAddressPack());
		
		
		layout->getFuncBlock()->getLeft()->addSpace(1);
		layout->getFuncBlock()->getLeft()->addConnectorPack(getComponent()->getInputPack());
		
		layout->getFuncBlock()->getRight()->addStretch(1);
		layout->getFuncBlock()->getRight()->addConnector(getComponent()->getOutputConnector(),0);
		layout->getFuncBlock()->getRight()->addStretch(1);
		
		layout->getFuncBlock()->setMinSize(4,5);
	
		new ComponentLayoutBlockContentText(layout->getFuncBlock(), QString::fromLatin1("MUX"), AlignCenter, 270.0);
	
		new ConnectorLabel(getComponent()->getLatchOutput(), QString::fromLatin1("EO"));
		new ConnectorLabel(getComponent()->getLatchAddress(), QString::fromLatin1("EA"));
	
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

void IntegerDataSelectorView::addInConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QChar('A' - 1 + getComponent()->getInputPack()->getConnectorCount()));
}

void IntegerDataSelectorView::addAdrConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString("Adr %1"). arg(QChar('A' - 1 + getComponent()->getAddressPack()->getConnectorCount())));
}

//###############################################################
//###############################################################

IntegerDataSelectorPropertyGeneralWidget::IntegerDataSelectorPropertyGeneralWidget(IntegerDataSelector * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_channelsLabel = new QLabel(i18n("Integer", "Number of channels: "), this, "m_channelsLabel");
	Q_CHECK_PTR(m_channelsLabel);
	
	m_channels = new KSimSpinBox(comp->getMinChannelCount(), MAX_CHANNELS, 1, this, "m_channels");
	Q_CHECK_PTR(m_channels);
	tip = i18n("Integer", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	m_channelsLabel->setBuddy(m_channels);
	
	
	m_resetValueLabel = new QLabel(i18n("Integer", "Reset value: "), this, "m_resetValueLabel");
	Q_CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimBaseIntEdit(this, "m_resetValue");
	Q_CHECK_PTR(m_resetValue);
	tip = i18n("Integer", "Change the reset value of the channels here.");
	addToolTip(tip, m_resetValue, m_resetValueLabel);
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
void IntegerDataSelectorPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	KSimBaseInt reset = m_resetValue->value();
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
void IntegerDataSelectorPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
	m_channels->setValue(QMAX(DEFAULT_CHANNELS, getComponent()->getMinChannelCount()));
}

#undef DEFAULT_RESET_VALUE
#undef MIN_CHANNELS
#undef MAX_CHANNELS
#undef DEFAULT_CHANNELS
#undef MIN_ADRESS
#undef MAX_ADRESS
#undef DEFAULT_ADRESS

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

#include "integerdataselector.moc"
