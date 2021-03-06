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

namespace KSimLibFloatingPoint
{

static Component * create(CompContainer * container, const ComponentInfo * ci)
{
	return new ExtConnFloatOut(container, ci);
}

const ComponentInfo ExtConnFloatOutInfo(I18N_NOOP("External Connector Floating Point Output"),
                                        I18N_NOOP("External Connector/Floating Point Output"),
                                        QString::null,
                                        VA_SHEETVIEW,
                                        create,
                                        QString::null,
                                        "component-float-extconn-float-out");

//###############################################################

ExtConnFloatOut::ExtConnFloatOut(CompContainer * container, const ComponentInfo * ci)
	: ExternalConnector(container, ci)
{
	out = new ConnectorFloatOut(this, I18N_NOOP("Output"), QPoint(4,1));
	CHECK_PTR(out);
	in = new ConnectorFloatIn(this, I18N_NOOP("Input"), QPoint(0,1));
	CHECK_PTR(in);
}

/*ExtConnFloatOut::~ExtConnFloatOut()
{
} */
/** Shift the result of calculation to output */
void ExtConnFloatOut::updateOutput()
{
	Component::updateOutput();
	out->setOutput(in->getInput());
}
//###############################################################


};  //namespace KSimLibFloatingPoint
