/***************************************************************************
                          extconnbooltrisate.cpp  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#include <klocale.h>

#include "extconnbooltristate.h"
#include "ksimus/resource.h"
#include "ksimus/connectorlist.h"
#include "ksimus/componentinfo.h"
#include "ksimus/connectorbooltristate.h"
//#include "ksimus/wireproperty.h"

namespace KSimLibBoolean
{


static Component * createIn(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTriStateIn(container, ci);
}

const ComponentInfo * getExtConnBoolTriStateInInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean TriState Input"),
	                                QString::fromLatin1("External Connector/Bool TriState Input"),
	                                i18n("Component", "External Connector/Bool TriState Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createIn/*,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-in") TODO */);
	
	return &Info;
}

//###############################################################
//###############################################################

ExtConnBoolTriStateIn::ExtConnBoolTriStateIn(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, true, true)
{
	ConnectorBoolTriState * internal;
	ConnectorBoolTriState * external;
	
	internal = new ConnectorBoolTriState(this,
	                                     QString::fromLatin1("Internal"),
	                                     i18n("Connector", "Internal"),
	                                     QPoint(4,1));
	CHECK_PTR(internal);
	setInternalConn(internal);
	
	external = new ConnectorBoolTriState(this,
	                                     QString::fromLatin1("External"),
	                                     i18n("Connector", "External"),
	                                     QPoint(0,1));
	CHECK_PTR(external);
	setExternalConn(external);
	external->setOrientation(CO_LEFT); // is on the left side
}

/*ExtConnBoolTriState::~ExtConnBoolTriState()
{
}*/

//###############################################################
//###############################################################
//###############################################################
//###############################################################


static Component * createOut(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolTriStateOut(container, ci);
}

const ComponentInfo * getExtConnBoolTriStateOutInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean TriState Output"),
	                                QString::fromLatin1("External Connector/Bool TriState Output"),
	                                i18n("Component", "External Connector/Bool TriState Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                createOut/*,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-in") TODO */);
	
	return &Info;
}

//###############################################################

ExtConnBoolTriStateOut::ExtConnBoolTriStateOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, false, true)
{
	ConnectorBoolTriState * internal;
	ConnectorBoolTriState * external;
	
	internal = new ConnectorBoolTriState(this,
	                                     QString::fromLatin1("Internal"),
	                                     i18n("Connector", "Internal"),
	                                     QPoint(0,1));
	CHECK_PTR(internal);
	setInternalConn(internal);
	internal->setOrientation(CO_LEFT); // is on the left side
	
	external = new ConnectorBoolTriState(this,
	                                     QString::fromLatin1("External"),
	                                     i18n("Connector", "External"),
	                                     QPoint(4,1));
	CHECK_PTR(external);
	setExternalConn(external);
}

/*ExtConnBoolTriStateOut::~ExtConnBoolTriStateOut()
{
}*/


//###############################################################

}; //namespace KSimLibBoolean

