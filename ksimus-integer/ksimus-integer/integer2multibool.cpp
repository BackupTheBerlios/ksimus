/***************************************************************************
                          integer2multibool.cpp  -  description
                             -------------------
    begin                : Sat Sep 6 2003
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/connectorlabel.h"

#include "integer2multibool.h"

// Forward declaration


namespace KSimLibInteger
{

//###############################################################
//###############################################################


Component * Integer2MultiBool::create(CompContainer * container, const ComponentInfo * ci)
{
	return new Integer2MultiBool(container, ci);
}


const ComponentInfo * Integer2MultiBool::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Integer to Multiple Booleans"),
	                                QString::fromLatin1("Converter/Integer to Multiple Booleans"),
	                                i18n("Component", "Converter/Integer to Multiple Booleans"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-latch")*/);
	return &Info;
}


#define MIN_OUTPUTS         1
#define MAX_OUTPUTS         32
#define DEFAULT_OUTPUTS     2


const KSimBaseInt Integer2MultiBool::DefaultResetValue(0, KSimBaseInt::Decimal);

Integer2MultiBool::Integer2MultiBool(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_resetValue(DefaultResetValue)
{
	
	m_input = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"));
	CHECK_PTR(m_input);
	
	m_outPack = new ConnectorPack(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Connector", "Output %1"),
	                             getConnectorBoolOutInfo(),
	                             MIN_OUTPUTS, MAX_OUTPUTS);
	CHECK_PTR(m_outPack);
	connect(m_outPack, SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addConn(ConnectorBase *)));
	m_outPack->setLetter(false);
	m_outPack->setConnectorCount(DEFAULT_OUTPUTS);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW, "Integer2MultiBoolSV");
	}
}

/*Integer2MultiBool::~Integer2MultiBool()
{
} */

void Integer2MultiBool::setResetValue(const KSimBaseInt & resetValue)
{
	m_resetValue = resetValue;
}

void Integer2MultiBool::reset()
{
	Component::reset();
	
	setValue(getResetValue().value());
}

/** Executes the simulation of this component */
void Integer2MultiBool::calculate()
{
	Component::calculate();

	setValue(getInputConnector()->getInput());
}

/** save component properties */
void Integer2MultiBool::save(KSimData & file) const
{
	Component::save(file);
	
	if (m_resetValue != DefaultResetValue)
	{
		m_resetValue.save(file, "Reset Value");
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Integer2MultiBool::load(KSimData & file, bool copyLoad)
{
	if(!m_resetValue.load(file, "Reset Value"))
	{
		m_resetValue = DefaultResetValue;
	}
	
	return Component::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * Integer2MultiBool::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void Integer2MultiBool::setValue(int newValue)
{
	unsigned int mask = 1;

	FOR_EACH_CONNECTOR(it, *getOutputConnectorPack()->getConnList())
	{
		((ConnectorBoolOut*)it.current())->setOutput((unsigned int)newValue & mask);
		mask <<= 1;
	}
};


void Integer2MultiBool::setOutputCount(unsigned int count)
{
	if (count < MIN_OUTPUTS) count = MIN_OUTPUTS;
	if (count > MAX_OUTPUTS) count = MAX_OUTPUTS;
	getOutputConnectorPack()->setConnectorCount(count);
}

int Integer2MultiBool::getOutputCount() const
{
	return getOutputConnectorPack()->getConnectorCount();
}

int Integer2MultiBool::tempConnCount;
Integer2MultiBool * Integer2MultiBool::tempConnCountOwner = 0;

void Integer2MultiBool::setOutputCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void Integer2MultiBool::menuExecuted()
{
	Component::menuExecuted();

	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = 0;
		if (tempConnCount != getOutputCount())
		{
			setOutputCount(tempConnCount);
		}
	}
}

void Integer2MultiBool::addConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString::number(getOutputConnectorPack()->getConnectorCount()));
}

//###############################################################
//###############################################################


Integer2MultiBool::View::View(Integer2MultiBool * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 6*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		CHECK_PTR(layout);
	
		layout->setMinSize(4, 5);

		layout->getLeft()->addStretch(2);
		layout->getLeft()->addConnector(comp->getInputConnector(),0);
		layout->getLeft()->addStretch(2);
	
		layout->getRight()->addSpace(1);
		layout->getRight()->addConnectorPack(comp->getOutputConnectorPack());

//		new ComponentLayoutBlockContentText(layout->getBlock(), "Int -> Bool", AlignTop|AlignHCenter, 270.0);
		new ComponentLayoutBlockContentText(layout->getBlock(), QString::fromLatin1("Int->Bool"), AlignCenter, 270.0);
	}
}
/*Integer2MultiBool::View::~View()
{
}*/

//###############################################################
//###############################################################

Integer2MultiBool::PropertyGeneralWidget::PropertyGeneralWidget(Integer2MultiBool * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel = new QLabel(i18n("Integer", "Reset value: "), this, "ResetValueLabel");
	CHECK_PTR(m_resetValueLabel);
	
	m_resetValue = new KSimBaseIntEdit(this, "ResetValue");
	CHECK_PTR(m_resetValue);
	QString tip(i18n("Integer", "Change the reset value of the component here."));
	addToolTip(tip, m_resetValue, m_resetValueLabel);
	addWhatsThis(tip, m_resetValue, m_resetValueLabel);
	m_resetValueLabel->setBuddy(m_resetValue);
	
	m_min = QMAX(MIN_OUTPUTS, comp->getOutputConnectorPack()->getNumberOfNotDeletableConnectors());

	m_outputsLabel = new QLabel(i18n("Integer", "Number of outputs:"), this, "m_outputsLabel");
	CHECK_PTR(m_outputsLabel);

	m_outputs = new KSimSpinBox(m_min, MAX_OUTPUTS, 1, this, "m_outputs");
	CHECK_PTR(m_outputs);
	tip = i18n("Integer", "Changes number of boolean outputs.");
	addToolTip(tip, m_outputs, m_outputsLabel);
	addWhatsThis(tip, m_outputs, m_outputsLabel);
	m_outputsLabel->setBuddy(m_outputs);


	// Setup value
	m_resetValue->setValue(comp->getResetValue());
	m_outputs->setValue(comp->getOutputCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void Integer2MultiBool::PropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	KSimBaseInt reset = m_resetValue->value();
	if (getInteger2MultiBool()->getResetValue() != reset)
	{
		changeData();
		getInteger2MultiBool()->setResetValue(reset);
	}
	if (getInteger2MultiBool()->getOutputCount() != m_outputs->value())
	{
		changeData();
		getInteger2MultiBool()->setOutputCountDelayed(m_outputs->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void Integer2MultiBool::PropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetValue->setValue(Integer2MultiBool::DefaultResetValue);
	m_outputs->setValue(QMAX(DEFAULT_OUTPUTS, m_min));
}


#undef MIN_OUTPUTS
#undef MAX_OUTPUTS
#undef DEFAULT_OUTPUTS

//###############################################################
//###############################################################


};  //namespace KSimLibInteger

#include "integer2multibool.moc"

