/***************************************************************************
                          extconnfloatout.cpp  -  description
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

#include "extconnfloatout.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/componentinfo.h"
#include "ksimus/wireproperty.h"

namespace KSimLibFloatingPoint
{

Component * ExtConnFloatOut::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnFloatOut(container, ci);
}

const ComponentInfo * ExtConnFloatOut::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Floating Point Output"),
	                                QString::fromLatin1("External Connector/Floating Point Output"),
	                                i18n("Component", "External Connector/Floating Point Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create,
	                                QString::null,
	                                QString::fromLatin1("component-float-extconn-float-out"));
	return &Info;
}

//###############################################################

ExtConnFloatOut::ExtConnFloatOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, false)
{
	ConnectorFloatOut * out;
	ConnectorFloatIn * in;
	
	out = new ConnectorFloatOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("FloatingPoint-Connector", "Output"),
	                             QPoint(4,1));
	CHECK_PTR(out);
	setExternalConn(out);
	
	in = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"),
	                             QPoint(0,1));
	CHECK_PTR(in);
	setInternalConn(in);

}

/*ExtConnFloatOut::~ExtConnFloatOut()
{
} */

void ExtConnFloatOut::calculate()
{
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
//		ExternalConnector::calculate();
	
		ConnectorFloatOut * out = (ConnectorFloatOut *)getUsedExternalConn();
		ConnectorFloatIn * in = (ConnectorFloatIn *)getInternalConn();
	
		out->setOutput(in->getInput(), false);
		if (out->getWireProperty())
		{
			out->getWireProperty()->execute();
		}
		setRecursionLocked(false);
	}
	else
	{
		executeNext();
	}
}
//###############################################################


};  //namespace KSimLibFloatingPoint
