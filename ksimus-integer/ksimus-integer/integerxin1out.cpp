/***************************************************************************
                          integerxin1out.cpp  -  description
                             -------------------
    begin                : Sat Jul 12 2003
    copyright            : (C) 2003 by Rasmus Diekenbrock
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
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"
#include "integerxin1out.h"



// Forward declaration


namespace KSimLibInteger
{

//###############################################################
//###############################################################


IntegerXIn1Out::IntegerXIn1Out(CompContainer * container, const ComponentInfo * ci)
	:	Integer1Out(container, ci)
{
	m_inPack = new ConnectorPack(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Interger-Connector", "Input %1"),
	                             ConnectorIntegerIn::getStaticInfo(),
	                             2, 52);
	CHECK_PTR(m_inPack);
	m_inPack->setConnectorCount(2);
}

/*IntegerXIn1Out::~IntegerXIn1Out()
{
} */


//###############################################################
//###############################################################



IntegerXIn1OutView::IntegerXIn1OutView(IntegerXIn1Out * comp, eViewType viewType, const QString & text)
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

/*IntegerXIn1OutView::~IntegerXIn1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

#include "integerxin1out.moc"
