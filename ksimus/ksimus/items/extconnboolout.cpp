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

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnBoolOut(container, ci);
}

const ComponentInfo ExtConnBoolOutInfo ("External Connector Boolean Output",
																				"External Connector/Bool Output",
																				QString::null,
																				VA_SHEETVIEW,
																				create );

//###############################################################

ExtConnBoolOut::ExtConnBoolOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci)
{
	out = new ConnectorBoolOut(this, I18N_NOOP("Output"), QPoint(4,1));
	CHECK_PTR(out);
	in = new ConnectorBoolIn(this, I18N_NOOP("Input"), QPoint(0,1));
	CHECK_PTR(in);
}

ExtConnBoolOut::~ExtConnBoolOut()
{
}
/** Shift the result of calculation to output */
void ExtConnBoolOut::updateOutput()
{
	Component::updateOutput();
	out->setOutput(in->getInput());
}
