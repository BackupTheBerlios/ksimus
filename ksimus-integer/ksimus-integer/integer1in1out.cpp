/***************************************************************************
                          integer1in1out.cpp  -  description
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
#include "integer1in1out.h"



// Forward declaration

namespace KSimLibInteger
{


//###############################################################
//###############################################################


Integer1In1Out::Integer1In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Integer1Out(container, ci)
{
	m_input = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("Integer-Connector", "Input"));
	Q_CHECK_PTR(m_input);
}

/*Integer1In1Out::~Integer1In1Out()
{
} */


//###############################################################
//###############################################################



Integer1In1OutView::Integer1In1OutView(Integer1In1Out * comp, eViewType viewType, const QString & text, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		Q_CHECK_PTR(layout);
	
		layout->setMinSize(3, 5);

		layout->getLeft()->addStretch(2);
		layout->getLeft()->addConnector(comp->getInputConnector());
		layout->getLeft()->addStretch(2);
		
		layout->getRight()->addStretch(2);
		layout->getRight()->addConnector(comp->getOutputConnector(),0);
		layout->getRight()->addStretch(2);
		
		if (!text.isEmpty())
		{
			new ComponentLayoutBlockContentText(layout->getBlock(), text);
		}
	}
}

/*Integer1In1OutView::~Integer1In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

#include "integer1in1out.moc"
