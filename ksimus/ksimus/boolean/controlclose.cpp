/***************************************************************************
                          controlclose.cpp  -  description
                             -------------------
    begin                : Thu Mar 28 2002
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
#include <limits.h>

// QT-Includes
#include <qpainter.h>
#include <qtimer.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qgrid.h>


// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "controlclose.h"
#include "resource.h"
#include "ksimdebug.h"
#include "connectorboolinedge.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimdata.h"
#include "ksimicon.h"
#include "ksimspinbox.h"

// Forward declaration

#define DEFAULT_NO_OF_EDGES    1




Component * ControlClose::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ControlClose(container, ci);
}

const ComponentInfo * ControlClose::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Control Close"),
	                                QString::fromLatin1("Control/Close"),
	                                i18n("Component", "Control/Close"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,	
	                                QString::null,
	                                QString::fromLatin1("component-close"));
	return &Info;
}


//###############################################################
//###############################################################


ControlClose::ControlClose(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
		m_numberOfEdges(DEFAULT_NO_OF_EDGES),
		m_edgeCnt(0)
{
	m_trigger = new ConnectorBoolInEdge(this,
	                               QString::fromLatin1("Trigger"),
	                               i18n("Connector", "Trigger"));
	CHECK_PTR(m_trigger);
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ControlCloseView(this, SHEET_VIEW, "ControlCloseSV");
		getTriggerConnector()->setGridPos(0, getSheetView()->getPlace().height() / gridY / 2);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
}

ControlClose::~ControlClose()
{
}

void ControlClose::reset()
{
	Component::reset();
	
	m_edgeCnt = getNumberOfEdges();
}

void ControlClose::calculate()
{
	Component::calculate();

	if (getTriggerConnector()->getInput())
	{
		m_edgeCnt--;
		if (m_edgeCnt <= 0)
		{
			QTimer::singleShot(0, getDoc()->getApp(), SLOT(slotFileClose()));
		}
	}
}

void ControlClose::save(KSimData & file) const
{
	Component::save(file);
	
	if (getNumberOfEdges() != DEFAULT_NO_OF_EDGES)
	{
		file.writeEntry("Number of edges", getNumberOfEdges());
	}
}

bool ControlClose::load(KSimData & file, bool copyLoad)
{
	setNumberOfEdges( file.readNumEntry("Number of edges", DEFAULT_NO_OF_EDGES) );
	
	return Component::load(file, copyLoad);
}

void ControlClose::setNumberOfEdges(int number)
{
	m_numberOfEdges = number;
}

int ControlClose::getNumberOfEdges() const
{
	return m_numberOfEdges;
}

ComponentPropertyBaseWidget * ControlClose::createGeneralProperty(QWidget *parent)
{
	ControlClosePropertyWidget * wid;
	wid = new ControlClosePropertyWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

// Static members
QPixmap * ControlCloseView::pIcon = (QPixmap *)0;
unsigned int ControlCloseView::compViewCnt = 0;
int ControlCloseView::offsetX = 0;
int ControlCloseView::offsetY = 0;
int ControlCloseView::height = 0;
int ControlCloseView::width = 0;


ControlCloseView::ControlCloseView(ControlClose * comp, eViewType viewType, const char * name)
	: CompView(comp, viewType, name)
{
	if (!pIcon)
	{
		pIcon = new QPixmap(KSimIcon::load("fileclose", KIcon::Small));
		CHECK_PTR(pIcon);
		
		width  = ((pIcon->width()  + 4 + gridX - 1) / gridX ) * gridX;
		height = ((pIcon->height() + 4 + gridY - 1) / gridY ) * gridY;
		offsetX = (width - pIcon->width()) / 2 + gridX + 1;
		offsetY = (height - pIcon->height()) / 2 + 1;
	}
	compViewCnt ++;
	
	setPlace(QRect(0, 0, width + gridX, height));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

ControlCloseView::~ControlCloseView()
{
	compViewCnt --;
	if (compViewCnt == 0)
	{
		delete pIcon;
		pIcon = (QPixmap *)0;
	}
}

void ControlCloseView::draw(QPainter * p)
{
	drawFrame(p);
	
	p->drawPixmap(offsetX, offsetY, *pIcon);
	
	CompView::draw(p);
}


//###############################################################
//###############################################################

ControlClosePropertyWidget::ControlClosePropertyWidget(ControlClose * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_counterLabel = new QLabel(i18n("Number of edges: "), this, "m_counterLabel");
	CHECK_PTR(m_counterLabel);
	
	m_counter = new KSimSpinBox(1, INT_MAX, 1, this, "m_counter");
	CHECK_PTR(m_counter);
	m_counterLabel->setBuddy(m_counter);
	tip = i18n("Set number of required edges until the window closes here.");
	addToolTip(tip, m_counter, m_counterLabel);
	addWhatsThis(tip, m_counter, m_counterLabel);
	
	// Setup value
	m_counter->setValue(comp->getNumberOfEdges());
}

ControlClosePropertyWidget::~ControlClosePropertyWidget()
{
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ControlClosePropertyWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();
	
	if (getComponent()->getNumberOfEdges() != m_counter->value())
	{
		changeData();
		getComponent()->setNumberOfEdges(m_counter->value());
	}
}


/** The function defaultPressed() is called, if user wants to set the default values.
    You have to reimplement this function, if you add new properties.
  */
void ControlClosePropertyWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_counter->setValue(DEFAULT_NO_OF_EDGES);
}

#undef DEFAULT_NO_OF_EDGES
