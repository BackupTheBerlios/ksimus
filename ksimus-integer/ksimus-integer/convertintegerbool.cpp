/***************************************************************************
                          convertintegerbool.cpp  -  description
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


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/wireproperty.h"
#include "convertintegerbool.h"

// Forward declaration

namespace KSimLibInteger
{

#define DEFAULT_TRUE_THRESHOLD   0.55
#define DEFAULT_FALSE_THRESHOLD  0.45

//###############################################################
//###############################################################

Component * ConvertIntegerBool::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertIntegerBool(container, ci);
}

const ComponentInfo * ConvertIntegerBool::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Integer to Boolean"),
	                                QString::fromLatin1("Converter/Integer to Boolean"),
	                                i18n("Component", "Converter/Integer to Boolean"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-float-bool")*/);
	return &Info;
}

const KSimBaseInt ConvertIntegerBool::DefaultFalseThreshold(0, KSimBaseInt::Decimal);
const KSimBaseInt ConvertIntegerBool::DefaultTrueThreshold(1, KSimBaseInt::Decimal);

//###############################################################
//###############################################################


ConvertIntegerBool::ConvertIntegerBool(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_falseThreshold(DefaultFalseThreshold),
		m_trueThreshold(DefaultTrueThreshold),
		m_recursionLocked(false)
	
{
	setZeroDelayComponent(true);
	
	m_input = new ConnectorIntegerIn(this,
	                               QString::fromLatin1("Input"),
	                               i18n("Integer-Connector", "Input"));
	CHECK_PTR(m_input);
	
	m_output = new ConnectorBoolOut(this,
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

/*ConvertIntegerBool::~ConvertIntegerBool()
{
} */

void ConvertIntegerBool::reset()
{
	Component::reset();
	
	m_recursionLocked = false;
}

void ConvertIntegerBool::calculate()
{
	Component::calculate();

	if (!m_recursionLocked)
	{
		m_recursionLocked = true;

		bool state, oldState;
		state = oldState = getOutput()->getOutput();
		int input = getInput()->getInput();

		if (getFalseThreshold().value() <= getTrueThreshold().value())
		{
			if(input >= getTrueThreshold().value())
			{
				state = true;
			}
			else if(input <= getFalseThreshold().value())
			{
				state = false;
			}
		}
		else
		{
			if(input <= getTrueThreshold().value())
			{
				state = true;
			}
			else if(input >= getFalseThreshold().value())
			{
				state = false;
			}
		}
		if (state != oldState)
		{
			getOutput()->setOutput(state, false);
			if (getOutput()->getWireProperty())
			{
				getOutput()->getWireProperty()->execute();
			}
		}
		m_recursionLocked = false;
	}
	else
	{
		executeNext();
	}
}

void ConvertIntegerBool::save(KSimData & file) const
{
	Component::save(file);
	
	if (m_falseThreshold != DefaultFalseThreshold)
	{
		m_falseThreshold.save(file, "False Threshold");
	}
	if (m_trueThreshold != DefaultTrueThreshold)
	{
		m_trueThreshold.save(file, "True Threshold");
	}
}

bool ConvertIntegerBool::load(KSimData & file, bool copyLoad)
{
	if(!m_falseThreshold.load(file, "False Threshold"))
	{
		m_falseThreshold = DefaultFalseThreshold;
	}
	if(!m_trueThreshold.load(file, "True Threshold"))
	{
		m_trueThreshold = DefaultTrueThreshold;
	}

	return Component::load(file, copyLoad);
}

void ConvertIntegerBool::setFalseThreshold(const KSimBaseInt & limit)
{
	m_falseThreshold = limit;
}
	
void ConvertIntegerBool::setTrueThreshold(const KSimBaseInt & limit)
{
	m_trueThreshold = limit;
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertIntegerBool::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


//###############################################################
//###############################################################


ConvertIntegerBool::View::View(ConvertIntegerBool * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getConvertIntegerBool()->getInput()->setGridPos(0,1);
	getConvertIntegerBool()->getOutput()->setGridPos(4,1);
}
/*ConvertIntegerBool::View::~View()
{
}*/

void ConvertIntegerBool::View::draw(QPainter * p)
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


ConvertIntegerBool::PropertyGeneralWidget::PropertyGeneralWidget(ConvertIntegerBool * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_trueThresholdLabel = new QLabel(i18n("Integer", "True Threshold: "), this, "m_trueThresholdLabel");
	CHECK_PTR(m_trueThresholdLabel);
	
	m_trueThreshold = new KSimBaseIntEdit(this, "m_trueThreshold");
	CHECK_PTR(m_trueThreshold);
	tip = i18n("Integer", "Sets the threshold where the converter switch to true.");
	addToolTip(tip, m_trueThreshold, m_trueThresholdLabel);
	addWhatsThis(tip, m_trueThreshold, m_trueThresholdLabel);
	m_trueThresholdLabel->setBuddy(m_trueThreshold);


	m_falseThresholdLabel = new QLabel(i18n("Integer", "False Threshold: "), this, "m_falseThresholdLabel");
	CHECK_PTR(m_falseThresholdLabel);

	m_falseThreshold = new KSimBaseIntEdit(this, "m_falseThreshold");
	CHECK_PTR(m_falseThreshold);
	tip = i18n("Integer", "Sets the threshold where the converter switch to false.");
	addToolTip(tip, m_falseThreshold, m_falseThresholdLabel);
	addWhatsThis(tip, m_falseThreshold, m_falseThresholdLabel);
	m_falseThresholdLabel->setBuddy(m_falseThreshold);

	// Setup value
	m_trueThreshold->setValue(comp->getTrueThreshold());
	m_falseThreshold->setValue(comp->getFalseThreshold());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ConvertIntegerBool::PropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getConvertIntegerBool()->getTrueThreshold() != m_trueThreshold->value())
	{
		changeData();
		getConvertIntegerBool()->setTrueThreshold(m_trueThreshold->value());
	}
	if (getConvertIntegerBool()->getFalseThreshold() != m_falseThreshold->value())
	{
		changeData();
		getConvertIntegerBool()->setFalseThreshold(m_falseThreshold->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ConvertIntegerBool::PropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_trueThreshold->setValue(DefaultTrueThreshold);
	m_falseThreshold->setValue(DefaultFalseThreshold);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

