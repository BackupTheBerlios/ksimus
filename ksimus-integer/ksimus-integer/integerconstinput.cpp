/***************************************************************************
                          integerconstinput.cpp  -  description
                             -------------------
    begin                : Sun Aug 11 2003
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimwidget.h"
#include "ksimus/resource.h"
#include "ksimus/ksimbaseintedit.h"
#include "ksimus/connectorintegerout.h"

#include "integerconstinput.h"


// Forward declaration

namespace KSimLibInteger
{

//#######################################################################
//#######################################################################

Component * IntegerConstInput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new IntegerConstInput(container, ci);
}

const ComponentInfo * IntegerConstInput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Integer Constant"),
	                                QString::fromLatin1("Integer/Input/Constant"),
	                                i18n("Component", "Integer/Input/Constant"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-input-const")*/);
	return &Info;
}



//############################################################################
//############################################################################


IntegerConstInput::IntegerConstInput(CompContainer * container, const ComponentInfo * ci)
	:	Integer1Out(container, ci)
{
/*	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);*/
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new IntegerConstInputView(this, SHEET_VIEW);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);
}

/*IntegerConstInput::~IntegerConstInput()
{
} */

/** Reset all simulation variables */
void IntegerConstInput::reset()
{
	Integer1Out::reset();

	updateValue(getResetValue());
}


/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool IntegerConstInput::load(KSimData & file, bool copyLoad)
{
	bool res = Integer1Out::load(file, copyLoad);
	
	updateValue(getResetValue());
	
	return res;
}

bool IntegerConstInput::initPopupMenu(QPopupMenu * popup)
{
	Integer1Out::initPopupMenu(popup);
	
//	popup->insertSeparator();
//	popup->insertItem(i18n("Edit &Value..."), this, SLOT(editValue()));
	
	return true;
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * IntegerConstInput::createGeneralProperty(QWidget *parent)
{
	IntegerConstInputPropertyGeneralWidget * wid;
	wid = new IntegerConstInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

void IntegerConstInput::updateValue(const KSimBaseInt & val)
{
	// Update value
	emit signalSetText(val.text());
}


//############################################################################
//############################################################################



IntegerConstInputView::IntegerConstInputView(IntegerConstInput * comp, eViewType viewType)
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
		KSIMDEBUG("IntegerConstInputView has only a SHEET_VIEW!");
	}	
}

/*IntegerConstInputView::~IntegerConstInputView()
{
} */


QWidget * IntegerConstInputView::createCompViewWidget(QWidget * parent)
{
	IntegerConstInputWidgetView * wid = new IntegerConstInputWidgetView(this, parent,"Label");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getIntegerConstInput(), SIGNAL(signalSetText(const QString &)), wid->m_label, SLOT(setText(const QString &)));

	return wid;
}

void IntegerConstInputView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos(getPlace().width()/gridX-1, getPlace().height()/gridY/2);
		getIntegerConstInput()->getOutputConnector()->setGridPos(connPos);
	}
}




//##########################################################################################
//##########################################################################################

IntegerConstInputWidgetView::IntegerConstInputWidgetView(IntegerConstInputView * cv, QWidget *parent, const char *name)
	:	CompViewHBox(cv,parent,name)
{
	setFrameEnabled(true);
	
	m_label = new QLabel(this);
	CHECK_PTR(m_label);
	m_label->setAlignment(AlignRight | AlignVCenter);
	m_label->setText(getIntegerConstInput()->getResetValue().text());
	
}

/*IntegerConstInputWidgetView::~IntegerConstInputWidgetView()
{
} */

//##########################################################################################
//##########################################################################################

IntegerConstInputPropertyGeneralWidget::IntegerConstInputPropertyGeneralWidget(IntegerConstInput * comp, QWidget *parent, const char *name)
	:	Integer1OutPropertyGeneralWidget(comp, parent, name)
{
	m_resetValueLabel->setText(i18n("Value: "));
}

void IntegerConstInputPropertyGeneralWidget::acceptPressed()
{
	Integer1OutPropertyGeneralWidget::acceptPressed();
	
	getIntegerConstInput()->updateValue(getIntegerConstInput()->getResetValue());
}
//###############################################################
//###############################################################


}; //namespace KSimLibInteger
