/***************************************************************************
                          simpleconditional.cpp  -  description
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

// QT-Includes
#include <qpainter.h>
#include <qlabel.h>
#include <qgrid.h>
#include <qcombobox.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorboolout.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/componentinfo.h"
#include "ksimus/componentlayout.h"
#include "ksimus/ksimdata.h"
#include "simpleconditional.h"

// Forward declaration

namespace KSimLibFloatingPoint
{

//###############################################################
//###############################################################

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new SimpleConditional(container, ci);
}

const ComponentInfo ConditionalLesserInfo(
                     I18N_NOOP("Floating Point Conditional Lesser"),
                     I18N_NOOP("Floating Point/Conditional/Lesser"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );

const ComponentInfo ConditionalLesserEqualInfo(
                     I18N_NOOP("Floating Point Conditional Lesser Equal"),
                     I18N_NOOP("Floating Point/Conditional/Lesser Equal"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );

const ComponentInfo ConditionalEqualInfo(
                     I18N_NOOP("Floating Point Conditional Equal"),
                     I18N_NOOP("Floating Point/Conditional/Equal"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );

const ComponentInfo ConditionalLargerEqualInfo(
                     I18N_NOOP("Floating Point Conditional Larger Equal"),
                     I18N_NOOP("Floating Point/Conditional/Larger Equal"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );

const ComponentInfo ConditionalLargerInfo(
                     I18N_NOOP("Floating Point Conditional Larger"),
                     I18N_NOOP("Floating Point/Conditional/Larger"),
                     QString::null,
                     VA_SHEETVIEW,
                     create );

//###############################################################
//###############################################################


SimpleConditional::SimpleConditional(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci)
{
	
	if      (ci == &ConditionalLesserInfo)      m_conditionalType = eLesser;
	else if (ci == &ConditionalLesserEqualInfo) m_conditionalType = eLesserEqual;
	else if (ci == &ConditionalEqualInfo)       m_conditionalType = eEqual;
	else if (ci == &ConditionalLargerEqualInfo) m_conditionalType = eLargerEqual;
	else /*if (ci == &ConditionalLargerInfo)*/  m_conditionalType = eLarger;
	
	
	m_inputA = new ConnectorFloatIn (this, I18N_NOOP("Input A"));
	CHECK_PTR(m_inputA);
	
	m_inputB = new ConnectorFloatIn (this, I18N_NOOP("Input B"));
	CHECK_PTR(m_inputB);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new SimpleConditionalView(this, SHEET_VIEW);
	}

	getAction().disable(KSimAction::UPDATEVIEW);
}

/*SimpleConditional::~SimpleConditional()
{
} */

void SimpleConditional::calculate()
{
	Boolean1Out::calculate();

	bool state = false;
	
	switch(getConditionalType())
	{
		case eLesser:      state = getInputA()->getInput() <  getInputB()->getInput(); break;
		case eLesserEqual: state = getInputA()->getInput() <= getInputB()->getInput(); break;
		case eEqual:       state = getInputA()->getInput() == getInputB()->getInput(); break;
		case eLargerEqual: state = getInputA()->getInput() >= getInputB()->getInput(); break;
		case eLarger:      state = getInputA()->getInput() >  getInputB()->getInput(); break;
	}
	setState(state);
}

void SimpleConditional::setConditionalType(eConditionalType newCond)
{
	switch(newCond)
	{
		case eLesser:      setInfo(&ConditionalLesserInfo);      break;
		case eLesserEqual: setInfo(&ConditionalLesserEqualInfo); break;
		case eEqual:       setInfo(&ConditionalEqualInfo);       break;
		case eLargerEqual: setInfo(&ConditionalLargerEqualInfo); break;
		case eLarger:      setInfo(&ConditionalLargerInfo);      break;
	}
	m_conditionalType = newCond;	
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * SimpleConditional::createGeneralProperty(Component * comp, QWidget *parent)
{
	SimpleConditionalPropertyGeneralWidget * wid;
	wid = new SimpleConditionalPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################


SimpleConditionalView::SimpleConditionalView(SimpleConditional * comp, eViewType viewType)
	: Boolean1OutView(comp, viewType)
{
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->setMinSize(6,5);
		
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(comp->getInputA());
		getComponentLayout()->getLeft()->addConnector(comp->getInputB());
	
		getComponentLayout()->updateLayout();
	}
}
/*SimpleConditionalView::~SimpleConditionalView()
{
}*/

void SimpleConditionalView::draw(QPainter * p)
{
	Boolean1OutView::draw(p);
		
	QFont newFont("helvetica",10);
	p->setFont(newFont);
	
	switch(getComponent()->getConditionalType())
	{
		case eLesser:      p->drawText(getDrawingPlace(), AlignCenter, "A<B");  break;
		case eLesserEqual: p->drawText(getDrawingPlace(), AlignCenter, "A<=B"); break;
		case eEqual:       p->drawText(getDrawingPlace(), AlignCenter, "A==B"); break;
		case eLargerEqual: p->drawText(getDrawingPlace(), AlignCenter, "A>=B"); break;
		case eLarger:      p->drawText(getDrawingPlace(), AlignCenter, "A>B");  break;
	}
}


//###############################################################
//###############################################################


SimpleConditionalPropertyGeneralWidget::SimpleConditionalPropertyGeneralWidget(SimpleConditional * comp, QWidget *parent, const char *name)
	:	Boolean1OutPropertyGeneralWidget(comp, parent, name)
{
	QString tip;
	
	m_conditionalLabel = new QLabel(i18n("Conditional: "), getGrid(), "m_conditionalLabel");
	CHECK_PTR(m_conditionalLabel);
	
	m_conditional = new QComboBox(getGrid(), "m_conditional");
	CHECK_PTR(m_conditional);
	m_conditional->insertItem(i18n("A < B"),  (int)eLesser);
	m_conditional->insertItem(i18n("A <= B"), (int)eLesserEqual);
	m_conditional->insertItem(i18n("A == B"), (int)eEqual);
	m_conditional->insertItem(i18n("A >= B"), (int)eLargerEqual);
	m_conditional->insertItem(i18n("A > B"),  (int)eLarger);
	
	tip = i18n("Sets the conditional.");
	addToolTip(tip, m_conditional, m_conditionalLabel);
	addWhatsThis(tip, m_conditional, m_conditionalLabel);

	// Setup value
	m_conditional->setCurrentItem((int)comp->getConditionalType());
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void SimpleConditionalPropertyGeneralWidget::acceptPressed()
{
	Boolean1OutPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getConditionalType() != (eConditionalType)m_conditional->currentItem())
	{
		changeData();
		getComponent()->setConditionalType((eConditionalType)m_conditional->currentItem());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void SimpleConditionalPropertyGeneralWidget::defaultPressed()
{
	Boolean1OutPropertyGeneralWidget::defaultPressed();

	m_conditional->setCurrentItem((int)getComponent()->getConditionalType());
}


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

