/***************************************************************************
                          booleanxin1out.cpp  -  description
                             -------------------
    begin                : Fri Dec 7 2001
    copyright            : (C) 2001 by Rasmus Diekenbrock
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

// C-Includes

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "booleanxin1out.h"
#include "resource.h"
#include "ksimdebug.h"
#include "connectorboolin.h"
#include "connectorpack.h"
#include "componentlayout.h"



// Forward declaration


//###############################################################
//###############################################################


BooleanXIn1Out::BooleanXIn1Out(CompContainer * container, const ComponentInfo * ci)
	:	Boolean1Out(container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             getConnectorBoolInInfo(),
	                             2, 26);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
}

/*BooleanXIn1Out::~BooleanXIn1Out()
{
} */


//###############################################################
//###############################################################



BooleanXIn1OutView::BooleanXIn1OutView(BooleanXIn1Out * comp, eViewType viewType)
	: Boolean1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnectorPack(comp->getInputConnectorPack());
	
		getComponentLayout()->updateLayout();
	}
}

/*BooleanXIn1OutView::~BooleanXIn1OutView()
{
}*/

