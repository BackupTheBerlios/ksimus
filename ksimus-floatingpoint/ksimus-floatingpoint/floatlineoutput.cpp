/***************************************************************************
                          floatlineoutput.cpp  -  description
                             -------------------
    begin                : Sat Dec 29 2001
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
#include <qlabel.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimus/componentinfo.h"
#include "ksimus/resource.h"
#include "ksimus/connectorfloatin.h"

#include "floatlineoutput.h"


// Forward declaration

namespace KSimLibFloatingPoint
{

//#######################################################################
//#######################################################################

Component * FloatLineOutput::create(CompContainer * container, const ComponentInfo * ci)
{
	return new FloatLineOutput(container, ci);
}

const ComponentInfo * FloatLineOutput::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "Floating Point Label"),
	                                QString::fromLatin1("Floating Point/Output/Label"),
	                                i18n("Component", "Floating Point/Output/Label"),
	                                QString::null,
	                                VA_SHEET_AND_USER,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-output-label"));
	return &Info;
}



//############################################################################
//############################################################################


FloatLineOutput::FloatLineOutput(CompContainer * container, const ComponentInfo * ci)
	: ComponentStyle(container, ci),
		m_number(0)
{
	m_input = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"));
	CHECK_PTR(m_input);
	
	setColorAdjustmentEnabled(true);
	setFrameAdjustmentEnabled(true);
	setFontAdjustmentEnabled(true);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new FloatLineOutputView(this, SHEET_VIEW);
	}
	// Initializes the user view
	if (getUserMap())
	{
		new FloatLineOutputView(this, USER_VIEW);
	}
}

/*FloatLineOutput::~FloatLineOutput()
{
} */

void FloatLineOutput::calculate()
{
	double newValue = getInput()->getInput();
	
	if(m_number != newValue)
	{
		m_number = newValue;
		emit signalSetNumber(m_number);
	}
}

/** Reset all simulation variables */
void FloatLineOutput::reset()
{
	ComponentStyle::reset();

	m_number = 0.0;
	
	emit signalSetNumber(m_number);
}


//############################################################################
//############################################################################



FloatLineOutputView::FloatLineOutputView(FloatLineOutput * comp, eViewType viewType)
	: CompViewSize(comp,viewType)
{
	if (viewType == SHEET_VIEW)
	{
		setPlace(QRect(0, 0, 7*gridX, 3*gridY));
		setMinSize(5*gridX, 3*gridY);
		enableConnectorSpacingTop(false);
		enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
		enableConnectorSpacingRight(false);
	}
	else
	{
		setPlace(QRect(0, 0, 6*gridX, 3*gridY));
		setMinSize(4*gridX, 3*gridY);
	}	
}


QWidget * FloatLineOutputView::createCompViewWidget(QWidget * parent)
{
	FloatLineOutputWidgetView * wid = new FloatLineOutputWidgetView(this, parent,"Label");
	
	/* Specific signals */
	// Number changed (Component->LCD)
	connect(getComponent(), SIGNAL(signalSetNumber(double)), wid->m_label, SLOT(setNum(double)));

	return wid;
}

void FloatLineOutputView::resize()
{
	CompViewSize::resize();	

	if (getViewType() == SHEET_VIEW)
	{
		QPoint connPos(0, getPlace().height()/gridY/2);
		getLineOutput()->getInput()->setGridPos(connPos);
	}
}


/*FloatLineOutputView::~FloatLineOutputView()
{
} */



//##########################################################################################
//##########################################################################################

FloatLineOutputWidgetView::FloatLineOutputWidgetView(FloatLineOutputView * cv, QWidget *parent, const char *name)
	:	CompViewVBox(cv,parent,name)
{
	m_label = new QLabel(this);
	CHECK_PTR(m_label);
	m_label->setAlignment(AlignRight | AlignVCenter);
	m_label->setNum(0.0);
	
}

/*FloatLineOutputWidgetView::~FloatLineOutputWidgetView()
{
} */

//##########################################################################################
//##########################################################################################


};  //namespace KSimLibFloatingPoint

