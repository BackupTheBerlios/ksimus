/***************************************************************************
                          externalconnector.cpp  -  description
                             -------------------
    begin                : Sat Jul 8 2000
    copyright            : (C) 2000 by Rasmus Diekenbrock
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

static const char * sPixPos     = "PixPos";
static const char * sPixOrient  = "PixOrient";
static const char * sUserPos    = "UserPos";
static const char * sUserOrient = "UserOrient";
static const char * sOption     = "Optional";
static const char * sOptionEna  = "Optional Enabled";

// QT-Includes
#include <qpainter.h>
#include <qpointarray.h>
#include <qvbox.h>
#include <qgroupbox.h>
#include <qcheckbox.h>

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "ksimdata.h"
#include "externalconnector.h"
#include "resource.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "compcontainer.h"

ExternalConnectorSV::ExternalConnectorSV(Component * comp)
	: CompView(comp, SHEET_VIEW)
{
	setPlace( QRect(0, 0, 5*gridX, 3*gridY) );
	enableRotation(true);
}
ExternalConnectorSV::~ExternalConnectorSV()
{
}

void ExternalConnectorSV::draw(QPainter * p)
{
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawEllipse(gridX*1, 0, gridX*3, gridY*3);
	p->drawLine(gridX*1, gridX*3/2, gridX*4, gridX*3/2 );

	QPointArray ar;
	if (((ExternalConnector*)getComponent())->isInput())
	{
		ar.setPoints(4, gridX*1,   gridY,
		                gridX*5/2, gridY*3/2,
		                gridX*1,   gridY*2,
		                gridX*1,   gridY);
	}
	else
	{
		ar.setPoints(4, gridX*5/2, gridY,
		                gridX*4,   gridY*3/2,
		                gridX*5/2, gridY*2,
		                gridX*5/2, gridY);
	}
	p->setPen(QPen(black, 1));
	p->setBrush(SolidPattern);
	p->drawPolygon( ar );
	
	CompView::draw(p);
}



//###############################################################

ExternalConnectorMultipleOutputSV::ExternalConnectorMultipleOutputSV(Component * comp)
	: CompView(comp, SHEET_VIEW)
{
	setPlace( QRect(0, 0, 5*gridX, 3*gridY) );
	enableRotation(true);
}
ExternalConnectorMultipleOutputSV::~ExternalConnectorMultipleOutputSV()
{
}

void ExternalConnectorMultipleOutputSV::draw(QPainter * p)
{
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawEllipse(gridX*1, 0, gridX*3, gridY*3);
	p->drawLine(gridX*1, gridX*3/2, gridX*4, gridX*3/2 );
	
	p->setPen(QPen(black, 1));
	p->setBrush(SolidPattern);

	if (((ExternalConnector*)getComponent())->isInput())
	{
		#define LEFT   (gridX*1)
		#define RIGHT  (gridX*3)
		#define LR_MID ((LEFT+RIGHT)/2)
		#define TOP    (gridY*1)
		#define BOTTOM (gridY*2)
		#define TB_MID ((TOP+BOTTOM)/2)
		
		QPointArray ar;
		
		ar.setPoints(4, LEFT,     TB_MID,
		                LR_MID-1, TOP,
		                LR_MID-1, BOTTOM,
		                LEFT,     TB_MID);
		p->drawPolygon( ar );
		
		ar.setPoints(4, LR_MID+1, TOP,
		                RIGHT,    TB_MID,
		                LR_MID+1, BOTTOM,
		                LR_MID+1, TOP);
		p->drawPolygon( ar );
		
		#undef LEFT
		#undef RIGHT
		#undef LR_MID
		#undef TOP
		#undef BOTTOM
		#undef TB_MID
	}
	else
	{
		#define LEFT   (gridX*2-1)
		#define RIGHT  (gridX*4-1)
		#define LR_MID ((LEFT+RIGHT)/2)
		#define TOP    (gridY*1)
		#define BOTTOM (gridY*2)
		#define TB_MID ((TOP+BOTTOM)/2)
		
		QPointArray ar;
		ar.setPoints(4, LEFT,     TB_MID,
		                LR_MID-1, TOP,
		                LR_MID-1, BOTTOM,
		                LEFT,     TB_MID);
		p->drawPolygon( ar );
		
		ar.setPoints(4, LR_MID+1, TOP,
		                RIGHT,    TB_MID,
		                LR_MID+1, BOTTOM,
		                LR_MID+1, TOP);
		p->drawPolygon( ar );
		
		#undef LEFT
		#undef RIGHT
		#undef LR_MID
		#undef TOP
		#undef BOTTOM
		#undef TB_MID
	}
	
	CompView::draw(p);
}

//###############################################################

#define DEFAULT_CO             (isInput() ? CO_LEFT : CO_RIGHT)
#define DEFAULT_OPT_CONN       false
#define DEFAULT_OPT_CONN_ENA   false

ExternalConnector::ExternalConnector(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput)
	: Component(container, ci)
{
	m_flags.input = input;
	init();
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		if (multiOutput)
		{
			CompView * cv = new ExternalConnectorMultipleOutputSV(this);
			CHECK_PTR(cv);
		}
		else
		{
			CompView * cv = new ExternalConnectorSV(this);
			CHECK_PTR(cv);
		}
	}
}

ExternalConnector::ExternalConnector(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci)
{
	m_flags.input = false;
	init();
}

ExternalConnector::~ExternalConnector()
{
}

void ExternalConnector::init()
{
	setComponentType(eExternalConnector);
	setZeroDelayComponent(true);
	setRecursionLocked(false);
	setOptionalConn(DEFAULT_OPT_CONN);
	setOptionalConnEnabled(DEFAULT_OPT_CONN_ENA);
	setModuleConn((ConnectorBase *) 0);
	
	// set to invalid
	m_pixmapPos.setX(-1);
	setPixmapOrientation(DEFAULT_CO);
	m_userViewPos.setX(-1);
	setUserViewOrientation(DEFAULT_CO);
}



void ExternalConnector::reset()
{
	Component::reset();
	
	setRecursionLocked(false);
}

int ExternalConnector::checkCircuit()
{
	int error;
	if(getContainer()->isParentComponent())
	{
		// Don't check the external connector
		error = getInternalConn()->checkCircuit();
	}
	else
	{
		error = Component::checkCircuit();
	}
	return error;
}

/*void ExternalConnector::checkProperty(QStringList & errorMsg)
{
	bool nameUnique(true);
	
	FOR_EACH_COMP(it, *getContainer()->getComponentList())
	{	
		if(  (it.current()->isExtConn())
			&& (it.current() != this)
			&& (it.current()->getName() == getName()))
		{
			nameUnique = false;
			break;
		}
	}
	
	if(!nameUnique)
	{
		errorMsg.append(i18n("Name of External Connectors have to be unique (%1).").arg(getName()));
	}
	
	Component::checkProperty(errorMsg);
} */

/** Save ExternalConnector properties */
void ExternalConnector::save(KSimData & file) const
{
	Component::save(file);
	
	// Pixmap view
	if (m_pixmapPos.x() != -1)
	{
		file.writeEntry(sPixPos, m_pixmapPos);
		
		if (m_pixmapOrient != DEFAULT_CO)
			file.writeEntry(sPixOrient, (int)m_pixmapOrient);
	}
	// User view
	if (m_userViewPos.x() != -1)
	{
		file.writeEntry(sUserPos, m_userViewPos);
		
		if (m_userViewOrient != DEFAULT_CO)
			file.writeEntry(sUserOrient, (int)m_userViewOrient);
	}

	if (isOptionalConn())
	{
		file.writeEntry(sOption, true);
	}
	if (isOptionalConnEnabled())
	{
		file.writeEntry(sOptionEna, true);
	}
}

/** Load ExternalConnector properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool ExternalConnector::load(KSimData & file, bool copyLoad)
{
	bool ok;
	
	QPoint defaultPos(-1,0);
	
	ok = Component::load(file, copyLoad);	
	
	if (copyLoad)
	{
		// set to invalid
		m_pixmapPos.setX(-1);
		m_pixmapOrient = CO_LEFT;
	
		m_userViewPos.setX(-1);
		m_userViewOrient = CO_LEFT;
	}
	else
	{
		// Pixmap view
		m_pixmapPos = file.readPointEntry(sPixPos, &defaultPos);
		setPixmapOrientation((ConnOrientationType)file.readNumEntry(sPixOrient, (getExternalConn()->getOrientation())));
		// User view
		m_userViewPos = file.readPointEntry(sUserPos, &defaultPos);
		setUserViewOrientation((ConnOrientationType)file.readNumEntry(sUserOrient, (getExternalConn()->getOrientation())));
	}

	setOptionalConn(file.readBoolEntry(sOption, false));
	setOptionalConnEnabled(file.readBoolEntry(sOptionEna, false));
	return ok;
}

/** Creates the general property page for the property dialog.
 * Overload this function if you want to use a modified General Propery Page. Use as base class
 * @ref ComponentPropertyGeneralWidget.
 * This function is called by @ref addGeneralProperty*/
ComponentPropertyBaseWidget * ExternalConnector::createGeneralProperty(QWidget *parent)
{
	ExternalConnectorPropertyGeneralWidget * wid;
	wid = new ExternalConnectorPropertyGeneralWidget(this, parent);
	CHECK_PTR(wid);

	return wid;
}

/** Returns the *external* connector */
ConnectorBase * ExternalConnector::getExternalConn() const
{
	return m_externalConn;
}

/** Returns the *internal* connector */
ConnectorBase * ExternalConnector::getInternalConn() const
{
	return m_internalConn;
}

ConnectorBase * ExternalConnector::getModuleConn() const
{
	return m_moduleConn;
}

ConnectorBase * ExternalConnector::getUsedExternalConn() const
{
	if (m_flags.useModuleConn)
	{
		return m_moduleConn;
	}
	else
	{
		return m_externalConn;
	}
}

/** Sets the *external* connector */
void ExternalConnector::setExternalConn(ConnectorBase * extConn)
{
	m_externalConn = extConn;
}

/** Sets the *internal* connector */
void ExternalConnector::setInternalConn(ConnectorBase * inConn)
{
	m_internalConn = inConn;
}

void ExternalConnector::setModuleConn(ConnectorBase * modConn)
{
	m_moduleConn = modConn;
	if (modConn)
	{
		m_flags.useModuleConn = 1;
	}
	else
	{
		m_flags.useModuleConn = 0;
	}
}

/** Sets the orientation of the connector in pixmap mode. */
void ExternalConnector::setPixmapOrientation(ConnOrientationType orientation)
{
	m_pixmapOrient = orientation;
};
		
/** Sets the orientation of the connector in user view mode. */
void ExternalConnector::setUserViewOrientation(ConnOrientationType orientation)
{
	m_userViewOrient = orientation;
};

//##########################################################################################
//##########################################################################################


ExternalConnectorPropertyGeneralWidget::ExternalConnectorPropertyGeneralWidget(ExternalConnector * comp, QWidget *parent, const char *name)
	:	ComponentPropertyGeneralWidget(comp, parent, name)
{
	QString str;

	m_optionalbox = new QGroupBox(1, Qt::Horizontal,
	         i18n("Component property dialog", "Optional connector:"), newRowVBox(), "optionConnBox");
	CHECK_PTR(m_optionalbox);

	m_optionalConn = new QCheckBox(i18n("Component property dialog", "External connector is optional"), m_optionalbox, "optionConn");
	CHECK_PTR(m_optionalConn);
	str = i18n("An external connector may be optional. Optional means the connector may be visible or hidden.");
	addToolTip(str, m_optionalConn);
	addWhatsThis(str, m_optionalConn);

	m_optionalConnEna = new QCheckBox(i18n("Component property dialog", "Default is visible"), m_optionalbox, "optionConnEna");
	CHECK_PTR(m_optionalConnEna);
	str = i18n("An optional external connector may be visible or hidden by default.");
	addToolTip(str, m_optionalConnEna);
	addWhatsThis(str, m_optionalConnEna);


	m_optionalConn->setChecked(getExtConn()->isOptionalConn());
	m_optionalConnEna->setChecked(getExtConn()->isOptionalConnEnabled());
	m_optionalConnEna->setEnabled(getExtConn()->isOptionalConn());
	connect(m_optionalConn, SIGNAL(toggled(bool)), this, SLOT(slotOptionalConnToggled(bool)));
}

/*ExternalConnectorPropertyGeneralWidget::~ExternalConnectorPropertyGeneralWidget()
{
} */

void ExternalConnectorPropertyGeneralWidget::acceptPressed()
{
	ComponentPropertyGeneralWidget::acceptPressed();

	if (getExtConn()->isOptionalConn() != m_optionalConn->isChecked())
	{
		changeData();
		getExtConn()->setOptionalConn( m_optionalConn->isChecked() );
	}

	if (getExtConn()->isOptionalConnEnabled() != m_optionalConnEna->isChecked())
	{
		changeData();
		getExtConn()->setOptionalConnEnabled(m_optionalConnEna->isChecked());
	}
}

void ExternalConnectorPropertyGeneralWidget::defaultPressed()
{
	ComponentPropertyGeneralWidget::defaultPressed();

	m_optionalConn->setChecked(DEFAULT_OPT_CONN);
	m_optionalConnEna->setChecked(DEFAULT_OPT_CONN_ENA);
}

void ExternalConnectorPropertyGeneralWidget::slotOptionalConnToggled(bool state)
{
	m_optionalConnEna->setEnabled(state);
}
