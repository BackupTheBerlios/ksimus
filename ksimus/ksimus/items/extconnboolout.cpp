/***************************************************************************
                          extconnboolout.cpp  -  description
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

#include <klocale.h>
#include "extconnboolout.h"
#include "resource.h"
#include "connectorlist.h"
#include "connectorboolin.h"
#include "connectorboolout.h"
#include "componentinfo.h"
#include "wireproperty.h"
#include "ksimdebug.h"
#include "ksimtimeserver.h"

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolOut(container, ci);
}

const ComponentInfo * getExtConnBoolOutInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Boolean Output"),
	                                QString::fromLatin1("External Connector/Bool Output"),
	                                i18n("Component", "External Connector/Bool Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-ext-conn-bool-out"));
	
	return &Info;
}

//###############################################################

ExtConnBoolOut::ExtConnBoolOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, false)
{
	ConnectorBoolOut * out;
	ConnectorBoolIn * in;
	
	out = new ConnectorBoolOut(this,
	                           QString::fromLatin1("Output"),
	                           i18n("Connector", "Output"),
	                           QPoint(4,1));
	CHECK_PTR(out);
	setExternalConn(out);
	
	in = new ConnectorBoolIn(this,
	                         QString::fromLatin1("Input"),
	                         i18n("Connector", "Input"),
	                         QPoint(0,1));
	CHECK_PTR(in);
	setInternalConn(in);

}

ExtConnBoolOut::~ExtConnBoolOut()
{
}

void ExtConnBoolOut::calculate()
{
	ConnectorBoolOut * out = (ConnectorBoolOut *)getExternalConn();
	ConnectorBoolIn * in = (ConnectorBoolIn *)getInternalConn();
	
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
//	ExternalConnector::calculate();
	
		out->setOutput(in->getInput(), false);
		if (out->getWireProperty())
		{
			out->getWireProperty()->execute();
		}
		setRecursionLocked(false);
	}
	else
	{
		out->setOutput(in->getInput());
	}
}
