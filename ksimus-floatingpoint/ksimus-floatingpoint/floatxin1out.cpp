/***************************************************************************
                          floatxin1out.cpp  -  description
                             -------------------
    begin                : Wen Jan 3 2002
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

// C-Includes

// QT-Includes

// KDE-Includes
#include <klocale.h>

// Project-Includes
#include "floatxin1out.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"



// Forward declaration


namespace KSimLibFloatingPoint
{

//###############################################################
//###############################################################


FloatXIn1Out::FloatXIn1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Connector", "Input %1"),
	                             getConnectorFloatInInfo(),
	                             2, 10);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
}

/*FloatXIn1Out::~FloatXIn1Out()
{
} */


//###############################################################
//###############################################################



FloatXIn1OutView::FloatXIn1OutView(FloatXIn1Out * comp, eViewType viewType, const QString & text)
	: CompView(comp, viewType)
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

/*FloatXIn1OutView::~FloatXIn1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

