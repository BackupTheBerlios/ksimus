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
#include <qcheckbox.h>
#include <qvbox.h>

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


BooleanButton::BooleanButton(CompContainer * container, const ComponentInfo * ci)
	:	Component(container, ci),
		m_buttonState(false),
		m_toggleButton(false)
{
						
	out = new ConnectorBoolOut (this, i18n("Output"), QPoint(6,1));
	CHECK_PTR(out);
	
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

BooleanButton::~BooleanButton()
{
}

/** Shift the result of calculation to output */
void BooleanButton::updateOutput()
{
	Component::updateOutput();
	out->setOutput(m_buttonState);
}

/** Reset all simulation variables */
void BooleanButton::reset()
{
	Component::reset();
	toggled(false);
}

void BooleanButton::toggled(bool pressed)
{
	if (m_toggleButton && (pressed != m_buttonState))
	{
		m_buttonState = pressed;
		out->setOutput(m_buttonState);
		emit buttonChanged(m_buttonState);
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
	if (!m_buttonState && !m_toggleButton)
	{
		m_buttonState = true;
		out->setOutput(true);
		emit buttonChanged(true);
	}
}
void BooleanButton::slotReleased()
{
	if (m_buttonState && !m_toggleButton)
	{
		m_buttonState = false;
		out->setOutput(false);
		emit buttonChanged(false);
	}
}

/** Init the property dialog */
void BooleanButton::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog(dialog);
	
	QVBox * page;
	BooleanButtonPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Settings"));
	wid = new BooleanButtonPropertyWidget(this, page);
	dialog->connectSlots(wid);
}

//##########################################################################################
//##########################################################################################

BooleanButtonView::BooleanButtonView(Component * comp, eViewType viewType)
	: CompViewSize(comp, viewType)
{
	setPlace(QRect(0, 0, 12*gridX, 3*gridY));
	setMinSize(7*gridX,3*gridY);
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
}

/** Draws the Button to a printer */
/*void BooleanButtonView::print(QPainter * paint)
{
	QPushButton * button;
	QPainter p;
	
	CompView::print(paint);
	
	QPushButton = (QButton*)getWidgetList()->getFirstWidget();
	CHECK_PTR(button);
	QPixmap pm(button->size());

	p.begin(&pm, button);
	QPushButton->drawButton(&p);
	p.end();
	
	paint->drawPixmap(button->pos(), pm);
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

	button->setOn(((BooleanButton*)getComponent())->m_buttonState);
	wid->slotSetToggleButton(((BooleanButton*)getComponent())->m_toggleButton);
	
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
		((BooleanButton*)getComponent())->out->setGridPos(connPos);
	}
}


BooleanButtonView::~BooleanButtonView()
{
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

BooleanButtonWidgetView::~BooleanButtonWidgetView()
{
}


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


BooleanButtonPropertyWidget::BooleanButtonPropertyWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
	QGridLayout * layout;
	
	m_toggle = new QCheckBox(i18n("Toggle Button"),this);
	m_toggle->setChecked(((BooleanButton*)comp)->m_toggleButton);
	
	QToolTip::add(m_toggle, i18n("Set toggle feature here"));
	
	// Set main layout
	layout = new QGridLayout(this,2,1);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	layout->addWidget(m_toggle,0,0);
	layout->setRowStretch(1,1);
}

BooleanButtonPropertyWidget::~BooleanButtonPropertyWidget()
{
}

void BooleanButtonPropertyWidget::slotAccept()
{
	ComponentPropertyBaseWidget::slotAccept();

	if (((BooleanButton*)getComponent())->isToggleButton() != m_toggle->isChecked())
	{
		changeData();
		((BooleanButton*)getComponent())->setToggleButton(m_toggle->isChecked());
	}
}

void BooleanButtonPropertyWidget::slotDefault()
{
	ComponentPropertyBaseWidget::slotDefault();

	if(getComponent()->getInfo() == &BooleanToggleButtonInfo)
	{
		m_toggle->setChecked(true);
	}
	else
	{
		m_toggle->setChecked(false);
	}
}
