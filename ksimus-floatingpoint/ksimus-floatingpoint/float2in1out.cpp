/***************************************************************************
                          float2in1out.cpp  -  description
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
#include "float2in1out.h"
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


Float2In1Out::Float2In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_inputA = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input A"),
	                             i18n("FloatingPoint-Connector", "Input A"));
	Q_CHECK_PTR(m_inputA);
	m_inputB = new ConnectorFloatIn(this,
	                             QString::fromLatin1("Input B"),
	                             i18n("FloatingPoint-Connector", "Input B"));
	Q_CHECK_PTR(m_inputB);
}

/*Float2In1Out::~Float2In1Out()
{
} */


//###############################################################
//###############################################################



Float2In1OutView::Float2In1OutView(Float2In1Out * comp, eViewType viewType, const QString & text, const char * name)
	: CompView(comp, viewType, name)
{
	setPlace(QRect(0, 0, 5*gridX, 5*gridY));
	enableRotation(true);
	
	if (viewType == SHEET_VIEW)
	{
		ComponentLayoutSimple * layout = new ComponentLayoutSimple(this);
		Q_CHECK_PTR(layout);
	
		layout->setMinSize(3, 5);

		layout->getLeft()->addSpace(1);
		layout->getLeft()->addConnector(comp->getInputA());
		layout->getLeft()->addConnector(comp->getInputB());
		
		layout->getRight()->addStretch(2);
		layout->getRight()->addConnector(comp->getOutputConnector());
		layout->getRight()->addStretch(2);
		
		if (!text.isEmpty())
		{
			new ComponentLayoutBlockContentText(layout->getBlock(), text);
		}
	}
}

/*Float2In1OutView::~Float2In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

