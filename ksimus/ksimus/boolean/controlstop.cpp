/***************************************************************************
                          controlstop.cpp  -  description
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
#include <qspinbox.h>
#include <qgrid.h>


// KDE-Includes
#include <klocale.h>
#include <kiconloader.h>

// Project-Includes
#include "controlstop.h"
#include "resource.h"
#include "ksimdebug.h"
#include "connectorboolinedge.h"
#include "componentinfo.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimdata.h"

// Forward declaration

#define DEFAULT_NO_OF_EDGES    1


Component * ControlStop::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ControlStop(container, ci);
}

const ComponentInfo * ControlStop::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Control Stop"),
	                                QString::fromLatin1("Control/Stop"),
	                                i18n("Component", "Control/Stop"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,	
	                                QString::null,
	                                QString::fromLatin1("component-stop"));
	return &Info;
}


//###############################################################
//###############################################################


ControlStop::ControlStop(CompContainer * container, const ComponentInfo * ci)
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
		new ControlStopView(this, SHEET_VIEW);
		getTriggerConnector()->setGridPos(0, getSheetView()->getPlace().height() / gridY / 2);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
}

ControlStop::~ControlStop()
{
}

void ControlStop::reset()
{
	Component::reset();
	
	m_edgeCnt = getNumberOfEdges();
}

void ControlStop::calculate()
{
	Component::calculate();

	if (getTriggerConnector()->getInput())
	{
		m_edgeCnt--;
		if (m_edgeCnt <= 0)
		{
			QTimer::singleShot(0, getDoc()->getApp(), SLOT(slotExecuteStop()));
		}
	}
}

void ControlStop::save(KSimData & file) const
{
	Component::save(file);
	
	if (getNumberOfEdges() != DEFAULT_NO_OF_EDGES)
	{
		file.writeEntry("Number of edges", getNumberOfEdges());
	}
}

bool ControlStop::load(KSimData & file, bool copyLoad)
{
	setNumberOfEdges( file.readNumEntry("Number of edges", DEFAULT_NO_OF_EDGES) );
	
	return Component::load(file, copyLoad);
}

void ControlStop::setNumberOfEdges(int number)
{
	m_numberOfEdges = number;
}

int ControlStop::getNumberOfEdges() const
{
	return m_numberOfEdges;
}

ComponentPropertyBaseWidget * ControlStop::createGeneralProperty(QWidget *parent)
{
	ControlStopPropertyWidget * wid;
	wid = new ControlStopPropertyWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

// Static members
QPixmap * ControlStopView::pIcon = (QPixmap *)0;
unsigned int ControlStopView::compViewCnt = 0;
int ControlStopView::offsetX = 0;
int ControlStopView::offsetY = 0;
int ControlStopView::height = 0;
int ControlStopView::width = 0;


ControlStopView::ControlStopView(ControlStop * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	if (!pIcon)
	{
		pIcon = new QPixmap(KGlobal::iconLoader()->loadIcon(QString::fromLatin1("stop"),KIcon::Toolbar));
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

ControlStopView::~ControlStopView()
{
	compViewCnt --;
	if (compViewCnt == 0)
	{
		delete pIcon;
		pIcon = (QPixmap *)0;
	}
}

void ControlStopView::draw(QPainter * p)
{
	drawFrame(p);
	
	p->drawPixmap(offsetX, offsetY, *pIcon);
	
	CompView::draw(p);
}



//###############################################################
//###############################################################

ControlStopPropertyWidget::ControlStopPropertyWidget(ControlStop * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_counterLabel = new QLabel(i18n("Number of edges: "), this, "m_counterLabel");
	CHECK_PTR(m_counterLabel);
	
	m_counter = new QSpinBox(1, INT_MAX, 1, this, "m_counter");
	CHECK_PTR(m_counter);
	tip = i18n("Set number of required edges until the simulation stops here.");
	addToolTip(tip, m_counter, m_counterLabel);
	addWhatsThis(tip, m_counter, m_counterLabel);
	
	// Setup value
	m_counter->setValue(comp->getNumberOfEdges());
}
	
ControlStopPropertyWidget::~ControlStopPropertyWidget()
{
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ControlStopPropertyWidget::acceptPressed()
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
void ControlStopPropertyWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_counter->setValue(DEFAULT_NO_OF_EDGES);
}

#undef DEFAULT_NO_OF_EDGES
