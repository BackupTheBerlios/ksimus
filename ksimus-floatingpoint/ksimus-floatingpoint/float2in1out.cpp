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
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"



// Forward declaration

namespace KSimLibFloatingPoint
{

static const char * sInputA = I18N_NOOP("Input A");
static const char * sInputB = I18N_NOOP("Input B");

//###############################################################
//###############################################################


Float2In1Out::Float2In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_inputA = new ConnectorFloatIn(this, sInputA);
	CHECK_PTR(m_inputA);
	m_inputB = new ConnectorFloatIn(this, sInputB);
	CHECK_PTR(m_inputB);
}

/*Float2In1Out::~Float2In1Out()
{
} */


//###############################################################
//###############################################################



Float2In1OutView::Float2In1OutView(Float2In1Out * comp, eViewType viewType)
	: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(comp->getInputA());
		getComponentLayout()->getLeft()->addConnector(comp->getInputB());
	
		getComponentLayout()->updateLayout();
	}
}

/*Float2In1OutView::~Float2In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

