/***************************************************************************
                          extconnintegerout.cpp  -  description
                             -------------------
    begin                : Sat Aug 16 2003
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

#include "extconnintegerout.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/wireproperty.h"

namespace KSimLibInteger
{

Component * ExtConnIntegerOut::create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnIntegerOut(container, ci);
}

const ComponentInfo * ExtConnIntegerOut::getStaticInfo()
{
	static const ComponentInfo Info(i18n("Component", "External Connector Integer Output"),
	                                QString::fromLatin1("External Connector/Integer Output"),
	                                i18n("Component", "External Connector/Integer Output"),
	                                QString::null,
	                                VA_SHEETVIEW,
	                                create /* TODO,
	                                QString::null,
	                                QString::fromLatin1("component-float-extconn-float-out")*/);
	return &Info;
}

//###############################################################

ExtConnIntegerOut::ExtConnIntegerOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci, false)
{
	ConnectorIntegerOut * out;
	ConnectorIntegerIn * in;
	
	out = new ConnectorIntegerOut(this,
	                             QString::fromLatin1("Output"),
	                             i18n("Integer-Connector", "Output"),
	                             QPoint(4,1));
	CHECK_PTR(out);
	setExternalConn(out);
	
	in = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"),
	                             QPoint(0,1));
	CHECK_PTR(in);
	setInternalConn(in);

}

/*ExtConnIntegerOut::~ExtConnIntegerOut()
{
} */

void ExtConnIntegerOut::calculate()
{
	// Protect against infinite recursion
	if (!isRecursionLocked())
	{
		setRecursionLocked(true);
		ExternalConnector::calculate();
	
		ConnectorIntegerOut * out = (ConnectorIntegerOut *)getUsedExternalConn();
		ConnectorIntegerIn * in = (ConnectorIntegerIn *)getInternalConn();
	
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


};  //namespace KSimLibInteger
