/***************************************************************************
                          convertboolfloat.cpp  -  description
                             -------------------
    begin                : Mon Feb 11 2002
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
#include <math.h>

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
#include "ksimus/connectorfloatout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/wireproperty.h"

#include "convertboolfloat.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################

#define DEFAULT_TRUE_VALUE   1.0
#define DEFAULT_FALSE_VALUE  0.0


Component * ConvertBoolFloat::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBoolFloat(container, ci);
}


const ComponentInfo * ConvertBoolFloat::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Boolean to Floating Point"),
	                                QString::fromLatin1("Converter/Boolean to Float"),
	                                i18n("Component", "Converter/Boolean to Float"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-bool-float"));
	return &Info;
}



//###############################################################
//###############################################################


ConvertBoolFloat::ConvertBoolFloat(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_falseValue(DEFAULT_FALSE_VALUE),
		m_trueValue(DEFAULT_TRUE_VALUE),
		m_recursionLocked(false)
{
	setZeroDelayComponent(true);
	
	m_input = new ConnectorBoolIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"));
	Q_CHECK_PTR(m_input);
	
	m_output = new ConnectorFloatOut(this,
	                                 QString::fromLatin1("Output"),
	                                 i18n("FloatingPoint-Connector", "Output"));
	Q_CHECK_PTR(m_output);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ConvertBoolFloatView(this, SHEET_VIEW, "ConvertBoolFloatSV");
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

void ConvertBoolFloat::reset()
{
	Component::reset();
	
	m_recursionLocked = false;
}

/** Executes the simulation of this component */
void ConvertBoolFloat::calculate()
{
	Component::calculate();
	
	if (!m_recursionLocked)
	{
		m_recursionLocked = true;
	
		double d = getInput()->getInput() ? getTrueValue() : getFalseValue();
		getOutput()->setOutput(d, false);
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

void ConvertBoolFloat::save(KSimData & file) const
{
	Component::save(file);
	
	if (getTrueValue() != DEFAULT_TRUE_VALUE)
	{
		file.writeEntry("True Value", getTrueValue());
	}
	if (getFalseValue() != DEFAULT_FALSE_VALUE)
	{
		file.writeEntry("False Value", getFalseValue());
	}
}

bool ConvertBoolFloat::load(KSimData & file, bool copyLoad)
{
	setTrueValue(file.readDoubleNumEntry("True Value", DEFAULT_TRUE_VALUE));
	setFalseValue(file.readDoubleNumEntry("False Value", DEFAULT_FALSE_VALUE));
	
	return Component::load(file, copyLoad);
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertBoolFloat::createGeneralProperty(QWidget *parent)
{
	ConvertBoolFloatPropertyGeneralWidget * wid;
	wid = new ConvertBoolFloatPropertyGeneralWidget(this, parent);
	Q_CHECK_PTR(wid);
	
	return wid;
}

void ConvertBoolFloat::setTrueValue(double value)
{
	m_trueValue = value;	
}

void ConvertBoolFloat::setFalseValue(double value)
{
	m_falseValue = value;	
}


//###############################################################
//###############################################################


ConvertBoolFloatView::ConvertBoolFloatView(ConvertBoolFloat * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getComponent()->getInput()->setGridPos(0,1);
	getComponent()->getOutput()->setGridPos(4,1);
}
/*ConvertBoolFloatView::~ConvertBoolFloatView()
{
}*/

void ConvertBoolFloatView::draw(QPainter * p)
{
	CompView::draw(p);
		
	drawFrame(p);
	QRect place(getDrawingPlace());
	p->setPen(QPen(black, 1));
	p->drawLine(place.bottomLeft()+QPoint(1,0), place.topRight()+QPoint(0,1));
	
	if(getRotation() != 0)
	{
		const int yMid = place.top() + place.bottom() / 2;
	
		p->drawLine(QPoint(place.left()+4, yMid) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid-3) , QPoint(place.right()-4, yMid));
		p->drawLine(QPoint(place.right()-8, yMid+3) , QPoint(place.right()-4, yMid));
	}
}


//###############################################################
//###############################################################

ConvertBoolFloatPropertyGeneralWidget::ConvertBoolFloatPropertyGeneralWidget(ConvertBoolFloat * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_trueValueLabel = new QLabel(i18n("FloatingPoint", "True Value: "), this, "m_trueValueLabel");
	Q_CHECK_PTR(m_trueValueLabel);
	
	m_trueValue = new KSimDoubleEdit(this, "m_trueValue");
	Q_CHECK_PTR(m_trueValue);
	tip = i18n("FloatingPoint", "Sets the value which represents the true level.");
	addToolTip(tip, m_trueValue, m_trueValueLabel);
	addWhatsThis(tip, m_trueValue, m_trueValueLabel);
	m_trueValueLabel->setBuddy(m_trueValue);
	
	
	m_falseValueLabel = new QLabel(i18n("FloatingPoint", "False Value: "), this, "m_falseValueLabel");
	Q_CHECK_PTR(m_falseValueLabel);
	
	m_falseValue = new KSimDoubleEdit(this, "m_falseValue");
	Q_CHECK_PTR(m_falseValue);
	tip = i18n("FloatingPoint", "Sets the value which represents the false level.");
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
void ConvertBoolFloatPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getTrueValue() != m_trueValue->value())
	{
		changeData();
		getComponent()->setTrueValue(m_trueValue->value());
	}
	if (getComponent()->getFalseValue() != m_falseValue->value())
	{
		changeData();
		getComponent()->setFalseValue(m_falseValue->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ConvertBoolFloatPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_trueValue->setValue(DEFAULT_TRUE_VALUE);
	m_falseValue->setValue(DEFAULT_FALSE_VALUE);
}


#undef DEFAULT_TRUE_VALUE
#undef DEFAULT_FALSE_VALUE

//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

#include "convertboolfloat.moc"
