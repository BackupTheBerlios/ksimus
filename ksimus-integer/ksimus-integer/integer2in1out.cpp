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
	CHECK_PTR(m_inputA);
	m_inputB = new ConnectorIntegerIn(this,
	                             QString::fromLatin1("Input B"),
	                             i18n("Integer", "Input B"));
	CHECK_PTR(m_inputB);
}

/*Integer2In1Out::~Integer2In1Out()
{
} */


//###############################################################
//###############################################################



Integer2In1OutView::Integer2In1OutView(Integer2In1Out * comp, eViewType viewType)
	: Integer1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(comp->getInputA());
		getComponentLayout()->getLeft()->addConnector(comp->getInputB());
	
		getComponentLayout()->updateLayout();
	}
}

/*Integer2In1OutView::~Integer2In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibInteger

