/***************************************************************************
                          multibool2integer.cpp  -  description
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// KSimus-Includes
#include "ksimus/ksimspinbox.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorpack.h"
#include "ksimus/connectorlabel.h"
#include "ksimus/componentlayout.h"

// Project-Includes
#include "multibool2integer.h"


// Forward declaration

namespace KSimLibInteger
{


//###############################################################
//###############################################################


Component * MultiBool2Integer::create(CompContainer * container, const ComponentInfo * ci)
{
	return new MultiBool2Integer(container, ci);
}


const ComponentInfo * MultiBool2Integer::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Multiple Booleans to Integer"),
	                                QString::fromLatin1("Converter/Multiple Booleans to Integer"),
	                                i18n("Component", "Converter/Multiple Booleans to Integer"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-control-latch")*/);
	return &Info;
}


#define MIN_INPUTS         1
#define MAX_INPUTS         32
#define DEFAULT_INPUTS     2

MultiBool2Integer::MultiBool2Integer(CompContainer * container, const ComponentInfo * ci)
	:	Integer1Out(container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             MIN_INPUTS, MAX_INPUTS);
	CHECK_PTR(m_inPack);
	connect(m_inPack, SIGNAL(signalAddConnector(ConnectorBase *)), this, SLOT(addConn(ConnectorBase *)));
	m_inPack->setLetter(false);
	m_inPack->setConnectorCount(DEFAULT_INPUTS);

	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}
}

/*MultiBool2Integer::~MultiBool2Integer()
{
} */


/** Executes the simulation of this component */
void MultiBool2Integer::calculate()
{
//	Integer1Out::calculate();

	unsigned int result = 0;
	unsigned int mask = 1;

	FOR_EACH_CONNECTOR(it, *getInputConnectorPack()->getConnList())
	{
		if (((ConnectorBoolIn*)it.current())->getInput())
			result |= mask;
		mask <<= 1;
	}

	setValue((int)result);
}

ComponentPropertyBaseWidget * MultiBool2Integer::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

void MultiBool2Integer::setInputCount(unsigned int count)
{
	if (count < MIN_INPUTS) count = MIN_INPUTS;
	if (count > MAX_INPUTS) count = MAX_INPUTS;
	getInputConnectorPack()->setConnectorCount(count);
}

int MultiBool2Integer::getInputCount() const
{
	return getInputConnectorPack()->getConnectorCount();
}

int MultiBool2Integer::tempConnCount;
MultiBool2Integer * MultiBool2Integer::tempConnCountOwner = 0;

void MultiBool2Integer::setInputCountDelayed(unsigned int count)
{
	tempConnCount = count;
	tempConnCountOwner = this;
}

void MultiBool2Integer::menuExecuted()
{
	Integer1Out::menuExecuted();

	// Delayed connector count setting.
	if (tempConnCountOwner == this)
	{
		tempConnCountOwner = 0;
		if (tempConnCount != getInputCount())
		{
			setInputCount(tempConnCount);
		}
	}
}

void MultiBool2Integer::addConn(ConnectorBase * conn)
{
	new ConnectorLabel(conn, QString::number(getInputConnectorPack()->getConnectorCount()));
}

//###############################################################
//###############################################################



MultiBool2Integer::View::View(MultiBool2Integer * comp, eViewType viewType)
	: Integer1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(2);
		getComponentLayout()->getLeft()->addConnectorPack(comp->getInputConnectorPack());

		getComponentLayout()->updateLayout();
	}
}

/*MultiBool2Integer::View::~View()
{
}*/


void MultiBool2Integer::View::draw(QPainter * p)
{
	Integer1OutView::draw(p);

	QFont newFont("helvetica",8);
	p->setFont(newFont);
//	p->drawText(getDrawingPlace(), AlignCenter, "Bool\nto\nInt");
	QRect rect(getDrawingPlace());
	rect.rTop() += 1;
	p->drawText(rect, AlignTop|AlignHCenter, "B>I");
}



//###############################################################
//###############################################################

MultiBool2Integer::PropertyGeneralWidget::PropertyGeneralWidget(MultiBool2Integer * comp, QWidget *parent, const char *name)
	:	Integer1OutPropertyGeneralWidget(comp, parent, name)
{

	QString tip;
	m_min = QMAX(MIN_INPUTS, comp->getInputConnectorPack()->getNumberOfNotDeletableConnectors());

	m_inputsLabel = new QLabel(i18n("Integer", "Number of inputs:"), this, "m_inputsLabel");
	CHECK_PTR(m_inputsLabel);

	m_inputs = new KSimSpinBox(m_min, MAX_INPUTS, 1, this, "m_inputs");
	CHECK_PTR(m_inputs);
	tip = i18n("Integer", "Changes number of boolean inputs.");
	addToolTip(tip, m_inputs, m_inputsLabel);
	addWhatsThis(tip, m_inputs, m_inputsLabel);
	m_inputsLabel->setBuddy(m_inputs);


	// Setup value
	m_inputs->setValue(comp->getInputCount());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void MultiBool2Integer::PropertyGeneralWidget::acceptPressed()
{
	Integer1OutPropertyGeneralWidget::acceptPressed();

	if (getComponent()->getInputCount() != m_inputs->value())
	{
		changeData();
		getComponent()->setInputCountDelayed(m_inputs->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void MultiBool2Integer::PropertyGeneralWidget::defaultPressed()
{
	Integer1OutPropertyGeneralWidget::defaultPressed();

	m_inputs->setValue(QMAX(DEFAULT_INPUTS, m_min));
}


#undef MIN_INPUTS
#undef MAX_INPUTS
#undef DEFAULT_INPUTS

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

