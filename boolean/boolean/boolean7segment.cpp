/***************************************************************************
                          boolean7segment.cpp  -  description
                             -------------------
    begin                : Wed Dec 26 2001
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
//#include <qpainter.h>
#include <qdrawutil.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qlcdnumber.h>

// KDE-Includes
#include <klocale.h>
#include <kcolorbtn.h>

// Project-Includes
#include "ksimus/connectorboolin.h"
#include "ksimus/componentinfo.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/ksimdata.h"
#include "ksimus/componentpropertydialog.h"

#include "boolean7segment.h"


// Forward declaration

namespace KSimLibBoolean
{

//#######################################################################
//#######################################################################

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new Boolean7Segment(container, ci);
}

const ComponentInfo BooleanBoolean7SegmentInfo("7 Segment Display",
                                               "Boolean/Output/7 Segment Display",
                                               QString::null,//"7 Segment Display",
                                               VA_SHEET_AND_USER,
                                               create	);



//############################################################################
//############################################################################


#define DEFAULT_FOREGROUND  red
#define DEFAULT_BACKGROUND  black
#define DEFAULT_ENA_FRAME   true


Boolean7Segment::Boolean7Segment(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_number(0),
		m_foreColor(DEFAULT_FOREGROUND),
		m_backColor(DEFAULT_BACKGROUND),
		m_enaFrame(DEFAULT_ENA_FRAME)
{
	m_in1 = new ConnectorBoolIn (this, i18n("Input 1"));
	CHECK_PTR(m_in1);
	
	m_in2 = new ConnectorBoolIn (this, i18n("Input 2"));
	CHECK_PTR(m_in2);
	
	m_in4 = new ConnectorBoolIn (this, i18n("Input 4"));
	CHECK_PTR(m_in4);
	
	m_in8 = new ConnectorBoolIn (this, i18n("Input 8"));
	CHECK_PTR(m_in8);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new Boolean7SegmentView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new Boolean7SegmentView(this, USER_VIEW);
	}
	getAction().disable(KSimAction::UPDATEOUTPUT);
}

/*Boolean7Segment::~Boolean7Segment()
{
} */

/** save component properties */
void Boolean7Segment::save(KSimData & file) const
{
	Component::save(file);
	
	if (getForeground() != DEFAULT_FOREGROUND)
	{
		file.writeEntry("Fore Color", getForeground());
	}
	if (getBackground() != DEFAULT_BACKGROUND)
	{
		file.writeEntry("Back Color", getBackground());
	}
	if (isFrameEnabed() != DEFAULT_ENA_FRAME)
	{
		file.writeEntry("Frame enabled", isFrameEnabed());
	}
	
}

/** load component properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool Boolean7Segment::load(KSimData & file, bool copyLoad)
{
	setForeground(file.readColorEntry("Fore Color",&DEFAULT_FOREGROUND));
	setBackground(file.readColorEntry("Back Color",&DEFAULT_BACKGROUND));
	setFrameEnabled(file.readBoolEntry("Frame enabled",DEFAULT_ENA_FRAME));
	
	return Component::load(file, copyLoad);
}

void Boolean7Segment::calculate()
{
	Component::calculate();

	int newValue = 0;
	
	if(getInput1()->getInput())
	{
		newValue += 1;
	}
	if(getInput2()->getInput())
	{
		newValue += 2;
	}
	if(getInput4()->getInput())
	{
		newValue += 4;
	}
	if(getInput8()->getInput())
	{
		newValue += 8;
	}
	
	if(m_number != newValue)
	{
		m_number = newValue;
		emit signalSetNumber(m_number);
	}
}

/** Reset all simulation variables */
void Boolean7Segment::reset()
{
	Component::reset();

	m_number = 0;
	
	emit signalSetNumber(m_number);
}

/** Init the property dialog */
void Boolean7Segment::initPropertyDialog(ComponentPropertyDialog * dialog)
{
	Component::initPropertyDialog(dialog);
	
	QVBox * page;
	Boolean7SegmentPropertyWidget * wid;
	page = dialog->addVBoxPage(i18n("Style"));
	wid = new Boolean7SegmentPropertyWidget(this, page);
	dialog->connectSlots(wid);
}

void Boolean7Segment::setForeground(const QColor & color)
{
	m_foreColor = color;
	
	emit signalSetForeground(color);
}

void Boolean7Segment::setBackground(const QColor & color)
{
	m_backColor = color;
	
	emit signalSetBackground(color);
}

void Boolean7Segment::setFrameEnabled(bool ena)
{
	m_enaFrame = ena;
	
	emit signalEnableFrame(ena);
}


//############################################################################
//############################################################################



Boolean7SegmentView::Boolean7SegmentView(Boolean7Segment * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 5*gridX, 6*gridY));
		setMinSize(5*gridX,6*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
		enableConnectorSpacingRight(false);
	}
	else
	{
		setPlace(QRect(0, 0, 4*gridX, 5*gridY));
		setMinSize(4*gridX,5*gridY);
	}	
}


QWidget * Boolean7SegmentView::createCompViewWidget(QWidget * parent)
{
	Boolean7SegmentWidgetView * wid = new Boolean7SegmentWidgetView(this, parent,"LCD");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getComponent(), SIGNAL(signalSetNumber(int)), wid->m_lcd, SLOT(display(int)));
	// Foreground color changed (Component->CompViewWidget)
	connect(getComponent(), SIGNAL(signalSetForeground(const QColor &)), wid, SLOT(slotForeground(const QColor &)));
	// Background color changed (Component->CompViewWidget)
	connect(getComponent(), SIGNAL(signalSetBackground(const QColor &)), wid, SLOT(slotBackground(const QColor &)));
	// Frame changed (Component->CompViewWidget)
	connect(getComponent(), SIGNAL(signalEnableFrame(bool)), wid, SLOT(slotEnableFrame(bool)));

	return wid;
}

void Boolean7SegmentView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		#define connectors	4
		int height = getPlace().height()/gridY;
		
		int deltaPlace = (height - connectors) / (connectors + 1) + 1;
		
		int start = (height - (deltaPlace * connectors)) / 2;
		
		QPoint connPos(0, start);
		get7Seg()->getInput1()->setGridPos(connPos);
		connPos.ry() += deltaPlace;
		get7Seg()->getInput2()->setGridPos(connPos);
		connPos.ry() += deltaPlace;
		get7Seg()->getInput4()->setGridPos(connPos);
		connPos.ry() += deltaPlace;
		get7Seg()->getInput8()->setGridPos(connPos);
	}
}


/*Boolean7SegmentView::~Boolean7SegmentView()
{
} */



//##########################################################################################
//##########################################################################################

Boolean7SegmentWidgetView::Boolean7SegmentWidgetView(Boolean7SegmentView * cv, QWidget *parent, const char *name)
	:	CompViewWidget(cv,parent,name)
{
	QGridLayout * lay = new QGridLayout(this,1,1);
	CHECK_PTR(lay);
	
	m_lcd = new QLCDNumber(1, this);
	CHECK_PTR(m_lcd);
	lay->addWidget(m_lcd,0,0);
	m_lcd->setMode(QLCDNumber::Hex);
	m_lcd->setSegmentStyle(QLCDNumber::Flat);
	
	slotForeground(get7Seg()->getForeground());
	slotBackground(get7Seg()->getBackground());
	slotEnableFrame(get7Seg()->isFrameEnabed());
}

/*Boolean7SegmentWidgetView::~Boolean7SegmentWidgetView()
{
} */

void Boolean7SegmentWidgetView::slotForeground(const QColor & color)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Active, QColorGroup::Foreground, color);
	setPalette(pal);
}

void Boolean7SegmentWidgetView::slotBackground(const QColor & color)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Active, QColorGroup::Background, color);
	setPalette(pal);
}

void Boolean7SegmentWidgetView::slotEnableFrame(bool enaFrame)
{
	if (enaFrame || (getCompView()->getViewType() == SHEET_VIEW))
	{
		m_lcd->setFrameStyle(QFrame::Box | QFrame::Raised);
	}
	else
	{
		m_lcd->setFrameStyle(QFrame::NoFrame);
	}
}

//##########################################################################################
//##########################################################################################

Boolean7SegmentPropertyWidget::Boolean7SegmentPropertyWidget(Boolean7Segment * comp, QWidget *parent, const char *name)
	:	ComponentPropertyBaseWidget(comp, parent, name),
		m_view(comp->getSheetView())
{
	QLabel * label;
	
	// Set main layout
	QGridLayout * layout = new QGridLayout(this,5,2);
	CHECK_PTR(layout);
	layout->setMargin(KDialog::marginHint());
	layout->setSpacing(KDialog::spacingHint());
	
	
	label = new QLabel(i18n("Foreground:"), this);
	CHECK_PTR(label);
	layout->addWidget(label,0,0);
	m_foreGround = new KColorButton(this);
	CHECK_PTR(m_foreGround);
	layout->addWidget(m_foreGround,0,1);
	
	label = new QLabel(i18n("Background:"), this);
	CHECK_PTR(label);
	layout->addWidget(label,1,0);
	m_backGround = new KColorButton(this);
	CHECK_PTR(m_backGround);
	layout->addWidget(m_backGround,1,1);
	
	m_enaFrame = new QCheckBox(i18n("Enable frame in user interface"), this, "frame");
	CHECK_PTR(m_enaFrame);
	layout->addMultiCellWidget(m_enaFrame,2,2,0,1);
	
	
	label = new QLabel(i18n("Result:"), this);
	CHECK_PTR(label);
	layout->addWidget(label, 3, 0);
	m_lcd = new QLCDNumber(1, this);
	CHECK_PTR(m_lcd);
	m_lcd->setSegmentStyle(QLCDNumber::Flat);
	m_lcd->setFixedSize(42,40);
	m_lcd->setFrameStyle(QFrame::NoFrame);
	connect(m_foreGround,SIGNAL(changed(const QColor &)),this,SLOT(slotForeground(const QColor &)));
	connect(m_backGround,SIGNAL(changed(const QColor &)),this,SLOT(slotBackground(const QColor &)));
	connect(m_enaFrame,SIGNAL(toggled(bool)),this,SLOT(slotEnableFrame(bool)));
	layout->addWidget(m_lcd, 3, 1, AlignCenter);
	
	
	layout->setRowStretch(4,1);
	layout->setColStretch(1,1);

	// Setup values
	slotForeground(get7Seg()->getForeground());
	m_foreGround->setColor(get7Seg()->getForeground());
	slotBackground(get7Seg()->getBackground());
	m_backGround->setColor(get7Seg()->getBackground());
	slotEnableFrame(get7Seg()->isFrameEnabed());
	m_enaFrame->setChecked(get7Seg()->isFrameEnabed());

}

Boolean7SegmentPropertyWidget::~Boolean7SegmentPropertyWidget()
{
}

void Boolean7SegmentPropertyWidget::acceptPressed()
{
	ComponentPropertyBaseWidget::acceptPressed();

	if (get7Seg()->getForeground() != m_foreGround->color())
	{
		changeData();
		get7Seg()->setForeground(m_foreGround->color());
	}
	if (get7Seg()->getBackground() != m_backGround->color())
	{
		changeData();
		get7Seg()->setBackground(m_backGround->color());
	}
	if (get7Seg()->isFrameEnabed() != m_enaFrame->isChecked())
	{
		changeData();
		get7Seg()->setFrameEnabled(m_enaFrame->isChecked());
	}
}

void Boolean7SegmentPropertyWidget::defaultPressed()
{
	ComponentPropertyBaseWidget::defaultPressed();

	m_foreGround->setColor(DEFAULT_FOREGROUND);
	m_backGround->setColor(DEFAULT_BACKGROUND);
	m_enaFrame->setChecked(DEFAULT_ENA_FRAME);
}

void Boolean7SegmentPropertyWidget::slotForeground(const QColor & color)
{
	QPalette pal = m_lcd->palette();
	pal.setColor(QPalette::Active, QColorGroup::Foreground, color);
	m_lcd->setPalette(pal);
	
	m_foreColor = color;
}

void Boolean7SegmentPropertyWidget::slotBackground(const QColor & color)
{
	QPalette pal = m_lcd->palette();
	pal.setColor(QPalette::Active, QColorGroup::Background, color);
	m_lcd->setPalette(pal);
	
	m_backColor = color;
}

void Boolean7SegmentPropertyWidget::slotEnableFrame(bool enaFrame)
{
	if (enaFrame)
	{
		m_lcd->setFrameStyle(QFrame::Box | QFrame::Raised);
	}
	else
	{
		m_lcd->setFrameStyle(QFrame::NoFrame);
	}
}

//###############################################################

}; //namespace KSimLibBoolean

