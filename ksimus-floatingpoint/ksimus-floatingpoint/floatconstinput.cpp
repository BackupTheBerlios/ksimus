/***************************************************************************
                          floatconstinput.cpp  -  description
                             -------------------
    begin                : Tue Feb 5 2002
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/componentinfo.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimwidget.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdoubleedit.h"
#include "ksimus/connectorfloatout.h"

#include "floatconstinput.h"


// Forward declaration

namespace KSimLibFloatingPoint
{

//#######################################################################
//#######################################################################

Component * FloatConstInput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatConstInput(container, ci);
}

const ComponentInfo * FloatConstInput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Constant"),
	                                QString::fromLatin1("Floating Point/Input/Constant"),
	                                i18n("Component", "Floating Point/Input/Constant"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-input-const"));
	return &Info;
}



//############################################################################
//############################################################################


FloatConstInput::FloatConstInput(CompContainer * container, const ComponentInfo * ci)
	: Float1Out(container, ci)
{
/*	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);*/
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatConstInputView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}

/*FloatLineOutput::~FloatLineOutput()
{
} */

/** Reset all simulation variables */
void FloatConstInput::reset()
{
	Float1Out::reset();

	emit signalSetNumber(getResetValue());
}


/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool FloatConstInput::load(KSimData & file, bool copyLoad)
{
	bool res = Float1Out::load(file, copyLoad);
	
	// Update value
	emit signalSetNumber(getResetValue());
	
	return res;
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * FloatConstInput::createGeneralProperty(QWidget *parent)
{
	FloatConstInputPropertyGeneralWidget * wid;
	wid = new FloatConstInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void FloatConstInput::updateValue(double val)
{
	emit signalSetNumber(val);
}


//############################################################################
//############################################################################



FloatConstInputView::FloatConstInputView(FloatConstInput * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 7*gridX, 3*gridY));
		setMinSize(5*gridX, 3*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
		enableConnectorSpacingLeft(false);
//		enableConnectorSpacingRight(false);
	}
	else
	{
		KSIMDEBUG("FloatConstInputView has only a SHEET_VIEW!");
	}	
}

/*FloatConstInputView::~FloatConstInputView()
{
} */


QWidget * FloatConstInputView::createCompViewWidget(QWidget * parent)
{
	FloatConstInputWidgetView * wid = new FloatConstInputWidgetView(this, parent,"Label");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getComponent(), SIGNAL(signalSetNumber(double)), wid->m_label, SLOT(setNum(double)));

	return wid;
}

void FloatConstInputView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos(getPlace().width()/gridX-1, getPlace().height()/gridY/2);
		getComponent()->getOutputConnector()->setGridPos(connPos);
	}
}




//##########################################################################################
//##########################################################################################

FloatConstInputWidgetView::FloatConstInputWidgetView(FloatConstInputView * cv, QWidget *parent, const char *name)
	:	CompViewHBox(cv,parent,name)
{
	setFrameEnabled(true);
	
	m_label = new QLabel(this);
	CHECK_PTR(m_label);
	m_label->setAlignment(AlignRight | AlignVCenter);
	m_label->setNum(getComponent()->getResetValue());
	
}

/*FloatConstInputWidgetView::~FloatConstInputWidgetView()
{
} */

//##########################################################################################
//##########################################################################################

FloatConstInputPropertyGeneralWidget::FloatConstInputPropertyGeneralWidget(FloatConstInput * comp, QWidget *parent, const char *name)
	:	Float1OutPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel->setText(i18n("Value: "));
}

void FloatConstInputPropertyGeneralWidget::acceptPressed()
{
	Float1OutPropertyGeneralWidget::acceptPressed();
	
	getComponent()->updateValue(getComponent()->getResetValue());
}
//###############################################################
//###############################################################


}; //namespace KSimLibFloatingPoint
