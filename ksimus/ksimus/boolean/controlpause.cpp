/***************************************************************************
                          controlpause.cpp  -  description
                             -------------------
    begin                : Sun Jul 14 2002
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
#include "controlpause.h"
#include "resource.h"
#include "ksimdebug.h"
#include "connectorboolinedge.h"
#include "componentinfo.h"
#include "ksimusdoc.h"
#include "ksimus.h"
#include "ksimdata.h"

// Forward declaration

#define DEFAULT_NO_OF_EDGES    1




static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ControlPause(container, ci);
}

const ComponentInfo * getControlPauseInfo()
{
	static const ComponentInfo Info(i18n("Control Pause"),
	                                QString::fromLatin1("Control/Pause"),
	                                i18n("Component", "Control/Pause"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create/*,	
	                                QString::null,
	                                QString::fromLatin1("component-close")  TODO */);
	return &Info;
}


//###############################################################
//###############################################################


ControlPause::ControlPause(CompContainer * container, const ComponentInfo * ci)
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
		new ControlPauseView(this, SHEET_VIEW);
		getTriggerConnector()->setGridPos(0, getSheetView()->getPlace().height() / gridY / 2);
	}
	getAction().disable(KSimAction::UPDATEVIEW);
}

ControlPause::~ControlPause()
{
}

void ControlPause::reset()
{
	Component::reset();
	
	m_edgeCnt = getNumberOfEdges();
}

void ControlPause::calculate()
{
	Component::calculate();

	if (getTriggerConnector()->getInput())
	{
		m_edgeCnt--;
		if (m_edgeCnt <= 0)
		{
			QTimer::singleShot(0, getDoc()->getApp(), SLOT(slotExecutePause()));
		}
	}
}

void ControlPause::save(KSimData & file) const
{
	Component::save(file);
	
	if (getNumberOfEdges() != DEFAULT_NO_OF_EDGES)
	{
		file.writeEntry("Number of edges", getNumberOfEdges());
	}
}

bool ControlPause::load(KSimData & file, bool copyLoad)
{
	setNumberOfEdges( file.readNumEntry("Number of edges", DEFAULT_NO_OF_EDGES) );
	
	return Component::load(file, copyLoad);
}

void ControlPause::setNumberOfEdges(int number)
{
	m_numberOfEdges = number;
}

int ControlPause::getNumberOfEdges() const
{
	return m_numberOfEdges;
}

ComponentPropertyBaseWidget * ControlPause::createGeneralProperty(QWidget *parent)
{
	ControlPausePropertyWidget * wid;
	wid = new ControlPausePropertyWidget(this, parent);
	CHECK_PTR(wid);
	
	return wid;
}

//###############################################################
//###############################################################

static QPixmap * pPauseIcon = 0;
static unsigned int compViewCnt = 0;
static int offsetX, offsetY, width, height;


ControlPauseView::ControlPauseView(ControlPause * comp, eViewType viewType)
	: CompView(comp, viewType)
{
	if (!pPauseIcon)
	{
		pPauseIcon = new QPixmap(KGlobal::iconLoader()->loadIcon(QString::fromLatin1("player_pause"),KIcon::Toolbar));
		CHECK_PTR(pPauseIcon);
		
		width  = ((pPauseIcon->width()  + 4 + gridX - 1) / gridX ) * gridX;
		height = ((pPauseIcon->height() + 4 + gridY - 1) / gridY ) * gridY;
		offsetX = (width - pPauseIcon->width()) / 2 + gridX + 1;
		offsetY = (height - pPauseIcon->height()) / 2 + 1;
	}
	compViewCnt ++;
	
	setPlace(QRect(0, 0, width + gridX, height));
	enableConnectorSpacingTop(false);
	enableConnectorSpacingRight(false);
	enableConnectorSpacingBottom(false);
//	enableConnectorSpacingLeft(false);
	enableRotation(true);
}

ControlPauseView::~ControlPauseView()
{
	compViewCnt --;
	if (compViewCnt == 0)
	{
		delete pPauseIcon;
		pPauseIcon = 0;
	}
}

void ControlPauseView::draw(QPainter * p)
{
	drawFrame(p);
	
	p->drawPixmap(offsetX, offsetY, *pPauseIcon);
	
	CompView::draw(p);
}


//###############################################################
//###############################################################

ControlPausePropertyWidget::ControlPausePropertyWidget(ControlPause * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	
	QString tip;
	
	m_counterLabel = new QLabel(i18n("Number of edges: "), this, "m_counterLabel");
	CHECK_PTR(m_counterLabel);
	
	m_counter = new QSpinBox(1, INT_MAX, 1, this, "m_counter");
	CHECK_PTR(m_counter);
	tip = i18n("Set number of required edges until the simulation is paused here.");
	addToolTip(tip, m_counter, m_counterLabel);
	addWhatsThis(tip, m_counter, m_counterLabel);
	
	// Setup value
	m_counter->setValue(comp->getNumberOfEdges());
}

ControlPausePropertyWidget::~ControlPausePropertyWidget()
{
}

/** The function acceptPressed() is called, if changes are accepted.
	You have to reimplement this function, if you add new properties.
	If you do so, then first call function changeData() and than changed data!
  */
void ControlPausePropertyWidget::acceptPressed()
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
void ControlPausePropertyWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_counter->setValue(DEFAULT_NO_OF_EDGES);
}
