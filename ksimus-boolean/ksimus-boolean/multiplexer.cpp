/***************************************************************************
                          multiplexer.h  -  description
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
#include <qpopupmenu.h>
#include <qlabel.h>
#include <qspinbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/componentblocklayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimbooleanbox.h"
#include "ksimus/optionalconnector.h"

#include "multiplexer.h"

// Forward declaration

// Project-Includes


namespace KSimLibBoolean
{


//###############################################################
//###############################################################


Component * Multiplexer::create(CompContainer * container, const ComponentInfo * ci)
{
	return new Multiplexer(container, ci);
}


const ComponentInfo * Multiplexer::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Multiplexer"),
	                                QString::fromLatin1("Boolean/Mux & Demux/Multiplexer"),
	                                i18n("Component", "Boolean/Mux & Demux/Multiplexer"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-multiplexer"));
	return &Info;
}


#define DEFAULT_RESET_STATE  false
#define MIN_CHANNELS         1
#define MAX_CHANNELS         16
#define DEFAULT_CHANNELS     2
#define MIN_ADRESS           1
#define MAX_ADRESS           4
#define DEFAULT_ADRESS       1

//###############################################################
//###############################################################

unsigned int Multiplexer::tempConnCount;
Multiplexer * Multiplexer::tempConnCountOwner = (Multiplexer *)0;

Multiplexer::Multiplexer(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci)
{
	setResetState(DEFAULT_RESET_STATE);
	
	m_latchOutput = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable Latch Output"),
	                             i18n("Boolean-Connector", "Enable Latch Output"));
	CHECK_PTR(m_latchOutput);
	m_latchOutput->setEdgeSensitive(false,true);
	// make Output Latch optional
	new OptionalConnector(m_latchOutput,
	                      QString::fromLatin1("Enable Output Latch"),
	                      i18n("Boolean", "Enable Output Latch:"));
	
	m_latchAddress = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable Latch Address"),
	                             i18n("Boolean-Connector", "Enable Latch Address"));
	CHECK_PTR(m_latchAddress);
	m_latchAddress->setEdgeSensitive(false,true);
	// make Address Latch optional
	new OptionalConnector(m_latchAddress,
	                      QString::fromLatin1("Enable Address Latch"),
	                      i18n("Boolean", "Enable Address Latch:"));
	
	m_inputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_addressPack = new ConnectorPack(this,
	                             QString::fromLatin1("Address"),
	                             i18n("Connector", "Address %1"),
	                             getConnectorBoolInInfo(),
	                             MIN_ADRESS, MAX_ADRESS);
	CHECK_PTR(m_addressPack);
	m_addressPack->setConnectorCount(DEFAULT_ADRESS);
	m_addressPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new MultiplexerView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void Multiplexer::reset()
{
	Boolean1Out::reset();
	
	m_addressLatch = 0;
}


/** Executes the simulation of this component */
void Multiplexer::calculate()
{
	Boolean1Out::calculate();
	
	
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
			ConnectorBoolIn * in = (ConnectorBoolIn *)getInputPack()->getConnList()->at(m_addressLatch);
			ASSERT(in != 0);
			setState(in->getInput());
		}
		else
		{
			setState(getResetState());
		}
	}
}

bool Multiplexer::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("Boolean", "&Add Channel"), this, SLOT(slotAddChannel()));
	if (getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("Boolean", "&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getConnectorCount() <= getMinChannelCount()))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * Multiplexer::createGeneralProperty(QWidget *parent)
{
	MultiplexerPropertyGeneralWidget * wid;
	wid = new MultiplexerPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void Multiplexer::slotAddChannel()
{
	setChannelCount(getChannelCount()+1);
}

void Multiplexer::slotDeleteChannel()
{
	setChannelCount(getChannelCount()-1);
}

void Multiplexer::setChannelCount(int count)
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

unsigned int Multiplexer::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

unsigned int Multiplexer::getMinChannelCount() const
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


void Multiplexer::setChannelCountDelayed(int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void Multiplexer::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (Multiplexer *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


MultiplexerView::MultiplexerView(Multiplexer * comp, eViewType viewType)
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

void MultiplexerView::draw(QPainter * p)
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


void MultiplexerView::addInConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QChar('A' - 1 + getComponent()->getInputPack()->getConnectorCount()));
}

void MultiplexerView::addAdrConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString("Adr %1"). arg(QChar('A' - 1 + getComponent()->getAddressPack()->getConnectorCount())));
}

//###############################################################
//###############################################################

MultiplexerPropertyGeneralWidget::MultiplexerPropertyGeneralWidget(Multiplexer * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_channelsLabel = new QLabel(i18n("Boolean", "Number of channels: "), this, "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new QSpinBox(comp->getMinChannelCount(), MAX_CHANNELS, 1, this, "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("Boolean", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	

	m_resetStateLabel = new QLabel(i18n("Boolean", "Reset state: "), this, "m_resetValueLabel");
	CHECK_PTR(m_resetStateLabel);
	
	m_resetState = new KSimBooleanBox(this, "m_resetValue");
	CHECK_PTR(m_resetState);
	tip = i18n("Boolean", "Change the reset state of the channels here.");
	addToolTip(tip, m_resetState, m_resetStateLabel);
	addWhatsThis(tip, m_resetState, m_resetStateLabel);
	
	
	// Setup value
	m_resetState->setValue(comp->getResetState());
	m_channels->setValue(comp->getInputPack()->getConnectorCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void MultiplexerPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	bool reset = m_resetState->getValue();
	if (getComponent()->getResetState() != reset)
	{
		changeData();
		getComponent()->setResetState(reset);
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
void MultiplexerPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetState->setValue(DEFAULT_RESET_STATE);
	m_channels->setValue(QMAX(DEFAULT_CHANNELS, getComponent()->getMinChannelCount()));
}

#undef DEFAULT_RESET_STATE
#undef MIN_CHANNELS
#undef MAX_CHANNELS
#undef DEFAULT_CHANNELS
#undef MIN_ADRESS
#undef MAX_ADRESS
#undef DEFAULT_ADRESS

//###############################################################
//###############################################################

};  //namespace KSimLibBoolean
