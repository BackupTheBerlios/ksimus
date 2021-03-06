/***************************************************************************
                          multidlatch.cpp  -  description
                             -------------------
    begin                : Sat Feb 23 2002
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
#include "ksimus/connectorboolinedge.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/ksimbooleanbox.h"
#include "ksimus/optionalconnector.h"

#include "multidlatch.h"

// Forward declaration

// Project-Includes


namespace KSimLibBoolean
{


//###############################################################
//###############################################################


Component * MultiDLatch::create(CompContainer * container, const ComponentInfo * ci)
{
	return new MultiDLatch(container, ci);
}



const ComponentInfo * MultiDLatch::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Multiple D Latches"),
	                                QString::fromLatin1("Boolean/Flip Flop/Multiple D Latches"),
	                                i18n("Component", "Boolean/Flip Flop/Multiple D Latches"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-boolean-multiple-d-latches"));
	return &Info;
}

#define DEFAULT_RESET_VALUE  false
#define MIN_CHANNELS         1
#define MAX_CHANNELS        26
#define DEFAULT_CHANNELS     2

//###############################################################
//###############################################################


MultiDLatch::MultiDLatch(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetValue(DEFAULT_RESET_VALUE)
{
	
	m_inputReset = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Reset"),
	                             i18n("Boolean-Connector", "Reset"));
	Q_CHECK_PTR(m_inputReset);
	m_inputReset->setEdgeSensitive(false,true);
	// make Reset optional
	new OptionalConnector(m_inputReset,
	                      QString::fromLatin1("Reset input"),
	                      i18n("Boolean", "Reset input:"));
	
	m_inputEnable = new ConnectorBoolInEdge(this,
	                             QString::fromLatin1("Enable"),
	                             i18n("Boolean-Connector", "Enable"));
	Q_CHECK_PTR(m_inputEnable);
	
	m_inputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Boolean-Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	Q_CHECK_PTR(m_inputPack);
	m_inputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_inputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	
	m_outputPack = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Boolean-Connector", "Output %1"),
	                             getConnectorBoolOutInfo(),
	                             MIN_CHANNELS, MAX_CHANNELS);
	Q_CHECK_PTR(m_outputPack);
	m_outputPack->setConnectorCount(DEFAULT_CHANNELS);
	m_outputPack->getAction().disable(KSimAction::INITPOPUPMENU);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new MultiDLatchView(this, SHEET_VIEW, "MultiDLatchSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void MultiDLatch::calculate()
{
	Component::calculate();
	
	if (getInputReset()->getInput() && !getInputReset()->isHidden())
	{
		m_values.fill(false);
		setOutput();
	}
	else if (getInputEnable()->getInput())
	{
		QPtrListIterator<ConnectorBase> it(*getInputPack()->getConnList());
		int i = 0;
		while(it.current())
		{
			m_values[i++] = ((ConnectorBoolIn*)it.current())->getInput();
			++it;
		};
		setOutput();
	}
}

void MultiDLatch::setOutput() const
{
	QPtrListIterator<ConnectorBase> it(*getOutputPack()->getConnList());
	int i = 0;
	while(it.current())
	{
		((ConnectorBoolOut*)it.current())->setOutput(m_values[i++]);
		++it;
	};
}
	

void MultiDLatch::reset()
{
	Component::reset();
	
	m_values.resize(QMAX(getInputPack()->getConnectorCount(),getOutputPack()->getConnectorCount()));
	m_values.fill(getResetValue());
	
	setOutput();
}

void MultiDLatch::save(KSimData & file) const
{
	Component::save(file);
	
	if (getResetValue() != DEFAULT_RESET_VALUE)
	{
		file.writeEntry("Reset Value", getResetValue());
	}
}

bool MultiDLatch::load(KSimData & file, bool copyLoad)
{
	setResetValue( file.readBoolEntry("Reset Value", DEFAULT_RESET_VALUE) );
	
	return Component::load(file, copyLoad);
}


bool MultiDLatch::initPopupMenu(QPopupMenu * popup)
{
	int i;
	
	Component::initPopupMenu(popup);
	
	popup->insertSeparator();
	i = popup->insertItem(i18n("Boolean", "&Add Latch"), this, SLOT(slotAddChannel()));
	if ((getInputPack()->getConnectorCount() >= MAX_CHANNELS)
	 || (getOutputPack()->getConnectorCount() >= MAX_CHANNELS))
	{
		popup->setItemEnabled(i, false);
	}
	
	
	i = popup->insertItem(i18n("Boolean", "&Delete Latch"), this, SLOT(slotDeleteChannel()));
	if ((getInputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getInputPack()->getNumberOfDeletableConnectors() == 0)
	 || (getOutputPack()->getConnectorCount() <= MIN_CHANNELS)
	 || (getOutputPack()->getNumberOfDeletableConnectors() == 0))
	{
		popup->setItemEnabled(i, false);
	}
	
	return true;
}

ComponentPropertyBaseWidget * MultiDLatch::createGeneralProperty(QWidget *parent)
{
	MultiDLatchPropertyGeneralWidget * wid;
	wid = new MultiDLatchPropertyGeneralWidget(this, parent);
	Q_CHECK_PTR(wid);
	
	return wid;
}


void MultiDLatch::slotAddChannel()
{
	getInputPack()->slotAddConnector();
	getOutputPack()->slotAddConnector();
}

void MultiDLatch::slotDeleteChannel()
{
	getInputPack()->slotDeleteConnector();
	getOutputPack()->slotDeleteConnector();
}

void MultiDLatch::setResetValue(bool resetValue)
{
	m_resetValue = resetValue;
// crashes ksimus	reset(); // Update values
}
	
bool MultiDLatch::getResetValue() const
{
	return m_resetValue;
};

void MultiDLatch::setChannelCount(int count)
{
	if (count < MIN_CHANNELS) count = MIN_CHANNELS;
	if (count > MAX_CHANNELS) count = MAX_CHANNELS;
	getInputPack()->setConnectorCount(count);
	getOutputPack()->setConnectorCount(count);
}

int MultiDLatch::getChannelCount() const
{
	return getInputPack()->getConnectorCount();
}

static int tempConnCount;
static MultiDLatch * tempConnCountOwner = (MultiDLatch *)0;

void MultiDLatch::setChannelCountDelayed(int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void MultiDLatch::menuExecuted()
{
	Component::menuExecuted();
	
	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = (MultiDLatch *)0;
		if (tempConnCount != getChannelCount())
		{
			setChannelCount(tempConnCount);
		}
	}
}

//###############################################################
//###############################################################


MultiDLatchView::MultiDLatchView(MultiDLatch * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	if (viewType == SHEET_VIEW)
	{
		enableRotation(true);
		m_layout = new ComponentLayoutVerticalCtrl(this);
		Q_CHECK_PTR(m_layout);
		
		m_layout->getCtrlBlock()->getLeft()->addSpace(1);
		m_layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getInputReset());
		m_layout->getCtrlBlock()->getLeft()->addConnector(getComponent()->getInputEnable());
		
		
		m_layout->getFuncBlock()->getLeft()->addSpace(1);
		m_layout->getFuncBlock()->getLeft()->addConnectorPack(getComponent()->getInputPack());
		
		m_layout->getFuncBlock()->getRight()->addSpace(1);
		m_layout->getFuncBlock()->getRight()->addConnectorPack(getComponent()->getOutputPack());
		
		m_layout->setMinSize(4,5);
	
		new ComponentLayoutBlockContentText(m_layout->getFuncBlock(), QString::fromLatin1("Latch"), AlignCenter);

		new ConnectorLabel(getComponent()->getInputReset(), QString::fromLatin1("R"));
		new ConnectorLabel(getComponent()->getInputEnable(), QString::fromLatin1("E"));
	}
}

//###############################################################
//###############################################################

MultiDLatchPropertyGeneralWidget::MultiDLatchPropertyGeneralWidget(MultiDLatch * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	m_min = QMAX ( comp->getInputPack()->getNumberOfNotDeletableConnectors(),
	               comp->getOutputPack()->getNumberOfNotDeletableConnectors());

	
	m_channelsLabel = new QLabel(i18n("Boolean", "Number of latches: "), this, "m_channelsLabel");
	Q_CHECK_PTR(m_channelsLabel);
	
	m_channels = new KSimSpinBox(m_min, MAX_CHANNELS, 1, this, "m_channels");
	Q_CHECK_PTR(m_channels);
	tip = i18n("Boolean", "Change number of latches here.");
	addToolTip(tip, m_channels, m_channelsLabel);
	addWhatsThis(tip, m_channels, m_channelsLabel);
	m_channelsLabel->setBuddy(m_channels);
	
	
	m_resetValueLabel = new QLabel(i18n("Boolean", "Reset value: "), this, "m_resetValueLabel");
	Q_CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimBooleanBox(this, "m_resetValue");
	Q_CHECK_PTR(m_resetValue);
	tip = i18n("Boolean", "Set the reset value of the latches here.");
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
void MultiDLatchPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getResetValue() != m_resetValue->getValue())
	{
		changeData();
		getComponent()->setResetValue(m_resetValue->getValue());
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
void MultiDLatchPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(DEFAULT_RESET_VALUE);
	m_channels->setValue(QMAX(DEFAULT_CHANNELS, m_min));
}

#undef DEFAULT_RESET_VALUE
#undef MIN_CHANNELS
#undef MAX_CHANNELS
#undef DEFAULT_CHANNELS


//###############################################################
//###############################################################

};  //namespace KSimLibBoolean

#include "multidlatch.moc"


