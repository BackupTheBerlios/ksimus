/***************************************************************************
                          booleanbutton.cpp  -  description
                             -------------------
    begin                : Fri Feb 2 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "ksimwidget.h"
#include "resource.h"
#include "booleanbutton.h"
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "componentinfo.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"
#include "ksimbooleanbox.h"
#include "ksimdata.h"

// Forward declaration

static Component * createBooleanButton(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanButton(container, ci);
}

const ComponentInfo BooleanButtonInfo("Button",
                                      "Boolean/Input/Button",
                                      QString::null,//"Button",
                                      VA_SHEET_AND_USER,
                                      createBooleanButton	);

//###############################################################
//###############################################################

static Component * createBooleanToggleButton(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanButton(container, ci);
}

const ComponentInfo BooleanToggleButtonInfo("TButton",
                                            "Boolean/Input/Toggle Button",
                                            QString::null,//"Toggle Button",
                                            VA_SHEET_AND_USER,
                                            createBooleanToggleButton	);

const ComponentInfoList BooleanButtonList = { &BooleanButtonInfo, &BooleanToggleButtonInfo, 0 };
										
//###############################################################
//###############################################################


#define FLAGS_RESET_TRUE			0x0001;
#define FLAGS_RESET_INIT_TRUE	0x0002;

//###############################################################
//###############################################################


BooleanButton::BooleanButton(CompContainer * container, const ComponentInfo * ci)
	:	ComponentStyle(container, ci),
		m_state(false),
		m_flags(0),
		m_toggleButton(false)
{
	
//	setColorAdjustmentEnabled(true);
//	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	setFrameEnabled(false);
	
	m_out = new ConnectorBoolOut (this, I18N_NOOP("Output"));
	CHECK_PTR(m_out);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanButtonView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new BooleanButtonView(this, USER_VIEW);
	}

	if(ci == &BooleanToggleButtonInfo)
	{
		m_toggleButton = true;
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);

}

/*BooleanButton::~BooleanButton()
{
}*/

void BooleanButton::updateOutput()
{
	ComponentStyle::updateOutput();
	getOutputConnector()->setOutput(getState());
}

void BooleanButton::setResetState(bool resetState, bool init)
{
	if (resetState)
	{
		m_flags |= FLAGS_RESET_TRUE;
		if (init)
		{
			m_flags |= FLAGS_RESET_INIT_TRUE;
		}
	}
	else
	{
		m_flags &= ~FLAGS_RESET_TRUE;
		if (init)
		{
			m_flags &= ~FLAGS_RESET_INIT_TRUE;
		}
	}
}
	
bool BooleanButton::getResetState() const
{
	return m_flags & FLAGS_RESET_TRUE;
};
	
bool BooleanButton::getResetStateInit() const
{
	return m_flags & FLAGS_RESET_INIT_TRUE;
};
	


void BooleanButton::reset()
{
	ComponentStyle::reset();
	
	setState( getResetState() );
	getOutputConnector()->setOutput(getState());
	
	toggled(getState());
	if (isToggleButton())
	{
		emit buttonChanged(getState());
	}
}

/** save component properties */
void BooleanButton::save(KSimData & file) const
{
	ComponentStyle::save(file);
	
	if (getResetState() != getResetStateInit())
	{
		file.writeEntry("Reset State", getResetState());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool BooleanButton::load(KSimData & file, bool copyLoad)
{
	setResetState( file.readBoolEntry("Reset State", getResetStateInit()) );
	
	return ComponentStyle::load(file, copyLoad);
}


/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * BooleanButton::createGeneralProperty(Component * comp, QWidget *parent)
{
	BooleanButtonPropertyGeneralWidget * wid;
	wid = new BooleanButtonPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}


void BooleanButton::toggled(bool pressed)
{
	if (isToggleButton() && (pressed != getState()))
	{
		setState(pressed);
		emit buttonChanged(getState());
	}
}

bool BooleanButton::isToggleButton() const
{
	return m_toggleButton;
}

void BooleanButton::setToggleButton(bool toggleButton)
{
	if(m_toggleButton != toggleButton)
	{
		m_toggleButton = toggleButton;
		if(m_toggleButton)
		{
			setInfo(&BooleanToggleButtonInfo);
		}
		else
		{
			setInfo(&BooleanButtonInfo);
		}
		emit signalSetToggleButton(m_toggleButton);
	}
}


void BooleanButton::slotPressed()
{
	if (!getState() && !isToggleButton())
	{
		setState(true);
		emit buttonChanged(true);
	}
}
void BooleanButton::slotReleased()
{
	if (getState() && !isToggleButton())
	{
		setState(false);
		emit buttonChanged(false);
	}
}


//##########################################################################################
//##########################################################################################

BooleanButtonView::BooleanButtonView(Component * comp, eViewType viewType)
	: CompViewSize(comp, viewType)
{
	if(viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 11*gridX, 3*gridY));
		setMinSize(6*gridX,3*gridY);
		enableConnectorSpacingRight(true);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
		enableConnectorSpacingLeft(false);
	}
	else
	{
		setPlace(QRect(0, 0, 10*gridX, 3*gridY));
		setMinSize(5*gridX,3*gridY);
	}
	
	resize();
}

/*BooleanButtonView::~BooleanButtonView()
{
}*/


/**  */
QWidget * BooleanButtonView::createCompViewWidget(QWidget * parent)
{
	BooleanButtonWidgetView * wid = new BooleanButtonWidgetView(this, parent,"Button");
	
	QPushButton * button = wid->m_button;
	CHECK_PTR(button);
	
	/* Specific signals */
	// Button pressed signal (Widget->Button)
	connect(button, SIGNAL(toggled(bool)), getComponent(), SLOT(toggled(bool)));
	connect(button, SIGNAL(pressed()), getComponent(), SLOT(slotPressed()));
	connect(button, SIGNAL(released()), getComponent(), SLOT(slotReleased()));
	// Button state changed	(Button->Widget)
	connect(getComponent(), SIGNAL(buttonChanged(bool)), button, SLOT(setOn(bool)));
	// Button type changed
	connect(getComponent(), SIGNAL(signalSetToggleButton(bool)),
			wid, SLOT(slotSetToggleButton(bool)));
	// Rename signal
	connect(getComponent(), SIGNAL(signalSetName(const QString & )),
			wid, SLOT(slotSetText(const QString & )));

	button->setOn(((BooleanButton*)getComponent())->getState());
	wid->slotSetToggleButton(((BooleanButton*)getComponent())->isToggleButton());
	
	return wid;
}

void BooleanButtonView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos;
		connPos.setX( (getPlace().width()/gridX) - 1);
		connPos.setY( getPlace().height()/(gridY*2) );
		((BooleanButton*)getComponent())->getOutputConnector()->setGridPos(connPos);
	}
}



//##########################################################################################
//##########################################################################################

BooleanButtonWidgetView::BooleanButtonWidgetView(CompView * cv, QWidget *parent, const char *name)
	:	CompViewWidget(cv,parent,name)
{
	m_button = new QPushButton(cv->getComponent()->getName(), this);
	CHECK_PTR(m_button);
	m_button->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
	

	QHBoxLayout * lay = new QHBoxLayout(this,0);
	CHECK_PTR(lay);

	lay->addWidget(m_button);
}

/*BooleanButtonWidgetView::~BooleanButtonWidgetView()
{
} */


/** This slot rename the button */
void BooleanButtonWidgetView::slotSetText(const QString & newName)
{
	m_button->setText(newName);
}

/** Enables/Disables toggle functionality */
void BooleanButtonWidgetView::slotSetToggleButton(bool toggle)
{
	m_button->setToggleButton(toggle);
}


//##########################################################################################
//##########################################################################################


BooleanButtonPropertyGeneralWidget::BooleanButtonPropertyGeneralWidget(BooleanButton * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Reset State: "), getGrid(), "ResetStateLabel");
	CHECK_PTR(lab);
	
	m_resetState = new KSimBooleanBox(comp->getResetState(), getGrid(), "ResetState");
	CHECK_PTR(m_resetState);
	
	str = i18n("Changes the reset state of the component to true or false.");
	addToolTip(str, lab, m_resetState);
	addWhatsThis(str, lab, m_resetState);

	lab = new QLabel(i18n("Toggle Function:"), getGrid());
	CHECK_PTR(lab);
	
	m_toggle = new KSimBooleanBox(comp->isToggleButton(), getGrid());
	CHECK_PTR(m_toggle);
	
	m_toggle->setTrueText(i18n("On"));
	m_toggle->setFalseText(i18n("Off"));
	
	str = i18n("Enables or disables the toggle feature.");
	addToolTip(str, lab, m_toggle);
	addWhatsThis(str, lab, m_toggle);
	
	connect(m_toggle, SIGNAL(activated(bool)), this, SLOT(slotActivateToggled(bool)));
}

/*BooleanButtonPropertyGeneralWidget::~BooleanButtonPropertyGeneralWidget()
{
} */

void BooleanButtonPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (((BooleanButton*)getComponent())->getResetState() != m_resetState->getValue())
	{
		changeData();
		((BooleanButton*)getComponent())->setResetState( m_resetState->getValue() );
	}
	
	if (((BooleanButton*)getComponent())->isToggleButton() != m_toggle->getValue())
	{
		changeData();
		((BooleanButton*)getComponent())->setToggleButton(m_toggle->getValue());
	}
}

void BooleanButtonPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_resetState->setValue(false);

	if(getComponent()->getInfo() == &BooleanToggleButtonInfo)
	{
		m_toggle->setValue(true);
	}
	else
	{
		m_toggle->setValue(false);
	}
}

void BooleanButtonPropertyGeneralWidget::slotActivateToggled(bool state)
{
	m_resetState->setEnabled(state);
//	getResetStateBoxLabel()->setEnabled(state);
}

