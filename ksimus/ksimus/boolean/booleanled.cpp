/***************************************************************************
                          booleanled.cpp  -  description
                             -------------------
    begin                : Sat Feb 17 2001
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
#include <qlayout.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qvbox.h>
#include <qvbuttongroup.h>
#include <qradiobutton.h>

// KDE-Includes
#include <klocale.h>
#include <kled.h>

// Project-Includes
#include "ksimdata.h"
#include "booleanled.h"
#include "connectorboolin.h"
#include "componentinfo.h"
#include "resource.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"

// Forward declaration


//############################################################################
//############################################################################

static Component * createBooleanLed(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanLed(container, ci);
}

const ComponentInfo BooleanLedInfo("LED",
                                   "Boolean/Output/LED",
                                   QString::null,//"LED",
                                   VA_SHEET_AND_USER,
                                   createBooleanLed	);


//############################################################################
//############################################################################



BooleanLed::BooleanLed(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_onState(false)
{
	m_inConn = new ConnectorBoolIn (this, i18n("Input"), QPoint(0,1));
	CHECK_PTR(m_inConn);
	
	m_color = new QColor(red);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanLedView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new BooleanLedView(this, USER_VIEW);
	}
	getAction().disable(KSimAction::UPDATEOUTPUT);
}

BooleanLed::~BooleanLed()
{
	delete m_color;
}

void BooleanLed::calculate()
{
	Component::calculate();

	if(m_onState != m_inConn->getInput())
	{
		m_onState = m_inConn->getInput();
		if (getSheetView())
		{
			((BooleanLedView*)getSheetView())->setViewChanged(true);
		}
		if (getUserView())
		{
			((BooleanLedView*)getUserView())->setViewChanged(true);
		}
	}
}

/** Reset all simulation variables */
void BooleanLed::reset()
{
	Component::reset();

	m_onState = false;
	if (getSheetView())
	{
		emit ((BooleanLedView*)getSheetView())->signalState(m_onState);
	}
	if (getUserView())
	{
		emit ((BooleanLedView*)getUserView())->signalState(m_onState);
	}
}

/** Init the property dialog */
void BooleanLed::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog(dialog);
	
	QVBox * page;
	BooleanLedPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Color"));
	wid = new BooleanLedPropertyWidget(this, page);
	dialog->connectSlots(wid);
}

/** save component properties */
void BooleanLed::save(KSimData & file) const
{
	Component::save(file);
	
	file.writeEntry("Color", *m_color);
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool BooleanLed::load(KSimData & file, bool copyLoad)
{
	*m_color = file.readColorEntry("Color",&red);
	
	return Component::load(file, copyLoad);
}


void BooleanLed::setColor(QColor & color)
{
	*m_color = color;
	emit signalColor(color);
}

const QColor & BooleanLed::getColor() const
{
	return *m_color;
}

//############################################################################
//############################################################################


BooleanLedView::BooleanLedView(Component * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 3*gridY));
	setMinSize(5*gridX,3*gridY);
	enableConnectorSpacingTop(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
//	enableConnectorSpacingRight(false);
}


QWidget * BooleanLedView::createCompViewWidget(QWidget * parent)
{
	BooleanLedWidgetView * wid = new BooleanLedWidgetView(this, parent,"Led");
	
	KLed * led = wid->m_led;
	CHECK_PTR(led);
	
	/* Specific signals */
	// LED state changed	(Component->Widget)
	connect(this, SIGNAL(signalState(bool)), wid, SLOT(slotState(bool)));
	connect(getComponent(), SIGNAL(signalColor(const QColor&)), wid, SLOT(slotColor(const QColor&)));

	return wid;
}

void BooleanLedView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos;
		connPos.setX( 0 );
		connPos.setY( getPlace().height()/(gridY*2) );
		((BooleanLed*)getComponent())->m_inConn->setGridPos(connPos);
	}
}


BooleanLedView::~BooleanLedView()
{
}

/** Draw the component view */
void BooleanLedView::draw(QPainter * p)
{
	CompViewSize::draw(p);
	emit signalState(((BooleanLed*)getComponent())->m_onState);
}


//##########################################################################################
//##########################################################################################

#define FRAME 2

BooleanLedWidgetView::BooleanLedWidgetView(CompView * cv, QWidget *parent, const char *name)
	:	CompViewWidget(cv,parent,name)
{
	QGridLayout * lay = new QGridLayout(this,1,1,FRAME);
	
	m_led = new KLed(red, this);
	lay->addWidget(m_led,0,0);
	slotColor(((BooleanLed*)cv->getComponent())->getColor());
}

BooleanLedWidgetView::~BooleanLedWidgetView()
{
}

void BooleanLedWidgetView::paintEvent(QPaintEvent *)
{
  QPainter p(this);
  QBrush fill(colorGroup().background());
  qDrawWinButton (&p, rect(), colorGroup(), false, &fill);
}

void BooleanLedWidgetView::slotState(bool on)
{
	if(on)
	{
		m_led->on();
	}
	else
	{
		m_led->off();
	}
}

void BooleanLedWidgetView::slotColor(const QColor & color)
{
	m_led->setColor(color);
}

//##########################################################################################
//##########################################################################################

BooleanLedPropertyWidget::BooleanLedPropertyWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
	m_newColor = new QColor(((BooleanLed*)comp)->getColor());
	
	QButtonGroup * grp = new QVButtonGroup(i18n("Color"), this);
	
	m_red = new QRadioButton(i18n("Red"),grp);
	m_green = new QRadioButton(i18n("Green"),grp);
	m_orange = new QRadioButton(i18n("Orange"),grp);
	
	connect(m_red,SIGNAL(clicked()),this,SLOT(slotRed()));
	connect(m_green,SIGNAL(clicked()),this,SLOT(slotGreen()));
	connect(m_orange,SIGNAL(clicked()),this,SLOT(slotOrange()));
	
	if (*m_newColor == darkYellow)
	{
		m_orange->setChecked(true);
	}
	else if (*m_newColor == green)
	{
		m_green->setChecked(true);
	}
	else
	{
		m_red->setChecked(true);
	}
		
	QGridLayout * layout;
	
		
//	m_toggle = new QCheckBox(i18n("Toggle Button"),this);
//	m_toggle->setChecked(((BooleanButton*)comp)->m_toggleButton);
	
//	QToolTip::add(m_toggle, i18n("Set toggle feature here"));
	
	// Set main layout
	layout = new QGridLayout(this,2,2);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	
	layout->addWidget(grp,0,0);
	layout->setRowStretch(1,1);
	layout->setColStretch(1,1);
}

BooleanLedPropertyWidget::~BooleanLedPropertyWidget()
{
	delete m_newColor;
}

void BooleanLedPropertyWidget::slotAccept()
{
	ComponentPropertyBaseWidget::slotAccept();

	if (((BooleanLed*)getComponent())->getColor() != *m_newColor)
	{
		changeData();
		((BooleanLed*)getComponent())->setColor(*m_newColor);
	}
}

void BooleanLedPropertyWidget::slotDefault()
{
	ComponentPropertyBaseWidget::slotDefault();

	*m_newColor =		 red;
	m_orange->setChecked(false);
	m_green->setChecked(false);
	m_red->setChecked(true);
}

void BooleanLedPropertyWidget::slotRed()
{
	*m_newColor = red;
}
void BooleanLedPropertyWidget::slotGreen()
{
	*m_newColor = green;
}
void BooleanLedPropertyWidget::slotOrange()
{
	*m_newColor = darkYellow;
}




