/***************************************************************************
                          floatlatch.cpp  -  description
                             -------------------
    begin                : Tue Feb 12 2002
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
#include "ksimus/optionalconnector.h"

#include "floatlatch.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################


Component * FloatLatch::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLatch(container, ci);
}


const ComponentInfo * FloatLatch::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Latch"),
	                                QString::fromLatin1("Floating Point/Control/Latch"),
	                                i18n("Component", "Floating Point/Control/Latch"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-latch"));
	return &Info;
}


#define DEFAULT_RESET_VALUE  0.0
#define MIN_CHANNELS         1
#define MAX_CHANNELS        26
#define DEFAULT_CHANNELS     1

//###############################################################
//###############################################################


FloatLatch::FloatLatch(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetValue(DEFAULT_RESET_VALUE)
{
	
	m_inputReset = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Reset"),
	                             i18n("FloatingPoint-Connector", "Reset"));
	CHECK_PTR(m_inputReset);
	m_inputReset->setEdgeSensitive(false,true);
	// make Reset optional
	new OptionalConnector(m_inputReset,
	                      QString::fromLatin1("Reset input"),
	                      i18n("FloatingPoint", "Reset input:"));
	
	m_inputEnable = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable"),
	                             i18n("FloatingPoint-Connector", "Enable"));
	CHECK_PTR(m_inputEnable);
	
	m_inputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             getConnectorFloatInInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_outputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Connector", "Output %1"),
	                             getConnectorFloatOutInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_outputPack);
	m_outputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_outputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatLatchView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void FloatLatch::calculate()
{
	Component::calculate();
	
	if (getInputReset()->getInput() && !getInputReset()->isHidden())
	{
		m_values.fill(getResetValue());
		setOutput();
	}
	else if (getInputEnable()->getInput())
	{
		QListIterator<ConnectorBase> it(*getInputPack()->getConnList());
		int i = 0;
		while(it.current())
		{
			m_values[i++] = ((ConnectorFloatIn*)it.current())->getInput();
			++it;
		};
		setOutput();
	}
}

void FloatLatch::setOutput()
{
	QListIterator<ConnectorBase> it(*getOutputPack()->getConnList());
	int i = 0;
	while(it.current())
	{
		((ConnectorFloatOut*)it.current())->setOutput(m_values[i++]);
		++it;
	};
}
	

void FloatLatch::reset()
{
	Component::reset();
	
	m_values.resize(QMAX(getInputPack()->getConnectorCount(),getOutputPack()->getConnectorCount()));
	m_values.fill(getResetValue());
	
	setOutput();
}

void FloatLatch::save(KSimData & file) const
{
	Component::save(file);
	
	if (getResetValue() != DEFAULT_RESET_VALUE)
	{
		file.writeEntry("Reset Value", getResetValue());
	}
}

bool FloatLatch::load(KSimData & file, bool copyLoad)
{
	setResetValue( file.readDoubleNumEntry("Reset Value", DEFAULT_RESET_VALUE) );
	
	return Component::load(file, copyLoad);
}


bool FloatLatch::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("FloatingPoint", "&Add Channel"), this, SLOT(slotAddChannel()));
	if ((getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	 || (getOutputPack()->getConnectorCount() >= MAX_CHANNELS))
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("FloatingPoint", "&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getNumberOfDeletableConnectors() == 0)
	 || (getOutputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getOutputPack()->getNumberOfDeletableConnectors() == 0))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * FloatLatch::createGeneralProperty(QWidget *parent)
{
	FloatLatchPropertyGeneralWidget * wid;
	wid = new FloatLatchPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void FloatLatch::slotAddChannel()
{
	getInputPack()->slotAddConnector();
	getOutputPack()->slotAddConnector();
}

void FloatLatch::slotDeleteChannel()
{
	getInputPack()->slotDeleteConnector();
	getOutputPack()->slotDeleteConnector();
}

void FloatLatch::setResetValue(double resetValue)
{
	m_resetValue = resetValue;
// crashes perhaps	reset(); // Update values
}
	
double FloatLatch::getResetValue() const
{
	return m_resetValue;
};

void FloatLatch::setChannelCount(unsigned int count)
{
	if (count < MIN_CHANNELS) count = MIN_CHANNELS;
	if (count > MAX_CHANNELS) count = MAX_CHANNELS;
	getInputPack()->setConnectorCount(count);
	getOutputPack()->setConnectorCount(count);
}

int FloatLatch::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

int FloatLatch::tempConnCount;
FloatLatch * FloatLatch::tempConnCountOwner = (FloatLatch *)0;

void FloatLatch::setChannelCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void FloatLatch::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (FloatLatch *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


FloatLatchView::FloatLatchView(FloatLatch * comp, eViewType viewType)
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

void FloatLatchView::draw(QPainter * p)
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

FloatLatchPropertyGeneralWidget::FloatLatchPropertyGeneralWidget(FloatLatch * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	m_min = QMAX ( comp->getInputPack()->getNumberOfNotDeletableConnectors(),
	               comp->getOutputPack()->getNumberOfNotDeletableConnectors());

	
	m_channelsLabel = new QLabel(i18n("FloatingPoint", "Number of channels: "), this, "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new QSpinBox(m_min, MAX_CHANNELS, 1, this, "m_channels");
	CHECK_PTR(m_channels);
	tip = i18n("FloatingPoint", "Change number of channels here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	
	
	m_resetValueLabel = new QLabel(i18n("FloatingPoint", "Reset value: "), this, "m_resetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimDoubleEdit(this, "m_resetValue");
	CHECK_PTR(m_resetValue);
	tip = i18n("FloatingPoint", "Change the reset value of the channels here.");
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
void FloatLatchPropertyGeneralWidget::acceptPressed()
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
void FloatLatchPropertyGeneralWidget::defaultPressed()
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

};  //namespace KSimLibFloatingPoint
