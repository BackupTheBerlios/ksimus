/***************************************************************************
                          demultiplexer.h  -  description
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

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorboolinedge.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/optionalconnector.h"

#include "demultiplexer.h"

// Forward declaration



namespace KSimLibBoolean
{


//###############################################################
//###############################################################


Component * Demultiplexer::create(CompContainer * container, const ComponentInfo * ci)
{
	return new Demultiplexer(container, ci);
}


const ComponentInfo * Demultiplexer::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Boolean Demultiplexer"),
	                                QString::fromLatin1("Boolean/Mux & Demux/Demultiplexer"),
	                                i18n("Component", "Boolean/Mux & Demux/Demultiplexer"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-demultiplexer"));
	return &Info;
}


#define MIN_CHANNELS         1
#define MAX_CHANNELS         16
#define DEFAULT_CHANNELS     2
#define MIN_ADRESS           1
#define MAX_ADRESS           4
#define DEFAULT_ADRESS       1

//###############################################################
//###############################################################

unsigned int Demultiplexer::tempConnCount;
Demultiplexer * Demultiplexer::tempConnCountOwner = (Demultiplexer *)0;

Demultiplexer::Demultiplexer(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{

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
	
	
	m_inputConn = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input"));
	CHECK_PTR(m_inputConn);
	
	
	
	m_outputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Connector", "Output %1"),
	                             getConnectorBoolOutInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_outputPack);
	m_outputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_outputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
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
		new DemultiplexerView(this, SHEET_VIEW, "DemultiplexerSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void Demultiplexer::reset()
{
	Component::reset();
	
	m_addressLatch = 0;
	
	FOR_EACH_CONNECTOR(it, *getOutputPack()->getConnList())
	{
		((ConnectorBoolOut*)it.current())->setOutput(false);
	}
}


/** Executes the simulation of this component */
void Demultiplexer::calculate()
{
	Component::calculate();
	
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
		// First reset last addressed output
		((ConnectorBoolOut*)getOutputPack()->getConnList()->current())->setOutput(false);

		// Fetch output value
		if (m_addressLatch < getOutputPack()->getConnectorCount())
		{
			ConnectorBoolOut * out = (ConnectorBoolOut *)getOutputPack()->getConnList()->at(m_addressLatch);
			ASSERT(out != 0);
			out->setOutput(getInputConn()->getInput());
		}
	}
}

bool Demultiplexer::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("Boolean", "&Add Channel"), this, SLOT(slotAddChannel()));
	if (getOutputPack()->getConnectorCount() >= MAX_CHANNELS)
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("Boolean", "&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getOutputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getOutputPack()->getConnectorCount() <= getMinChannelCount()))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * Demultiplexer::createGeneralProperty(QWidget *parent)
{
	DemultiplexerPropertyGeneralWidget * wid;
	wid = new DemultiplexerPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void Demultiplexer::slotAddChannel()
{
	undoChangeProperty(i18n("Boolean", "Add Channel"));
	setChannelCount(getChannelCount()+1);
}

void Demultiplexer::slotDeleteChannel()
{
	undoChangeProperty(i18n("Boolean", "Delete Channel"));
	setChannelCount(getChannelCount()-1);
}

void Demultiplexer::setChannelCount(unsigned int count)
{
	if (count < MIN_CHANNELS) count = MIN_CHANNELS;
	if (count > MAX_CHANNELS) count = MAX_CHANNELS;
	getOutputPack()->setConnectorCount(count);
	
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

unsigned int Demultiplexer::getChannelCount() const
{
	return getOutputPack()->getConnectorCount();
}

unsigned int Demultiplexer::getMinChannelCount() const
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
	
	return QMAX(getOutputPack()->getNumberOfNotDeletableConnectors(), i);
}

void Demultiplexer::setChannelCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void Demultiplexer::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (Demultiplexer *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


DemultiplexerView::DemultiplexerView(Demultiplexer * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		ComponentLayoutVerticalCtrl * layout = new ComponentLayoutVerticalCtrl(this);
		CHECK_PTR(layout);
		
		layout->getCtrlBlock()->getLeft()->addSpace(1);
		layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getLatchOutput());
		layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getLatchAddress());
		layout->getCtrlBlock()->getLeft()->addConnectorPack(getComponent()->getAddressPack());
		
		
		layout->getFuncBlock()->getLeft()->addStretch(1);
		layout->getFuncBlock()->getLeft()->addConnector(getComponent()->getInputConn(),0);
		layout->getFuncBlock()->getLeft()->addStretch(1);
		
		layout->getFuncBlock()->getRight()->addSpace(1);
		layout->getFuncBlock()->getRight()->addConnectorPack(getComponent()->getOutputPack());
		
		
		layout->getFuncBlock()->setMinSize(4,5);
	
		new ComponentLayoutBlockContentText(layout->getFuncBlock(), QString::fromLatin1("DMUX"), AlignCenter, 270.0);
	
		new ConnectorLabel(getComponent()->getLatchOutput(), QString::fromLatin1("EO"));
		new ConnectorLabel(getComponent()->getLatchAddress(), QString::fromLatin1("EA"));
	
		unsigned int i = 1;
		FOR_EACH_CONNECTOR(it, *getComponent()->getOutputPack()->getConnList())
		{
			new ConnectorLabel(it.current(), QChar('A' - 1 + i));
			i ++;
		}
		connect(getComponent()->getOutputPack(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addOutConn(ConnectorBase *)));
		
		i = 1;
		FOR_EACH_CONNECTOR(it, *getComponent()->getAddressPack()->getConnList())
		{
			new ConnectorLabel(it.current(), QString("Adr %1"). arg(QChar('A' - 1 + i)));
			i ++;
		}
		connect(getComponent()->getAddressPack(), SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addAdrConn(ConnectorBase *)));
	}
}

void DemultiplexerView::addOutConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QChar('A' - 1 + getComponent()->getOutputPack()->getConnectorCount()));
}

void DemultiplexerView::addAdrConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString("Adr %1"). arg(QChar('A' - 1 + getComponent()->getAddressPack()->getConnectorCount())));
}

//###############################################################
//###############################################################

DemultiplexerPropertyGeneralWidget::DemultiplexerPropertyGeneralWidget(Demultiplexer * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_channelsLabel = new QLabel(i18n("Boolean", "Number of channels: "), this, "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new KSimSpinBox(comp->getMinChannelCount(), MAX_CHANNELS, 1, this, "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("Boolean", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	

	// Setup value
	m_channels->setValue(comp->getOutputPack()->getConnectorCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void DemultiplexerPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getOutputPack()->getConnectorCount() != (unsigned int)m_channels->value())
	{
		changeData();
		getComponent()->setChannelCountDelayed(m_channels->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void DemultiplexerPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_channels->setValue(QMAX(DEFAULT_CHANNELS, getComponent()->getMinChannelCount()));
}


//###############################################################
//###############################################################


#undef MIN_CHANNELS
#undef MAX_CHANNELS
#undef DEFAULT_CHANNELS
#undef MIN_ADRESS
#undef MAX_ADRESS
#undef DEFAULT_ADRESS



};  //namespace KSimLibBoolean

#include "demultiplexer.moc"
