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
#include <qgrid.h>

// KDE-Includes
#include <klocale.h>
#include <kdialog.h>

// Project-Includes
#include "ksimwidget.h"
#include "resource.h"
#include "booleanbutton.h"
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"
#include "ksimbooleanbox.h"
#include "ksimdata.h"

// Forward declaration


Component * BooleanButton::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanButton(container, ci);
}

const ComponentInfo * BooleanButton::getStaticButtonInfo()
{
	static const ComponentInfo Info(i18n("Component", "Button"),
	                                QString::fromLatin1("Boolean/Input/Button"),
	                                i18n("Component", "Boolean/Input/Button"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-button"));
	return &Info;
}

const ComponentInfo * BooleanButton::getStaticToggleButtonInfo()
{
	static const ComponentInfo Info(i18n("Component", "TButton"),
	                                QString::fromLatin1("Boolean/Input/Toggle Button"),
	                                i18n("Component", "Boolean/Input/Toggle Button"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-button"));
	return &Info;
}


BooleanButton::BooleanButton(CompContainer * container, const ComponentInfo * ci)
	:	ComponentStyle(container, ci),
		m_flags(0),
		m_toggleButton(false)
{
//	setColorAdjustmentEnabled(true);
//	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	setFrameEnabled(false);
	
	m_out = new ConnectorBoolOut (this,
	                              QString::fromLatin1("Output"),
	                              i18n("Connector", "Output"));
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

	if(ci == getStaticToggleButtonInfo())
	{
		m_toggleButton = true;
	}
	
	getAction().disable(KSimAction::UPDATEVIEW);
	getAction().disable(KSimAction::CALCULATE);

}

/*BooleanButton::~BooleanButton()
{
}*/

void BooleanButton::calculate()
{
	ComponentStyle::calculate();
//	getOutputConnector()->setOutput(getState());
}

void BooleanButton::setState(bool newState)
{
	getOutputConnector()->setOutput(newState);
}
	
bool BooleanButton::getState() const
{
	return getOutputConnector()->getOutput();
}

#define FLAGS_RESET_TRUE       0x0001
#define FLAGS_RESET_INIT_TRUE  0x0002

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
}
	
bool BooleanButton::getResetStateInit() const
{
	return m_flags & FLAGS_RESET_INIT_TRUE;
}


#undef FLAGS_RESET_TRUE
#undef FLAGS_RESET_INIT_TRUE



void BooleanButton::reset()
{
	ComponentStyle::reset();
	
	if (isToggleButton())
	{
		setState(getResetState());
//		getOutputConnector()->setOutput(getState());
	
		toggled(getState());
		emit buttonChanged(getState());
	}
	else
	{
		setState(false);
		getOutputConnector()->setOutput(false);
		// Force released state!!!
		emit releaseButton();
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
	QString id = file.readEntry(Component::sType);
	
	bool res = ComponentStyle::load(file, copyLoad);

	if (id == getStaticButtonInfo()->getLibName())
	{
		setToggleButton(false);
	}
	else
	{
		setToggleButton(true);
	}

	return res;
}

bool BooleanButton::isProperReloadType(const QString & type) const
{
	return (type == getStaticButtonInfo()->getLibName())
	    || (type == getStaticToggleButtonInfo()->getLibName());
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * BooleanButton::createGeneralProperty(QWidget *parent)
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
//		executeNext();
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
			setInfo(getStaticToggleButtonInfo());
		}
		else
		{
			setInfo(getStaticButtonInfo());
		}
		emit signalSetToggleButton(m_toggleButton);
		if (hasDefaultName())
		{
			// Set new default name if required
			setName(getDefaultName());
		}
	}
}


void BooleanButton::slotPressed()
{
	if (!getState() && !isToggleButton())
	{
		setState(true);
		emit buttonChanged(true);
		executeNext();
	}
}
void BooleanButton::slotReleased()
{
	if (getState() && !isToggleButton())
	{
		setState(false);
		emit buttonChanged(false);
		executeNext();
	}
}

const QString & BooleanButton::getDefaultButtonText() const
{
	static const QString i18nText(i18n(
	       "The default text of the button. Use an abbreviation if the text is too long for the button",
	       "Button"));

	static const QString i18nTextToggle(i18n(
	       "The default text of the toggle button. Use an abbreviation if the text is too long for the button",
	       "TButton"));
	       
	if (isToggleButton())
	{
		return i18nTextToggle;
	}
	else
	{
		return i18nText;
	}
}

//##########################################################################################
//##########################################################################################

BooleanButtonView::BooleanButtonView(BooleanButton * comp, eViewType viewType)
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
	
	QPushButton * button = wid->m_buttonWidget;
	CHECK_PTR(button);
	
	/* Specific signals */
	// Button pressed signal (Widget->Button)
	connect(button, SIGNAL(toggled(bool)), getComponent(), SLOT(toggled(bool)));
	connect(button, SIGNAL(pressed()), getComponent(), SLOT(slotPressed()));
	connect(button, SIGNAL(released()), getComponent(), SLOT(slotReleased()));
	// Button state changed	(Button->Widget)
	connect(getComponent(), SIGNAL(buttonChanged(bool)), button, SLOT(setOn(bool)));
	connect(getComponent(), SIGNAL(releaseButton()), wid, SLOT(slotReleaseButton()));
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

BooleanButtonWidgetView::BooleanButtonWidgetView(BooleanButtonView * cv, QWidget *parent, const char *name)
	:	CompViewWidget(cv,parent,name)
{
	m_buttonWidget = new QPushButton(this);
	CHECK_PTR(m_buttonWidget);
	m_buttonWidget->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
	slotSetText(getButton()->getName());  // Set real name
	

	QHBoxLayout * lay = new QHBoxLayout(this,0);
	CHECK_PTR(lay);

	lay->addWidget(m_buttonWidget);
}

/*BooleanButtonWidgetView::~BooleanButtonWidgetView()
{
} */

BooleanButton * BooleanButtonWidgetView::getButton()
{
	return (BooleanButton *) getComponent();
}



/** This slot rename the button */
void BooleanButtonWidgetView::slotSetText(const QString & newName)
{
	if (getButton()->hasDefaultName())
	{
		m_buttonWidget->setText(getButton()->getDefaultButtonText());
	}
	else
	{
		m_buttonWidget->setText(newName);
	}
}

/** Enables/Disables toggle functionality */
void BooleanButtonWidgetView::slotSetToggleButton(bool toggle)
{
	m_buttonWidget->setToggleButton(toggle);
}

void BooleanButtonWidgetView::slotReleaseButton()
{
	// Force to release a sticking key
	m_buttonWidget->setToggleButton(true);
	m_buttonWidget->setOn(false);
	m_buttonWidget->setOn(true);
	m_buttonWidget->setOn(false);
	m_buttonWidget->setToggleButton(false);
	m_buttonWidget->setDown(true);
	m_buttonWidget->setDown(false);
}

//##########################################################################################
//##########################################################################################


BooleanButtonPropertyGeneralWidget::BooleanButtonPropertyGeneralWidget(BooleanButton * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QLabel * lab;
	QString str;	

	lab = new QLabel(i18n("Reset State: "), this, "ResetStateLabel");
	CHECK_PTR(lab);
	
	m_resetState = new KSimBooleanBox(comp->getResetState(), this, "ResetState");
	CHECK_PTR(m_resetState);
	m_resetState->setEnabled(comp->isToggleButton());
	
	str = i18n("Changes the reset state of the component to true or false.");
	addToolTip(str, lab, m_resetState);
	addWhatsThis(str, lab, m_resetState);
	

	lab = new QLabel(i18n("Toggle Function:"), this);
	CHECK_PTR(lab);
	
	m_toggle = new KSimBooleanBox(comp->isToggleButton(), this);
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

	if(getComponent()->getInfo() == BooleanButton::getStaticToggleButtonInfo())
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

