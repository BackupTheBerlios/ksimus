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
#include <qhbox.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>
#include <kled.h>
#include <kcolorbtn.h>

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

#define DEFAULT_COLOR       red


BooleanLed::BooleanLed(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(false, true, container, ci),
		m_onState(false),
		m_onColor(DEFAULT_COLOR),
		m_offColor(QColor())
{
	m_inConn = new ConnectorBoolIn (this, i18n("Input"), QPoint(0,1));
	CHECK_PTR(m_inConn);
	
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
}

void BooleanLed::calculate()
{
	ComponentStyle::calculate();

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
	ComponentStyle::reset();

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
	ComponentStyle::initPropertyDialog(dialog);
	
	QVBox * page;
	BooleanLedPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Color"));
	wid = new BooleanLedPropertyWidget(this, page);
	dialog->connectSlots(wid);
}

/** save component properties */
void BooleanLed::save(KSimData & file) const
{
	ComponentStyle::save(file);
	
	if (getOnColor() != DEFAULT_COLOR)
	{
		file.writeEntry("Color", getOnColor());
	}
	if (getOffColor() != QColor())
	{
		file.writeEntry("Off Color", getOffColor());
	}
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool BooleanLed::load(KSimData & file, bool copyLoad)
{
	setOnColor(file.readColorEntry("Color",&DEFAULT_COLOR));
	QColor def = QColor();
	setOffColor(file.readColorEntry("Off Color",&def));
	
	return ComponentStyle::load(file, copyLoad);
}


void BooleanLed::setOnColor(const QColor & color)
{
	if (m_onColor != color)
	{
		m_onColor = color;
		emit signalColorChanged();
	}
}

void BooleanLed::setOffColor(const QColor & color)
{
	if (m_offColor != color)
	{
		m_offColor = color;
		emit signalColorChanged();
	}
}

//############################################################################
//############################################################################


BooleanLedView::BooleanLedView(Component * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 4*gridX, 3*gridY));
		setMinSize(4*gridX,3*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
		enableConnectorSpacingRight(false);
	}
	else
	{
		setPlace(QRect(0, 0, 3*gridX, 3*gridY));
		setMinSize(3*gridX,3*gridY);
	}	
}


QWidget * BooleanLedView::createCompViewWidget(QWidget * parent)
{
	BooleanLedWidgetView * wid = new BooleanLedWidgetView(this, parent,"Led");
	
	KLed * led = wid->m_led;
	CHECK_PTR(led);
	
	/* Specific signals */
	// LED state changed	(Component->Widget)
	connect(this, SIGNAL(signalState(bool)), wid, SLOT(slotState(bool)));
	// Color changed (Component->CompViewWidget)
	connect(getComponent(), SIGNAL(signalColorChanged()), wid, SLOT(slotColorChanged()));
	
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
	:	CompViewVBox(cv,parent,name)
{
	m_led = new KLed(red, this);
	m_boolLed = (BooleanLed*)cv->getComponent();
	
	slotColorChanged();  //Setup
}

BooleanLedWidgetView::~BooleanLedWidgetView()
{
}

void BooleanLedWidgetView::slotState(bool on)
{
	if (m_boolLed->getOffColor().isValid())
	{
		if(on)
		{
			m_led->setColor(m_boolLed->getOnColor());
		}
		else
		{
			m_led->setColor(m_boolLed->getOffColor());
		}
	}
	else
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
}

void BooleanLedWidgetView::slotColorChanged()
{
	if (m_boolLed->getOffColor().isValid())
	{
		m_led->on();
	}
	else
	{
		m_led->setColor(m_boolLed->getOnColor());
	}
	
	slotState(m_boolLed->getState());
}

//##########################################################################################
//##########################################################################################


#define RED		  red
#define GREEN		green
#define ORANGE	QColor(255,128,0)


BooleanLedColorPropertyWidget::BooleanLedColorPropertyWidget(const QString & text, QWidget *parent=0, const char *name=0)
	:	QVButtonGroup(text, parent, name)
{
	
	m_defaultColor = new QCheckBox(i18n("Automatic colored"), this);
	CHECK_PTR(m_defaultColor);
	
	m_red = new QRadioButton(i18n("Red"), this);
	CHECK_PTR(m_red);
	m_green = new QRadioButton(i18n("Green"), this);
	CHECK_PTR(m_green);
	m_orange = new QRadioButton(i18n("Orange"), this);
	CHECK_PTR(m_orange);
	
	QHBox * userBox = new QHBox(this);
	CHECK_PTR(userBox);
	userBox->setSpacing(KDialog::spacingHint());
	
	QLabel * label = new QLabel(i18n("User defined:"),userBox);
	CHECK_PTR(label);
	m_userColor = new KColorButton(userBox);;
	CHECK_PTR(m_userColor);


	connect(m_red,SIGNAL(clicked()),this,SLOT(slotRed()));
	connect(m_green,SIGNAL(clicked()),this,SLOT(slotGreen()));
	connect(m_orange,SIGNAL(clicked()),this,SLOT(slotOrange()));
	connect(m_userColor,SIGNAL(changed(const QColor &)),this,SLOT(setColor(const QColor &)));
	connect(m_defaultColor,SIGNAL(toggled(bool)),this,SLOT(slotDefault(bool)));
	
}

BooleanLedColorPropertyWidget::~BooleanLedColorPropertyWidget()
{
}

void BooleanLedColorPropertyWidget::setEnableDefault(bool ena)
{
	if (ena)
	{
		m_defaultColor->show();
	}
	else
	{
		m_defaultColor->hide();
	}
}
	
void BooleanLedColorPropertyWidget::setColor(const QColor & newColor)
{
	if (newColor.isValid() == m_defaultColor->isChecked())
	{
		m_defaultColor->setChecked(!newColor.isValid());
		slotDefault(!newColor.isValid());
	}
	
	if (m_color != newColor)
	{
		m_color = newColor;
		
		emit changed(color());
	
		m_userColor->setColor(newColor);
	
		if (m_color == ORANGE)
		{
			m_orange->setChecked(true);
		}
		else if (m_color == GREEN)
		{
			m_green->setChecked(true);
		}
		else if (m_color == RED)
		{
			m_red->setChecked(true);
		}
		else
		{	
			m_orange->setChecked(false);
			m_green->setChecked(false);
			m_red->setChecked(false);
		}
	}
}

QColor BooleanLedColorPropertyWidget::color() const
{
	if(m_defaultColor->isChecked())
	{
		return QColor();
	}
	else
	{
		return m_color;
	}
};


void BooleanLedColorPropertyWidget::slotRed()
{
	setColor(RED);
}

void BooleanLedColorPropertyWidget::slotGreen()
{
	setColor(GREEN);
}

void BooleanLedColorPropertyWidget::slotOrange()
{
	setColor(ORANGE);
}

//void BooleanLedColorPropertyWidget::slotUserColor(const QColor &);

void BooleanLedColorPropertyWidget::slotDefault(bool def)
{
	m_red->setEnabled(!def);	
	m_green->setEnabled(!def);	
	m_orange->setEnabled(!def);	
	m_userColor->setEnabled(!def);	
	emit changed(color());
}

bool BooleanLedColorPropertyWidget::isDefault() const
{
	return m_defaultColor->isChecked();
}


//##########################################################################################
//##########################################################################################




BooleanLedPropertyWidget::BooleanLedPropertyWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name)
{
/*	m_newColor = new QColor(((BooleanLed*)comp)->getColor());
	CHECK_PTR(m_newColor);
	
	QButtonGroup * grp = new QVButtonGroup(i18n("Color"), this);
	CHECK_PTR(grp);
	
	m_red = new QRadioButton(i18n("Red"),grp);
	CHECK_PTR(m_red);
	m_green = new QRadioButton(i18n("Green"),grp);
	CHECK_PTR(m_green);
	m_orange = new QRadioButton(i18n("Orange"),grp);
	CHECK_PTR(m_orange);
	
	QHBox * userBox = new QHBox(grp);
	CHECK_PTR(userBox);
	userBox->setSpacing(KDialog::spacingHint());
	
	QLabel * label = new QLabel(i18n("User defined:"),userBox);
	CHECK_PTR(label);
	m_userColor = new KColorButton(userBox);;
	CHECK_PTR(m_userColor);

	
		
	connect(m_red,SIGNAL(clicked()),this,SLOT(slotRed()));
	connect(m_green,SIGNAL(clicked()),this,SLOT(slotGreen()));
	connect(m_orange,SIGNAL(clicked()),this,SLOT(slotOrange()));
	connect(m_userColor,SIGNAL(changed(const QColor &)),this,SLOT(slotUserColor(const QColor &)));
	
	m_userColor->setColor(*m_newColor);
	slotUserColor(*m_newColor);*/
	
	
	QHBox * exampleBox = new QHBox(this);
	CHECK_PTR(exampleBox);
	exampleBox->setSpacing(KDialog::spacingHint());
	
	m_onColor = new BooleanLedColorPropertyWidget(i18n("Color On"), this);
	m_onColor->setEnableDefault(false);
	
	m_offColor = new BooleanLedColorPropertyWidget(i18n("Color Off"), this);
	m_offColor->setEnableDefault(true);

	QLabel * label = new QLabel(i18n("Result:"),exampleBox);
	CHECK_PTR(label);
	label = new QLabel(i18n("On"),exampleBox);
	CHECK_PTR(label);
	m_exampleOn = new KLed(exampleBox);;
	m_exampleOn->setFixedSize(20,20);
	CHECK_PTR(m_exampleOn);
	
	label = new QLabel(i18n("Off"),exampleBox);
	CHECK_PTR(label);
	m_exampleOff = new KLed(exampleBox);
	m_exampleOff->setFixedSize(20,20);
	CHECK_PTR(m_exampleOff);
	
	connect(m_onColor, SIGNAL(changed(const QColor &)), this, SLOT(slotOnColor(const QColor &)));
	connect(m_offColor, SIGNAL(changed(const QColor &)), this, SLOT(slotOffColor(const QColor &)));
 		
	
	m_onColor->setColor(((BooleanLed*)comp)->getOnColor());
	m_offColor->setColor(((BooleanLed*)comp)->getOffColor());
	m_exampleOn->on();
	slotOnColor(m_onColor->color());
	slotOffColor(m_offColor->color());
	
	
	QGridLayout * layout;
	
		
	// Set main layout
	layout = new QGridLayout(this,4,2);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	
/*	layout->addWidget(grp,0,0);
	layout->addWidget(exampleBox,1,0);*/
	layout->addWidget(m_onColor,0,0);
	layout->addWidget(m_offColor,1,0);
	layout->addWidget(exampleBox,2,0);
	layout->setRowStretch(3,1);
	layout->setColStretch(1,1);
}

BooleanLedPropertyWidget::~BooleanLedPropertyWidget()
{
}

void BooleanLedPropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	if (((BooleanLed*)getComponent())->getOnColor() != m_onColor->color())
	{
		changeData();
		((BooleanLed*)getComponent())->setOnColor(m_onColor->color());
	}
	
	if (((BooleanLed*)getComponent())->getOffColor() != m_offColor->color())
	{
		changeData();
		((BooleanLed*)getComponent())->setOffColor(m_offColor->color());
	}
}

void BooleanLedPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	m_onColor->setColor(DEFAULT_COLOR);
	m_offColor->setColor(QColor());
	
/*	*m_newColor =		 RED;
	m_orange->setChecked(false);
	m_green->setChecked(false);
	m_red->setChecked(true);*/
}

/*void BooleanLedPropertyWidget::slotRed()
{
	*m_newColor = RED;
	m_userColor->setColor(RED);
	m_exampleOff->setColor(RED);
	m_exampleOn->setColor(RED);
}
void BooleanLedPropertyWidget::slotGreen()
{
	*m_newColor = GREEN;
	m_userColor->setColor(GREEN);
	m_exampleOff->setColor(GREEN);
	m_exampleOn->setColor(GREEN);
}
void BooleanLedPropertyWidget::slotOrange()
{
	*m_newColor = ORANGE;
	m_userColor->setColor(ORANGE);
	m_exampleOff->setColor(ORANGE);
	m_exampleOn->setColor(ORANGE);
}

void BooleanLedPropertyWidget::slotUserColor(const QColor & color)
{
	*m_newColor = color;
	
	if (*m_newColor == ORANGE)
	{
		m_orange->setChecked(true);
	}
	else if (*m_newColor == GREEN)
	{
		m_green->setChecked(true);
	}
	else if (*m_newColor == RED)
	{
		m_red->setChecked(true);
	}
	else
	{	
		m_orange->setChecked(false);
		m_green->setChecked(false);
		m_red->setChecked(false);
	}
	
	m_exampleOff->setColor(*m_newColor);
	m_exampleOn->setColor(*m_newColor);
} */

void BooleanLedPropertyWidget::slotOnColor(const QColor & color)
{
	m_exampleOn->setColor(color);
	if (!m_offColor->color().isValid())
	{
		m_exampleOff->setColor(color);
		m_exampleOff->off();
	}		
}

void BooleanLedPropertyWidget::slotOffColor(const QColor & color)
{
	if(color.isValid())
	{
		m_exampleOff->setColor(color);
		m_exampleOff->on();
	}
	else
	{
		m_exampleOff->setColor(m_onColor->color());
		m_exampleOff->off();
	}		
}




