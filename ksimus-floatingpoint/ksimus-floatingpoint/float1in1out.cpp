/***************************************************************************
                          float1in1out.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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
#include "float1in1out.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorfloatin.h"
#include "ksimus/connectorfloatout.h"
#include "ksimus/componentlayout.h"



// Forward declaration

namespace KSimLibFloatingPoint
{


//###############################################################
//###############################################################


Float1In1Out::Float1In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_input = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input"),
	                             i18n("FloatingPoint-Connector", "Input"));
	CHECK_PTR(m_input);
}

/*Float1In1Out::~Float1In1Out()
{
} */


//###############################################################
//###############################################################



Float1In1OutView::Float1In1OutView(Float1In1Out * comp, eViewType viewType, const QString & text)
	: CompView(comp, viewType)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		CHECK_PTR(layout);
	
		layout->setMinSize(3, 5);

		layout->getLeft()->addStretch(2);
		layout->getLeft()->addConnector(comp->getInput());
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

/*Float1In1OutView::~Float1In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

