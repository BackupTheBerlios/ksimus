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
#include <qgrid.h>

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

#include "floatlatch.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLatch(container, ci);
}


const ComponentInfo FloatLatchInfo(
                     I18N_NOOP("Floating Point Latch"),
                     I18N_NOOP("Floating Point/Control/Latch"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );


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
	
	m_inputReset = new ConnectorBoolInEdge (this, I18N_NOOP("Reset"));
	CHECK_PTR(m_inputReset);
	m_inputReset->setEdgeSensitive(false,true);
	m_inputReset->setHideEnabled(true);
	m_inputReset->setHide(true,true);
	
	m_inputEnable = new ConnectorBoolInEdge (this, I18N_NOOP("Enable"));
	CHECK_PTR(m_inputEnable);
	
	m_inputPack = new ConnectorPack(this, I18N_NOOP("Input"), &ConnectorFloatInInfo, MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->setConnectorName(I18N_NOOP("Input %1"));
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_outputPack = new ConnectorPack(this, I18N_NOOP("Output"), &ConnectorFloatOutInfo, MIN_CHANNELS, MAX_CHANNELS);
	CHECK_PTR(m_outputPack);
	m_outputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_outputPack->setConnectorName(I18N_NOOP("Output %1"));
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
	
	if (getInputReset()->getInput())
	{
		m_values.fill(getResetValue());
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
	}
}

void FloatLatch::updateOutput()
{
	Component::updateOutput();
	
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
	
	updateOutput();
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
	i = popup->insertItem(i18n("&Add Channel"), this, SLOT(slotAddChannel()));
	if ((getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	 || (getOutputPack()->getConnectorCount() >= MAX_CHANNELS))
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("&Delete Channel"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getNumberOfDeletableConnectors() == 0)
	 || (getOutputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getOutputPack()->getNumberOfDeletableConnectors() == 0))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * FloatLatch::createGeneralProperty(Component * comp, QWidget *parent)
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
	reset(); // Update values
}
	
double FloatLatch::getResetValue() const
{
	return m_resetValue;
};

void FloatLatch::setChannelCount(int count)
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

static int tempConnCount;
static FloatLatch * tempConnCountOwner = (FloatLatch *)0;

void FloatLatch::setChannelCountDelayed(int count)
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

	
	m_channelsLabel = new QLabel(i18n("Channel count: "), getGrid(), "m_channelsLabel");
	CHECK_PTR(m_channelsLabel);
	
	m_channels = new QSpinBox(m_min, MAX_CHANNELS, 1, getGrid(), "m_channels");
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

};  //namespace KSimLibFloatingPoint
