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

// QT-Includes
#include <qpainter.h>
#include <qpointarray.h>

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

#define DEFAULT_CO (isInput() ? CO_LEFT : CO_RIGHT)

ExternalConnector::ExternalConnector(CompContainer * container, const ComponentInfo * ci, bool input, bool multiOutput)
	: Component(container, ci),
	  m_input(input),
	  m_recursionLocked(false)
{
	init();
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		if (multiOutput)
		{
			new ExternalConnectorMultipleOutputSV(this);
		}
		else
		{
			new ExternalConnectorSV(this);
		}
	}
}

ExternalConnector::ExternalConnector(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
	  m_input(false),
	  m_recursionLocked(false)
{
	init();
}

ExternalConnector::~ExternalConnector()
{
}

void ExternalConnector::init()
{
	setComponentType(eExternalConnector);
	setZeroDelayComponent(true);
	
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

	return ok;
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

/** Removes the wire of the test connector */
void ExternalConnector::removeTestConnector()
{
	if (isInput())
	{
		// Remove input connection
			if (getConnList()->at(1)->getWire())
				getContainer()->delConnection(getConnList()->at(1));
	}
	else
	{
		// Remove output connection
			if (getConnList()->at(0)->getWire())
				getContainer()->delConnection(getConnList()->at(0));
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
		
