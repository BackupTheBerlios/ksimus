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

const ComponentInfo Boolean7SegmentInfo(I18N_NOOP("7 Segment Display"),
                                        I18N_NOOP("Boolean/Output/7 Segment Display"),
                                        QString::null,
                                        VA_SHEET_AND_USER,
                                        create	);



//############################################################################
//############################################################################


#define DEFAULT_FOREGROUND  red
#define DEFAULT_BACKGROUND  black
#define DEFAULT_ENA_FRAME   true


Boolean7Segment::Boolean7Segment(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_number(0)
{
	
	
	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
//	setFontAdjustmentEnabled(true);
	
	setDefaultColors(DEFAULT_FOREGROUND,DEFAULT_BACKGROUND);
	setFrameEnabled(true);
	
	m_in1 = new ConnectorBoolIn (this, I18N_NOOP("Input 1"));
	CHECK_PTR(m_in1);
	
	m_in2 = new ConnectorBoolIn (this, I18N_NOOP("Input 2"));
	CHECK_PTR(m_in2);
	
	m_in4 = new ConnectorBoolIn (this, I18N_NOOP("Input 4"));
	CHECK_PTR(m_in4);
	
	m_in8 = new ConnectorBoolIn (this, I18N_NOOP("Input 8"));
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


void Boolean7Segment::calculate()
{
	ComponentStyle::calculate();

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
	ComponentStyle::reset();

	m_number = 0;
	
	emit signalSetNumber(m_number);
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
		setPlace(QRect(0, 0, 4*gridX, 6*gridY));
		setMinSize(4*gridX,6*gridY);
	}	
}


QWidget * Boolean7SegmentView::createCompViewWidget(QWidget * parent)
{
	Boolean7SegmentWidgetView * wid = new Boolean7SegmentWidgetView(this, parent,"LCD");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getComponent(), SIGNAL(signalSetNumber(int)), wid->m_lcd, SLOT(display(int)));

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
	:	CompViewHBox(cv,parent,name)
{
	m_lcd = new QLCDNumber(1, this);
	CHECK_PTR(m_lcd);
	m_lcd->setMode(QLCDNumber::Hex);
	m_lcd->setSegmentStyle(QLCDNumber::Flat);
	m_lcd->setFrameStyle(QFrame::NoFrame);
}

/*Boolean7SegmentWidgetView::~Boolean7SegmentWidgetView()
{
} */


//###############################################################

}; //namespace KSimLibBoolean

