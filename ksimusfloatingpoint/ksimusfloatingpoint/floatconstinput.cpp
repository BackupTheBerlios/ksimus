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
#include <qpopupmenu.h>
#include <qdialog.h>
#include <qlayout.h>

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

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatConstInput(container, ci);
}

const ComponentInfo FloatConstInputInfo(I18N_NOOP("Floating Point Constant"),
                                        I18N_NOOP("Floating Point/Input/Constant"),
                                        QString::null,
                                        VA_SHEETVIEW,
                                        create	);



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
	getAction().disable(KSimAction::UPDATEOUTPUT);
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


bool FloatConstInput::initPopupMenu(QPopupMenu * popup)
{
	Float1Out::initPopupMenu(popup);
	
	popup->insertSeparator();
	popup->insertItem(i18n("Edit &Value..."), this, SLOT(editValue()));
	
	return true;
}
		
void FloatConstInput::editValue()
{
	QWidget * activeWidget = getSheetView()->getWidgetList()->getActiveWidget();
		
	if (activeWidget)
	{
			
//		KSIMDEBUG("Found activeWidget");
		QDialog * dia = new QDialog((QWidget*)activeWidget->parent(), 0 , true, WType_Popup);
		
		// Set layout
		QGridLayout * layout = new QGridLayout(dia,1,1);
		
		KSimDoubleEdit * edit = new KSimDoubleEdit(dia);
		layout->addWidget(edit,0,0);
		
		dia->move(((QWidget*)activeWidget->parent())->mapToGlobal(activeWidget->pos()));
		dia->resize(activeWidget->size());
		connect(edit, SIGNAL(valueChanged(double)), dia, SLOT(accept()));
		
		edit->setFocus();
		edit->setEdited(true);
		edit->setValue(getResetValue());
		dia->exec();
		
		setResetValue(edit->value());
		
		delete dia;
	}
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * FloatConstInput::createGeneralProperty(Component * comp, QWidget *parent)
{
	FloatConstInputPropertyGeneralWidget * wid;
	wid = new FloatConstInputPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
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

//###############################################################
//###############################################################


}; //namespace KSimLibFloatingPoint
