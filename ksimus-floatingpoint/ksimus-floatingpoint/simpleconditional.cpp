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

const ComponentInfo * getConditionalLesserInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Conditional Lesser"),
	                                QString::fromLatin1("Floating Point/Conditional/Lesser"),
	                                i18n("Component", "Floating Point/Conditional/Lesser"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-lesser"));
	return &Info;
}

const ComponentInfo * getConditionalLesserEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Conditional Lesser Equal"),
	                                QString::fromLatin1("Floating Point/Conditional/Lesser Equal"),
	                                i18n("Component", "Floating Point/Conditional/Lesser Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-lesser-equal"));
	return &Info;
}

const ComponentInfo * getConditionalEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Conditional Equal"),
	                                QString::fromLatin1("Floating Point/Conditional/Equal"),
	                                i18n("Component", "Floating Point/Conditional/Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-equal"));
	return &Info;
}

const ComponentInfo * getConditionalLargerEqualInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Conditional Larger Equal"),
	                                QString::fromLatin1("Floating Point/Conditional/Larger Equal"),
	                                i18n("Component", "Floating Point/Conditional/Larger Equal"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-larger-equal"));
	return &Info;
}

const ComponentInfo * getConditionalLargerInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Conditional Larger"),
	                                QString::fromLatin1("Floating Point/Conditional/Larger"),
	                                i18n("Component", "Floating Point/Conditional/Larger"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-conditional-larger"));
	return &Info;
}

//###############################################################
//###############################################################


SimpleConditional::SimpleConditional(CompContainer * container, const ComponentInfo * ci)
	: Boolean1Out(container, ci)
{
	
	if      (ci == getConditionalLesserInfo())      m_conditionalType = eLesser;
	else if (ci == getConditionalLesserEqualInfo()) m_conditionalType = eLesserEqual;
	else if (ci == getConditionalEqualInfo())       m_conditionalType = eEqual;
	else if (ci == getConditionalLargerEqualInfo()) m_conditionalType = eLargerEqual;
	else /*if (ci == getConditionalLargerInfo())*/  m_conditionalType = eLarger;
	
	
	m_inputA = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input A"),
	                             i18n("FloatingPoint-Connector", "Input A"));
	CHECK_PTR(m_inputA);
	
	m_inputB = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input B"),
	                             i18n("FloatingPoint-Connector", "Input B"));
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

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool SimpleConditional::load(KSimData & file, bool copyLoad)
{
	bool res = Boolean1Out::load(file, copyLoad);

	QString id = file.readEntry(Component::sType);
	setConditionalType(id);
	
	return res;
}

bool SimpleConditional::isProperReloadType(const QString & type) const
{
	return (type == getConditionalLesserInfo()->getLibName())
	    || (type == getConditionalLesserEqualInfo()->getLibName())
	    || (type == getConditionalEqualInfo()->getLibName())
	    || (type == getConditionalLargerEqualInfo()->getLibName())
	    || (type == getConditionalLargerInfo()->getLibName());
}

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
		case eLesser:      setInfo(getConditionalLesserInfo());      break;
		case eLesserEqual: setInfo(getConditionalLesserEqualInfo()); break;
		case eEqual:       setInfo(getConditionalEqualInfo());       break;
		case eLargerEqual: setInfo(getConditionalLargerEqualInfo()); break;
		case eLarger:      setInfo(getConditionalLargerInfo());      break;
	}
	m_conditionalType = newCond;
	if (hasDefaultName())
	{
		// Set new default name if required
		setName(getDefaultName());
	}
}

void SimpleConditional::setConditionalType(const QString & type)
{
	if (type == getConditionalLesserInfo()->getLibName())
		setConditionalType(eLarger);
	else if (type == getConditionalLesserEqualInfo()->getLibName())
		setConditionalType(eLesserEqual);
	else if (type == getConditionalEqualInfo()->getLibName())
		setConditionalType(eEqual);
	else if (type == getConditionalLargerEqualInfo()->getLibName())
		setConditionalType(eLargerEqual);
	else if (type == getConditionalLargerInfo()->getLibName())
		setConditionalType(eLarger);
	else
	{
		KSIMDEBUG_VAR("Unknown type", type);
	}
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * SimpleConditional::createGeneralProperty(QWidget *parent)
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
	
	m_conditionalLabel = new QLabel(i18n("FloatingPoint", "Conditional: "), this, "m_conditionalLabel");
	CHECK_PTR(m_conditionalLabel);
	
	m_conditional = new QComboBox(this, "m_conditional");
	CHECK_PTR(m_conditional);
	m_conditional->insertItem(i18n("FloatingPoint", "A < B"),  (int)eLesser);
	m_conditional->insertItem(i18n("FloatingPoint", "A <= B"), (int)eLesserEqual);
	m_conditional->insertItem(i18n("FloatingPoint", "A == B"), (int)eEqual);
	m_conditional->insertItem(i18n("FloatingPoint", "A >= B"), (int)eLargerEqual);
	m_conditional->insertItem(i18n("FloatingPoint", "A > B"),  (int)eLarger);
	
	tip = i18n("FloatingPoint", "Sets the conditional.");
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

