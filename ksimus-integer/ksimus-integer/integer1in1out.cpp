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
	CHECK_PTR(m_input);
}

/*Integer1In1Out::~Integer1In1Out()
{
} */


//###############################################################
//###############################################################



Integer1In1OutView::Integer1In1OutView(Integer1In1Out * comp, eViewType viewType, unsigned int width, unsigned int height)
	: Integer1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->setMinSize(width, height);
		getComponentLayout()->getLeft()->addStretch(2);
		getComponentLayout()->getLeft()->addConnector(comp->getInputConnector());
		getComponentLayout()->getLeft()->addStretch(2);
	
		getComponentLayout()->updateLayout();
	}
}

/*Integer1In1OutView::~Integer1In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

