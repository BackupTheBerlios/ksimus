/***************************************************************************
                          convertfloatinteger.cpp  -  description
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
#include <math.h>
#include <limits.h>

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>
#include <qcombobox.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/ksimdata.h"
#include "ksimus/wireproperty.h"
#include "ksimus/enumdict.h"
#include "convertfloatinteger.h"

// Forward declaration

namespace KSimLibInteger
{

//###############################################################
//###############################################################

Component * ConvertFloatInteger::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ConvertFloatInteger(container, ci);
}

const ComponentInfo * ConvertFloatInteger::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Converter Floating Point to Integer"),
	                                QString::fromLatin1("Converter/Float to Integer"),
	                                i18n("Component", "Converter/Float to Integer"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-converter-float-bool")*/);
	return &Info;
}


//###############################################################
//###############################################################

const ConvertFloatInteger::eRoundType ConvertFloatInteger::DefaultRoundType = ConvertFloatInteger::eRound;

static EnumDict<ConvertFloatInteger::eRoundType> convertFloatIntegerRoundTypeDict(3);


EnumDict<ConvertFloatInteger::eRoundType>::tData EnumDict<ConvertFloatInteger::eRoundType>::data[]
      = { {"Round", KSimLibInteger::ConvertFloatInteger::eRound},
          {"Floor", KSimLibInteger::ConvertFloatInteger::eFloor},
          {"Ceil",  KSimLibInteger::ConvertFloatInteger::eCeil},
          {0,       (KSimLibInteger::ConvertFloatInteger::eRoundType)0}};

ConvertFloatInteger::ConvertFloatInteger(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_roundType(DefaultRoundType),
		m_recursionLocked(false)
	
{
	setZeroDelayComponent(true);
	
	m_input = new ConnectorFloatIn(this,
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

/*ConvertFloatInteger::~ConvertFloatInteger()
{
} */

void ConvertFloatInteger::reset()
{
	Component::reset();
	
	m_recursionLocked = false;
}

void ConvertFloatInteger::calculate()
{
//	Component::calculate();

	if (!m_recursionLocked)
	{
		
		m_recursionLocked = true;

		double newValue = 0;
		switch(getRoundType())
		{
			case eRound: newValue = qRound(getInput()->getInput()); break;
			case eFloor: newValue = floor(getInput()->getInput());  break;
			case eCeil:  newValue = ceil(getInput()->getInput());   break;
		}

		int val;
		if      (newValue >= (double)INT_MAX) val = INT_MAX;
		else if (newValue <= (double)INT_MIN) val = INT_MIN;
		else                                  val = (int)newValue;

		if (getOutput()->getOutput() != val)
		{
			getOutput()->setOutput(val, false);
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

void ConvertFloatInteger::save(KSimData & file) const
{
	Component::save(file);
	
	if (getRoundType() != DefaultRoundType)
	{
		convertFloatIntegerRoundTypeDict.save(file, "Round Type", getRoundType());
	}
}

bool ConvertFloatInteger::load(KSimData & file, bool copyLoad)
{
	setRoundType(convertFloatIntegerRoundTypeDict.load(file, "Round Type", DefaultRoundType));
	
	return Component::load(file, copyLoad);
}

void ConvertFloatInteger::setRoundType(eRoundType type)
{
	m_roundType = type;
}
	


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ConvertFloatInteger::createGeneralProperty(QWidget *parent)
{
	PropertyGeneralWidget * wid;
	wid = new PropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


//###############################################################
//###############################################################


ConvertFloatInteger::View::View(ConvertFloatInteger * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
	
	enableRotation(true);
	
	getConvertFloatInteger()->getInput()->setGridPos(0,1);
	getConvertFloatInteger()->getOutput()->setGridPos(4,1);
}
/*ConvertFloatInteger::View::~View()
{
}*/

void ConvertFloatInteger::View::draw(QPainter * p)
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


ConvertFloatInteger::PropertyGeneralWidget::PropertyGeneralWidget(ConvertFloatInteger * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_roundTypeLabel = new QLabel(i18n("Integer", "Rounding Type: "), this, "m_roundTypeLabel");
	CHECK_PTR(m_roundTypeLabel);

	m_roundType = new QComboBox(this, "m_roundType");
	CHECK_PTR(m_roundType);
	m_roundType->insertItem(i18n("Integer", "Round"), (int)ConvertFloatInteger::eRound);
	m_roundType->insertItem(i18n("Integer", "Floor"), (int)ConvertFloatInteger::eFloor);
	m_roundType->insertItem(i18n("Integer", "Ceil"),  (int)ConvertFloatInteger::eCeil);
	tip = i18n("Integer", "Sets the rounding type.");
	addToolTip(tip, m_roundType, m_roundTypeLabel);
	tip += i18n("Integer", "\nRound:\tRounds to the nearest integer"
	                       "\nFloor:\tRounds down to the nearest integer"
	                       "\nCeil:\tRounds up to the nearest integer");
	addWhatsThis(tip, m_roundType, m_roundTypeLabel);
	m_roundTypeLabel->setBuddy(m_roundType);

	// Setup value
	m_roundType->setCurrentItem((int)comp->getRoundType());
	
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ConvertFloatInteger::PropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getConvertFloatInteger()->getRoundType() != (ConvertFloatInteger::eRoundType)m_roundType->currentItem())
	{
		changeData();
		getConvertFloatInteger()->setRoundType((ConvertFloatInteger::eRoundType)m_roundType->currentItem());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ConvertFloatInteger::PropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_roundType->setCurrentItem(ConvertFloatInteger::DefaultRoundType);
}

//###############################################################
//###############################################################

};  //namespace KSimLibInteger

