/***************************************************************************
                          convertboolinteger.cpp  -  description
                             -------------------
    begin                : Sun Sep 7 2003
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
#include <qcombobox.h>
#include <qpixmap.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/wireproperty.h"

#include "convertboolinteger.h"

// Forward declaration

// Project-Includes


namespace KSimLibInteger
{


//###############################################################
//###############################################################

Component * ConvertBoolInteger::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBoolInteger(container, ci);
}


const ComponentInfo * ConvertBoolInteger::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Boolean to Integer"),
	                                QString::fromLatin1("Converter/Boolean to Integer"),
	                                i18n("Component", "Converter/Boolean to Integer"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO ,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-bool-float")*/);
	return &Info;
}

const KSimBaseInt ConvertBoolInteger::DefaultFalseValue(0, KSimBaseInt::Decimal);
const KSimBaseInt ConvertBoolInteger::DefaultTrueValue(1, KSimBaseInt::Decimal);



//###############################################################
//###############################################################


ConvertBoolInteger::ConvertBoolInteger(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_falseValue(DefaultFalseValue),
		m_trueValue(DefaultTrueValue),
		m_recursionLocked(false)
{
	setZeroDelayComponent(true);
	
	m_input = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"));
	CHECK_PTR(m_input);
	
	m_output = new ConnectorIntegerOut(this,
	                                 QString::fromLatin1("Output"),
	                                 i18n("Integer-Connector", "Output"));
	CHECK_PTR(m_output);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new View(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void ConvertBoolInteger::reset()
{
	Component::reset();
	
	m_recursionLocked = false;
}

/** Executes the simulation of this component */
void ConvertBoolInteger::calculate()
{
	Component::calculate();
	
	if (!m_recursionLocked)
	{
		m_recursionLocked = true;
	
		int i = getInput()->getInput() ? getTrueValue().value() : getFalseValue().value();
		getOutput()->setOutput(i, false);
		if (getOutput()->getWireProperty())
		{
			getOutput()->getWireProperty()->execute();
		}
		m_recursionLocked = false;
	}
	else
	{
		executeNext();
	}
}

void ConvertBoolInteger::save(KSimData & file) const
{
	Component::save(file);
	
	if (m_falseValue != DefaultFalseValue)
	{
		m_falseValue.save(file, "False Value");
	}
	if (m_trueValue != DefaultTrueValue)
	{
		m_trueValue.save(file, "True Value");
	}
}

bool ConvertBoolInteger::load(KSimData & file, bool copyLoad)
{
	if(!m_falseValue.load(file, "False Value"))
	{
		m_falseValue = DefaultFalseValue;
	}
	if(!m_trueValue.load(file, "True Value"))
	{
		m_trueValue = DefaultTrueValue;
	}

	return Component::load(file, copyLoad);
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertBoolInteger::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void ConvertBoolInteger::setTrueValue(const KSimBaseInt & value)
{
	m_trueValue = value;
}

void ConvertBoolInteger::setFalseValue(const KSimBaseInt & value)
{
	m_falseValue = value;
}


//###############################################################
//###############################################################


ConvertBoolInteger::View::View(ConvertBoolInteger * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getConvertBoolInteger()->getInput()->setGridPos(0,1);
	getConvertBoolInteger()->getOutput()->setGridPos(4,1);
}
/*ConvertBoolInteger::View::~View()
{
}*/

void ConvertBoolInteger::View::draw(QPainter * p)
{
	CompView::draw(p);
		
	drawFrame(p);
	QRect place(getDrawingPlace());
	p->setPen(QPen(black, 1));
	p->drawLine(place.bottomLeft()+QPoint(1,0), place.topRight()+QPoint(0,1));
	
	if(getRotation() != 0)
	{
		int yMid = place.top() + place.bottom() / 2;
	
		p->drawLine(QPoint(place.left()+4, yMid) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid-3) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid+3) , QPoint(place.right()-4, yMid));
	}
}


//###############################################################
//###############################################################

ConvertBoolInteger::PropertyGeneralWidget::PropertyGeneralWidget(ConvertBoolInteger * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_trueValueLabel = new QLabel(i18n("Integer", "True Value: "), this, "m_trueValueLabel");
	CHECK_PTR(m_trueValueLabel);
	
	m_trueValue = new KSimBaseIntEdit(this, "m_trueValue");
	CHECK_PTR(m_trueValue);
	tip = i18n("Integer", "Sets the value which represents the true level.");
	addToolTip(tip, m_trueValue, m_trueValueLabel);
	addWhatsThis(tip, m_trueValue, m_trueValueLabel);
	m_trueValueLabel->setBuddy(m_trueValue);
	
	
	m_falseValueLabel = new QLabel(i18n("Integer", "False Value: "), this, "m_falseValueLabel");
	CHECK_PTR(m_falseValueLabel);
	
	m_falseValue = new KSimBaseIntEdit(this, "m_falseValue");
	CHECK_PTR(m_falseValue);
	tip = i18n("Integer", "Sets the value which represents the false level.");
	addToolTip(tip, m_falseValue, m_falseValueLabel);
	addWhatsThis(tip, m_falseValue, m_falseValueLabel);
	m_falseValueLabel->setBuddy(m_falseValue);
	
	
	
	// Setup value
	m_trueValue->setValue(comp->getTrueValue());
	m_falseValue->setValue(comp->getFalseValue());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ConvertBoolInteger::PropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getConvertBoolInteger()->getTrueValue() != m_trueValue->value())
	{
		changeData();
		getConvertBoolInteger()->setTrueValue(m_trueValue->value());
	}
	if (getConvertBoolInteger()->getFalseValue() != m_falseValue->value())
	{
		changeData();
		getConvertBoolInteger()->setFalseValue(m_falseValue->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ConvertBoolInteger::PropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_trueValue->setValue(ConvertBoolInteger::DefaultTrueValue);
	m_falseValue->setValue(ConvertBoolInteger::DefaultFalseValue);
}



//###############################################################
//###############################################################

};  //namespace KSimLibInteger

