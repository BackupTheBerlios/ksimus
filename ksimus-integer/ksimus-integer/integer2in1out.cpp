/***************************************************************************
                          integer2in1out.cpp  -  description
                             -------------------
    begin                : Fri Aug 1 2003
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
#include "integer2in1out.h"
#include "ksimus/resource.h"
#include "ksimus/ksimdebug.h"
#include "ksimus/connectorintegerin.h"
#include "ksimus/connectorintegerout.h"
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"



// Forward declaration

namespace KSimLibInteger
{


//###############################################################
//###############################################################


Integer2In1Out::Integer2In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Integer1Out(container, ci)
{
	m_inputA = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input A"),
	                             i18n("Integer", "Input A"));
	Q_CHECK_PTR(m_inputA);
	m_inputB = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input B"),
	                             i18n("Integer", "Input B"));
	Q_CHECK_PTR(m_inputB);
}

/*Integer2In1Out::~Integer2In1Out()
{
} */


//###############################################################
//###############################################################



Integer2In1OutView::Integer2In1OutView(Integer2In1Out * comp, eViewType viewType, const QString & text, const char * name)
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
		layout->getLeft()->addConnector(comp->getInputA(),1);
		layout->getLeft()->addConnector(comp->getInputB(),0);
		layout->getLeft()->addStretch(2);
		
		layout->getRight()->addStretch(2);
		layout->getRight()->addConnector(comp->getOutputConnector());
		layout->getRight()->addStretch(2);
		
		if (!text.isEmpty())
		{
			new ComponentLayoutBlockContentText(layout->getBlock(), text);
		}
	}
}

/*Integer2In1OutView::~Integer2In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

