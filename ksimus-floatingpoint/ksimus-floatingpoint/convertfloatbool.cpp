/***************************************************************************
                          convertfloatbool.cpp  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "ksimus/ksimdoubleedit.h"
#include "convertfloatbool.h"

// Forward declaration

namespace KSimLibFloatingPoint
{

#define DEFAULT_TRUE_THRESHOLD   0.55
#define DEFAULT_FALSE_THRESHOLD  0.45

//###############################################################
//###############################################################

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertFloatBool(container, ci);
}

const ComponentInfo * getConvertFloatBoolInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Floating Point to Boolean"),
	                                QString::fromLatin1("Converter/Float to Boolean"),
	                                i18n("Component", "Converter/Float to Boolean"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-float-bool"));
	return &Info;
}


//###############################################################
//###############################################################


ConvertFloatBool::ConvertFloatBool(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci),
		m_falseThreshold(DEFAULT_FALSE_THRESHOLD),
		m_trueThreshold(DEFAULT_TRUE_THRESHOLD)
	
{
	m_input = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"));
	CHECK_PTR(m_input);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ConvertFloatBoolView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/*ConvertFloatBool::~ConvertFloatBool()
{
} */

void ConvertFloatBool::calculate()
{
	Boolean1Out::calculate();

	bool state = getState();
	double input = getInput()->getInput();
	
	if (getFalseThreshold() <= getTrueThreshold())
	{
		if(input >= getTrueThreshold())
		{
			state = true;
		}
		else if(input <= getFalseThreshold())
		{
			state = false;
		}
	}
	else
	{
		if(input <= getTrueThreshold())
		{
			state = true;
		}
		else if(input >= getFalseThreshold())
		{
			state = false;
		}
	}
	
	setState(state);
}

void ConvertFloatBool::save(KSimData & file) const
{
	Boolean1Out::save(file);
	
	if (getTrueThreshold() != DEFAULT_TRUE_THRESHOLD)
	{
		file.writeEntry("True Threshold", getTrueThreshold());
	}
	if (getFalseThreshold() != DEFAULT_FALSE_THRESHOLD)
	{
		file.writeEntry("False Threshold", getFalseThreshold());
	}
}

bool ConvertFloatBool::load(KSimData & file, bool copyLoad)
{
	setTrueThreshold(file.readDoubleNumEntry("True Threshold", DEFAULT_TRUE_THRESHOLD));
	setFalseThreshold(file.readDoubleNumEntry("False Threshold", DEFAULT_FALSE_THRESHOLD));
	
	return Boolean1Out::load(file, copyLoad);
}

void ConvertFloatBool::setFalseThreshold(double limit)
{
	m_falseThreshold = limit;
}
	
void ConvertFloatBool::setTrueThreshold(double limit)
{
	m_trueThreshold = limit;
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertFloatBool::createGeneralProperty(QWidget *parent)
{
	ConvertFloatBoolPropertyGeneralWidget * wid;
	wid = new ConvertFloatBoolPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


//###############################################################
//###############################################################


ConvertFloatBoolView::ConvertFloatBoolView(ConvertFloatBool * comp, eViewType viewType)
	: Boolean1OutView(comp, viewType)
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
/*ConvertFloatBoolView::~ConvertFloatBoolView()
{
}*/

void ConvertFloatBoolView::draw(QPainter * p)
{
	Boolean1OutView::draw(p);
		
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


ConvertFloatBoolPropertyGeneralWidget::ConvertFloatBoolPropertyGeneralWidget(ConvertFloatBool * comp, QWidget *parent, const char *name)
	:	Boolean1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_trueThresholdLabel = new QLabel(i18n("FloatingPoint", "True Threshold: "), getGrid(), "m_trueThresholdLabel");
	CHECK_PTR(m_trueThresholdLabel);
	
	m_trueThreshold = new KSimDoubleEdit(getGrid(), "m_trueThreshold");
	CHECK_PTR(m_trueThreshold);
	tip = i18n("FloatingPoint", "Sets the threshold where the converter switch to true.");
	addToolTip(tip, m_trueThreshold, m_trueThresholdLabel);
	addWhatsThis(tip, m_trueThreshold, m_trueThresholdLabel);
	
	
	
	
	m_falseThresholdLabel = new QLabel(i18n("FloatingPoint", "False Threshold: "), getGrid(), "m_falseThresholdLabel");
	CHECK_PTR(m_falseThresholdLabel);
	
	m_falseThreshold = new KSimDoubleEdit(getGrid(), "m_falseThreshold");
	CHECK_PTR(m_falseThreshold);
	tip = i18n("FloatingPoint", "Sets the threshold where the converter switch to false.");
	addToolTip(tip, m_falseThreshold, m_falseThresholdLabel);
	addWhatsThis(tip, m_falseThreshold, m_falseThresholdLabel);
	
	// Some label
	m_levelLabel = new QLabel(i18n("FloatingPoint", "Medium Threshold: "), getGrid(), "m_levelLabel");
	CHECK_PTR(m_levelLabel);
	m_level = new QLabel(QString::null, getGrid(), "m_level");
	CHECK_PTR(m_level);
	m_level->setAlignment(AlignRight);
	tip = i18n("FloatingPoint", "Shows the medium value between 'False Threshold' and 'True Threshold'.");
	addToolTip(tip, m_level, m_levelLabel);
	addWhatsThis(tip, m_level, m_levelLabel);
	
	m_hysteresisLabel = new QLabel(i18n("FloatingPoint", "Hysteresis: "), getGrid(), "m_hysteresisLabel");
	CHECK_PTR(m_hysteresisLabel);
	m_hysteresis = new QLabel(QString::null, getGrid(), "m_hysteresis");
	CHECK_PTR(m_hysteresis);
	m_hysteresis->setAlignment(AlignRight);
	tip = i18n("FloatingPoint", "Shows the hysteresis between 'False Threshold' and 'True Threshold'.");
	addToolTip(tip, m_hysteresis, m_hysteresisLabel);
	addWhatsThis(tip, m_hysteresis, m_hysteresisLabel);
	
	connect(m_trueThreshold, SIGNAL(valueChangedTracking(double)), this, SLOT(valChanged()));
	connect(m_trueThreshold, SIGNAL(valueChanged(double)), this, SLOT(valChanged()));
	connect(m_falseThreshold, SIGNAL(valueChangedTracking(double)), this, SLOT(valChanged()));
	connect(m_falseThreshold, SIGNAL(valueChanged(double)), this, SLOT(valChanged()));
	
	
	// Setup value
	m_trueThreshold->setValue(comp->getTrueThreshold());
	m_falseThreshold->setValue(comp->getFalseThreshold());
	
	valChanged();
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ConvertFloatBoolPropertyGeneralWidget::acceptPressed()
{
	Boolean1OutPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getTrueThreshold() != m_trueThreshold->value())
	{
		changeData();
		getComponent()->setTrueThreshold(m_trueThreshold->value());
	}
	if (getComponent()->getFalseThreshold() != m_falseThreshold->value())
	{
		changeData();
		getComponent()->setFalseThreshold(m_falseThreshold->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ConvertFloatBoolPropertyGeneralWidget::defaultPressed()
{
	Boolean1OutPropertyGeneralWidget::defaultPressed();

	m_trueThreshold->setValue(DEFAULT_TRUE_THRESHOLD);
	m_falseThreshold->setValue(DEFAULT_FALSE_THRESHOLD);
}

void ConvertFloatBoolPropertyGeneralWidget::valChanged()
{
	m_level->setNum((m_trueThreshold->value() + m_falseThreshold->value())/2.0);
	m_hysteresis->setNum(fabs(m_trueThreshold->value() - m_falseThreshold->value()));
}

//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

