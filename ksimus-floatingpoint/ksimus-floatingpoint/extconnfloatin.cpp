/***************************************************************************
                          extconnfloatin.cpp  -  description
                             -------------------
    begin                : Sun Feb 10 2002
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

#include "extconnfloatin.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/wireproperty.h"

namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnFloatIn(container, ci);
}

const ComponentInfo * getExtConnFloatInInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Floating Point Input"),
	                                QString::fromLatin1("External Connector/Floating Point Input"),
	                                i18n("Component", "External Connector/Floating Point Input"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-extconn-float-in"));
	return &Info;
}

//###############################################################

ExtConnFloatIn::ExtConnFloatIn(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, true)
{
	ConnectorFloatOut * out;
	ConnectorFloatIn * in;
	
	out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"),
	                             QPoint(4,1));
	CHECK_PTR(out);
	setInternalConn(out);
	
	in = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"),
	                             QPoint(0,1));
	CHECK_PTR(in);
	setExternalConn(in);
	
}

/*ExtConnFloatIn::~ExtConnFloatIn()
{
} */

void ExtConnFloatIn::calculate()
{
	//ExternalConnector::calculate();
	
	ConnectorFloatOut * out = (ConnectorFloatOut *)getInternalConn();
	ConnectorFloatIn * in = (ConnectorFloatIn *)getExternalConn();
	
	out->setOutput(in->getInput());
	if (out->getWireProperty())
	{
		out->getWireProperty()->execute();
	}
}
//###############################################################


};  //namespace KSimLibFloatingPoint
