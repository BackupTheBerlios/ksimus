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
#include "connectorboolout.h"
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



BooleanXIn1OutView::BooleanXIn1OutView(BooleanXIn1Out * comp, eViewType viewType, const QString & text, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		CHECK_PTR(layout);
	
		layout->setMinSize(3, 5);

		layout->getLeft()->addSpace(1);
		layout->getLeft()->addConnectorPack(comp->getInputConnectorPack());
		
		layout->getRight()->addStretch(2);
		layout->getRight()->addConnector(comp->getOutputConnector());
		layout->getRight()->addStretch(2);
		
		if (!text.isEmpty())
		{
			new ComponentLayoutBlockContentText(layout->getBlock(), text);
		}
	}
}

/*BooleanXIn1OutView::~BooleanXIn1OutView()
{
}*/

#include "booleanxin1out.moc"
