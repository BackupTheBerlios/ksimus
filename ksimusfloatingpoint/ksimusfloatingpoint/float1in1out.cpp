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
#include "ksimus/connectorpack.h"
#include "ksimus/componentlayout.h"



// Forward declaration

namespace KSimLibFloatingPoint
{

static const char * sInput = I18N_NOOP("Input");

//###############################################################
//###############################################################


Float1In1Out::Float1In1Out(CompContainer * container, const ComponentInfo * ci)
	:	Float1Out(container, ci)
{
	m_input = new ConnectorFloatIn(this, sInput);
	CHECK_PTR(m_input);
}

/*Float1In1Out::~Float1In1Out()
{
} */


//###############################################################
//###############################################################



Float1In1OutView::Float1In1OutView(Float1In1Out * comp, eViewType viewType)
	: Float1OutView(comp, viewType)
{
	if (viewType == SHEET_VIEW)
	{
		getComponentLayout()->setMinSize(5, 3);
		getComponentLayout()->getLeft()->addSpace(1);
		getComponentLayout()->getLeft()->addConnector(comp->getInput());
	
		getComponentLayout()->updateLayout();
	}
}

/*Float1In1OutView::~Float1In1OutView()
{
}*/


//###############################################################
//###############################################################

};  //namespace KSimLibFloatingPoint

