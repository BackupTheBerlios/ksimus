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
#include <qgrid.h>
#include <qcombobox.h>
#include <qpixmap.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"

#include "convertboolfloat.h"

// Forward declaration

// Project-Includes


namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################

#define DEFAULT_TRUE_VALUE   1.0
#define DEFAULT_FALSE_VALUE  0.0


static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertBoolFloat(container, ci);
}


const ComponentInfo ConvertBoolFloatInfo(
                     I18N_NOOP("Converter Boolean to Floating Point"),
                     I18N_NOOP("Converter/Boolean to Float"),
                     QString::null,
                     VA_SHEETVIEW,
                     create,
                     QString::null,
                     "component-float-converter-bool-float");



//###############################################################
//###############################################################


ConvertBoolFloat::ConvertBoolFloat(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci),
		m_falseValue(DEFAULT_FALSE_VALUE),
		m_trueValue(DEFAULT_TRUE_VALUE)
{
	m_input = new ConnectorBoolIn (this, I18N_NOOP("Input"));
	CHECK_PTR(m_input);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ConvertBoolFloatView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/** Executes the simulation of this component */
void ConvertBoolFloat::calculate()
{
	Float1Out::calculate();
	
	if (getInput()->getInput())
	{
		setValue(getTrueValue());
	}
	else
	{
		setValue(getFalseValue());
	}
}

void ConvertBoolFloat::save(KSimData & file) const
{
	Float1Out::save(file);
	
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
	
	return Float1Out::load(file, copyLoad);
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertBoolFloat::createGeneralProperty(Component * comp, QWidget *parent)
{
	ConvertBoolFloatPropertyGeneralWidget * wid;
	wid = new ConvertBoolFloatPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
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


ConvertBoolFloatView::ConvertBoolFloatView(ConvertBoolFloat * comp, eViewType viewType)
	: Float1OutView(comp, viewType)
{
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->setMinSize(5,3);
		
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(comp->getInput());
	
		getComponentLayout()->updateLayout();
	}
}
/*ConvertBoolFloatView::~ConvertBoolFloatView()
{
}*/

void ConvertBoolFloatView::draw(QPainter * p)
{
	Float1OutView::draw(p);
		
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

ConvertBoolFloatPropertyGeneralWidget::ConvertBoolFloatPropertyGeneralWidget(ConvertBoolFloat * comp, QWidget *parent, const char *name)
	:	Float1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_trueValueLabel = new QLabel(i18n("True Value: "), getGrid(), "m_trueValueLabel");
	CHECK_PTR(m_trueValueLabel);
	
	m_trueValue = new KSimDoubleEdit(getGrid(), "m_trueValue");
	CHECK_PTR(m_trueValue);
	tip = i18n("Sets the value which represents the true level.");
	addToolTip(tip, m_trueValue, m_trueValueLabel);
	addWhatsThis(tip, m_trueValue, m_trueValueLabel);
	
	
	
	
	m_falseValueLabel = new QLabel(i18n("False Value: "), getGrid(), "m_falseValueLabel");
	CHECK_PTR(m_falseValueLabel);
	
	m_falseValue = new KSimDoubleEdit(getGrid(), "m_falseValue");
	CHECK_PTR(m_falseValue);
	tip = i18n("Sets the value which represents the false level.");
	addToolTip(tip, m_falseValue, m_falseValueLabel);
	addWhatsThis(tip, m_falseValue, m_falseValueLabel);
	
	
	
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
	Float1OutPropertyGeneralWidget::acceptPressed();
	
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
	Float1OutPropertyGeneralWidget::defaultPressed();

	m_trueValue->setValue(DEFAULT_TRUE_VALUE);
	m_falseValue->setValue(DEFAULT_FALSE_VALUE);
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

