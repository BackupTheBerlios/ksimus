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
#include <kcolorbutton.h>

// Project-Includes
#include "ksimdata.h"
#include "booleanled.h"
#include "connectorboolin.h"
#include "resource.h"
#include "ksimdebug.h"
#include "componentpropertydialog.h"

// Forward declaration


//############################################################################
//############################################################################

Component * BooleanLed::create(CompContainer * container, const ComponentInfo * ci)
{
	return new BooleanLed(container, ci);
}

const ComponentInfo * BooleanLed::getStaticInfo()
{
	static const ComponentInfo Info(i18n("LED"),
	                                QString::fromLatin1("Boolean/Output/LED"),
	                                i18n("Component", "Boolean/Output/LED"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-led"));
	return &Info;
}


const QColor BooleanLed::defaultColor(Qt::red);

//############################################################################
//############################################################################

//#define DEFAULT_COLOR       red


BooleanLed::BooleanLed(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_onState(false),
		m_onColor(BooleanLed::defaultColor),
		m_offColor(QColor())
{
//	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
//	setFontAdjustmentEnabled(true);
	
	m_inConn = new ConnectorBoolIn(this,
	                               QString::fromLatin1("Input"),
	                               i18n("Connector", "Input"),
	                               QPoint(0,1));
	Q_CHECK_PTR(m_inConn);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new BooleanLedView(this, SHEET_VIEW, "BooleanLedSV");
	}
	// Initializes the user view
	if (getUserMap())
	{
		new BooleanLedView(this, USER_VIEW, "BooleanLedUV");
	}
}

BooleanLed::~BooleanLed()
{
}

void BooleanLed::calculate()
{
	ComponentStyle::calculate();

	bool in = m_inConn->getInput();

//	KSIMDEBUG(QString::fromLatin1("BooleanLed::calculate() %1  %2 >> %3").arg(getName()).arg(m_onState).arg(in));
	if(m_onState != in)
	{
		m_onState = in;
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
	
	if (getOnColor() != BooleanLed::defaultColor)
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
	setOnColor(file.readColorEntry("Color",&BooleanLed::defaultColor));
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


BooleanLedView::BooleanLedView(Component * comp, eViewType viewType, const char * name)
	: CompViewSize(comp, viewType, name)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 4*gridX, 3*gridY));
		enableRotation(true);
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
	Q_CHECK_PTR(led);
	
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
		connPos.setY( getDrawingPlace().height()/(gridY*2) );
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


#define RED     red
#define GREEN   green
#define ORANGE  QColor(255,128,0)


BooleanLedColorPropertyWidget::BooleanLedColorPropertyWidget(const QString & text, bool withDefault, QWidget *parent, const char *name)
	:	QVButtonGroup(text, parent, name)
{
	
	if (withDefault)
	{
		m_defaultColor = new QCheckBox(i18n("Automatic colored"), this);
		Q_CHECK_PTR(m_defaultColor);
	}
	else
	{
		m_defaultColor = 0;
	}
	
	
	m_red = new QRadioButton(i18n("Red"), this);
	Q_CHECK_PTR(m_red);
	m_green = new QRadioButton(i18n("Green"), this);
	Q_CHECK_PTR(m_green);
	m_orange = new QRadioButton(i18n("Orange"), this);
	Q_CHECK_PTR(m_orange);
	
	QHBox * userBox = new QHBox(this);
	Q_CHECK_PTR(userBox);
	userBox->setSpacing(KDialog::spacingHint());
	
	QLabel * label = new QLabel(i18n("User defined:"),userBox);
	Q_CHECK_PTR(label);
	m_userColor = new KColorButton(userBox);;
	Q_CHECK_PTR(m_userColor);
	label->setBuddy(m_userColor);
	// TODO add ToolTip



	connect(m_red,SIGNAL(clicked()),this,SLOT(slotRed()));
	connect(m_green,SIGNAL(clicked()),this,SLOT(slotGreen()));
	connect(m_orange,SIGNAL(clicked()),this,SLOT(slotOrange()));
	connect(m_userColor,SIGNAL(changed(const QColor &)),this,SLOT(setColor(const QColor &)));
	if (m_defaultColor)
	{
		connect(m_defaultColor,SIGNAL(toggled(bool)),this,SLOT(slotDefault(bool)));
	}
}

BooleanLedColorPropertyWidget::~BooleanLedColorPropertyWidget()
{
}

void BooleanLedColorPropertyWidget::setColor(const QColor & newColor)
{
	if (m_defaultColor && (newColor.isValid() == m_defaultColor->isChecked()))
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
	if(m_defaultColor && (m_defaultColor->isChecked()))
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
	return (m_defaultColor && m_defaultColor->isChecked());
}


//##########################################################################################
//##########################################################################################




BooleanLedPropertyWidget::BooleanLedPropertyWidget(Component * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, 1, parent, name)
{
	m_onColor = new BooleanLedColorPropertyWidget(i18n("Color On"), false, this);
	Q_CHECK_PTR(m_onColor);
	
	m_offColor = new BooleanLedColorPropertyWidget(i18n("Color Off"), true, this);
	Q_CHECK_PTR(m_offColor);

	QHBox * exampleBox = new QHBox(this);
	Q_CHECK_PTR(exampleBox);
	exampleBox->setSpacing(KDialog::spacingHint());
	
	QLabel * label = new QLabel(i18n("Result:"),exampleBox);
	Q_CHECK_PTR(label);
	label = new QLabel(i18n("On"),exampleBox);
	Q_CHECK_PTR(label);
	m_exampleOn = new KLed(exampleBox);;
	m_exampleOn->setFixedSize(20,20);
	Q_CHECK_PTR(m_exampleOn);
	
	label = new QLabel(i18n("Off"),exampleBox);
	Q_CHECK_PTR(label);
	m_exampleOff = new KLed(exampleBox);
	m_exampleOff->setFixedSize(20,20);
	Q_CHECK_PTR(m_exampleOff);
	
	connect(m_onColor, SIGNAL(changed(const QColor &)), this, SLOT(slotOnColor(const QColor &)));
	connect(m_offColor, SIGNAL(changed(const QColor &)), this, SLOT(slotOffColor(const QColor &)));
 		
	
	m_onColor->setColor(((BooleanLed*)comp)->getOnColor());
	m_offColor->setColor(((BooleanLed*)comp)->getOffColor());
	m_exampleOn->on();
	slotOnColor(m_onColor->color());
	slotOffColor(m_offColor->color());
	
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

	m_onColor->setColor(BooleanLed::defaultColor);
	m_offColor->setColor(QColor());
}


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

#include "booleanled.moc"


