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

static const char * sPixPos = "PixPos";
static const char * sPixOrient = "PixOrient";
static const char * sUserPos = "UserPos";
static const char * sUserOrient = "UserOrient";

// QT-Includes
#include <qpainter.h>
#include <qpointarray.h>

// KDE-Includes
#include <ksimpleconfig.h>
#include <klocale.h>

// Project-Includes
#include "externalconnector.h"
#include "resource.h"
#include "connectorbase.h"
#include "connectorlist.h"
#include "compcontainer.h"

ExternalConnectorSV::ExternalConnectorSV(Component * comp)
	: CompView(comp, SHEET_VIEW)
{
	setPlace( QRect(0, 0, 5*gridX, 3*gridY) );
}
ExternalConnectorSV::~ExternalConnectorSV()
{
}

void ExternalConnectorSV::draw(QPainter * p)
{
	QRect place(getPlace());
	
	p->setPen(QPen(black, 2));
	p->setBrush(NoBrush);
	p->drawEllipse(place.x()+gridX*1, place.y(), gridX*3, gridY*3);
	p->drawLine(place.x()+gridX*1, place.y()+gridX*3/2 ,
				place.x()+gridX*4, place.y()+gridX*3/2 );

	QPointArray ar;
	if (((ExternalConnector*)getComponent())->isInput())
	{
		ar.setPoints (4, place.x()+gridX*1 , place.y()+gridY,
						 place.x()+gridX*5/2 , place.y()+gridY*3/2,
						 place.x()+gridX*1 , place.y()+gridY*2,
						 place.x()+gridX*1 , place.y()+gridY);
	}
	else
	{
		ar.setPoints (4, place.x()+gridX*5/2 , place.y()+gridY,
						 place.x()+gridX*4 , place.y()+gridY*3/2,
						 place.x()+gridX*5/2 , place.y()+gridY*2,
						 place.x()+gridX*5/2 , place.y()+gridY);
	}
	p->setPen(QPen(black, 1));
	p->setBrush(SolidPattern);
	p->drawPolygon( ar );
	
	
	CompView::draw(p);
}


//###############################################################

ExternalConnector::ExternalConnector(CompContainer * container, const ComponentInfo * ci)
	: Component(container, ci),
	  input(false)
{
	m_isExtConn = true;
	
	// Initializes the sheet view
	if (getSheetMap())
	{
		new ExternalConnectorSV(this);
	}
	// set to invalid
	pixmapPos.setX(-1);
	setPixmapOrientation(CO_LEFT);
	userViewPos.setX(-1);
	setUserViewOrientation(CO_LEFT);
	
}

ExternalConnector::~ExternalConnector()
{
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

void ExternalConnector::checkProperty(QStringList & errorMsg)
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
}

/** Save ExternalConnector properties */
void ExternalConnector::save(KSimpleConfig & file) const
{
	Component::save(file);	
	
	// Pixmap view
	if (pixmapPos.x() != -1)
	{
		file.writeEntry(sPixPos, pixmapPos);
		
//		if (pixmapOrient != (getConnType() & CON_ORIENT_MASK))
		if (pixmapOrient != (getExternalConn()->getOrientation()))
			file.writeEntry(sPixOrient, (int)pixmapOrient);
	}
	// User view
	if (userViewPos.x() != -1)
	{
		file.writeEntry(sUserPos, userViewPos);
		
//		if (userViewOrient != (getConnType() & CON_ORIENT_MASK))
		if (userViewOrient != (getExternalConn()->getOrientation()))
			file.writeEntry(sUserOrient, (int)userViewOrient);
	}
}

/** Load ExternalConnector properties
*   copyLoad is true, if the load function is used as a copy function
*	Returns true if successful */
bool ExternalConnector::load(KSimpleConfig & file, bool copyLoad)
{
	bool ok;
	
	QPoint defaultPos(-1,0);
	
	ok = Component::load(file, copyLoad);	
	
	if (copyLoad)
	{
		// set to invalid
		pixmapPos.setX(-1);
		pixmapOrient = CO_LEFT;
	
		userViewPos.setX(-1);
		userViewOrient = CO_LEFT;
	}
	else
	{
		// Pixmap view
		pixmapPos = file.readPointEntry(sPixPos, &defaultPos);
		setPixmapOrientation((ConnOrientationType)file.readNumEntry(sPixOrient, (getExternalConn()->getOrientation())));
		// User view
		userViewPos = file.readPointEntry(sUserPos, &defaultPos);
		setUserViewOrientation((ConnOrientationType)file.readNumEntry(sUserOrient, (getExternalConn()->getOrientation())));
	}

	return ok;
}

/** Returns the *external* connector */
ConnectorBase * ExternalConnector::getExternalConn() const
{
	if (isInput())
		return getConnList()->at(1);
	else
		return getConnList()->at(0);
}

/** Returns the *internal* connector */
ConnectorBase * ExternalConnector::getInternalConn() const
{
	if (isInput())
		return getConnList()->at(0);
	else
		return getConnList()->at(1);
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
	pixmapOrient = orientation;
};
		
/** Sets the orientation of the connector in user view mode. */
void ExternalConnector::setUserViewOrientation(ConnOrientationType orientation)
{
	userViewOrient = orientation;
};
		
